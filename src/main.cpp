#include <Arduino.h>
#include <FastLED.h>
#include <OneButton.h>
#include <avr/eeprom.h>

#include "const.h"

CRGB leds[LED_COUNT];

OneButton buttonUp(PIN_UP, true, true);
OneButton buttonDown(PIN_DOWN, true, true);
OneButton buttonPrevious(PIN_LEFT, true, true);
OneButton buttonNext(PIN_RIGHT, true, true);
OneButton buttonOk(PIN_OK, true, true);

CRGB::HTMLColorCode colors[] = {CRGB::White, CRGB::OldLace, CRGB::LightYellow, CRGB::White, CRGB::Wheat, CRGB::WhiteSmoke, CRGB::AliceBlue, CRGB::OldLace, CRGB::Olive, CRGB::Azure, CRGB::Amethyst, CRGB::Gainsboro, CRGB::DarkGoldenrod, CRGB::Gold, CRGB::Red, CRGB::OrangeRed, CRGB::MediumPurple, CRGB::SeaGreen, CRGB::MediumVioletRed, CRGB::Orchid, CRGB::HotPink, CRGB::Honeydew, CRGB::FairyLight, CRGB::Fuchsia, CRGB::SandyBrown, CRGB::SkyBlue, CRGB::Chocolate, CRGB::Cornsilk, CRGB::PaleVioletRed, CRGB::Bisque, CRGB::BurlyWood, CRGB::MidnightBlue, CRGB::FireBrick, CRGB::GreenYellow, CRGB::Khaki, CRGB::LawnGreen, CRGB::LemonChiffon, CRGB::LightSalmon, CRGB::RoyalBlue, CRGB::Thistle, CRGB::Tan, CRGB::Teal, CRGB::LightGoldenrodYellow, CRGB::Indigo, CRGB::Ivory, CRGB::OliveDrab, CRGB::Plum};

int currentMode;
int brightness;

byte position1 = 0;

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

void initFlatMode(boolean fromEeprom)
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

void flatModeNext()
{
  position1++;
  if (position1 >= lengthOf(colors))
  {
    position1 = 0;
  }
  writeAddress(ADDRESS_DATA_1, position1);
  Serial.print("Flat position Increace: ");
  Serial.println(position1, DEC);
}

void flatModePrevious()
{
  position1--;
  if (position1 == -1)
  {
    position1 = lengthOf(colors) - 1;
  }
  writeAddress(ADDRESS_DATA_1, position1);
  Serial.print("Flat position Decreace: ");
  Serial.println(position1, DEC);
}

void flatModeLoop()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = colors[position1];
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

void ledModeLoop()
{
  if (currentMode == MODE_FLAT)
  {
    flatModeLoop();
  }
}

void initCurrentMode(boolean readEeprom)
{
  if (currentMode == MODE_FLAT)
  {
    initFlatMode(readEeprom);
  }
}

//--------------––--------------––--------------––--------------––--------------––--------------––--------------––
//--------------––--------------––--------------––--------------––--------------––--------------––--------------––

/**
 * Function to increace LED brighness
 **/
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
 **/
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
 **/
void previous()
{
  if (currentMode == MODE_FLAT)
  {
    flatModePrevious();
  }
}

/**
 * Function triggers previous change in the current mode
 **/
void next()
{
  if (currentMode == MODE_FLAT)
  {
    flatModeNext();
  }
}

/**
 * Function to switch next color mode
 **/
void modeSwitch()
{
  // Switch modes
}

/**
 * Function to hanle all button ticks
 **/
void buttonTick()
{
  buttonUp.tick();
  buttonDown.tick();
  buttonPrevious.tick();
  buttonNext.tick();
  buttonOk.tick();
}

/**
 * ----------------------------------------------------------------------------------------------------------------
 * Default setup function
 * ----------------------------------------------------------------------------------------------------------------
 **/
void setup()
{
  Serial.begin(9600);

  delay(200);

  FastLED.addLeds<CHIPSET, PIN_LED, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);

  buttonUp.attachClick(increaseBrightness);
  buttonDown.attachClick(decreaseBrightness);
  buttonPrevious.attachClick(previous);
  buttonNext.attachClick(next);
  buttonOk.attachClick(modeSwitch);

  brightness = readAddress(ADDRESS_BRIGHTNESS, BRIGHTNESS_DEFAULT);
  currentMode = readAddress(ADDRESS_MODE, MODE_DEFAULT);
  initCurrentMode(true);
}

/**
 * ----------------------------------------------------------------------------------------------------------------
 * Default loop function
 * ----------------------------------------------------------------------------------------------------------------
 **/
void loop()
{
  buttonTick();
  ledModeLoop();

  FastLED.setBrightness(brightness);
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}