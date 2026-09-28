#pragma once
#include <stdint.h>
#include <string.h>
#include "IDisplay.h"
#include "DisplayTypes.h"

IDisplay* createDisplay(const DisplayConfig& cfg);

class DisplayManager {
public:
    static constexpr uint8_t MAX_DISPLAYS = 4;

    bool begin(const DisplayConfig* configs, uint8_t count) {
        if (configs == nullptr) return false;
        if (count > MAX_DISPLAYS) count = MAX_DISPLAYS;

        _count = count;
        for (uint8_t i = 0; i < _count; i++) {
            _drivers[i] = createDisplay(configs[i]);
            if (_drivers[i] == nullptr) return false;
            if (!_drivers[i]->begin()) return false;
        }
        return true;
    }

    IDisplay* get(int index) {
        if (index >= _count) return nullptr;
        return _drivers[index];
    }

    Adafruit_GFX* gfx(int index) {
        IDisplay* d = get(index);
        return d ? d->gfx() : nullptr;
    }

    IDisplay* get(const char* name) {
        if (name == nullptr) return nullptr;
        for (uint8_t i = 0; i < _count; i++) {
            if (_drivers[i] == nullptr) continue;
            const char* dname = _drivers[i]->name();
            if (dname != nullptr && strcmp(dname, name) == 0) {
                return _drivers[i];
            }
        }
        return nullptr;
    }

    Adafruit_GFX* gfx(const char* name) {
        IDisplay* d = get(name);
        return d ? d->gfx() : nullptr;
    }

    uint8_t count() const { return _count; }

    void flushAll() {
        for (uint8_t i = 0; i < _count; i++) {
            if (_drivers[i]) _drivers[i]->flush();
        }
    }

    void setBrightnessAll(uint8_t value) {
    for (uint8_t i = 0; i < _count; i++) {
        if (_drivers[i]) _drivers[i]->setBrightness(value);
    }
}

private:
    IDisplay* _drivers[MAX_DISPLAYS] = {};
    uint8_t _count = 0;
};