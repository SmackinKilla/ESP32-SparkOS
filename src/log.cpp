#include "log.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>

static const char* TAG_NAME[LT_COUNT] = {
    "BOOT", "HW", "NET", "WEB", "FS", "SET", "TIME", "WX", "UI"
};

static uint8_t s_level[LT_COUNT];

void Log::init() {
    for (uint8_t i = 0; i < LT_COUNT; i++) s_level[i] = LV_INFO;
    LOGI(LT_BOOT, "=== StudyOS ===");
}

void Log::setLevel(uint8_t tag, uint8_t level) {
    if (tag < LT_COUNT) s_level[tag] = level;
}

void Log::setLevelAll(uint8_t level) {
    for (uint8_t i = 0; i < LT_COUNT; i++) s_level[i] = level;
}

void Log::write(uint8_t tag, uint8_t level, const char* fmt, ...) {
    if (tag >= LT_COUNT || level > s_level[tag]) return;   
    char buf[256];  
    va_list args;                                         
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    static const char LCH[] = { '-', 'E', 'W', 'I', 'D' };
    Serial.printf("[%6lu][%c][%s] %s\n",
                  (unsigned long)millis(), LCH[level], TAG_NAME[tag], buf);
}