#include "DHTSensor.h"

bool DHTSensor::setup()
{
    // Initialize device.
    dht.begin();

    sensor_t sensor;

    dht.temperature().getSensor(&sensor);
    unsigned long _tdelayMS = sensor.min_delay / 1000;

    dht.humidity().getSensor(&sensor);
    unsigned long _hdelayMS = sensor.min_delay / 1000;

    delayMS = _tdelayMS > _hdelayMS ? _tdelayMS : _hdelayMS;

    return true;
}

bool DHTSensor::read(DHTData_t *data)
{
    unsigned long currentMS = millis();

    if (currentMS - lastReadMS < delayMS)
    {
        return false;
    }

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    float temperature = event.temperature;
    temperature = isnan(temperature) ? 0.0f : temperature;

    dht.humidity().getEvent(&event);
    float humidity = event.relative_humidity;
    humidity = isnan(humidity) ? 0.0f : humidity;

    bool dataChanged = false;

    if (data->temperature != temperature)
    {
        dataChanged = true;
        data->temperature = temperature;
        dtostrf(temperature, 6, 2, data->temperatureStr);
        strcat(data->temperatureStr, "C");
    }
    if (data->humidity != humidity)
    {
        dataChanged = true;
        data->humidity = humidity;
        dtostrf(humidity, 6, 2, data->humidityStr);
        strcat(data->humidityStr, "%");
    }
    return dataChanged;
}