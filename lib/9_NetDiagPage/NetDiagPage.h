#pragma once
#include "Page.h"
#include "PageManager.h"

class NetDiagPage : public Page {
public:
    NetDiagPage(DisplayManager* displays, PageManager* pm)
        : Page(displays, pm) {}
    void onShortClick() override;
    void onLongClick() override;
    void OnEnter() override;
    void Update(uint32_t deltaTimeMs) override;
private:
    void Draw();
    int _idx = 0;
};