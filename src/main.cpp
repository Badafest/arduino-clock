#include <DHTSensor.h>
#include <OLEDDisplay.h>
#include <Clock.h>
#include <Button.h>

#define UPDATE_DELAY_MS (2000)
#define SETTINGS_WAIT_MS (5000)
#define BUTTON_DELAY_MS (50)

DHTSensor dhtSensor(
    2 // DHT Pin
);

OLEDDisplay oledDisplay(
    128, // width
    64,  // height
    0x3C // address
);

Clock clock(
    20, // hour
    30  // minute
);

Button hourButton(8, BUTTON_DELAY_MS);
uint8_t lastHourButtonState = LOW;

Button minuteButton(9, BUTTON_DELAY_MS);
uint8_t lastMinuteButtonState = LOW;

enum mode_t
{
  SETTINGS_MODE,
  TIME_MODE,
  TEMPERATURE_MODE,
  HUMIDITY_MODE
};

mode_t currentMode = TIME_MODE;
unsigned long lastModeChangedAt = 0;

unsigned long delays[4];

bool changeMode(mode_t newMode)
{
  if (newMode == currentMode)
  {
    return false;
  }
  currentMode = newMode;
  lastModeChangedAt = millis();
  return true;
}

DHTData_t data;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);

  for (uint8_t mode = 0; mode < 4; mode++)
  {
    delays[mode] = mode == SETTINGS_MODE ? SETTINGS_WAIT_MS : UPDATE_DELAY_MS;
  }

  hourButton.setup();
  lastHourButtonState = hourButton.getState();

  minuteButton.setup();
  lastMinuteButtonState = minuteButton.getState();

  if (!dhtSensor.setup())
  {
    Serial.println("Failed to setup DHT sensor");
    while (true)
    {
      // block the execution
    }
  }

  if (!oledDisplay.setup())
  {
    Serial.println("Failed to setup OLED display");
    while (true)
    {
      // block the execution
    }
  }

  Serial.println("Setup done!");
}

void loop()
{
  hourButton.loop();
  minuteButton.loop();

  bool minuteChanged = minuteButton.getState() != lastMinuteButtonState;
  bool hourChanged = hourButton.getState() != lastHourButtonState;

  if (minuteChanged || hourChanged)
  {
    Serial.println("TIME CHANGED");

    changeMode(SETTINGS_MODE);

    clock.update(hourChanged, minuteChanged, true);
    oledDisplay.clear().textWithTitle(clock.read(), "SET").show();

    lastMinuteButtonState = minuteButton.getState();
    lastHourButtonState = hourButton.getState();
  }

  if ((millis() - lastModeChangedAt) < delays[currentMode])
  {
    return;
  }

  switch (currentMode)
  {
  case TIME_MODE:
    dhtSensor.read(&data);
    Serial.print("TEMPERATURE: ");
    Serial.println(data.temperatureStr);
    oledDisplay.clear().textWithTitle(data.temperatureStr, "TEMP").show();
    changeMode(TEMPERATURE_MODE);
    break;

  case TEMPERATURE_MODE:
    dhtSensor.read(&data);
    Serial.print("HUMIDITY: ");
    Serial.println(data.humidityStr);
    oledDisplay.clear().textWithTitle(data.humidityStr, "HUMD").show();
    changeMode(HUMIDITY_MODE);
    break;

  case HUMIDITY_MODE:
  case SETTINGS_MODE:
    oledDisplay.clear().textWithTitle(clock.read(), "TIME").show();
    changeMode(TIME_MODE);
    break;

  default:
    break;
  }
}