#pragma once
#include "Page.h"
#include <array>
#include <vector>
#include <cstdint>
#include <stddef.h>

enum class PageIndex : uint8_t {
    WEATHER = 0,
    TASKS,
    TIMEZONES,
    GAMES,
    SETTINGS,
    HOME,
    SCREENSAVER,
    WEATHER1,
    NETDIAG,
    SYSINFO,
    COUNT 
};

struct TitleInfo {
    const char* title;
    bool hasCounter;
    bool showInMenu;
    
    TitleInfo(const char* t = "Unknown", bool counter = false, bool show = true) 
        : title(t), hasCounter(counter), showInMenu(show) {}
};

class PageManager {
public:
    PageManager();
    void registerPage(PageIndex index, Page* page, const TitleInfo& info);
    Page* getCurrent();
    const TitleInfo& getTitleByIndex(PageIndex index) const;
    const std::vector<PageIndex>& getMenuPages() const;
    bool SwitchToIndex(PageIndex index);
    void next();
    void prev();
private:
    static constexpr size_t MAX_PAGES = static_cast<size_t>(PageIndex::COUNT);
    static constexpr size_t MAX_MENU_PAGES = 10; 
    
    std::array<Page*, MAX_PAGES> _pages;
    std::array<TitleInfo, MAX_PAGES> _titles;
    std::vector<PageIndex> _menuPages;  
    PageIndex _currentIndex = PageIndex::COUNT;
};
