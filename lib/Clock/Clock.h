#include <Arduino.h>

#ifndef __CLOCK_H__
#define __CLOCK_H__

class Clock
{
private:
    uint8_t hour_;
    uint8_t minute_;
    char timestr_[8]; // HH:MMTT
    unsigned long lastReadTimestamp_;

    unsigned long currentTimestamp() { return millis() / 60000; }

public:
    Clock(uint8_t hour, uint8_t minute) : lastReadTimestamp_(currentTimestamp())
    {
        update(hour, minute);
    }

    void update(uint8_t hour = 0, uint8_t minute = 0, bool delta = false);

    const char *read(bool &changed);
};

#endif