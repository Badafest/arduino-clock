#include <Clock.h>

void Clock::updateTimestr(char *timestr, uint8_t hour, uint8_t minute)
{
    uint8_t displayHour = hour % 12;
    if (displayHour == 0)
    {
        displayHour = 12;
    }

    sprintf(timestr, "%02d:%02d%s",
            displayHour,
            minute,
            hour >= 12 ? "PM" : "AM");
}

void Clock::update(uint8_t hour, uint8_t minute, bool delta)
{
    hour_ = (delta ? hour_ + hour : hour) % 24;
    minute_ = (delta ? minute_ + minute : minute) % 60;

    auto currentts = currentTimestamp();
    if (hour_ == alarmHour_ && minute_ == alarmMinute_ && alarmStartedAt_ < 0)
    {
        alarmStartedAt_ = millis();
    }

    lastReadTimestamp_ = currentts;

    updateTimestr(timestr_, hour_, minute_);
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

const char *Clock::setAlarm(uint8_t hour, uint8_t minute, uint8_t delta = true, long durationMS)
{
    pinMode(alarmLedPin_, OUTPUT);
    pinMode(alarmBuzzerPin_, OUTPUT);

    alarmHour_ = (delta ? alarmHour_ + hour : hour) % 24;
    alarmMinute_ = (delta ? alarmMinute_ + minute : minute) % 60;

    if (durationMS > 300000L)
    {
        durationMS = 300000L;
    }

    if (durationMS >= 0)
    {
        alarmDuration_ = static_cast<unsigned long>(durationMS);
    }

    uint8_t displayHour = alarmHour_ % 12;
    if (displayHour == 0)
    {
        displayHour = 12;
    }

    sprintf(alarmstr_, "%02d:%02d%s",
            displayHour,
            alarmMinute_,
            alarmHour_ >= 12 ? "PM" : "AM");

    alarmStartedAt_ = -1;

    return alarmstr_;
}

bool Clock::checkAlarm(bool turnOff)
{
    long alarmDiffMS = millis() - alarmStartedAt_;

    if (turnOff || alarmDiffMS > static_cast<long>(alarmDuration_))
    {
        alarmStartedAt_ = -1;
    }

    uint16_t normalizedVal = static_cast<uint16_t>(alarmDiffMS % 1000L);

    if (
        alarmStartedAt_ < 0 ||
        normalizedVal > 750 ||
        normalizedVal % 150 > 75)
    {
        noTone(alarmBuzzerPin_);
        analogWrite(alarmLedPin_, LOW);
        return (alarmStartedAt_ >= 0);
    }

    tone(alarmBuzzerPin_, 2500);
    analogWrite(alarmLedPin_, HIGH);
    return true;
}
