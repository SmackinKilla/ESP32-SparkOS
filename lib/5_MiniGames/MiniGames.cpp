#include "MiniGames.h"
#include <Adafruit_GFX.h>
#include "PageManager.h"

void MiniGames::onShortClick() {
    if (_pm) _pm->SwitchToIndex(PageIndex::HOME);  
}

void MiniGames::onLongClick() {

}

void MiniGames::onDoubleClick() {
    
}

void MiniGames::OnEnter() {
    DrawFrame("GAMES");
}

void MiniGames::Update(uint32_t deltaTimeMs) {

}

void MiniGames::OnExit() {

}

