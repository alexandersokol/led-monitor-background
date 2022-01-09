#include <Arduino.h>
#include <FastLED.h>
#include <EncButton.h>
#include <avr/eeprom.h>

#include "const.h"

CRGB leds[LED_COUNT];

EncButton<EB_TICK, PIN_UP> buttonUp;
EncButton<EB_TICK, PIN_DOWN> buttonDown;
EncButton<EB_TICK, PIN_LEFT> buttonPrevious;
EncButton<EB_TICK, PIN_RIGHT> buttonNext;
EncButton<EB_TICK, PIN_OK> buttonOk;

int modePosition;
int brightness;

byte position1 = 0;

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   FLAT MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize flat mode with inital values
 */
void flatModeInit(boolean fromEeprom)
{
  if (fromEeprom)
  {
    position1 = readAddress(ADDRESS_DATA_1, FLAT_MODE_DEFAULT_POSITION);
  }
  else
  {
    position1 = FLAT_MODE_DEFAULT_POSITION;
  }
  writeAddress(ADDRESS_DATA_1, position1);
}

/**
 * Flat mode next action
 */
void flatModeNext()
{
  position1++;
  if (position1 >= lengthOf(colors))
  {
    position1 = 0;
  }
  writeAddress(ADDRESS_DATA_1, position1);
  Serial.print("Position Increace: ");
  Serial.println(position1, DEC);
}

/**
 * Flat mode previous action
 */
void flatModePrevious()
{
  position1--;
  if (position1 == 255)
  {
    position1 = lengthOf(colors) - 1;
  }
  writeAddress(ADDRESS_DATA_1, position1);
  Serial.print("Position Decreace: ");
  Serial.println(position1, DEC);
}

/**
 * Flat mode led drawing loop
 */
void flatModeLoop()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = colors[position1];
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   BOTTOM MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize bottom mode with inital values
 */
void bottomModeInit(boolean fromEeprom)
{
  if (fromEeprom)
  {
    position1 = readAddress(ADDRESS_DATA_1, BOTTOM_MODE_DEFAULT_POSITION);
  }
  else
  {
    position1 = BOTTOM_MODE_DEFAULT_POSITION;
  }
  writeAddress(ADDRESS_DATA_1, position1);
}

/**
 * Bottom mode next action
 */
void bottomModeNext()
{
  flatModeNext();
}

/**
 * Bottom mode previous action
 */
void bottomModePrevious()
{
  flatModePrevious();
}

/**
 * Bottom mode led drawing loop
 */
void bottomModeLoop()
{
  int half = LED_COUNT / 2;
  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i > half)
    {
      leds[i] = CRGB::Black;
    }
    else
    {
      leds[i] = colors[position1];
    }
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   TOP MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize top mode with inital values
 */
void topModeInit(boolean fromEeprom)
{
  if (fromEeprom)
  {
    position1 = readAddress(ADDRESS_DATA_1, TOP_MODE_DEFAULT_POSITION);
  }
  else
  {
    position1 = TOP_MODE_DEFAULT_POSITION;
  }
  writeAddress(ADDRESS_DATA_1, position1);
}

/**
 * Top mode next action
 */
void topModeNext()
{
  flatModeNext();
}

/**
 * Top mode previous action
 */
void topModePrevious()
{
  flatModePrevious();
}

/**
 * Top mode led drawing loop
 */
void topModeLoop()
{
  int half = LED_COUNT / 2;
  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i < half)
    {
      leds[i] = CRGB::Black;
    }
    else
    {
      leds[i] = colors[position1];
    }
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Returns current mode identifier
 */
inline byte getCurrentMode()
{
  return modes[modePosition];
}

/**
 * Function to delegate led loop to current mode
 */
void ledModeLoop()
{
  byte currentMode = getCurrentMode();
  if (currentMode == MODE_FLAT)
  {
    flatModeLoop();
  }
  else if (currentMode == MODE_BOTTOM)
  {
    bottomModeLoop();
  }
  else if (currentMode == MODE_TOP)
  {
    topModeLoop();
  }
}

/**
 * Function to delegate current mode init state
 */
void initCurrentMode(boolean readEeprom)
{
  byte currentMode = getCurrentMode();
  if (currentMode == MODE_FLAT)
  {
    flatModeInit(readEeprom);
  }
  else if (currentMode == MODE_BOTTOM)
  {
    bottomModeInit(readEeprom);
  }
  else if (currentMode == MODE_TOP)
  {
    topModeInit(readEeprom);
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Function to increace LED brighness
 */
void increaseBrightness()
{
  brightness = brightness + 5;
  if (brightness > BRIGHTNESS_MAX)
  {
    brightness = BRIGHTNESS_MAX;
  }
  Serial.print("Increase brightness: ");
  Serial.println(brightness);
}

/**
 * Function to decreace LED brighness
 */
void decreaseBrightness()
{
  brightness = brightness - 5;
  if (brightness < BRIGHTNESS_MIN)
  {
    brightness = BRIGHTNESS_MIN;
  }
  Serial.print("Decreace brightness: ");
  Serial.println(brightness);
}

/**
 * Function triggers next change in the current mode
 */
void previous()
{
  byte currentMode = getCurrentMode();
  if (currentMode == MODE_FLAT)
  {
    flatModePrevious();
  }
  else if (currentMode == MODE_BOTTOM)
  {
    bottomModePrevious();
  }
  else if (currentMode == MODE_TOP)
  {
    topModePrevious();
  }
}

/**
 * Function triggers previous change in the current mode
 */
void next()
{
  byte currentMode = getCurrentMode();
  if (currentMode == MODE_FLAT)
  {
    flatModeNext();
  }
  else if (currentMode == MODE_BOTTOM)
  {
    bottomModeNext();
  }
  else if (currentMode == MODE_TOP)
  {
    topModeNext();
  }
}

/**
 * Function to switch next color mode
 */
void modeSwitch()
{
  modePosition++;
  if (modePosition >= lengthOf(modes))
  {
    modePosition = 0;
  }
  writeAddress(ADDRESS_MODE, modePosition);

  Serial.print("Mode switched to: ");
  Serial.println(modePosition);

  initCurrentMode(false);
}

/**
 * Resets current mode settings to the default values
 */
void modeReset()
{
  initCurrentMode(false);
}

/**
 * Function to hanle all button ticks
 */
void buttonTick()
{
  buttonUp.tick();
  buttonDown.tick();
  buttonPrevious.tick();
  buttonNext.tick();
  buttonOk.tick();

  if (buttonUp.click())
    increaseBrightness();
  if (buttonDown.click())
    decreaseBrightness();
  if (buttonPrevious.click())
    previous();
  if (buttonNext.click())
    next();
  if (buttonOk.click())
    modeSwitch();
  if (buttonOk.held())
    modeReset();
}

/**
 * ----------------------------------------------------------------------------------------------------------------
 * Default setup function
 * ----------------------------------------------------------------------------------------------------------------
 */
void setup()
{
  Serial.begin(9600);

  delay(200);

  FastLED.addLeds<CHIPSET, PIN_LED, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);

  brightness = readAddress(ADDRESS_BRIGHTNESS, BRIGHTNESS_DEFAULT);
  modePosition = readAddress(ADDRESS_MODE, MODE_DEFAULT);
  initCurrentMode(true);
}

/**
 * ----------------------------------------------------------------------------------------------------------------
 * Default loop function
 * ----------------------------------------------------------------------------------------------------------------
 */
void loop()
{
  buttonTick();
  ledModeLoop();

  FastLED.setBrightness(brightness);
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}