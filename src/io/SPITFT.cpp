#include "io/SPITFT.h"

// ==========================
// INITIALIZATION
// ==========================

void SPITFT::begin(uint8_t rotation)
{
    display.init();

    display.setRotation(rotation);

    display.fillScreen(TFT_BLACK);

    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(1);
    display.setTextWrap(true);
}

// ==========================
// DISPLAY CONTROL
// ==========================

void SPITFT::clear(uint16_t color)
{
    display.fillScreen(color);
}

void SPITFT::setRotation(uint8_t rotation)
{
    display.setRotation(rotation);
}

// ==========================
// TEXT CONTROL
// ==========================

void SPITFT::setCursor(int16_t x, int16_t y)
{
    display.setCursor(x, y);
}

void SPITFT::setTextColor(
    uint16_t foreground,
    uint16_t background
)
{
    display.setTextColor(foreground, background);
}

void SPITFT::setTextSize(uint8_t size)
{
    display.setTextSize(size);
}

// ==========================
// TEXT OUTPUT
// ==========================

size_t SPITFT::print(const char* text)
{
    if (text == nullptr) {
        return 0;
    }

    return display.print(text);
}

size_t SPITFT::print(const String& text)
{
    return display.print(text);
}

size_t SPITFT::print(float value, uint8_t decimal)
{
    return display.print(value, decimal);
}

size_t SPITFT::print(int32_t value)
{
    return display.print(value);
}

// ==========================
// GRAPHICS
// ==========================

void SPITFT::drawPixel(
    int16_t x,
    int16_t y,
    uint16_t color
)
{
    display.drawPixel(x, y, color);
}

void SPITFT::drawLine(
    int16_t x0, int16_t y0,
    int16_t x1, int16_t y1,
    uint16_t color
)
{
    display.drawLine(x0, y0, x1, y1, color);
}

void SPITFT::drawRect(
    int16_t x, int16_t y,
    int16_t width, int16_t height,
    uint16_t color
)
{
    display.drawRect(x, y, width, height, color);
}

void SPITFT::fillRect(
    int16_t x, int16_t y,
    int16_t width, int16_t height,
    uint16_t color
)
{
    display.fillRect(x, y, width, height, color);
}

// ==========================
// SCREEN INFORMATION
// ==========================

int16_t SPITFT::width()
{
    return display.width();
}

int16_t SPITFT::height()
{
    return display.height();
}