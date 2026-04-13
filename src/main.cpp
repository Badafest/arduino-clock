#include <OLEDDisplay.h>
#include <Clock.h>
#include <Button.h>

#define UPDATE_DELAY_MS (2000)
#define SETTINGS_WAIT_MS (5000)
#define BUTTON_DELAY_MS (50)

#define ALARM_ICON_W (16)
#define ALARM_ICON_H (16)

const unsigned char ALARM_ICON[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x23, 0x18, 0x53, 0xb8, 0x64, 0x54, 0x28, 0x10, 0x00, 0x00,
    0x10, 0x20, 0x10, 0x20, 0x10, 0x10, 0x20, 0x10, 0x10, 0x30, 0x06, 0xc0, 0x03, 0x80, 0x00, 0x00};

OLEDDisplay oledDisplay(
    128, // width
    64,  // height
    0x3C // address
);

Clock clock(
    21, // hour
    20, // minute
    3,  // alarm led pin
    5   // alarm buzzer pin
);

Button modeButton(2, BUTTON_DELAY_MS);
uint8_t lastModeButtonState = LOW;

Button hourButton(8, BUTTON_DELAY_MS);
uint8_t lastHourButtonState = LOW;

Button minuteButton(9, BUTTON_DELAY_MS);
uint8_t lastMinuteButtonState = LOW;

bool timeChanged = false;

const char *timestr;
const char *alarmstr;

enum mode_t
{
  TIME_MODE,
  ALARM_MODE
};

mode_t currentMode = TIME_MODE;

void updateDisplay()
{
  oledDisplay
      .clear()
      .text(currentMode == TIME_MODE ? "TIME" : "ALARM", 2, 6)
      .bitmap(ALARM_ICON, 65, 0, ALARM_ICON_W, ALARM_ICON_H)
      .text(alarmstr, 68 + ALARM_ICON_W, 6, 1)
      .text(timestr, 2, 28, 3)
      .show();
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);

  modeButton.setup();
  lastModeButtonState = modeButton.getState();

  hourButton.setup();
  lastHourButtonState = hourButton.getState();

  minuteButton.setup();
  lastMinuteButtonState = minuteButton.getState();

  if (!oledDisplay.setup())
  {
    Serial.println("Failed to setup OLED display");
    while (true)
    {
      // block the execution
    }
  }

  timestr = clock.read(timeChanged);

  alarmstr = clock.setAlarm(0, 0, false, 0);

  updateDisplay();

  Serial.println("Setup done!");
}

void loop()
{
  hourButton.loop();
  minuteButton.loop();
  modeButton.loop();

  bool modeChanged = modeButton.getState() != lastModeButtonState;
  bool alarmOn = clock.checkAlarm();

  if (modeChanged)
  {
    if (alarmOn)
    {
      clock.checkAlarm(true);
      return;
    }

    switch (currentMode)
    {
    case TIME_MODE:
      currentMode = ALARM_MODE;
      break;

    default:
      currentMode = TIME_MODE;
      break;
    }

    lastModeButtonState = modeButton.getState();

    updateDisplay();

    return;
  }

  bool minuteChanged = minuteButton.getState() != lastMinuteButtonState;
  bool hourChanged = hourButton.getState() != lastHourButtonState;

  if (minuteChanged || hourChanged)
  {

    if (currentMode == ALARM_MODE)
    {
      clock.setAlarm(hourChanged, minuteChanged, true, 60000L);
    }
    else if (currentMode == TIME_MODE)
    {
      clock.update(hourChanged, minuteChanged, true);
    }

    updateDisplay();

    lastMinuteButtonState = minuteButton.getState();
    lastHourButtonState = hourButton.getState();

    return;
  }

  timestr = clock.read(timeChanged);

  if (timeChanged)
  {
    updateDisplay();
  }
}