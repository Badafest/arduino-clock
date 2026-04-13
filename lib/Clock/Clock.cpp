#include <Clock.h>

void Clock::update(uint8_t hour, uint8_t minute, bool delta)
{
    hour_ = (delta ? hour_ + hour : hour) % 24;
    minute_ = (delta ? minute_ + minute : minute) % 60;

    uint8_t displayHour = hour_ % 12;
    if (displayHour == 0)
    {
        displayHour = 12;
    }

    sprintf(timestr_, "%02d:%02d%s",
            displayHour,
            minute_,
            hour_ >= 12 ? "PM" : "AM");

    lastReadTimestamp_ = currentTimestamp();
}

const char *Clock::read(bool &changed)
{
    auto diffMinutes = currentTimestamp() - lastReadTimestamp_;
    if (diffMinutes < 1)
    {
        changed = false;
        return timestr_;
    }

    unsigned long totalMinutes = hour_ * 60 + minute_ + diffMinutes;

    update(
        static_cast<uint8_t>(totalMinutes / 60),
        static_cast<uint8_t>(totalMinutes % 60));

    changed = true;
    return timestr_;
};
