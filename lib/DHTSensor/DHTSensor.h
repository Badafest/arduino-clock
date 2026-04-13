#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#ifndef __DHT_SENSOR_H__
#define __DHT_SENSOR_H__

struct DHTData_t
{
    float temperature;
    float humidity;
    char temperatureStr[7];
    char humidityStr[7];
};

class DHTSensor
{
private:
    DHT_Unified dht;
    uint32_t delayMS = 1000;
    DHTData_t data = {0};
    unsigned long lastReadMS = 0;

public:
    DHTSensor(uint8_t dhtPin, uint8_t dhtType = DHT11) : dht(dhtPin, dhtType), lastReadMS(millis()) {}

    bool setup();

    bool read(DHTData_t *data);
};

#endif