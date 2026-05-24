#include <cstdint>
#ifndef RECEIPT_H
#define RECEIPT_H

//Struck Input Menu
//Header1
//Header2
//Footer1
//Footer2
void UI_RenderSlipNumberInput(const char* input);
void UI_RenderHeader1Input(const char* input);
void UI_RenderHeader2Input(const char* input);
void UI_RenderFooter1Input(const char* input);
void UI_RenderFooter2Input(const char* input);


void UI_RenderSlipNumberShow();
void UI_RenderHeader1Show();
void UI_RenderHeader2Show();
void UI_RenderFooter1Show();
void UI_RenderFooter2Show();

bool handle_SlipNumberInput(char key, char* buffer, uint8_t &len, uint8_t maxLen);
void handle_Header1Input(const char* typed, char preview, const char* output);
void handle_Header2Input(const char* typed, char preview, const char* output);
void handle_Footer1Input(const char* typed, char preview, const char* output);
void handle_Footer2Input(const char* typed, char preview, const char* output);
#endif