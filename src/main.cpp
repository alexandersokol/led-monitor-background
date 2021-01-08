#include <Arduino.h>
#include <FastLED.h>
#include <OneButton.h>

#include "const.h"

CRGB leds[LED_COUNT];

OneButton buttonUp(PIN_UP, true, true);
OneButton buttonDown(PIN_DOWN, true, true);
OneButton buttonPrevious(PIN_LEFT, true, true);
OneButton buttonNext(PIN_RIGHT, true, true);
OneButton buttonOk(PIN_OK, true, true);

int position = 0;
CRGB::HTMLColorCode colors[] = {CRGB::OldLace, CRGB::LightYellow, CRGB::White, CRGB::Wheat, CRGB::WhiteSmoke, CRGB::AliceBlue, CRGB::OldLace, CRGB::Olive, CRGB::Azure, CRGB::Amethyst, CRGB::Gainsboro, CRGB::DarkGoldenrod, CRGB::Gold, CRGB::Red, CRGB::OrangeRed, CRGB::MediumPurple, CRGB::SeaGreen, CRGB::MediumVioletRed, CRGB::Orchid, CRGB::HotPink, CRGB::Honeydew, CRGB::FairyLight, CRGB::Fuchsia, CRGB::SandyBrown, CRGB::SkyBlue, CRGB::Chocolate, CRGB::Cornsilk, CRGB::PaleVioletRed, CRGB::Bisque, CRGB::BurlyWood, CRGB::MidnightBlue, CRGB::FireBrick, CRGB::GreenYellow, CRGB::Khaki, CRGB::LawnGreen, CRGB::LemonChiffon, CRGB::LightSalmon, CRGB::RoyalBlue, CRGB::Thistle, CRGB::Tan, CRGB::Teal, CRGB::LightGoldenrodYellow, CRGB::Indigo, CRGB::Ivory, CRGB::OliveDrab, CRGB::Plum};

void increaseBrightness()
{
  Serial.println("Increase Brightness");
}

void decreaseBrightness()
{
  Serial.println("Decrease Brightness");
}

void previousMode()
{
  position--;
  if (position == -1)
  {
    position = lengthOf(colors) - 1;
  }
  Serial.print("Position Decr: ");
  Serial.println(position, DEC);
}

void nextMode()
{
  position++;
  if (position >= lengthOf(colors))
  {
    position = 0;
  }
  Serial.print("Position Inc: ");
  Serial.println(position, DEC);
}

void okFunc()
{
  Serial.print("Array Length: ");
  Serial.println(lengthOf(colors), DEC);
}

void setup()
{
  Serial.begin(9600);

  delay(2000);

  FastLED.addLeds<CHIPSET, PIN_LED, COLOR_ORDER>(leds, LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LED_BRIGHTNESS);

  buttonUp.attachClick(increaseBrightness);
  buttonDown.attachClick(decreaseBrightness);
  buttonPrevious.attachClick(previousMode);
  buttonNext.attachClick(nextMode);
  buttonOk.attachClick(okFunc);
}

void buttonTick()
{
  buttonUp.tick();
  buttonDown.tick();
  buttonPrevious.tick();
  buttonNext.tick();
  buttonOk.tick();
}

void loop()
{
  buttonTick();

  for (int i = 0; i < LED_COUNT; i++)
  {
    leds[i] = colors[position];
  }

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}