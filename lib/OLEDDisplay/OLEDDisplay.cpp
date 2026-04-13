#include "OLEDDisplay.h"

bool OLEDDisplay::setup()
{
    // initialize the OLED object
    if (!display_.begin(SSD1306_SWITCHCAPVCC, screenAddress))
    {
        return false;
    }

    delay(2000);

    clear();
    show();

    return true;
}

OLEDDisplay &OLEDDisplay::clear()
{
    display_.clearDisplay();
    return *this;
}

OLEDDisplay &OLEDDisplay::show()
{
    display_.display();
    return *this;
}

OLEDDisplay &OLEDDisplay::text(
    const char *text,
    const int16_t x,
    const int16_t y,
    const uint8_t size,
    const uint16_t color,
    const uint16_t background)
{
    display_.setCursor(x, y);
    display_.setTextSize(size);
    display_.setTextColor(color, background);
    display_.println(text);

    return *this;
}

OLEDDisplay &OLEDDisplay::bitmap(
    const uint8_t *bitmap,
    const int16_t x,
    const int16_t y,
    const int16_t w,
    const uint16_t h,
    const uint16_t color,
    const uint16_t background)
{
    display_.drawBitmap(x, y, bitmap, w, h, color, background);
    return *this;
}