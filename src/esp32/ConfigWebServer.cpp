#include "BoardConfig.h"
#include "ConfigServer.h"
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

static WebServer server(80);

static void handleRoot() {
    File f = LittleFS.open("/www/index.html", "r");
    if (f) {
        server.streamFile(f, "text/html");
        f.close();
        return;
    }
    server.send(404, "text/plain", "index.html not found. Run: pio run -t uploadfs");
}

static void handleGetConfig() {
    JsonDocument doc;
    
    JsonObject wifi = doc["wifi"].to<JsonObject>();
    wifi["ssid"] = g_board.wifiSsid;
    wifi["pass"] = g_board.wifiPass;
    
    JsonObject ap = doc["ap"].to<JsonObject>();
    ap["name"] = g_board.apName;
    ap["pass"] = g_board.apPass;
    
    JsonArray arr = doc["displays"].to<JsonArray>();
    for (uint8_t i = 0; i < g_board.displayCount; i++) {
        JsonObject o = arr.add<JsonObject>();
        o["name"] = g_board.displays[i].name;
        const char* type = (g_board.displays[i].type == DisplayType::ST7735) ? "ST7735" : "SSD1306";
        o["type"] = type;
        o["width"] = g_board.displays[i].width;
        o["height"] = g_board.displays[i].height;
        o["rotation"] = g_board.displays[i].rotation;
        o["cs"] = g_board.displays[i].cs;
        o["dc"] = g_board.displays[i].dc;
        o["rst"] = g_board.displays[i].rst;
        o["bl"] = g_board.displays[i].bl;
        o["addr"] = g_board.displays[i].i2cAddr;
        o["sda"] = g_board.displays[i].sda;
        o["scl"] = g_board.displays[i].scl;
    }
    
    JsonArray ca = doc["cities"].to<JsonArray>();
    for (uint8_t i = 0; i < g_board.cityCount; i++) {
        JsonObject o = ca.add<JsonObject>();
        o["name"] = g_board.cities[i].name;
        o["offMin"] = g_board.cities[i].offMin;
    }
    
    doc["weatherCity"] = g_board.weatherCity;
    
    JsonObject loc = doc["loc"].to<JsonObject>();
    loc["lat"] = g_board.lat;
    loc["lon"] = g_board.lon;
    JsonObject q = doc["quiet"].to<JsonObject>();
    q["from"] = g_board.quietFrom;
    q["to"] = g_board.quietTo;
    doc["weatherPeriodMin"] = g_board.weatherPeriodMin;
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

static void handleSaveConfig() {
    String body = "";
    if (server.hasArg("plain")) {
        body = server.arg("plain");
    } else {
        WiFiClient client = server.client();
        while (client.available()) body += (char)client.read();
    }
    
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err) {
        server.send(400, "text/plain", String("JSON error: ") + err.c_str());
        return;
    }
    
    JsonObject wifi = doc["wifi"];
    if (!wifi.isNull()) {
        strlcpy(g_board.wifiSsid, wifi["ssid"] | "", sizeof(g_board.wifiSsid));
        strlcpy(g_board.wifiPass, wifi["pass"] | "", sizeof(g_board.wifiPass));
    }
    
    JsonObject ap = doc["ap"];
    if (!ap.isNull()) {
        strlcpy(g_board.apName, ap["name"] | "", sizeof(g_board.apName));
        strlcpy(g_board.apPass, ap["pass"] | "", sizeof(g_board.apPass));
    }
    
    JsonArray arr = doc["displays"];
    if (!arr.isNull()) {
        uint8_t n = 0;
        for (JsonObject o : arr) {
            if (n >= 4) break;
            DisplayConfig& d = g_board.displays[n];
            strlcpy(d.name, o["name"] | "", sizeof(d.name));
            const char* t = o["type"] | "";
            if (strcmp(t, "ST7735") == 0) d.type = DisplayType::ST7735;
            else if (strcmp(t, "SSD1306") == 0) d.type = DisplayType::SSD1306;
            else d.type = DisplayType::None;
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
        g_board.displayCount = n;
    }
    
    JsonArray ca = doc["cities"];
    if (!ca.isNull()) {
        uint8_t n = 0;
        for (JsonObject o : ca) {
            if (n >= 12) break;
            strlcpy(g_board.cities[n].name, o["name"] | "", sizeof(g_board.cities[n].name));
            g_board.cities[n].offMin = o["offMin"] | 0;
            if (g_board.cities[n].name[0] != '\0') n++;
        }
        g_board.cityCount = n;
    }
    
    strlcpy(g_board.weatherCity, doc["weatherCity"] | "", sizeof(g_board.weatherCity));
    g_board.lat = doc["loc"]["lat"] | g_board.lat;
    g_board.lon = doc["loc"]["lon"] | g_board.lon;
    g_board.quietFrom = doc["quiet"]["from"] | g_board.quietFrom;
    g_board.quietTo = doc["quiet"]["to"] | g_board.quietTo;
    g_board.weatherPeriodMin = doc["weatherPeriodMin"] | g_board.weatherPeriodMin;
    if (!g_board.save()) {
        server.send(500, "text/plain", "save failed");
        return;
    }
    
    server.send(200, "text/plain", "saved");
}

static void handleRestart() {
    server.send(200, "text/plain", "restarting");
    delay(500);
    ESP.restart();
}

static void handleGetNet() {
    JsonDocument doc;
    doc["state"] = (WiFi.status() == WL_CONNECTED) ? "STA" : "OFF";
    doc["ip"] = WiFi.localIP().toString();
    doc["weatherAgeSec"] = -1;
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

static void handleWifiScan() {
    int n = WiFi.scanNetworks();
    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    for (int i = 0; i < n; i++) {
        JsonObject o = arr.add<JsonObject>();
        o["ssid"] = WiFi.SSID(i);
        o["rssi"] = WiFi.RSSI(i);
    }
    WiFi.scanDelete();
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

static void handleCapabilities() {
    JsonDocument doc;
    JsonArray disp = doc["displays"].to<JsonArray>();
    disp.add("ST7735");
    disp.add("SSD1306");
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

void ConfigServer::begin() {
    server.on("/", handleRoot);
    server.on("/api/config", HTTP_GET, handleGetConfig);
    server.on("/api/config", HTTP_POST, handleSaveConfig);
    server.on("/api/restart", HTTP_POST, handleRestart);
    server.on("/api/net", HTTP_GET, handleGetNet);
    server.on("/api/wifi/scan", HTTP_GET, handleWifiScan);
    server.on("/api/capabilities", HTTP_GET, handleCapabilities);
    server.begin();
}


void ConfigServer::loop() {
    server.handleClient();
}