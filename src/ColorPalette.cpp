#include <Adafruit_ST7735.h>
#include "ColorPalette.h"

uint16_t COLOR_BLUE   = 0x001F;
uint16_t COLOR_WHITE  = 0xffff;
uint16_t COLOR_BLACK  = 0x0000;
uint16_t COLOR_RED    = 0xF800;
uint16_t COLOR_GREEN  = 0x07E0;
uint16_t COLOR_CYAN   = 0x07FF;
uint16_t COLOR_YELLOW = 0xFFE0;

uint16_t COLOR_BG     = 0x001F;
uint16_t COLOR_ACCENT = 0xffff;
uint16_t COLOR_TEXT   = 0xffff;
uint16_t COLOR_GRAY   = 0xa534; 
uint16_t COLOR_Win95_BLUE = 0x0010;

void applyTheme() {
    switch (g_settings.theme) {
        case Theme::BLUE:
            COLOR_BG     = 0x001F;
            COLOR_TEXT   = 0xffff;
            COLOR_ACCENT = 0x0010;
            break;
            
        case Theme::RED:
            COLOR_BG     = 0x9800; 
            COLOR_TEXT   = 0xffff;
            COLOR_ACCENT = 0x6000;
            break;
            
        case Theme::PURPLE:
            COLOR_BG     = 0x8030; 
            COLOR_TEXT   = 0xffff;
            COLOR_ACCENT = 0x500a; 
            break;
            
        case Theme::GREEN:
            COLOR_BG     = 0x0d00;
            COLOR_TEXT   = 0xffff; 
            COLOR_ACCENT = 0x0ae0;
            break;
            
        case Theme::DARK:
            COLOR_BG     = 0x5aeb;
            COLOR_TEXT   = 0xffff; 
            COLOR_ACCENT = 0xFBE0;
            break;
        case Theme::Win95:
            COLOR_BG     = 0x8410; 
            COLOR_TEXT   = 0x0000; 
            COLOR_ACCENT = 0x0010; 
            break;
    }
}