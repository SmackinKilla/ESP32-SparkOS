#include "Page.h"
#include <Adafruit_GFX.h>
#include <ColorPalette.h>
#include <string.h>
#include "DisplayManager.h"
#include "Icons.h"
#include "NetService.h"
#include "ImageScaler.h"

void Page::DrawWeatherIcons(int x, int y, int w, int h, int weatherCode) {
    Img::draw(screen(0), iconPath(getIconFromCode(weatherCode)), x, y, w, h);
}

void Page::DrawClock() {
    char buf[8];
    NetService::formatClock(buf, sizeof(buf));
    strncpy(_clockText, buf, sizeof(_clockText));
    _clockText[sizeof(_clockText) - 1] = '\0';
    screen(0)->fillRect(SCREEN_WIDTH - 39, 3, 31, 12, COLOR_ACCENT);
    drawBevel(screen(0),SCREEN_WIDTH - 58, 3, 55, 13, false, false);
    screen(0)->setCursor(SCREEN_WIDTH - 38, 6);
    screen(0)->setTextSize(1);
    screen(0)->setTextColor(COLOR_WHITE);
    screen(0)->print(_clockText);
}

void Page::TickClock() {
    char buf[8];
    NetService::formatClock(buf, sizeof(buf));
    if (strcmp(buf, _clockText) != 0) {
        DrawClock();
    }
}

static uint16_t mix565(uint16_t a, uint16_t b, uint8_t t) {
    uint8_t ar = (a >> 11) & 0x1F, ag = (a >> 5) & 0x3F, ab = a & 0x1F;
    uint8_t br = (b >> 11) & 0x1F, bg6 = (b >> 5) & 0x3F, bb = b & 0x1F;
    uint8_t r  = ar + ((int)(br - ar) * t) / 255;
    uint8_t g  = ag + ((int)(bg6 - ag) * t) / 255;
    uint8_t bl = ab + ((int)(bb - ab) * t) / 255;
    return (uint16_t)((r << 11) | (g << 5) | bl);
}

void Page::drawBevel(Adafruit_GFX* d, int x, int y, int w, int h, bool raised, bool thick) {
    uint16_t hi2 = COLOR_WHITE;
    uint16_t hi1 = mix565(COLOR_BG, COLOR_WHITE, 140);
    uint16_t lo1 = mix565(COLOR_BG, COLOR_BLACK, 140);
    uint16_t lo2 = COLOR_BLACK;

    uint16_t oTL, oBR, iTL, iBR;
    if (raised) { oTL = hi2; iTL = hi1; iBR = lo1; oBR = lo2; }
    else        { oTL = lo1; iTL = lo2; iBR = hi1; oBR = hi2; }

    d->drawFastHLine(x, y, w, oTL);
    d->drawFastVLine(x, y, h, oTL);
    d->drawFastHLine(x, y + h - 1, w, oBR);
    d->drawFastVLine(x + w - 1, y, h, oBR);

    if (thick) {
        d->drawFastHLine(x + 1, y + 1, w - 2, iTL);
        d->drawFastVLine(x + 1, y + 1, h - 2, iTL);
        d->drawFastHLine(x + 1, y + h - 2, w - 2, iBR);
        d->drawFastVLine(x + w - 2, y + 1, h - 2, iBR);
    }
}

static int calcScrollOffset(int selectedIdx, int itemCount, int maxVisible) {
    if (itemCount <= maxVisible) return 0;

    int scrollOffset = 0;
    if (selectedIdx >= maxVisible) {
        scrollOffset = selectedIdx - maxVisible + 1;
    }
    int maxScroll = itemCount - maxVisible;
    if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    return scrollOffset;
}

void Page::DrawFrame(const char* title, const char* LHint, const char* RHint) {
    screen(0)->fillScreen(COLOR_BG);
    screen(0)->fillRect(1, 2, SCREEN_WIDTH - 2, 14, COLOR_ACCENT);
    screen(0)->setCursor(5, 6);    
    screen(0)->setTextSize(1);
    screen(0)->setTextColor(COLOR_WHITE);
    screen(0)->print(title);
    drawNetIcon(SCREEN_WIDTH - 57, 4);
    DrawClock();
    drawBevel(screen(0), 1, 2, SCREEN_WIDTH - 2, 15, true, false); // bevel topbar
    drawBevel(screen(0), 1, SCREEN_HEIGHT - 15, SCREEN_WIDTH - 1, 14, true, false); // bevel bottombar
    screen(0)->setTextColor(COLOR_TEXT);
    int LHintLeght = strlen(LHint) * 6;
    int RHintLeght = strlen(RHint) * 6;
    screen(0)->setCursor(3, SCREEN_HEIGHT - 12);
    screen(0)->print(LHint);
    screen(0)->setCursor(SCREEN_WIDTH - RHintLeght - 2, SCREEN_HEIGHT - 12);
    screen(0)->print(RHint);
}

void Page::DrawMenu(const char* items[], int count, int currentIndex) {
    int areaTop = 16;
    int areaBottom = SCREEN_HEIGHT - 14;
    int charStart = areaTop + 4;
    int charOffset = 11;

    drawBevel(screen(0), 1, areaTop, SCREEN_WIDTH - 2, areaBottom - areaTop, false, true); // bevel menu area

    int maxVisible = (areaBottom - charStart - 3) / charOffset;
    int scrollOffset = calcScrollOffset(currentIndex, count, maxVisible);
    int visibleCount = (count < maxVisible) ? count : maxVisible;

    for (int i = 0; i < visibleCount; i++) {
        int actualIdx = scrollOffset + i;
        int y = charStart + charOffset * i;

        if (actualIdx == currentIndex) {
            screen(0)->fillRect(3, y - 2, SCREEN_WIDTH - 6, charOffset, COLOR_ACCENT);
            screen(0)->setTextColor(COLOR_WHITE);
        } else {
            screen(0)->fillRect(3, y - 2, SCREEN_WIDTH - 6, charOffset, COLOR_BG);
            screen(0)->setTextColor(COLOR_TEXT);
        }
        screen(0)->setCursor(4, y);
        screen(0)->setTextSize(1);
        screen(0)->print(items[actualIdx]);
    }

    if (scrollOffset > 0) {
        screen(0)->setTextColor((scrollOffset == currentIndex) ? COLOR_BG : COLOR_ACCENT);
        screen(0)->setCursor(SCREEN_WIDTH - 8, charStart);
        screen(0)->print("^");
    }
    if (scrollOffset + visibleCount < count) {
        int lastVisible = scrollOffset + visibleCount - 1;
        screen(0)->setTextColor((lastVisible == currentIndex) ? COLOR_BG : COLOR_ACCENT);
        screen(0)->setCursor(SCREEN_WIDTH - 8, charStart + (visibleCount - 1) * charOffset);
        screen(0)->print("v");
    }
}

void Page::drawModalWindow(const char* title, const char* items[], int itemCount, int selectedIdx, int x, int y, int w, int h, int maxVisibleItems) {
    screen(0)->fillRect(x + 6, y + 6, w, h, COLOR_BLACK);
    screen(0)->fillRect(x, y, w, h, COLOR_BG);
    screen(0)->drawRect(x, y, w, h, COLOR_WHITE);
    int contentY = y + 2;
    if (title != nullptr) {
        screen(0)->setTextColor(COLOR_TEXT);
        screen(0)->setTextSize(1);
        screen(0)->setCursor(x + 4, contentY);
        screen(0)->print(title);
        contentY += 12; 
        screen(0)->drawFastHLine(x + 2, contentY - 2, w - 4, COLOR_TEXT);
    }

    int itemHeight = 10;
    int maxVisible = (y + h - contentY) / itemHeight;
    int scrollOffset = calcScrollOffset(selectedIdx, itemCount, maxVisible);
    int visibleCount = (itemCount < maxVisible) ? itemCount : maxVisible;
    for (int i = 0; i < visibleCount; i++) {
        int actualIdx = scrollOffset + i; 
        int iy = contentY + i * itemHeight;
        if (actualIdx == selectedIdx) {
            screen(0)->fillRect(x + 2, iy - 1, w - 4, itemHeight, COLOR_ACCENT);
            screen(0)->setTextColor(COLOR_WHITE);
        } else {
            screen(0)->setTextColor(COLOR_TEXT);
        }
        screen(0)->setCursor(x + 6, iy);
        screen(0)->setTextSize(1);
        screen(0)->print(items[actualIdx]); 
    }


    if (scrollOffset > 0) {
        screen(0)->setTextColor(COLOR_WHITE);
        screen(0)->setCursor(x + w - 10, contentY);
        screen(0)->print("^");
    }
    if (scrollOffset + visibleCount < itemCount) {
        screen(0)->setTextColor(COLOR_WHITE);
        screen(0)->setCursor(x + w - 10, contentY + (visibleCount - 1) * itemHeight);
        screen(0)->print("v");
    }
}

void Page::drawNetIcon(int x, int y) {
    NetState st = NetService::state();
    const char* path = nullptr;
    
    switch (st) {
        case NetState::OFF:
            path = "/img/ConnectFail.bmp";
            break;
        case NetState::CONNECTING:
            if ((millis() / 500) % 2 == 0) return;
            path = "/img/Connect.bmp";
            break;
        case NetState::STA:
            path = "/img/Connect.bmp";
            break;
        case NetState::AP:
            path = "/img/APMode.bmp";
            break;
    }
    
    if (path) {
        Img::draw(screen(0), path, x, y, 12, 12);
    }
}
