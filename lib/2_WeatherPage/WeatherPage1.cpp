#include "WeatherPage1.h"
#include <Adafruit_GFX.h>
#include "PageManager.h"
#include <ColorPalette.h>
#include "Icons.h"
#include "WeatherPage.h"
#include "ImageScaler.h"
void WeatherPage1::onShortClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::WEATHER);
}
void WeatherPage1::onLongClick() {

}

void WeatherPage1::onDoubleClick() {

}

void WeatherPage1::OnEnter() {
    DrawFrame("INFO");
    screen(0)->fillRect(1, SCREEN_HEIGHT - 15, SCREEN_WIDTH - 2, 14, COLOR_BG);
    drawBevel(screen(0), 1, SCREEN_HEIGHT - 15, SCREEN_WIDTH - 2, 14, true, false);
    drawBevel(screen(0), 1, 16, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 14 - 16, false, true);
    screen(0)->setCursor(3, SCREEN_HEIGHT - 12);
    screen(0)->setTextColor(COLOR_TEXT);
    screen(0)->setTextSize(1);
    screen(0)->print("Back: x1");
    Img::draw(screen(0), "/img/Connect.bmp", 10, 20, 10, 10);
    Img::draw(screen(0), "/img/ConnectFail.bmp", 58, 20, 10, 10);
    Img::draw(screen(0), "/img/APMode.bmp", 98, 20, 10, 10);
}

void WeatherPage1::Update(uint32_t deltaTimeMs) {

}

void WeatherPage1::OnExit() {
}

