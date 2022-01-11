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
byte position2 = 0;
byte spotPosition = 0;
unsigned long lastSpotUpdateTime = 0;

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
//----   SMALL SPOT MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize small spot mode with inital values
 */
void smallSpotModeInit(boolean fromEeprom)
{
  flatModeInit(fromEeprom);
}

/**
 * Small spot mode next action
 */
void smallSpotModeNext()
{
  flatModeNext();
}

/**
 * Small spot mode previous action
 */
void smallSpotModePrevious()
{
  flatModePrevious();
}

/**
 * Default spot mode loop function
 */
void spotModeLoop(unsigned int delay, int size)
{
  long currentTime = millis();
  if (lastSpotUpdateTime == 0 || (currentTime - lastSpotUpdateTime) >= delay)
  {
    lastSpotUpdateTime = currentTime;

    spotPosition++;
    if (spotPosition >= LED_COUNT)
    {
      spotPosition = 0;
    }
  }

  byte spotRange0Start = spotPosition;
  byte spotRange0End = spotPosition + size;

  byte spotRange1Start = 255;
  byte spotRange1End = 255;

  if (spotRange0End >= LED_COUNT)
  {
    spotRange1Start = 0;
    spotRange1End = spotRange0End - LED_COUNT;
  }

  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i >= spotRange0Start && i <= spotRange0End)
    {
      leds[i] = colors[position1];
    }
    else if (i >= spotRange1Start && i <= spotRange1End)
    {
      leds[i] = colors[position1];
    }
    else
    {
      leds[i] = CRGB::Black;
    }
  }
}

/**
 * Small spot mode led drawing loop
 */
void smallSpotModeLoop()
{
  spotModeLoop(SMALL_SPOT_MODE_DELAY, SMALL_SPOT_MODE_SIZE);
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   MEDIUM SPOT MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize nedium spot mode with inital values
 */
void mediumSpotModeInit(boolean fromEeprom)
{
  flatModeInit(fromEeprom);
}

/**
 * Medium spot mode next action
 */
void mediumSpotModeNext()
{
  flatModeNext();
}

/**
 * Medium spot mode previous action
 */
void mediumSpotModePrevious()
{
  flatModePrevious();
}

/**
 * Medium spot mode led drawing loop
 */
void mediumSpotModeLoop()
{
  spotModeLoop(MEDIUM_SPOT_MODE_DELAY, MEDIUM_SPOT_MODE_SIZE);
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   LARGE SPOT MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize large spot mode with inital values
 */
void largeSpotModeInit(boolean fromEeprom)
{
  flatModeInit(fromEeprom);
}

/**
 * Large spot mode next action
 */
void largeSpotModeNext()
{
  flatModeNext();
}

/**
 * Large spot mode previous action
 */
void largeSpotModePrevious()
{
  flatModePrevious();
}

/**
 * Large spot mode led drawing loop
 */
void largeSpotModeLoop()
{
  spotModeLoop(LARGE_SPOT_MODE_DELAY, LARGE_SPOT_MODE_SIZE);
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   TWO COLORS MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize two colors mode with inital values
 */
void twoColorsModeInit(boolean fromEeprom)
{
  if (fromEeprom)
  {
    position1 = readAddress(ADDRESS_DATA_1, FLAT_MODE_DEFAULT_POSITION);
    position1 = readAddress(ADDRESS_DATA_2, FLAT_MODE_DEFAULT_POSITION);
  }
}

/**
 * Two colors mode next action
 */
void twoColorsModeNext()
{
  position1++;
  if (position1 >= lengthOf(colors))
  {
    position1 = 0;
  }
  writeAddress(ADDRESS_DATA_1, position1);
  Serial.print("Position1 Increace: ");
  Serial.println(position1, DEC);
}

/**
 * Two colors mode previous action
 */
void twoColorsModePrevious()
{
  position2++;
  if (position2 >= lengthOf(colors))
  {
    position2 = 0;
  }
  writeAddress(ADDRESS_DATA_2, position2);
  Serial.print("Position2 Increace: ");
  Serial.println(position2, DEC);
}

/**
 * Two colors mode led drawing loop
 */
void twoColorsModeLoop()
{
  int half = LED_COUNT / 2;
  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i >= half)
    {
      leds[i] = colors[position2];
    }
    else
    {
      leds[i] = colors[position1];
    }
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//----   TWO MOVING COLORS MODE
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Initialize two moving colors mode with inital values
 */
void twoMovingColorsModeInit(boolean fromEeprom)
{
  twoColorsModeInit(fromEeprom);
}

/**
 * Two colors mode next action
 */
void twoMovingColorsModeNext()
{
  twoColorsModeNext();
}

/**
 * Two colors mode previous action
 */
void twoMovingColorsModePrevious()
{
  twoColorsModePrevious();
}

/**
 * Two colors mode led drawing loop
 */
void twoMovingColorsModeLoop()
{
  long currentTime = millis();
  if (lastSpotUpdateTime == 0 || (currentTime - lastSpotUpdateTime) >= TWO_MOVING_COLORS_MODE_DELAY)
  {
    lastSpotUpdateTime = currentTime;

    spotPosition++;
    if (spotPosition >= LED_COUNT)
    {
      spotPosition = 0;
    }
  }

  byte spotRange0Start = spotPosition;
  byte spotRange0End = spotPosition + (LED_COUNT / 2);

  byte spotRange1Start = 255;
  byte spotRange1End = 255;

  if (spotRange0End >= LED_COUNT)
  {
    spotRange1Start = 0;
    spotRange1End = spotRange0End - LED_COUNT;
  }

  for (int i = 0; i < LED_COUNT; i++)
  {
    if (i >= spotRange0Start && i <= spotRange0End)
    {
      leds[i] = colors[position1];
    }
    else if (i >= spotRange1Start && i <= spotRange1End)
    {
      leds[i] = colors[position1];
    }
    else
    {
      leds[i] = colors[position2];
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
  else if (currentMode == MODE_SMALL_SPOT)
  {
    smallSpotModeLoop();
  }
  else if (currentMode == MODE_MEDIUM_SPOT)
  {
    mediumSpotModeLoop();
  }
  else if (currentMode == MODE_LARGE_SPOT)
  {
    largeSpotModeLoop();
  }
  else if (currentMode == MODE_TWO_COLORS)
  {
    twoColorsModeLoop();
  }
  else if (currentMode == MODE_TWO_MOVING_COLORS)
  {
    twoMovingColorsModeLoop();
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
  else if (currentMode == MODE_SMALL_SPOT)
  {
    smallSpotModeInit(readEeprom);
  }
  else if (currentMode == MODE_MEDIUM_SPOT)
  {
    mediumSpotModeInit(readEeprom);
  }
  else if (currentMode == MODE_LARGE_SPOT)
  {
    largeSpotModeInit(readEeprom);
  }
  else if (currentMode == MODE_TWO_COLORS)
  {
    twoColorsModeInit(readEeprom);
  }
  else if (currentMode == MODE_TWO_MOVING_COLORS)
  {
    twoMovingColorsModeInit(readEeprom);
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
  else if (currentMode == MODE_SMALL_SPOT)
  {
    smallSpotModePrevious();
  }
  else if (currentMode == MODE_MEDIUM_SPOT)
  {
    mediumSpotModePrevious();
  }
  else if (currentMode == MODE_LARGE_SPOT)
  {
    largeSpotModePrevious();
  }
  else if (currentMode == MODE_TWO_COLORS)
  {
    twoColorsModePrevious();
  }
  else if (currentMode == MODE_TWO_MOVING_COLORS)
  {
    twoMovingColorsModePrevious();
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
  else if (currentMode == MODE_SMALL_SPOT)
  {
    smallSpotModeNext();
  }
  else if (currentMode == MODE_MEDIUM_SPOT)
  {
    mediumSpotModeNext();
  }
  else if (currentMode == MODE_LARGE_SPOT)
  {
    largeSpotModeNext();
  }
  else if (currentMode == MODE_TWO_COLORS)
  {
    twoColorsModeNext();
  }
  else if (currentMode == MODE_TWO_MOVING_COLORS)
  {
    twoMovingColorsModeNext();
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