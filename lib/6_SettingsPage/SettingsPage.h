#pragma once
#include "Page.h"
#include "PageManager.h"
#include <string.h>

class SettingsPage : public Page {
public:
    SettingsPage(DisplayManager* displays, PageManager* pm) 
        : Page(displays, pm), _currentIndex(0), _previousIndex(-1) {}

    void onShortClick() override;
    void onLongClick() override;
    void onDoubleClick() override;
    void OnEnter() override;
    void Update(uint32_t deltaTimeMs) override;
    void OnExit() override;
private:
    void drawWindow();
    static constexpr int SETTINGS_COUNT = 6;
    const char* _settingsItems[6] = {
        "Theme",
        "Click Speed",
        "Brightness",
        "Network",
        "Info",
        "Reset Settings"
    };
    const char* _themeOptions[6] = {"Blue", "Red", "Purple", "Green", "Dark", "Win95"};
    const char* _speedOptions[3] = {"Fast", "Normal", "Slow"};
    const char* _BrOptions[5] = {"25%", "50%", "75%", "100%", "125%"};
    const char** _subMenus[SETTINGS_COUNT] = {_themeOptions, _speedOptions, _BrOptions, nullptr};
    const int _subMenuCounts[SETTINGS_COUNT] = {6, 3, 5, 0};
    int _currentIndex;
    int _previousIndex;
    bool _IsWindowOpen = false;
    int _WindowIndex = 0;
};
