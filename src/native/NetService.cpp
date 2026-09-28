#include "NetService.h"
#include <stdio.h>

static WeatherData s_wx;
static NetState s_state = NetState::OFF;
namespace NetService {
    void begin() {}
    bool retrySta() { return false; }
    void startAp() {}
    NetState state() { return NetState::OFF; }
    WeatherData weather() { return s_wx; }
    bool timeReady() { return false; }
    void formatClock(char* buf, size_t n) { snprintf(buf, n, "--:--"); }
    void goOffline() { s_state = NetState::OFF; }
    void formatIp(char* buf, size_t n) { snprintf(buf, n, "127.0.0.1"); }
}