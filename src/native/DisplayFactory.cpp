#include "DisplayManager.h"
#include "EmuDisplay.h"

IDisplay* createDisplay(const DisplayConfig& cfg) {
    return new EmuDisplay(cfg);
}