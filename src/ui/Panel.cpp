#include "io/LCD.h"
#include "services/TimeService.h"
#include "config/Config.h"

static char dateTime[13] = {0}; // 12 digit + null terminator
static uint8_t dateTimeLen = 0;

void UI_RenderDateTimeInput()
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set date time");

    LCD_SetCursor(4, 1);
    LCD_Print("DDMMYYYYHHMM");

    LCD_SetCursor(4, 2);
    LCD_Print(dateTime);
}

void UI_RenderAppNameInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set AppName :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderIdPanelInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Id Panel :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderCountryCodeInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Country Code :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderCompanyCodeInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Company Code :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}


void UI_RenderLocationInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Location :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}






void UI_RenderDateTimeShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Date Time : ");
  DateTimeSimple t = Time_GetNow();

    char buffer[21];

    snprintf(buffer, sizeof(buffer),
             "%02d-%02d-%04d %02d:%02d",
             t.day, t.month, t.year,
             t.hour, t.minute);

    LCD_SetCursor(0, 3);
    LCD_Print(buffer);
}

void UI_RenderAppNameShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("App Name : ");
  // ===== App Name (center) =====
    const char* appName = Config::GetAppName();

    int len = strlen(appName);
    if (len > 20) len = 20;

    int startCol = (20 - len) / 2;

    LCD_SetCursor(startCol, 3);

    for (int i = 0; i < len; i++) {
        char c[2] = {appName[i], '\0'};
        LCD_Print(c);
    }
}

void UI_RenderIdPanelShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Id Panel : ");

  char buffer[8];

    snprintf(
        buffer,
        sizeof(buffer),
        "%u",
        Config::GetPanelId()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderCountryCodeShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Country Code : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetCountryCode()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderCompanyCodeShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Company Code : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetCompanyCode()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}


void UI_RenderLocationShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Country Code : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetLocation()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}


bool handle_DateTimeInput(char key)
{
    // ===== INPUT ANGKA =====
    if (key >= '0' && key <= '9')
    {
        if (dateTimeLen < 12)
        {
            dateTime[dateTimeLen++] = key;
            dateTime[dateTimeLen] = '\0';
        }
    }

    // ===== BACKSPACE =====
    else if (key == 'B')
    {
        if (dateTimeLen > 0)
        {
            dateTimeLen--;
            dateTime[dateTimeLen] = '\0';
        }
    }

    // ===== SUBMIT =====
    else if (key == '#')
    {
        if (dateTimeLen == 12)
        {
            char temp[5];

            // DD
            memcpy(temp, &dateTime[0], 2);
            temp[2] = '\0';
            int day = atoi(temp);

            // MM
            memcpy(temp, &dateTime[2], 2);
            temp[2] = '\0';
            int month = atoi(temp);

            // YYYY
            memcpy(temp, &dateTime[4], 4);
            temp[4] = '\0';
            int year = atoi(temp);

            // HH
            memcpy(temp, &dateTime[8], 2);
            temp[2] = '\0';
            int hour = atoi(temp);

            // MM
            memcpy(temp, &dateTime[10], 2);
            temp[2] = '\0';
            int minute = atoi(temp);

            LCD_SetCursor(18, 3);
            LCD_Print("OK");
            setDateTime(DateTime(
                year,
                month,
                day,
                hour,
                minute,
                0
            ));

            // reset buffer
            memset(dateTime, 0, sizeof(dateTime));
            dateTimeLen = 0;

            return true;
        }
    }

    UI_RenderDateTimeInput();

    return false;
}

void handle_AppNameInput(const char* typed, char preview, const char* output)
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

    UI_RenderAppNameInput(display);
}


void handle_IdPanelInput(const char* typed, char preview, const char* output)
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

    UI_RenderIdPanelInput(display);
}



void handle_CountryCodeInput(const char* typed, char preview, const char* output)
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

    UI_RenderCountryCodeInput(display);
}

void handle_CompanyCodeInput(const char* typed, char preview, const char* output)
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

    UI_RenderCompanyCodeInput(display);
}

void handle_LocationInput(const char* typed, char preview, const char* output)
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

    UI_RenderLocationInput(display);
}




