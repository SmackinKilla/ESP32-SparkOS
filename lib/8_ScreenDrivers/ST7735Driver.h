#pragma once

#include "IDisplay.h"
#include "DisplayTypes.h"
#include <Arduino.h>
#include <Adafruit_ST7735.h>

class St7735Display final : public IDisplay {
public:
    explicit St7735Display(const DisplayConfig& cfg)
        : _cfg(cfg),
          _tft(cfg.cs, cfg.dc, cfg.rst) {}

    bool begin() override {
    _tft.initR(INITR_BLACKTAB);
    _tft.setRotation(_cfg.rotation);
    if (_cfg.bl >= 0) {
        analogWrite(_cfg.bl, 255);
    }
    return true;
}

    void setBrightness(uint8_t value) override {
    if (_cfg.bl >= 0) {
        analogWrite(_cfg.bl, value);
    }
}

    void fillScreen(uint16_t color) override {
        _tft.fillScreen(color);
    }

    int16_t width() const override {
        return _tft.width();
    }

    int16_t height() const override {
        return _tft.height();
    }

    Adafruit_GFX* gfx() override {
        return &_tft;
    }

    const char* name() const override {
    return _cfg.name; 
    } 

private:
    DisplayConfig _cfg;
    Adafruit_ST7735 _tft;
};