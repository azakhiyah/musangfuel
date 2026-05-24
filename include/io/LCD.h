#pragma once
#include <stdint.h>

void LCD_Init();
void LCD_Clear();
void LCD_SetCursor(int col, int row);
void LCD_Print(const char* text);
void LCD_PrintFloat(float value, uint8_t decimal);