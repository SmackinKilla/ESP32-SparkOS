#include "SystemSettings.h"
#include <Preferences.h>
#include <Arduino.h>
#include "log.h"
SystemSettings g_settings;

const char* getThemeName(Theme t) {
    switch (t) {
        case Theme::BLUE:   return "Blue";
        case Theme::RED:    return "Red";
        case Theme::PURPLE: return "Purple";
        case Theme::GREEN:  return "Green";
        case Theme::DARK:   return "Dark";
        case Theme::Win95:  return "Win95";
        default:            return "Unknown";
    }
}

const char* getClickSpeedName(ClickSpeed s) {
    switch (s) {
        case ClickSpeed::FAST:   return "Fast (200ms)";
        case ClickSpeed::NORMAL: return "Normal (300ms)";
        case ClickSpeed::SLOW:   return "Slow (450ms)";
        default:                 return "Unknown";
    }
}

uint16_t getClickMs(ClickSpeed s) {
    switch (s) {
        case ClickSpeed::FAST:   return 200;
        case ClickSpeed::NORMAL: return 300;
        case ClickSpeed::SLOW:   return 450;
        default:                 return 300;
    }
}

void SystemSettings::load() {
    Preferences prefs;
    prefs.begin("app", true); 
    uint8_t t = prefs.getUChar("theme", (uint8_t)Theme::BLUE);
    uint8_t cs = prefs.getUChar("clkSpd", (uint8_t)ClickSpeed::NORMAL);

    brightness = prefs.getUChar("bright", 255);
    use24hFormat = prefs.getBool("24h", true);
    prefs.end();
    
    if (t >= (uint8_t)Theme::COUNT) t = 0;
    if (cs >= (uint8_t)ClickSpeed::COUNT) cs = 1;
    
    theme = (Theme)t;
    clickSpeed = (ClickSpeed)cs;
    
    LOGI(LT_FS,"[Settings] Loaded: Theme = %s, Click Speed = %s, Brightness = %d, 24h = %d",
                  getThemeName(theme), getClickSpeedName(clickSpeed),
                  brightness, use24hFormat);
}

void SystemSettings::save() {
    Preferences prefs;
    prefs.begin("app", false);  
    prefs.putUChar("theme", (uint8_t)theme);
    prefs.putUChar("clkSpd", (uint8_t)clickSpeed);
    prefs.putUChar("bright", brightness);
    prefs.putBool("24h", use24hFormat);
    prefs.end();
    LOGI(LT_FS,"[Settings] Saved to NVS");
}

void SystemSettings::reset() {
    Preferences prefs;
    prefs.begin("app", false);
    prefs.clear();
    prefs.end();
    *this = SystemSettings();
    save();
    LOGW(LT_FS,"[Settings] Reset to defaults");
}