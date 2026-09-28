#include "ImageScaler.h"
#include <LittleFS.h>
#include "log.h"

struct ImgFile {
    File f;
    bool open(const char* p) { f = LittleFS.open(p, "r"); return (bool)f; }
    void close() { f.close(); }
    bool seek(uint32_t pos) { return f.seek(pos); }
    int read(uint8_t* b, int n) { return (int)f.read(b, n); }
};

static uint16_t rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
static uint32_t rd32(const uint8_t* p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

struct BmpInfo { uint32_t dataOff; int w, h, bpp; bool topDown; };

static bool parseHeader(ImgFile& f, BmpInfo& o) {
    uint8_t hdr[54];
    if (!f.seek(0) || f.read(hdr, 54) != 54 ||
        hdr[0] != 'B' || hdr[1] != 'M') {
        return false;
    }
    o.bpp = rd16(hdr + 28);
    if (o.bpp != 24 && o.bpp != 32) return false;
    o.dataOff = rd32(hdr + 10);
    o.w = (int)rd32(hdr + 18);
    int32_t rawH = (int32_t)rd32(hdr + 22);
    o.topDown = rawH < 0;
    o.h = o.topDown ? -rawH : rawH;
    return o.w > 0 && o.w <= 320 && o.h > 0;
}

bool Img::draw(Adafruit_GFX* d, const char* path, int x, int y, int dstW, int dstH) {
    if (!d) return false;
    ImgFile f;
    if (!f.open(path)) { LOGE(LT_UI, "[Img] open fail: %s", path); return false; }
    BmpInfo bi;
    if (!parseHeader(f, bi)) { LOGE(LT_UI, "[Img] bad header: %s", path); f.close(); return false; }
    if (dstW <= 0) dstW = bi.w;
    if (dstH <= 0) dstH = bi.h;

    int px = (bi.bpp == 32) ? 4 : 3;
    int rowSize = (bi.w * px + 3) & ~3;
    uint8_t row[320 * 4];

    bool useAlpha = false;
    uint16_t keyColor = 0xF81F;                   
    if (bi.bpp == 32) {
        for (int ry = 0; ry < bi.h && !useAlpha; ry++) {
            if (!f.seek(bi.dataOff + (uint32_t)ry * rowSize)) break;
            if (f.read(row, rowSize) != rowSize) break;
            if (ry == 0) {                   
                uint16_t c = (uint16_t)(((row[2] >> 3) << 11) | ((row[1] >> 2) << 5) | (row[0] >> 3));
                keyColor = c;
            }
            for (int rx = 0; rx < bi.w; rx++)
                if (row[rx * 4 + 3] >= 128) { useAlpha = true; break; }
        }
    }
    LOGI(LT_UI, "[Img] ok: %s %dx%d bpp=%d mode=%s", path, bi.w, bi.h, bi.bpp,
           useAlpha ? "alpha" : (bi.bpp == 32 ? "corner" : "magenta"));

    int lastRow = -1;
    for (int dy = 0; dy < dstH; dy++) {
        int sy = dy * bi.h / dstH;
        int fileRow = bi.topDown ? sy : (bi.h - 1 - sy);
        if (fileRow != lastRow) {
            if (!f.seek(bi.dataOff + (uint32_t)fileRow * rowSize)) break;
            if (f.read(row, rowSize) != rowSize) break;
            lastRow = fileRow;
        }
        for (int dx = 0; dx < dstW; dx++) {
            int sx = dx * bi.w / dstW;
            const uint8_t* p = row + sx * px;
            uint16_t c = (uint16_t)(((p[2] >> 3) << 11) | ((p[1] >> 2) << 5) | (p[0] >> 3));
            if (useAlpha) { if (p[3] < 128) continue; }
            else if (c == keyColor) continue;
            d->drawPixel(x + dx, y + dy, c);
        }
    }
    f.close();
    return true;
}