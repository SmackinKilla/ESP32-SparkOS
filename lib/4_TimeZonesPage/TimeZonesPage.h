#pragma once
#include "Page.h"
#include "PageManager.h"

class TimeZonesPage : public Page {
public:
    TimeZonesPage(DisplayManager* displays, PageManager* pm) 
        : Page(displays, pm) {}

    void onShortClick() override;
    void onLongClick() override;
    void onDoubleClick() override;
    void OnEnter() override;
    void Update(uint32_t deltaTimeMs) override;
    void OnExit() override;
private:
    static constexpr int TimeZonesCount = 12;
    const char* _timeZonesItems[12] = {
        "Kaliningrad",
        "Moscow",
        "Samara",
        "Yeakaterinburg",
        "Omsk",
        "Krasnoyarsk",
        "Irkutsk",
        "Yakutsk",
        "Vladivostok",
        "Magadan",
        "Kamchatka",
        "UTC"
    };
    const char** _subMenus[TimeZonesCount] = {nullptr};
    const int _subMenuCounts[TimeZonesCount] = {0};
    int _currentIndex;
    int _previousIndex;
};