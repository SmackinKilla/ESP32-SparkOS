#include "Arduino.h"
#include "DisplayManager.h"
#include "EmuDisplay.h"
#include "OneButton.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cstdint>
#include <optional>
#include <cstring>

extern DisplayManager displays;
extern OneButton button;
extern void setup();
extern void loop();

static void rgb565ToRgba(uint16_t color, std::uint8_t *out) {
    std::uint8_t r5 = (color >> 11) & 0x1F;
    std::uint8_t g6 = (color >> 5) & 0x3F;
    std::uint8_t b5 = color & 0x1F;
    out[0] = (r5 << 3) | (r5 >> 2);
    out[1] = (g6 << 2) | (g6 >> 4);
    out[2] = (b5 << 3) | (b5 >> 2);
    out[3] = 255;
}

int main() {
    setup();

    EmuDisplay *mainDisp = dynamic_cast<EmuDisplay *>(displays.get("main"));
    EmuDisplay *oledDisp = dynamic_cast<EmuDisplay *>(displays.get("oled"));
    if (!mainDisp) {
        Serial.println("[Emu] Main display is not EmuDisplay");
        return 1;
    }

    const unsigned int gap = 16;
    const unsigned int scale = 3;

    const unsigned int mw = (unsigned int)mainDisp->width();
    const unsigned int mh = (unsigned int)mainDisp->height();
    const unsigned int ow = oledDisp ? (unsigned int)oledDisp->width() : 0;
    const unsigned int oh = oledDisp ? (unsigned int)oledDisp->height() : 0;

    const unsigned int totalW = ow ? mw + gap + ow : mw;
    const unsigned int totalH = mh > oh ? mh : oh;

    sf::RenderWindow window(
        sf::VideoMode({totalW * scale, totalH * scale}),
        "ESP32 UI Emulator"
    );
    window.setFramerateLimit(60);

    sf::Texture texture({totalW, totalH});
    texture.setSmooth(false);
    std::vector<std::uint8_t> pixels((size_t)totalW * totalH * 4);

    auto blit = [&](EmuDisplay *d, unsigned int offX, bool mono) {
        const uint16_t *fb = d->framebuffer();
        const unsigned int w = (unsigned int)d->width();
        const unsigned int h = (unsigned int)d->height();
        for (unsigned int y = 0; y < h; y++) {
            for (unsigned int x = 0; x < w; x++) {
                uint16_t c = fb[y * w + x];
                if (mono) c = c ? 0xFFFF : 0x0000;
                rgb565ToRgba(c, &pixels[((size_t)y * totalW + offX + x) * 4]);
            }
        }
    };

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        bool pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        button.simulateState(pressed);

        loop();

        for (size_t i = 0; i < pixels.size(); i += 4) {
            pixels[i] = 40; pixels[i + 1] = 40; pixels[i + 2] = 40; pixels[i + 3] = 255;
        }

        blit(mainDisp, 0, false);
        if (oledDisp) blit(oledDisp, mw + gap, true);

        texture.update(pixels.data());
        sf::Sprite sprite(texture);
        sprite.setScale({(float)scale, (float)scale});

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}