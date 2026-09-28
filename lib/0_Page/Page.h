#pragma once
#include <stdint.h>
#include "DisplayManager.h"
#include <Adafruit_GFX.h>
class Adafruit_GFX;
class PageManager;
class Page {
public:
    Page(DisplayManager* displays, PageManager* pm = nullptr) 
        : _displays(displays), _pm(pm), SCREEN_WIDTH(_displays->gfx(0)->width()), SCREEN_HEIGHT(_displays->gfx(0)->height()) {}
    virtual ~Page() = default;
    virtual void onShortClick() {}
    virtual void onLongClick() {}
    virtual void OnEnter() {}
    virtual void Update(uint32_t deltaTimeMs) {}
    virtual void OnExit() {}
    virtual void onDoubleClick() {}

protected:
    void DrawWeatherIcons(int x, int y, int w, int h, int weatherCode);
    void DrawFrame(const char* title);
    void DrawMenu(const char* items[], int count, int currentIndex);
    void drawModalWindow( const char* title, const char* items[], int itemCount, int selectedIdx, int x, int y, int w, int h, int maxVisibleItems = 4);
    void drawBevel(Adafruit_GFX* d, int x, int y, int w, int h, bool raised, bool thick);
    void drawNetIcon(int x, int base);
    Adafruit_GFX* screen(int index) {
    return _displays ? _displays->gfx((int)index) : nullptr;
    }
    Adafruit_GFX* screen(const char* name) {
        return _displays ? _displays->gfx(name) : nullptr;
    }
    DisplayManager* _displays;
    PageManager* _pm;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
};
