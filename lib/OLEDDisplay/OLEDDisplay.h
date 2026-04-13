#include <Adafruit_SSD1306.h>

#ifndef __OLED_DISPLAY_H__
#define __OLED_DISPLAY_H__

class OLEDDisplay
{
private:
    Adafruit_SSD1306 display_;
    uint8_t screenAddress;

public:
    OLEDDisplay(uint8_t width,
                uint8_t height,
                uint8_t address) : display_(width, height),
                                   screenAddress(address) {};
    bool setup();

    // display methods
    OLEDDisplay &clear();
    OLEDDisplay &show();

    OLEDDisplay &text(
        const char *text,
        const int16_t x = 0,
        const int16_t y = 0,
        const uint8_t size = 1,
        const uint16_t color = WHITE,
        const uint16_t background = BLACK);

    OLEDDisplay &bitmap(
        const uint8_t *bitmap,
        const int16_t x = 0,
        const int16_t y = 0,
        const int16_t w = 16,
        const uint16_t h = 16,
        const uint16_t color = WHITE,
        const uint16_t background = BLACK);
};

#endif