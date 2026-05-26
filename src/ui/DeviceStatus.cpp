#include "io/LCD.h"
#include "config/Config.h"
#include <cstring>
#include <cstdio>

#ifdef ENABLE_NETWORK
#include "services/NetworkService.h"
#endif

void UI_RenderRfidStatusInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Rfid Status :");

    LCD_SetCursor(0, 3);

    char buffer[8] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}


void UI_RenderGpsStatusInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Gps Status :");

    LCD_SetCursor(0, 3);

    char buffer[8] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderModemStatusInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Modem Status :");

    LCD_SetCursor(0, 3);

    char buffer[8] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderGpsStatusShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Gps Status : ");

  char buffer[8];

    snprintf(
        buffer,
        sizeof(buffer),
        "%d",
        Config::GetGPSStatus()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderRfidStatusShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Rfid Status : ");

  char buffer[8];

    snprintf(
        buffer,
        sizeof(buffer),
        "%d",
        Config::GetRFIDStatus()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderModemStatusShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Modem Status : ");

  char buffer[8];

    snprintf(
        buffer,
        sizeof(buffer),
        "%d",
        Config::GetModemStatus()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderWifiStatusShow()
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("WiFi Status");

    LCD_SetCursor(0, 1);
    #ifdef ENABLE_NETWORK
    if (!network.isWiFiConnected())
    {
        LCD_Print("Disconnected");

        delay(2000);

        return;
    }

    LCD_Print(
        network.getIPAddress()
    );

    LCD_SetCursor(0, 2);

    LCD_Print(
        network.getMACAddress()
    );
    #endif

    // LCD_SetCursor(0, 3);

    // LCD_Print(
    //     network.getSSID()
    // );
}



void handle_GpsStatusInput(const char* typed, char preview, const char* output)
{

     char display[8] = {0};

    // copy typed
    strncpy(display, typed, sizeof(display) - 1);

    // tambah preview
    if (preview != '\0') {
        uint8_t len = strlen(display);
        if (len < sizeof(display) - 1) {
            display[len] = preview;
            display[len + 1] = '\0';
        }
    }

    UI_RenderGpsStatusInput(display);
}

void handle_ModemStatusInput(const char* typed, char preview, const char* output)
{

     char display[8] = {0};

    // copy typed
    strncpy(display, typed, sizeof(display) - 1);

    // tambah preview
    if (preview != '\0') {
        uint8_t len = strlen(display);
        if (len < sizeof(display) - 1) {
            display[len] = preview;
            display[len + 1] = '\0';
        }
    }

    UI_RenderModemStatusInput(display);
}

void handle_RfidStatusInput(const char* typed, char preview, const char* output)
{

     char display[8] = {0};

    // copy typed
    strncpy(display, typed, sizeof(display) - 1);

    // tambah preview
    if (preview != '\0') {
        uint8_t len = strlen(display);
        if (len < sizeof(display) - 1) {
            display[len] = preview;
            display[len + 1] = '\0';
        }
    }

    UI_RenderRfidStatusInput(display);
}






