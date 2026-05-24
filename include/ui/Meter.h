#include <cstdint>
#ifndef METER_H
#define METER_H

//Meter Input Menu Input
void UI_RenderKFactorInput(const char* input);
void UI_RenderCFactorInput(const char* input);
//SD Threshold
//Pulser Channel
void UI_RenderTotaliserInput(const char* input);
void UI_RenderPulserChannelInput(const char* input);
void UI_RenderSDThresholdInput(const char* input);

void UI_RenderKFactorShow();
void UI_RenderCFactorShow();
void UI_RenderTotaliserShow();
void UI_RenderPulserChannelShow();
void UI_RenderSDThresholdShow();

void handle_KFactorInput(const char* typed, char preview, const char* output);
void handle_CFactorInput(const char* typed, char preview, const char* output);
bool handle_TotaliserInput(char key, char* buffer, uint8_t &len, uint8_t maxLen);
void handle_PulserChannelInput(const char* typed, char preview, const char* output);
void handle_SDThresholdInput(const char* typed, char preview, const char* output);



#endif