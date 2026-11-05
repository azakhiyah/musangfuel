#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class SPITFT {
public:

    // Initialization
    void begin(uint8_t rotation = 1);

    // Display control
    void clear(uint16_t color = TFT_BLACK);
    void setRotation(uint8_t rotation);

    // Text control
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t foreground, uint16_t background);
    void setTextSize(uint8_t size);

    // Text output
    size_t print(const char* text);
    size_t print(const String& text);
    size_t print(float value, uint8_t decimal = 2);
    size_t print(int32_t value);

    // Graphics
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawLine(
        int16_t x0, int16_t y0,
        int16_t x1, int16_t y1,
        uint16_t color
    );

    void drawRect(
        int16_t x, int16_t y,
        int16_t width, int16_t height,
        uint16_t color
    );

    void fillRect(
        int16_t x, int16_t y,
        int16_t width, int16_t height,
        uint16_t color
    );

    // Screen information
    int16_t width();
    int16_t height();

private:
    TFT_eSPI display;
};