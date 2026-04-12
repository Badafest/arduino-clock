#include "Button.h"

void Button::setup()
{
    pinMode(_pin, INPUT_PULLUP);
    _currentState = digitalRead(_pin);

    _lastChangedState = _currentState;
    _lastStateChangedAt = millis();
}

void Button::loop()
{
    int state = digitalRead(_pin);
    long currentTimestamp = millis();

    if (state != _lastChangedState)
    {
        _lastStateChangedAt = currentTimestamp;
    }

    if (currentTimestamp - _lastStateChangedAt > _delayMs)
    {
        _currentState = state;
    }

    _lastChangedState = state;
}