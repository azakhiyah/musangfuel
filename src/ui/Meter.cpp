#include <Arduino.h>
#include "io/LCD.h"
#include "ui/Meter.h"
#include "config/Config.h"
#include "core/RuntimeStorage.h"

void UI_RenderKFactorInput(const char* input) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("K Factor : ");
    LCD_SetCursor(0, 3);
    LCD_Print(input); // 
}

void UI_RenderCFactorInput(const char* input) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("C Factor : ");
    LCD_SetCursor(0, 3);
    LCD_Print(input); // 
}

void UI_RenderTotaliserInput(const char* input) {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Totaliser : ");
  LCD_SetCursor(0, 3);
  LCD_Print(input); // 
}

void UI_RenderPulserChannelInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Pulser Channel:");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderSDThresholdInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set SD Threshold:");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}



void UI_RenderKFactorShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("KFactor : ");
  char buffer[32];

    snprintf(
        buffer,
        sizeof(buffer),
        "%.10f",
        Config::GetKFactor()
    );

    LCD_SetCursor(0, 3);
    LCD_Print(buffer);
}

void UI_RenderCFactorShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("CFactor : ");
  char buffer[32];

    snprintf(
        buffer,
        sizeof(buffer),
        "%.10f",
        Config::GetCFactor()
    );

    LCD_SetCursor(0, 3);
    LCD_Print(buffer);
}

void UI_RenderTotaliserShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Totaliser : ");
  char buffer[32];

    snprintf(
        buffer,
        sizeof(buffer),
        "%.2f",
        RuntimeStorage::getTotaliser()
    );

    LCD_SetCursor(0, 3);
    LCD_Print(buffer);
}

void UI_RenderSDThresholdShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("SD Threshold: ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%d",
        Config::GetSlowDownThreshold()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderPulserChannelShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Pulser Channel:");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%d",
        Config::GetPulserChannel()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}



void handle_KFactorInput(const char* typed, char preview, const char* output)
{

     char display[20] = {0};

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

    UI_RenderKFactorInput(display);
}

void handle_CFactorInput(const char* typed, char preview, const char* output)
{

     char display[20] = {0};

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


    UI_RenderCFactorInput(display);
}


bool handle_TotaliserInput(char key, char* buffer, uint8_t &len, uint8_t maxLen)
{
    // ===== INPUT ANGKA =====
    if (key >= '0' && key <= '9') {

        if (len < maxLen - 1) {
            buffer[len++] = key;
            buffer[len] = '\0';
        }
    }

    // ===== BACKSPACE =====
    else if (key == 'B') {

        if (len > 0) {
            len--;
            buffer[len] = '\0';
        }
    }

    // ===== SUBMIT =====
    else if (key == '#') {

        return true;
    }

    // ===== RENDER =====
    UI_RenderTotaliserInput(buffer);

    return false;
}

void handle_SDThresholdInput(const char* typed, char preview, const char* output)
{

     char display[21] = {0};

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

    UI_RenderSDThresholdInput(display);
}

void handle_PulserChannelInput(const char* typed, char preview, const char* output)
{

     char display[21] = {0};

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

    UI_RenderPulserChannelInput(display);
}
