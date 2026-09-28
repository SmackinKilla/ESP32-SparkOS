#include "TasksPage.h"
#include <Adafruit_GFX.h>
#include "PageManager.h"

void TasksPage::onShortClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::HOME);  
}

void TasksPage::onLongClick() {
    
}

void TasksPage::onDoubleClick() {
    
}

void TasksPage::OnEnter() {
    DrawFrame("TASKS");
}

void TasksPage::Update(uint32_t deltaTimeMs) {
   
}

void TasksPage::OnExit() {
   
}

