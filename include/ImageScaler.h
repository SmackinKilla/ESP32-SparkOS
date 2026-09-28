#pragma once
#include <Adafruit_GFX.h>
#include <stdint.h>

namespace Img {
    bool draw(Adafruit_GFX* d, const char* path, int x, int y,
              int dstW = 0, int dstH = 0);
}