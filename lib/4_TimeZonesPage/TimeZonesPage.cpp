#include "TimeZonesPage.h"
#include <Adafruit_GFX.h>
#include "PageManager.h"

void  TimeZonesPage::onShortClick() {
    int menuCount = sizeof(_timeZonesItems) / sizeof(_timeZonesItems[0]);
    if (menuCount <= 0) return;
    
    _currentIndex++;
    if (_currentIndex >= menuCount) _currentIndex = 0;
    _previousIndex = -1;
}

void TimeZonesPage::onLongClick() {
    
}

void TimeZonesPage::onDoubleClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::HOME);  
}

void TimeZonesPage::OnEnter() {
    DrawFrame("TIME ZONES");
    Page::DrawMenu(_timeZonesItems, TimeZonesCount, _currentIndex);
}

void TimeZonesPage::Update(uint32_t deltaTimeMs) {
    if (_currentIndex != _previousIndex) {
    Page::DrawMenu(_timeZonesItems, TimeZonesCount, _currentIndex);
    _previousIndex = _currentIndex;
    }
}

void TimeZonesPage::OnExit() {
    
}

