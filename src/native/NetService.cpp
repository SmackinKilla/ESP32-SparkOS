#include "NetService.h"
#include <stdio.h>
#include <time.h>
static WeatherData s_wx;
static NetState s_state = NetState::OFF;
namespace NetService {
    void begin() {}
    bool retrySta() { return false; }
    void startAp() {}
    NetState state() { return NetState::OFF; }
    WeatherData weather() { return s_wx; }
    bool timeReady() { return true; }
    void formatClock(char* buf, size_t n) {
    time_t now = time(nullptr);
    struct tm* t = localtime(&now);
    snprintf(buf, n, "%02d:%02d", t->tm_hour, t->tm_min);
    }
    void goOffline() { s_state = NetState::OFF; }
    void formatIp(char* buf, size_t n) { snprintf(buf, n, "127.0.0.1"); }
}