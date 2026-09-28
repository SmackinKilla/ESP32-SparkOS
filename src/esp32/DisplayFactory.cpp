#include "DisplayManager.h"
#include "ST7735Driver.h"
#include "SSD1306Driver.h"

IDisplay* createDisplay(const DisplayConfig& cfg) {
    switch (cfg.type) {
        case DisplayType::ST7735: return new St7735Display(cfg);
        case DisplayType::SSD1306: return new Ssd1306Display(cfg);
        default: return nullptr;
    }
}