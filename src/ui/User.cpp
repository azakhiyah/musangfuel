#include <LittleFS.h>
#include "io/LCD.h"
#include "config/Config.h"
#include <cstring>
#include <cstdio>

static uint8_t userPage = 0;
static uint8_t selectedRow = 0;
static const uint8_t rowsPerPage = 4;

void UI_RenderUserInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Add User :");
     LCD_SetCursor(0, 1);
    LCD_Print("NM1,Anameu,aBc123,1");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void handle_UserInput(const char* typed, char preview, const char* output)
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

    UI_RenderUserInput(display);
}

// ======================================================
// TOTAL USER
// ======================================================

static uint16_t UI_UserCount()
{
    File file = LittleFS.open("/users.db", "r");

    if (!file) {
        return 0;
    }

    uint16_t count = 0;

    char line[128];

    while (file.available()) {

        size_t len =
            file.readBytesUntil(
                '\n',
                line,
                sizeof(line) - 1
            );

        line[len] = '\0';

        if (strlen(line) > 0) {
            count++;
        }
    }

    file.close();

    return count;
}

// ======================================================
// RESET PAGE
// ======================================================

void UI_ResetUserPage()
{
    userPage = 0;
    selectedRow = 0;
}

// ======================================================
// NEXT PAGE
// ======================================================

void UI_UserNextPage()
{
    uint16_t totalUsers =
        UI_UserCount();

    if ((userPage + 1) * rowsPerPage
        < totalUsers)
    {
        userPage++;

    } else {

        userPage = 0;
    }

    selectedRow = 0;
}

// ======================================================
// CURSOR UP
// ======================================================

void UI_UserPrevSelection()
{
    if (selectedRow > 0) {
        selectedRow--;
    }
}

// ======================================================
// CURSOR DOWN
// ======================================================

void UI_UserNextSelection()
{
    uint16_t totalUsers =
        UI_UserCount();

    uint16_t startIdx =
        userPage * rowsPerPage;

    uint16_t usersInPage =
        totalUsers - startIdx;

    if (usersInPage > rowsPerPage) {
        usersInPage = rowsPerPage;
    }

    if (selectedRow + 1 < usersInPage) {
        selectedRow++;
    }
}

// ======================================================
// DELETE SELECTED USER
// ======================================================

void UI_UserDeleteSelected()
{
    uint16_t targetIndex =
        (userPage * rowsPerPage)
        + selectedRow;

    File src =
        LittleFS.open("/users.db", "r");

    if (!src) {
        return;
    }

    File tmp =
        LittleFS.open("/users.tmp", "w");

    if (!tmp) {

        src.close();
        return;
    }

    char line[128];

    uint16_t currentIndex = 0;

    while (src.available()) {

        size_t len =
            src.readBytesUntil(
                '\n',
                line,
                sizeof(line) - 1
            );

        line[len] = '\0';

        while (len > 0 &&
              (line[len - 1] == '\r' ||
               line[len - 1] == '\n'))
        {
            line[--len] = '\0';
        }

        if (strlen(line) == 0) {
            continue;
        }

        // skip selected
        if (currentIndex != targetIndex) {

            tmp.println(line);
        }

        currentIndex++;
    }

    src.close();
    tmp.close();

    LittleFS.remove("/users.db");

    LittleFS.rename(
        "/users.tmp",
        "/users.db"
    );

    // safety selectedRow
    uint16_t totalUsers =
        UI_UserCount();

    if (totalUsers == 0) {

        userPage = 0;
        selectedRow = 0;
        return;
    }

    uint16_t startIdx =
        userPage * rowsPerPage;

    uint16_t usersInPage =
        totalUsers - startIdx;

    if (usersInPage > rowsPerPage) {
        usersInPage = rowsPerPage;
    }

    if (selectedRow >= usersInPage) {

        if (usersInPage > 0) {
            selectedRow = usersInPage - 1;
        } else {
            selectedRow = 0;
        }
    }
}

// ======================================================
// RENDER
// ======================================================

void UI_RenderUserShow()
{
    LCD_Clear();

    File file =
        LittleFS.open("/users.db", "r");

    if (!file) {

        LCD_SetCursor(0,0);
        LCD_Print("users.db missing");
        return;
    }

    uint16_t startIdx =
        userPage * rowsPerPage;

    uint16_t lineIndex = 0;

    uint8_t row = 0;

    char line[128];

    while (file.available()) {

        size_t len =
            file.readBytesUntil(
                '\n',
                line,
                sizeof(line) - 1
            );

        line[len] = '\0';

        while (len > 0 &&
              (line[len - 1] == '\r' ||
               line[len - 1] == '\n'))
        {
            line[--len] = '\0';
        }

        if (strlen(line) == 0) {
            continue;
        }

        if (lineIndex < startIdx) {

            lineIndex++;
            continue;
        }

        if (row >= rowsPerPage) {
            break;
        }

        char temp[128];

        strncpy(temp,
                line,
                sizeof(temp)-1);

        temp[sizeof(temp)-1] = '\0';

        char* userid =
            strtok(temp, ",");

        char* username =
            strtok(NULL, ",");

        char* password =
            strtok(NULL, ",");

        if (userid && password) {

            char display[21];

            snprintf(display,
                     sizeof(display),
                     "%c%s:%s",
                     (row == selectedRow)
                        ? '>'
                        : ' ',
                     userid,
                     password);

            LCD_SetCursor(0, row);
            LCD_Print(display);

            row++;
        }

        lineIndex++;
    }

    file.close();

  
}
