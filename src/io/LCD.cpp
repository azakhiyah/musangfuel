#include "io/LCD.h"
#include <LiquidCrystal_I2C.h>

// alamat 0x27, 20x4
static LiquidCrystal_I2C lcd(0x27, 20, 4);

void LCD_Init() {
    lcd.init();
    lcd.backlight();
}

void LCD_Clear() {
    lcd.clear();
}

void LCD_SetCursor(int col, int row) {
    lcd.setCursor(col, row);
}

void LCD_Print(const char* text) {
    lcd.print(text);
}

void LCD_PrintFloat(float value, uint8_t decimal) {
    lcd.print(value, decimal);
}