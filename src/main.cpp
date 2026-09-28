#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <OneButton.h>
#include <Adafruit_GFX.h>

#include "PageManager.h"
#include "WeatherPage.h"
#include "WeatherPage1.h"
#include "TasksPage.h"
#include "TimeZonesPage.h"
#include "MiniGames.h"
#include "SettingsPage.h"
#include "HomePage.h"
#include "ScreenSaver.h"
#include "ColorPalette.h"
#include "SystemSettings.h"
#include "DisplayManager.h"
#include "BoardConfig.h"
#include "ConfigServer.h"
#include "NetService.h"
#include "NetDiagPage.h"
#include "SysInfoPage.h"
#include "log.h"

#define BUTTON_PIN 4


DisplayManager displays;
OneButton button(BUTTON_PIN, true);
PageManager pm;

void setup() {
    Serial.begin(115200);
    Log::init();   
    if (!g_board.load()) {       
        g_board.setDefaults();   
        LOGE(LT_HW, "Config error, set defaults");
        g_board.save();
    }

    g_settings.load();
    applyTheme();

    NetService::begin();        
    ConfigServer::begin(); 

    button.setClickMs(getClickMs(g_settings.clickSpeed));
    button.setPressMs(500);
        button.attachClick([]() {
        Page* p = pm.getCurrent();
        if (p) p->onShortClick();
    });

    button.attachDoubleClick([]() {
        Page* p = pm.getCurrent();
        if (p) p->onDoubleClick();
    });

    button.attachLongPressStart([]() {
        Page* p = pm.getCurrent();
        if (p) p->onLongClick();
    });
    if (!displays.begin(g_board.displays, g_board.displayCount)) {
        LOGE(LT_HW, "Display init failed");
        while (true) delay(1000);
    }
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    displays.gfx((uint8_t)0)->fillScreen(COLOR_BG);
    displays.flushAll();
    displays.setBrightnessAll(g_settings.brightness);
    static WeatherPage weatherPage(&displays, &pm);
    static WeatherPage1 weatherPage1(&displays, &pm);
    static TasksPage tasksPage(&displays, &pm);
    static TimeZonesPage TZPage(&displays, &pm);
    static MiniGames minigamesPage(&displays, &pm);
    static SettingsPage settings(&displays, &pm);
    static HomePage homePage(&displays, &pm);
    static ScreenSaver screenSaver(&displays, &pm);     
    static NetDiagPage netDiag(&displays, &pm);
    static SysInfo sysInfo(&displays, &pm);

    
    pm.registerPage(PageIndex::WEATHER, &weatherPage, TitleInfo("Weather", false, true));
    pm.registerPage(PageIndex::WEATHER1, &weatherPage1, TitleInfo("Weather1", false, false));
    pm.registerPage(PageIndex::TASKS, &tasksPage, TitleInfo("Tasks", true, true));
    pm.registerPage(PageIndex::TIMEZONES, &TZPage, TitleInfo("World Time", false, true));
    pm.registerPage(PageIndex::GAMES, &minigamesPage, TitleInfo("Gambling", false, true));
    pm.registerPage(PageIndex::SETTINGS, &settings, TitleInfo("Settings", false, true));
    pm.registerPage(PageIndex::HOME, &homePage, TitleInfo("Home", false, false));
    pm.registerPage(PageIndex::SCREENSAVER, &screenSaver, TitleInfo("ScreenSaver", false, false));
    pm.registerPage(PageIndex::NETDIAG, &netDiag, TitleInfo("Network", false, false));
    pm.registerPage(PageIndex::SYSINFO, &sysInfo, TitleInfo("Network", false, false));
    
    if (NetService::state() == NetState::OFF) {
    pm.SwitchToIndex(PageIndex::NETDIAG);
    } else {
    pm.SwitchToIndex(PageIndex::HOME);
    }

    displays.flushAll();
}

void loop() {
    ConfigServer::loop(); 
    button.tick();

    static uint32_t lastTime = 0;

    uint32_t now = millis();
    uint32_t dt = now - lastTime;
    lastTime = now;

    Page* current = pm.getCurrent();

    if (current) {
        current->Update(dt);
    }

    displays.flushAll();

    delay(16);
}