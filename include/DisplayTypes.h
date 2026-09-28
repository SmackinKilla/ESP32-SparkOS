#pragma once
#include <stdint.h>

enum class DisplayType : uint8_t {
    None,
    ST7735,
    SSD1306
};

struct DisplayConfig {
    char name[16];
    DisplayType type;
    uint16_t width;
    uint16_t height;
    uint8_t rotation;
    int8_t cs;
    int8_t dc;
    int8_t rst;
    int8_t bl;
    uint8_t i2cAddr;
    int8_t sda;
    int8_t scl;
};

