// ScreenSaver.h
#pragma once
#include "Page.h"

class ScreenSaver : public Page {
public:
    ScreenSaver(DisplayManager* displays, PageManager* pm) 
        : Page(displays, pm) {}
    
    void OnEnter() override;
    void Update(uint32_t deltaTimeMs) override;
    void OnExit() override;

};
