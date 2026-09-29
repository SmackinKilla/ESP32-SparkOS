#include <Adafruit_GFX.h>
#include <ColorPalette.h>
#include <DHT.h>
#include <math.h>
#include "Icons.h"
#include "ImageScaler.h"
#include "NetService.h"
#include "PageManager.h"
#include "WeatherPage.h"
void WeatherPage::onShortClick() {
    
}

void WeatherPage::onLongClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::WEATHER1); 
}

void WeatherPage::onDoubleClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::HOME); 
}

void WeatherPage::OnEnter() {
    WeatherData wx = NetService::weather();
    _stamp = wx.updatedAt;
    int code = wx.valid ? wx.code : 1;
    float temp = wx.valid ? wx.temp : 0.0f;
    int humidity = wx.valid ? wx.humidity : 0;

    DrawFrame("WEATHER", "Hold: Info", "Back: x2");
    //screen(0)->fillRect(1, SCREEN_HEIGHT - 15, SCREEN_WIDTH - 2, 14, COLOR_BG);
    drawBevel(screen(0), 1, SCREEN_HEIGHT - 15, SCREEN_WIDTH - 2, 14, true, false);
/*     screen(0)->setCursor(3, SCREEN_HEIGHT - 12);
    screen(0)->setTextColor(COLOR_TEXT);
    screen(0)->setTextSize(1);
    screen(0)->print("Hold: Info");
    screen(0)->setCursor(SCREEN_WIDTH - 51, SCREEN_HEIGHT - 12);
    screen(0)->print("Back: x2"); */

    drawBevel(screen(0), SCREEN_WIDTH - 44, 17, 43, 12, true, false); 
    screen(0)->setCursor(SCREEN_WIDTH - 33, 19);
    screen(0)->setTextColor(COLOR_TEXT);
    screen(0)->setTextSize(1);
    screen(0)->print("ICON");
    DrawWeatherIcons(SCREEN_WIDTH - 44, 29, 43, 39, code);

    drawBevel(screen(0), SCREEN_WIDTH - 109, 17, 65, 12, true, false); 
    screen(0)->setCursor(SCREEN_WIDTH - 95, 19);
    screen(0)->print("OUTSIDE");
    screen(0)->setCursor(SCREEN_WIDTH - 107, 30);
    screen(0)->printf("T:%.1fC", temp);
    screen(0)->setCursor(SCREEN_WIDTH - 107, 40);
    screen(0)->printf("H:%d%%", humidity);
    screen(0)->setCursor(SCREEN_WIDTH - 107, 50);
    screen(0)->print(getDescriptionFromCode(code));    

    drawBevel(screen(0), 1, 17, 50, 12, true, false);
    screen(0)->setCursor(7, 19);
    screen(0)->print("LOCAL");
    screen(0)->setCursor(2, 30);
    screen(0)->printf("T:%.1fC", _t);
    screen(0)->setCursor(2, 40);
    screen(0)->printf("H:%d%%", _h);

    screen(0)->drawFastVLine(SCREEN_WIDTH - 110, 29, 40, COLOR_WHITE);
    screen(0)->drawFastVLine(SCREEN_WIDTH - 45, 29, 40, COLOR_WHITE);

    drawBevel(screen(0), 1, 69, 40, 12, true, false); // bevel left far column
    drawBevel(screen(0), 40, 69, 40, 12, true, false); // bevel left column
    drawBevel(screen(0), 80, 69, 40, 12, true, false); // bevel right column
    drawBevel(screen(0), 120, 69, 39, 12, true, false); // bevel far right column

    screen(0)->drawFastVLine(40, 81, 32, COLOR_WHITE);
    screen(0)->drawFastVLine(80, 81, 32, COLOR_WHITE);
    screen(0)->drawFastVLine(120, 81, 32, COLOR_WHITE);

    screen(0)->setCursor(5, 71);
    screen(0)->print("00-06");
    screen(0)->setCursor(5, 90);
    screen(0)->setTextSize(2);
    screen(0)->printf("%.0fC", wx.valid ? wx.forecastTemp[0] : 0.0f);
    screen(0)->setTextSize(1);

    screen(0)->setCursor(45, 71);
    screen(0)->print("06-12");
    screen(0)->setCursor(45, 90);
    screen(0)->setTextSize(2);
    screen(0)->printf("%.0fC", wx.valid ? wx.forecastTemp[1] : 0.0f);
    screen(0)->setTextSize(1);
    screen(0)->setCursor(45, 95);

    screen(0)->setCursor(85, 71);
    screen(0)->print("12-18");
    screen(0)->setCursor(85, 90);
    screen(0)->setTextSize(2);
    screen(0)->printf("%.0fC", wx.valid ? wx.forecastTemp[2] : 0.0f);
    screen(0)->setTextSize(1);
    screen(0)->setCursor(85, 95);

    screen(0)->setCursor(125, 71);
    screen(0)->print("18-24");
    screen(0)->setCursor(125, 90);
    screen(0)->setTextSize(2);
    screen(0)->printf("%.0fC", wx.valid ? wx.forecastTemp[3] : 0.0f);
    screen(0)->setTextSize(1);
    screen(0)->setCursor(125, 95);
}

void WeatherPage::Update(uint32_t deltaTimeMs) {
    if (!_dhtStarted) { dht.begin(); _dhtStarted = true; } 

    uint32_t now = millis();
    if (now - _lastDhtRead >= 2000) {
        _lastDhtRead = now;
        float nh = dht.readHumidity();      
        float nt = dht.readTemperature();
        if (!isnan(nh) && !isnan(nt)) {     
            _h = nh;                        
            _t = nt;
            DrawLocal();                   
        }
    }
    if (NetService::weather().updatedAt != _stamp) {
        OnEnter();
    }
}

void WeatherPage::DrawLocal() {
    screen(0)->fillRect(2, 29, 44, 20, COLOR_BG);
    screen(0)->setTextColor(COLOR_TEXT);
    screen(0)->setTextSize(1);
    screen(0)->setCursor(2, 30);
    screen(0)->printf("T:%.1fC", _t);
    screen(0)->setCursor(2, 40);
    screen(0)->printf("H:%d%%", (int)_h);
}

void WeatherPage::OnExit() {
    
}

