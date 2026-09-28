#pragma once
#include <stdint.h>
#include "DisplayTypes.h"

struct CityInfo {
    char name[16] = "";
    int offMin = 0;
};

struct BoardConfig {
    char wifiSsid[32] = "";
    char wifiPass[64] = "";
    
    char apName[32] = "ESP32-AP";
    char apPass[64] = "12345678";
    
    DisplayConfig displays[4];
    uint8_t displayCount = 0;
    
    CityInfo cities[12];
    uint8_t cityCount = 0;
    
    char weatherCity[32] = "Moscow";
    float lat = 55.75f;
    float lon = 37.62f;
    
    uint8_t quietFrom = 23;
    uint8_t quietTo = 7;
    uint16_t weatherPeriodMin = 30;
    void setDefaults();
    bool load();
    bool save();
};

extern BoardConfig g_board;