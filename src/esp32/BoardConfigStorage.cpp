#include <Arduino.h>
#include <string.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "log.h"
#include "BoardConfig.h"

static DisplayType parseType(const char* t) {
    if (!t) return DisplayType::None;
    if (strcmp(t, "ST7735") == 0) return DisplayType::ST7735;
    if (strcmp(t, "SSD1306") == 0) return DisplayType::SSD1306;
    return DisplayType::None;
}

static const char* typeName(DisplayType t) {
    switch (t) {
        case DisplayType::ST7735: return "ST7735";
        case DisplayType::SSD1306: return "SSD1306";
        default: return "None";
    }
}

bool BoardConfig::load() {
    if (!LittleFS.begin(true)) {
        LOGE(LT_HW, "LittleFS mount failed");
        return false;
    }
    File f = LittleFS.open("/config.json", "r");
    if (!f) {
        LOGE(LT_HW,"config.json not found");
        return false;
    }
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, f);
    f.close();
    if (err) {
        LOGE(LT_HW,"Parse error: %s\n", err.c_str());
        return false;
    }
    
    JsonObject wifi = doc["wifi"];
    if (!wifi.isNull()) {
        strlcpy(wifiSsid, wifi["ssid"] | "", sizeof(wifiSsid));
        strlcpy(wifiPass, wifi["pass"] | "", sizeof(wifiPass));
    }
    
    JsonObject ap = doc["ap"];
    if (!ap.isNull()) {
        strlcpy(apName, ap["name"] | "Yougille-Config", sizeof(apName));
        strlcpy(apPass, ap["pass"] | "12345678", sizeof(apPass));
    }
    
    JsonArray arr = doc["displays"];
    if (!arr.isNull()) {
        uint8_t n = 0;
        for (JsonObject o : arr) {
            if (n >= 4) break;
            DisplayConfig& d = displays[n];
            strlcpy(d.name, o["name"] | "", sizeof(d.name));
            d.type = parseType(o["type"] | "");
            d.width = o["width"] | 0;
            d.height = o["height"] | 0;
            d.rotation = o["rotation"] | 0;
            d.cs = o["cs"] | -1;
            d.dc = o["dc"] | -1;
            d.rst = o["rst"] | -1;
            d.bl = o["bl"] | -1;
            d.i2cAddr = o["addr"] | 0;
            d.sda = o["sda"] | -1;
            d.scl = o["scl"] | -1;
            if (d.type != DisplayType::None && d.name[0] != '\0') n++;
        }
        displayCount = n;
    }
    
    JsonArray ca = doc["cities"];
    if (!ca.isNull()) {
        uint8_t n = 0;
        for (JsonObject o : ca) {
            if (n >= 12) break;
            strlcpy(cities[n].name, o["name"] | "", sizeof(cities[n].name));
            cities[n].offMin = o["offMin"] | 0;
            if (cities[n].name[0] != '\0') n++;
        }
        cityCount = n;
    }
    
    strlcpy(weatherCity, doc["weatherCity"] | "Moscow", sizeof(weatherCity));
    lat = doc["loc"]["lat"] | 55.75f;
    lon = doc["loc"]["lon"] | 37.62f;
    quietFrom = doc["quiet"]["from"] | 23;
    quietTo = doc["quiet"]["to"] | 7;
    weatherPeriodMin = doc["weatherPeriodMin"] | 30;
    return displayCount > 0;
}

bool BoardConfig::save() {
    JsonDocument doc;
    
    JsonObject wifi = doc["wifi"].to<JsonObject>();
    wifi["ssid"] = wifiSsid;
    wifi["pass"] = wifiPass;
    
    JsonObject ap = doc["ap"].to<JsonObject>();
    ap["name"] = apName;
    ap["pass"] = apPass;
    
    JsonArray arr = doc["displays"].to<JsonArray>();
    for (uint8_t i = 0; i < displayCount; i++) {
        JsonObject o = arr.add<JsonObject>();
        o["name"] = displays[i].name;
        o["type"] = typeName(displays[i].type);
        o["width"] = displays[i].width;
        o["height"] = displays[i].height;
        o["rotation"] = displays[i].rotation;
        o["cs"] = displays[i].cs;
        o["dc"] = displays[i].dc;
        o["rst"] = displays[i].rst;
        o["bl"] = displays[i].bl;
        o["addr"] = displays[i].i2cAddr;
        o["sda"] = displays[i].sda;
        o["scl"] = displays[i].scl;
    }
    
    JsonArray ca = doc["cities"].to<JsonArray>();
    for (uint8_t i = 0; i < cityCount; i++) {
        JsonObject o = ca.add<JsonObject>();
        o["name"] = cities[i].name;
        o["offMin"] = cities[i].offMin;
    }
    
    doc["weatherCity"] = weatherCity;
    JsonObject loc = doc["loc"].to<JsonObject>();
    loc["lat"] = lat;
    loc["lon"] = lon;
    JsonObject q = doc["quiet"].to<JsonObject>();
    q["from"] = quietFrom;
    q["to"] = quietTo;
    doc["weatherPeriodMin"] = weatherPeriodMin;
    
    File f = LittleFS.open("/config.json", "w");
    if (!f) return false;
    serializeJson(doc, f);
    f.close();
    return true;
}
