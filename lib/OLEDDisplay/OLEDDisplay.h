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
    OLEDDisplay &textWithTitle(const char *text, const char *title);
};

#endif