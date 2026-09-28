#pragma once
#include <stdint.h>

class Adafruit_GFX;

class IDisplay {
    public:
        virtual ~IDisplay() = default;
        virtual bool begin() = 0;
        virtual void fillScreen(uint16_t color) = 0;
        virtual void flush() {}
        virtual int16_t width() const = 0;
        virtual int16_t height() const = 0;
        virtual Adafruit_GFX* gfx() = 0;
        virtual const char* name() const = 0;
        virtual void setBrightness(uint8_t value) {}
};