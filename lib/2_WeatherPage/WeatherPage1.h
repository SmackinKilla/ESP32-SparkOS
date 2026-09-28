#pragma once
#include "Page.h"
#include "PageManager.h"
// Эта страница предназначена для демонстрации другой страницы погоды, которая может быть вызвана из основной страницы погоды. 
// Она может содержать дополнительные функции или информацию о погоде, которые не отображаются на основной странице.
class WeatherPage1 : public Page {
public:
    WeatherPage1(DisplayManager* displays, PageManager* pm) 
        : Page(displays, pm) {}

    void onShortClick() override;
    void onLongClick() override;
    void onDoubleClick() override;
    void OnEnter() override;
    void Update(uint32_t deltaTimeMs) override;
    void OnExit() override;
    
private:

};