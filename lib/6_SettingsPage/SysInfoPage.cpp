#include <Adafruit_GFX.h>
#include "SysInfoPage.h"
#include "PageManager.h"
#include "ColorPalette.h"
#include "ImageScaler.h"
#include "NetService.h"      
#include "BoardConfig.h"     


void SysInfo::onShortClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::SETTINGS);  
}

void SysInfo::onLongClick() {

}

void SysInfo::onDoubleClick() {
    
}

void SysInfo::OnEnter() {
    DrawFrame("INFO");
    drawBevel(screen(0), 0, 18, SCREEN_WIDTH, SCREEN_HEIGHT - 18, false, true);
    char ip[24];
    NetService::formatIp(ip, sizeof(ip));
    screen(0)->setCursor(3, 23);
    screen(0)->printf("IP: %s", ip);
    screen(0)->setCursor(3, 33);
    screen(0)->printf("API: %s", "Open-Meteo");
    screen(0)->setCursor(3, 43);
    screen(0)->printf("Version: 0.1");
    screen(0)->setCursor(5, 67);
    screen(0)->setTextSize(1);
    screen(0)->setTextColor(COLOR_TEXT);
    screen(0)->printf("Powered by: ");
    drawBevel(screen(0), 4, 76, 152, 37, true, false);
    Img::draw(screen(0), "/img/logo_160x38.bmp", 4, 77, 150, 36);
}

void SysInfo::Update(uint32_t deltaTimeMs) {

}

void SysInfo::OnExit() {

}

