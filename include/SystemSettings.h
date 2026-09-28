#pragma once
#include <stdint.h>

enum class Theme : uint8_t {
    BLUE = 0,
    RED,
    PURPLE,
    GREEN,
    DARK,
    Win95,
    COUNT       
};

enum class ClickSpeed : uint8_t {
    FAST = 0,   
    NORMAL,     
    SLOW,       
    COUNT
};

struct SystemSettings {
    Theme theme = Theme::BLUE;

    ClickSpeed clickSpeed = ClickSpeed::NORMAL;
    bool use24hFormat = true;
    uint8_t brightness = 255;

    void load();      
    void save();      
    void reset();     
};

extern SystemSettings g_settings;

const char* getThemeName(Theme t);
const char* getClickSpeedName(ClickSpeed s);
uint16_t getClickMs(ClickSpeed s);