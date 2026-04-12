#include "OLEDDisplay.h"

bool OLEDDisplay::setup()
{
    // initialize the OLED object
    if (!display_.begin(SSD1306_SWITCHCAPVCC, screenAddress))
    {
        return false;
    }

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

OLEDDisplay &OLEDDisplay::textWithTitle(const char *text, const char *title)
{
    display_.setTextSize(2);
    display_.setTextColor(WHITE);
    display_.setCursor(0, 0);
    display_.println(title);
    display_.display();

    display_.setCursor(0, 28);
    display_.setTextSize(3);
    display_.setTextColor(WHITE);
    display_.println(text);

    return *this;
}