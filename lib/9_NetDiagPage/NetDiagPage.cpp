#include "NetDiagPage.h"
#include "NetService.h"
#include "PageManager.h"
#include <ColorPalette.h>

void NetDiagPage::OnEnter() {
    _idx = 0;
    Draw();
}

void NetDiagPage::onShortClick() {
    _idx = (_idx + 1) % 3;
    Draw();
}

void NetDiagPage::onLongClick() {
    if (_idx == 0) {
        DrawFrame("NETWORK");
        screen(0)->setCursor(2, 20);
        screen(0)->setTextColor(COLOR_TEXT);
        screen(0)->print("Connecting...");
        _displays->flushAll();              
        if (NetService::retrySta()) _pm->SwitchToIndex(PageIndex::HOME);
        else Draw();
    } else if (_idx == 1) {
        NetService::startAp();
        _pm->SwitchToIndex(PageIndex::HOME);
    } else { NetService::goOffline(); 
        _pm->SwitchToIndex(PageIndex::HOME);
    }
}

void NetDiagPage::Draw() {
    DrawFrame("NETWORK");
    static const char* items[3] = {"1. Retry", "2. AP mode", "3. Offline"};
    Page::DrawMenu(items, 3, _idx);
}

void NetDiagPage::Update(uint32_t deltaTimeMs) {
}