#include "Arduino.h"

#ifndef __BUTTON_H__
#define __BUTTON_H__

class Button
{
private:
    uint8_t _pin;
    unsigned long _delayMs;
    int _currentState;
    int _lastChangedState;
    unsigned long _lastStateChangedAt;

public:
    Button(
        uint8_t pin,
        unsigned long delayMs) : _pin(pin),
                                 _delayMs(delayMs),
                                 _currentState(LOW),
                                 _lastStateChangedAt(0)
    {
    }

    int getState() const { return _currentState; }

    void setup();
    void loop();
};

#endif