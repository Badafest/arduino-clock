#include <DHTSensor.h>
#include <OLEDDisplay.h>
#include <Clock.h>
#include <Button.h>

#define UPDATE_DELAY_MS (2000)
#define SETTINGS_WAIT_MS (5000)
#define BUTTON_DELAY_MS (50)

#define WEATHER_ICON_W (18)
#define WEATHER_ICON_H (16)

const unsigned char WEATHER_ICON[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x00, 0x12,
    0x80, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x78, 0x00, 0x0e, 0x7e, 0x00, 0x09, 0xff, 0x00, 0x17, 0xff,
    0x80, 0x0f, 0xff, 0x80, 0x0f, 0xff, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

DHTSensor dhtSensor(
    2 // DHT Pin
);

OLEDDisplay oledDisplay(
    128, // width
    64,  // height
    0x3C // address
);

Clock clock(
    13, // hour
    57  // minute
);

Button hourButton(8, BUTTON_DELAY_MS);
uint8_t lastHourButtonState = LOW;

Button minuteButton(9, BUTTON_DELAY_MS);
uint8_t lastMinuteButtonState = LOW;

bool timeChanged = false;
const char *timestr;
DHTData_t data;

void updateDisplay()
{
  char weatherstr[16] = {0};
  sprintf(weatherstr, data.temperatureStr);
  strcat(weatherstr, " - ");
  strcat(weatherstr, data.humidityStr);

  oledDisplay
      .clear()
      .bitmap(WEATHER_ICON, 6, 0, WEATHER_ICON_W, WEATHER_ICON_H)
      .text(weatherstr, 8 + WEATHER_ICON_W, 6)
      .text(timestr, 0, 28, 3)
      .show();
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
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

  dhtSensor.read(&data);
  timestr = clock.read(timeChanged);

  updateDisplay();

  Serial.println("Setup done!");
}

void loop()
{
  bool dhtChanged = dhtSensor.read(&data);

  hourButton.loop();
  minuteButton.loop();

  bool minuteChanged = minuteButton.getState() != lastMinuteButtonState;
  bool hourChanged = hourButton.getState() != lastHourButtonState;

  if (minuteChanged || hourChanged)
  {
    clock.update(hourChanged, minuteChanged, true);
    updateDisplay();

    lastMinuteButtonState = minuteButton.getState();
    lastHourButtonState = hourButton.getState();

    return;
  }

  timestr = clock.read(timeChanged);
  if (timeChanged || dhtChanged)
  {
    updateDisplay();
  }
}