#ifndef DEVICE_STATUS_H
#define DEVICE_STATUS_H
#include <cstdint>


//Device Status Input Menu
/*
1.Rfid
2.Modem
3.GPS STatus
*/

void UI_RenderRfidStatusInput(const char* input);
void UI_RenderGpsStatusInput(const char* input);
void UI_RenderModemStatusInput(const char* input);

void handle_RfidStatusInput(const char* typed, char preview, const char* output);
void handle_GpsStatusInput(const char* typed, char preview, const char* output);
void handle_ModemStatusInput(const char* typed, char preview, const char* output);

void UI_RenderRfidStatusShow();
void UI_RenderGpsStatusShow();
void UI_RenderModemStatusShow();



#endif