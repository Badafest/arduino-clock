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

    uint8_t alarmBuzzerPin_;
    uint8_t alarmLedPin_;

    uint8_t alarmHour_;
    uint8_t alarmMinute_;
    unsigned long alarmDuration_;
    char alarmstr_[8]; // HH:MMTT
    long alarmStartedAt_;

    unsigned long currentTimestamp() { return millis() / 60000; }
    void updateTimestr(char *timestr, uint8_t hour, uint8_t minute);

public:
    Clock(
        uint8_t hour,
        uint8_t minute,
        uint8_t alarmBuzzerPin,
        uint8_t alarmLedPin) : lastReadTimestamp_(currentTimestamp()),
                               alarmBuzzerPin_(alarmBuzzerPin),
                               alarmLedPin_(alarmLedPin)
    {
        update(hour, minute);
        alarmDuration_ = 0;
    }

    void update(uint8_t hour = 0, uint8_t minute = 0, bool delta = false);

    const char *read(bool &changed);

    const char *setAlarm(uint8_t hour, uint8_t minute, uint8_t delta = true, long durationMS = -1);
    bool checkAlarm(bool turnOff = false);
};

#endif