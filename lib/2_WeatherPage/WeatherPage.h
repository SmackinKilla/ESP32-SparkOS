#pragma once
#include <DHT.h>

#include "Page.h"
#include "PageManager.h"

class WeatherPage : public Page {
public:
    WeatherPage(DisplayManager* displays, PageManager* pm) 
        : Page(displays, pm), dht(14, DHT22) {}

    void onShortClick() override;
    void onLongClick() override;
    void onDoubleClick() override;
    void OnEnter() override;
    void Update(uint32_t deltaTimeMs) override;
    void OnExit() override;

private:
    void DrawLocal();
    DHT dht;
    bool _dhtStarted = false;
    uint32_t _lastDhtRead = 0;
    uint32_t _stamp = 0;
    float _t = 0.0f;
    float _h = 0.0f;
};