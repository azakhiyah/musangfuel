#include "io/LCD.h"
#include "ui/Receipt.h"
#include "core/RuntimeStorage.h"
#include "config/Config.h"


void UI_RenderSlipNumberInput(const char* input) {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Set Slip Number : ");
  LCD_SetCursor(0, 3);
  LCD_Print(input); // 
}

void UI_RenderHeader1Input(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Header1 :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderHeader2Input(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Header2 :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderFooter1Input(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Footer1 :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderFooter2Input(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Footer2 :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderSlipNumberShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("SlipNumber : ");
  char buffer[32];

    snprintf(
        buffer,
        sizeof(buffer),
        "%.lu",
        RuntimeStorage::getSlip()
    );

    LCD_SetCursor(0, 3);
    LCD_Print(buffer);
}


void UI_RenderHeader1Show() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Header1 : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetHeader()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderHeader2Show() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Header2 : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetHeader2()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderFooter1Show() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Footer1 : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetFooter()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderFooter2Show() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Footer2 : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetFooter2()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

bool handle_SlipNumberInput(char key, char* buffer, uint8_t &len, uint8_t maxLen)
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


    UI_RenderSlipNumberInput(buffer);
    return false;
}


void handle_Header1Input(const char* typed, char preview, const char* output)
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

    UI_RenderHeader1Input(display);
}

void handle_Header2Input(const char* typed, char preview, const char* output)
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

    UI_RenderHeader2Input(display);
}

void handle_Footer1Input(const char* typed, char preview, const char* output)
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

    UI_RenderFooter1Input(display);
}


void handle_Footer2Input(const char* typed, char preview, const char* output)
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

    UI_RenderFooter2Input(display);
}




