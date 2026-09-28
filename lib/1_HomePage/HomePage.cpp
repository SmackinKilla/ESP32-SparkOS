// HomePage.cpp
#include "HomePage.h"
#include "PageManager.h"
#include <ColorPalette.h>
#include <stdio.h>
#include <Adafruit_GFX.h>
#include <string.h>
#include "log.h"

void HomePage::DrawMenu() {

    const auto& menuPages = _pm->getMenuPages();
    int menuCount = menuPages.size();
    

    if (menuCount <= 0 || menuCount > MAX_MENU_ITEMS) return;
    

    const char* items[MAX_MENU_ITEMS];
    char buffers[MAX_MENU_ITEMS][32];
    memset(buffers, 0, sizeof(buffers));
    

    for (int i = 0; i < menuCount; i++) {
        PageIndex idx = menuPages[i];
        const TitleInfo& info = _pm->getTitleByIndex(idx);
        

        if (info.hasCounter) {
            snprintf(buffers[i], sizeof(buffers[i]), "%d. %.12s(0)", i + 1, info.title);
        } else {
            snprintf(buffers[i], sizeof(buffers[i]), "%d. %.14s", i + 1, info.title);
        }
        items[i] = buffers[i];
    }
    
    Page::DrawMenu(items, menuCount, _currentIndex);
}

void HomePage::onShortClick() {
    int menuCount = _pm->getMenuPages().size();
    LOGI(LT_HW, "Short Click Detected");
    if (menuCount <= 0) return;
    
    _currentIndex++;
    if (_currentIndex >= menuCount) _currentIndex = 0;
    _previousIndex = -1;
}

void HomePage::onLongClick() {
    LOGI(LT_HW, "Long Click Detected");
    if (!_pm) return;
    
    const auto& menuPages = _pm->getMenuPages();
    if (_currentIndex >= 0 && _currentIndex < static_cast<int>(menuPages.size())) {
        _pm->SwitchToIndex(menuPages[_currentIndex]);
    }
}

void HomePage::onDoubleClick() {

}

void HomePage::OnEnter() {
    DrawFrame("HOME");
    DrawMenu();
    screen(1)->setCursor(3, 10);
    screen(1)->setTextColor(1);
    screen(1)->setTextSize(1);
    screen(1)->print("HOME PAGE");
}

void HomePage::Update(uint32_t deltaTimeMs) {
    if (_currentIndex != _previousIndex) {
        DrawMenu();
        _previousIndex = _currentIndex;
    }
}

void HomePage::OnExit() {
    screen(0)->fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BG);
}
