#pragma once

#include "io/SPITFT.h"

static void DrawHomeLogo(SPITFT& tft);
void UI_TFT_RenderHome(SPITFT& tft);
void UI_TFT_UpdateHomeTime(SPITFT& tft);
void UI_TFT_RenderShift(SPITFT& tft, uint8_t page);
//void UI_TFT_RenderShiftOpenUser(SPITFT& tft,const char* input,bool isPassword);
void UI_TFT_RenderShiftOpenUser(SPITFT& tft,bool isPassword);
void handle_UserIdInput(const char* typed, char preview);
