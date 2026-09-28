#pragma once

#include "IDisplay.h"
#include "DisplayTypes.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

class Ssd1306Display final : public IDisplay {
public:
    explicit Ssd1306Display(const DisplayConfig& cfg)
        : _cfg(cfg),
          _oled(cfg.width, cfg.height, &Wire, cfg.rst) {
    }

    bool begin() override {
        if (_cfg.sda >= 0 && _cfg.scl >= 0) {
            Wire.begin(_cfg.sda, _cfg.scl);
        } else {
            Wire.begin();
        }

        if (!_oled.begin(SSD1306_SWITCHCAPVCC, _cfg.i2cAddr)) {
            return false;
        }

        _oled.setRotation(_cfg.rotation);
        _oled.clearDisplay();
        _oled.display();

        return true;
    }

    void fillScreen(uint16_t color) override {
        _oled.fillScreen(color ? 1 : 0);
    }

    void flush() override {
        _oled.display();
    }

    int16_t width() const override {
        return _oled.width();
    }

    int16_t height() const override {
        return _oled.height();
    }

    Adafruit_GFX* gfx() override {
        return &_oled;
    }

    const char* name() const override {
    return _cfg.name; 
    } 

private:
    DisplayConfig _cfg;
    Adafruit_SSD1306 _oled;
};