#include "SettingsPage.h"
#include <ColorPalette.h>
#include <Adafruit_GFX.h>
#include "PageManager.h"
#include <OneButton.h>
extern OneButton button;  
void SettingsPage::drawWindow() {
    const char** items = _subMenus[_currentIndex];
    int count = _subMenuCounts[_currentIndex];
    if (items == nullptr || count == 0) return;
    drawModalWindow(_settingsItems[_currentIndex], items, count, _WindowIndex, 33, 30, 100, 63);
}

void SettingsPage::onShortClick() {
    if (_IsWindowOpen) {
        _WindowIndex++;
        if (_WindowIndex >= _subMenuCounts[_currentIndex]) _WindowIndex = 0;
        drawWindow();
    } else {
        _currentIndex++;
        if (_currentIndex >= SETTINGS_COUNT) _currentIndex = 0;
        _previousIndex = -1;
    }
}

void SettingsPage::onLongClick() {
    if (_IsWindowOpen) {
        switch (_currentIndex) {
            case 0: g_settings.theme = static_cast<Theme>(_WindowIndex); break;
            case 1: g_settings.clickSpeed = static_cast<ClickSpeed>(_WindowIndex); break;
            case 2: {
                    static const uint8_t levels[] = {25, 64, 128, 191, 255};
                    g_settings.brightness = levels[_WindowIndex];
                    break;
            }
        }
        g_settings.save();
        applyTheme();
        _displays->setBrightnessAll(g_settings.brightness);
        _IsWindowOpen = false;
        OnEnter();
    } else {
        if (_subMenus[_currentIndex] != nullptr) {
            _IsWindowOpen = true;
            _WindowIndex = 0;
            drawWindow();
        } else if (_currentIndex == 3) { 
            _pm->SwitchToIndex(PageIndex::NETDIAG);
        } else if (_currentIndex == 4) {
            _pm->SwitchToIndex(PageIndex::SYSINFO);
        } else if (_currentIndex == 5) {
            g_settings.reset();
            button.setClickMs(getClickMs(g_settings.clickSpeed));
            applyTheme();
            OnEnter();
        }
    }
}

void SettingsPage::onDoubleClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::HOME);
}

void SettingsPage::OnEnter() {
    DrawFrame("SETTINGS");
    Page::DrawMenu(_settingsItems, SETTINGS_COUNT, _currentIndex);
}

void SettingsPage::Update(uint32_t deltaTimeMs) {
    if (_currentIndex != _previousIndex) {
        Page::DrawMenu(_settingsItems, SETTINGS_COUNT, _currentIndex);
        _previousIndex = _currentIndex;
    }
}

void SettingsPage::OnExit() {
    screen(0)->fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_BG);
}
