#include <FastLED.h>
#include <avr/eeprom.h>

#define LED_COUNT 110
#define FRAMES_PER_SECOND 60
#define CHIPSET WS2811
#define COLOR_ORDER GRB

#define BRIGHTNESS_MIN 70
#define BRIGHTNESS_MAX 255
#define BRIGHTNESS_DEFAULT BRIGHTNESS_MAX

#define PIN_UP 6
#define PIN_DOWN 5
#define PIN_LEFT 3
#define PIN_RIGHT 2
#define PIN_OK 4

#define PIN_LED 7

#define ADDRESS_BRIGHTNESS 0
#define ADDRESS_MODE 1
#define ADDRESS_DATA_1 2
#define ADDRESS_DATA_2 3
#define ADDRESS_DATA_3 4

#define NO_DATA 255

#define MODE_FLAT 0
#define MODE_BOTTOM 1
#define MODE_TOP 2
#define MODE_CROSS 3
#define MODE_CORNERS 4
#define MODE_CENTER 5

#define MODE_DEFAULT MODE_FLAT

#define FLAT_MODE_DEFAULT_POSITION 0
#define CORNER_MODE_LED_COUNT 10
#define CENTER_MODE_LED_COUNT 10

#define lengthOf(x) (sizeof(x) / sizeof(x[0]))

byte modes[] = {
    MODE_FLAT,
    MODE_BOTTOM,
    MODE_TOP,
    MODE_CROSS,
    MODE_CORNERS,
    MODE_CENTER,
};

long int colors[] = {
    0xFFE4C4,
    0xFFFFFF,

    0x8B0000,
    0x800000,
    0xA52A2A,
    0xB22222,
    0xCD5C5C,
    0xF08080,
    0xFF0000,
    0xBC8F8F,
    0xFA8072,
    0xFF6347,
    0xCC5533,
    0xE9967A,
    0xFF7F50,
    0xFF4500,
    0xFFA07A,
    0xA0522D,
    0x8B4513,
    0xD2691E,
    0xF4A460,
    0xCD853F,
    0xFF9D2A,
    0xFF8C00,
    0xDEB887,
    0xFFA500,
    0xB8860B,
    0xDAA520,
    0xFFD700,
    0xFFE42D,
    0xF0E68C,
    0xEEE8AA,
    0xBDB76B,
    0xFFFF00,
    0x808000,
    0x6B8E23,
    0x9ACD32,
    0x556B2F,
    0xADFF2F,
    0x7FFF00,
    0x7CFC00,
    0x008000,
    0x006400,
    0x8FBC8F,
    0x90EE90,
    0x00FF00,
    0x32CD32,
    0x228B22,
    0x98FB98,
    0x2E8B57,
    0x3CB371,
    0x00FF7F,
    0x00FA9A,
    0x66CDAA,
    0x7FFFD4,
    0x40E0D0,
    0x20B2AA,
    0x48D1CC,
    0x00FFFF,
    0x008080,
    0xAFEEEE,
    0x00FFFF,
    0x008B8B,
    0x2F4F4F,
    0x00CED1,
    0x5F9EA0,
    0xB0E0E6,
    0xADD8E6,
    0x00BFFF,
    0x87CEEB,
    0x87CEFA,
    0x4682B4,
    0x1E90FF,
    0x6495ED,
    0x4169E1,
    0x0000CD,
    0x0000FF,
    0x00008B,
    0x191970,
    0x000080,
    0x6A5ACD,
    0x483D8B,
    0x7B68EE,
    0x9370DB,
    0x9966CC,
    0x8A2BE2,
    0x4B0082,
    0x9932CC,
    0x9400D3,
    0xBA55D3,
    0x800080,
    0xDDA0DD,
    0xEE82EE,
    0xFF00FF,
    0x8B008B,
    0xDA70D6,
    0xC71585,
    0xFF1493,
    0xFF69B4,
    0xDB7093,
    0xDC143C,
    0xFFC0CB,
    0xFFB6C1,

    0xDCDCDC,
    0xC0C0C0,
    0xFFFAFA,
    0xF5F5F5,
    0xD3D3D3,
    0xFFE4E1,
    0xFFF5EE,
    0xFFDAB9,
    0xFAF0E6,
    0xD2B48C,
    0xFAEBD7,
    0xFFDEAD,
    0xFFEBCD,
    0xFFEFD5,
    0xFFE4B5,
    0xF5DEB3,
    0xFDF5E6,
    0xFFFAF0,
    0xFFF8DC,
    0xFFFACD,
    0xF5F5DC,
    0xFAFAD2,
    0xFFFFE0,
    0xFFFFF0,
    0xF0FFF0,
    0xF5FFFA,
    0xE0FFFF,
    0xF0FFFF,
    0xF0F8FF,
    0xB0C4DE,
    0xE6E6FA,
    0xF8F8FF,
    0xD8BFD8,
    0xFFF0F5,
};

void writeAddress(byte address, byte data)
{
    eeprom_write_byte(address, data);
}

byte readAddress(byte address, byte defaultValue)
{
    byte data = eeprom_read_byte(address);
    if (data == NO_DATA)
    {
        return defaultValue;
    }
    return data;
}