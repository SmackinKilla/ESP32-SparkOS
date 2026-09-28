#pragma once
#include <stdint.h>

enum : uint8_t {
    LV_NONE = 0,   
    LV_ERR  = 1,  
    LV_WARN = 2,   
    LV_INFO = 3, 
    LV_DBG  = 4,  
};

enum : uint8_t {
    LT_BOOT = 0, LT_HW, LT_NET, LT_WEB, LT_FS, LT_SET, LT_TIME, LT_WX, LT_UI, LT_COUNT
};

namespace Log {
    void init();                               
    void write(uint8_t tag, uint8_t level, const char* fmt, ...);
    void setLevel(uint8_t tag, uint8_t level);  
    void setLevelAll(uint8_t level);           
}

#define LOGE(tag, ...) Log::write(tag, LV_ERR,  __VA_ARGS__)
#define LOGW(tag, ...) Log::write(tag, LV_WARN, __VA_ARGS__)
#define LOGI(tag, ...) Log::write(tag, LV_INFO, __VA_ARGS__)
#define LOGD(tag, ...) Log::write(tag, LV_DBG,  __VA_ARGS__)