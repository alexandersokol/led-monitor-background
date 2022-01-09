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
int previousModePosition;

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
  // else
  // {
  //   int mode = modes[previousModePosition];

  //   Serial.print("current mode position: ");
  //   Serial.print(modePosition);
  //   Serial.print(" previous: ");
  //   Serial.println(previousModePosition);

  //   if (mode == MODE_FLAT || mode == MODE_BOTTOM || mode == MODE_TOP || mode == MODE_CROSS){
  //     Serial.println("No need to reinit color position");
  //   }
  //   {
  //     position1 = FLAT_MODE_DEFAULT_POSITION;
  //   }
  // }
  // writeAddress(ADDRESS_DATA_1, position1);
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
  flatModeInit(fromEeprom);
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
    if (i >= half)
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
  flatModeInit(fromEeprom);
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
  int half = (LED_COUNT / 2);
  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i <= half)
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
//----   CROSS MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize cross mode with inital values
 */
void crossModeInit(boolean fromEeprom)
{
  flatModeInit(fromEeprom);
}

/**
 * Cross mode next action
 */
void crossModeNext()
{
  flatModeNext();
}

/**
 * Cross mode previous action
 */
void crossModePrevious()
{
  flatModePrevious();
}

/**
 * Cross mode led drawing loop
 */
void crossModeLoop()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i % 2 == 0)
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
//----   CORNERS MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize corners mode with inital values
 */
void cornersModeInit(boolean fromEeprom)
{
  flatModeInit(fromEeprom);
}

/**
 * Corners mode next action
 */
void cornersModeNext()
{
  flatModeNext();
}

/**
 * Corners mode previous action
 */
void cornersModePrevious()
{
  flatModePrevious();
}

/**
 * Corners mode led drawing loop
 */
void cornersModeLoop()
{
  int corner0Start = 0;
  int corner0End = CORNER_MODE_LED_COUNT;

  int corner1Start = (LED_COUNT / 2) - CORNER_MODE_LED_COUNT;
  int corner1End = (LED_COUNT / 2) + CORNER_MODE_LED_COUNT;

  int corner2Start = LED_COUNT - CORNER_MODE_LED_COUNT;
  int corner2End = LED_COUNT;

  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i >= corner0Start && i <= corner0End)
    {
      leds[i] = colors[position1];
    }
    else if (i >= corner1Start && i <= corner1End)
    {
      leds[i] = colors[position1];
    }
    else if (i >= corner2Start && i <= corner2End)
    {
      leds[i] = colors[position1];
    }
    else
    {
      leds[i] = CRGB::Black;
    }
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   CENTER MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize center mode with inital values
 */
void centerModeInit(boolean fromEeprom)
{
  flatModeInit(fromEeprom);
}

/**
 * Center mode next action
 */
void centerModeNext()
{
  flatModeNext();
}

/**
 * Corners mode previous action
 */
void centerModePrevious()
{
  flatModePrevious();
}

/**
 * Center mode led drawing loop
 */
void centerModeLoop()
{
  int center0Start = (LED_COUNT / 4) - CENTER_MODE_LED_COUNT;
  int center0End = (LED_COUNT / 4) + CENTER_MODE_LED_COUNT;

  int center1Start = (LED_COUNT - (LED_COUNT / 4)) - CENTER_MODE_LED_COUNT;
  int center1End = (LED_COUNT - (LED_COUNT / 4)) + CENTER_MODE_LED_COUNT;

  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i >= center0Start && i <= center0End)
    {
      leds[i] = colors[position1];
    }
    else if (i >= center1Start && i <= center1End)
    {
      leds[i] = colors[position1];
    }
    else
    {
      leds[i] = CRGB::Black;
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
  else if (currentMode == MODE_CROSS)
  {
    crossModeLoop();
  }
  else if (currentMode == MODE_CORNERS)
  {
    cornersModeLoop();
  }
  else if (currentMode == MODE_CENTER)
  {
    centerModeLoop();
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
  else if (currentMode == MODE_CROSS)
  {
    crossModeInit(readEeprom);
  }
  else if (currentMode == MODE_CORNERS)
  {
    cornersModeInit(readEeprom);
  }
  else if (currentMode == MODE_CENTER)
  {
    centerModeInit(readEeprom);
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
  else if (currentMode == MODE_CROSS)
  {
    crossModePrevious();
  }
  else if (currentMode == MODE_CORNERS)
  {
    cornersModePrevious();
  }
  else if (currentMode == MODE_CENTER)
  {
    centerModePrevious();
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
  else if (currentMode == MODE_CROSS)
  {
    crossModeNext();
  }
  else if (currentMode == MODE_CORNERS)
  {
    cornersModeNext();
  }
  else if (currentMode == MODE_CENTER)
  {
    centerModeNext();
  }
}

/**
 * Function to switch next color mode
 */
void modeSwitch()
{
  previousModePosition = modePosition;
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
  position1 = 0;
  writeAddress(ADDRESS_DATA_1, position1);

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
  previousModePosition = modePosition;

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