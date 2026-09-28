#pragma once
#include <stdint.h>
#include <stddef.h>

enum class NetState : uint8_t {
    OFF,  
    CONNECTING,
    STA,        
    AP          
};

struct WeatherData {
    int code = 1;
    float temp = 0.0f;
    int humidity = 0;
    bool valid = false;
    uint32_t updatedAt = 0;
    float forecastTemp[4] = {0};
    int forecastCode[4] = {800, 800, 800, 800};
};

namespace NetService {
    void begin();         
    bool retrySta();       
    void startAp();     
    void goOffline();    
    NetState state();
    WeatherData weather();
    bool timeReady();
    void formatClock(char* buf, size_t n);
    void formatIp(char* buf, size_t n);
}