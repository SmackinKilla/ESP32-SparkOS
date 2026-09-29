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
    DrawFrame("INFO", " ", "Back: x2");
    screen(0)->setCursor(2, 20);
    screen(0)->setTextSize(1);
    screen(0)->setTextColor(COLOR_TEXT);
    screen(0)->printf("Weather API: %s", "Open-Meteo");
    screen(0)->setCursor(SCREEN_WIDTH - 51, SCREEN_HEIGHT - 12);

}

void WeatherPage1::Update(uint32_t deltaTimeMs) {

}

void WeatherPage1::OnExit() {
}

