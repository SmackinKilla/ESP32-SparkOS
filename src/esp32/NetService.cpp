#include "NetService.h"
#include <stdio.h>
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <time.h>
#include <math.h>
#include "BoardConfig.h"
#include "log.h"

static WeatherData s_wx;
static SemaphoreHandle_t s_mtx = nullptr;
static volatile bool s_timeReady = false;
static volatile NetState s_state = NetState::OFF;
static const int LOCAL_UTC_OFFSET_H = 3;

static void fmt2f(char* buf, size_t n, float v) {
    float av = fabsf(v);
    int whole = (int)av;
    int frac = (int)(av * 100.0f + 0.5f) % 100;
    if (v < 0) snprintf(buf, n, "-%d.%02d", whole, frac);
    else       snprintf(buf, n, "%d.%02d", whole, frac);
}

static bool isQuiet() {
    struct tm tmv;
    if (!s_timeReady || !getLocalTime(&tmv, 0)) return false;
    return tmv.tm_hour >= g_board.quietFrom || tmv.tm_hour < g_board.quietTo;
}

static bool trySta() {
    LOGI(LT_NET, "STA: connecting to '%s'", g_board.wifiSsid);
    s_state = NetState::CONNECTING;
    WiFi.mode(WIFI_STA);
    WiFi.begin(g_board.wifiSsid, g_board.wifiPass);
    for (int i = 0; i < 14; i++) {
        if (WiFi.status() == WL_CONNECTED) {
            s_state = NetState::STA;
            LOGI(LT_NET, "STA up: ip=%s rssi=%d dBm",
                 WiFi.localIP().toString().c_str(), WiFi.RSSI());
            return true;
        }
        delay(500);
    }
    WiFi.disconnect();
    s_state = NetState::OFF;
    LOGW(LT_NET, "STA failed: router not found");
    return false;
}

static void syncTime() {
    configTime(LOCAL_UTC_OFFSET_H * 3600, 0, "pool.ntp.org", "time.cloudflare.com");
    struct tm tmv;
    for (int i = 0; i < 10; i++) {
        if (getLocalTime(&tmv, 1000)) { s_timeReady = true; LOGI(LT_TIME, "NTP synced"); return; }
    }
}

static bool httpGetJson(const char* url, JsonDocument& doc) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    http.setTimeout(8000);
    if (!http.begin(client, url)) {
        LOGE(LT_WX, "http.begin failed, heap=%u", ESP.getFreeHeap());
        return false;
    }
    int code = http.GET();
    if (code != 200) {
        LOGW(LT_WX, "GET -> %d %s", code, http.errorToString(code).c_str());
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();  
    
    if (payload.length() == 0) {
        LOGW(LT_WX, "empty payload");
        return false;
    }
    
    DeserializationError err = deserializeJson(doc, payload);
    if (err) {
        LOGW(LT_WX, "json parse: %s (len=%d)", err.c_str(), payload.length());
        return false;
    }
    return true;
}

static bool fetchOpenMeteo(WeatherData& out) {
    char lat[12], lon[12];
    fmt2f(lat, sizeof(lat), g_board.lat);
    fmt2f(lon, sizeof(lon), g_board.lon);
    char url[256];
    snprintf(url, sizeof(url),
        "https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s"
        "&current=temperature_2m,relative_humidity_2m,weather_code"
        "&hourly=temperature_2m,weather_code&forecast_days=1",
        lat, lon);
    LOGI(LT_WX, "GET %s", url);
    
    JsonDocument doc;
    if (!httpGetJson(url, doc)) return false;

    JsonObject cur = doc["current"];
    if (cur.isNull()) { LOGW(LT_WX, "no 'current'"); return false; }
    out.temp     = cur["temperature_2m"]       | 0.0f;
    out.humidity = cur["relative_humidity_2m"] | 0;
    out.code     = cur["weather_code"]         | 800;

    JsonArray hourlyTemp = doc["hourly"]["temperature_2m"];
    JsonArray hourlyCode = doc["hourly"]["weather_code"];
    if (!hourlyTemp.isNull() && hourlyTemp.size() >= 24) {
        for (int block = 0; block < 4; block++) {
            float sum = 0;
            int count = 0;
            for (int h = block * 6; h < (block + 1) * 6; h++) {
                sum += hourlyTemp[h].as<float>();
                count++;
            }
            out.forecastTemp[block] = (count > 0) ? (sum / count) : 0.0f;
            out.forecastCode[block] = hourlyCode[block * 6 + 3].as<int>();
        }
    }
    
    return true;
}

static void fetchWeather() {
    WeatherData tmp;
    if (!fetchOpenMeteo(tmp)) { LOGW(LT_WX, "fetch failed"); return; }
    tmp.valid = true;
    tmp.updatedAt = millis();
    xSemaphoreTake(s_mtx, portMAX_DELAY);
    s_wx = tmp;
    xSemaphoreGive(s_mtx);
    LOGI(LT_WX, "ok: %.1fC code=%d", tmp.temp, tmp.code);
}

static void netTask(void*) {
    uint32_t lastWeatherMs = 0;
    for (;;) {
        if (s_state == NetState::STA) {
            if (WiFi.status() != WL_CONNECTED) {
                LOGW(LT_NET, "Link lost, reconnecting");
                s_state = NetState::CONNECTING;
                int i = 0;
                for (; i < 10; i++) {
                    if (WiFi.status() == WL_CONNECTED) break;
                    delay(500);
                }
                if (i < 10) { s_state = NetState::STA; lastWeatherMs = 0; LOGI(LT_NET, "Reconnected"); }
                else { s_state = NetState::OFF; LOGE(LT_NET, "Reconnect failed -> OFF"); }
            } else {
                if (!s_timeReady) syncTime();
                uint32_t now = millis();
                uint32_t periodMs = (uint32_t)g_board.weatherPeriodMin * 60000UL;
                bool due = (lastWeatherMs == 0) || (now - lastWeatherMs >= periodMs);
                if (due && !isQuiet()) {
                    fetchWeather();
                    lastWeatherMs = now;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(15000));
    }
}

static void raiseAp() {
    if (s_state == NetState::AP) return;
    WiFi.mode(WIFI_AP);
    WiFi.softAP(g_board.apName, g_board.apPass);
    s_state = NetState::AP;
    LOGI(LT_NET, "AP '%s' up, ip=%s", g_board.apName, WiFi.softAPIP().toString().c_str());
}

namespace NetService {
    void begin() {
        s_mtx = xSemaphoreCreateMutex();
        if (strlen(g_board.wifiSsid) > 0) {
            if (!trySta()) raiseAp();
        } else {
            raiseAp();
        }
        xTaskCreatePinnedToCore(netTask, "net", 12288, nullptr, 1, nullptr, 0); 
    }
    bool retrySta() { return trySta(); }
    void startAp() { raiseAp(); }
    void goOffline() {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        s_state = NetState::OFF;
        LOGI(LT_NET, "Offline mode");
    }
    NetState state() { return s_state; }
    WeatherData weather() {
        WeatherData copy;
        xSemaphoreTake(s_mtx, portMAX_DELAY);
        copy = s_wx;
        xSemaphoreGive(s_mtx);
        return copy;
    }
    bool timeReady() { return s_timeReady; }
    void formatClock(char* buf, size_t n) {
        struct tm tmv;
        if (s_timeReady && getLocalTime(&tmv, 0)) {
            snprintf(buf, n, "%02d:%02d", tmv.tm_hour, tmv.tm_min);
        } else {
            snprintf(buf, n, "--:--");
        }
    }
    void formatIp(char* buf, size_t n) {
        if (s_state == NetState::STA)      snprintf(buf, n, "%s", WiFi.localIP().toString().c_str());
        else if (s_state == NetState::AP)  snprintf(buf, n, "%s", WiFi.softAPIP().toString().c_str());
        else                               snprintf(buf, n, "no net");
    }
}