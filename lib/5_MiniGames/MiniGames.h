#pragma once
#include "Page.h"
#include "PageManager.h"

class MiniGames : public Page {
public:
    MiniGames(DisplayManager* displays, PageManager* pm) 
        : Page(displays, pm) {}

    void onShortClick() override;
    void onLongClick() override;
    void onDoubleClick() override;
    void OnEnter() override;
    void Update(uint32_t deltaTimeMs) override;
    void OnExit() override;
};