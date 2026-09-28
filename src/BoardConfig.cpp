#include "BoardConfig.h"
#include <Arduino.h>
#include <string.h>

BoardConfig g_board;

void BoardConfig::setDefaults() {
    displayCount = 2;
    memset(wifiSsid, 0, sizeof(wifiSsid));
    memset(wifiPass, 0, sizeof(wifiPass));
    
    displays[0] = DisplayConfig{"main", DisplayType::ST7735, 160, 128, 1, 5, 16, 17, 32, 0, -1, -1};
    displays[1] = DisplayConfig{"oled", DisplayType::SSD1306, 128, 64, 0, -1, -1, -1, -1, 0x3C, 21, 22};
    
    strlcpy(apName, "ESP32-AP", sizeof(apName));
    strlcpy(apPass, "12345678", sizeof(apPass));
    
    static const struct { const char* name; int offMin; } defCities[] = {
        {"Kaliningrad", 120}, {"Moscow", 180}, {"Samara", 240},
        {"Yekaterinburg", 300}, {"Omsk", 360}, {"Krasnoyarsk", 420},
        {"Irkutsk", 480}, {"Yakutsk", 540}, {"Vladivostok", 600},
        {"UTC", 0}, {"New York", -300}, {"Tokyo", 540}
    };
    cityCount = sizeof(defCities) / sizeof(defCities[0]);
    for (uint8_t i = 0; i < cityCount; i++) {
        strlcpy(cities[i].name, defCities[i].name, sizeof(cities[i].name));
        cities[i].offMin = defCities[i].offMin;
    }
    
    strlcpy(weatherCity, "Moscow", sizeof(weatherCity));
    lat = 55.75f;
    lon = 37.62f;
    quietFrom = 23;
    quietTo = 7;
    weatherPeriodMin = 30;
}
