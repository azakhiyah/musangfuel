#include <Arduino.h>
#include "io/LCD.h"
#include "ui/OperationMenu.h"    
#include "services/TimeService.h"
#include "config/Config.h"
#include "core/RuntimeStorage.h"


// ==========================
// HOME
// ==========================
void UI_RenderHome() {

    // ===== App Name (center) =====
    const char* appName = Config::GetAppName();

    int len = strlen(appName);
    if (len > 20) len = 20;

    int startCol = (20 - len) / 2;

    LCD_SetCursor(startCol, 0);

    for (int i = 0; i < len; i++) {
        char c[2] = {appName[i], '\0'};
        LCD_Print(c);
    }

    // ===== Time =====
    DateTimeSimple t = Time_GetNow();

    char buffer[21];

    snprintf(buffer, sizeof(buffer),
             "%02d-%02d-%04d %02d:%02d",
             t.day, t.month, t.year,
             t.hour, t.minute);

    LCD_SetCursor(2, 2);
    LCD_Print(buffer);

    // ===== Footer =====
    LCD_SetCursor(0, 3);
    LCD_Print("Press D to Continue");
}

// ==========================
// SHIFT
// ==========================

const char *shiftItems[] = {
  "1.Open Shift",
  "2.Close Shift"
};

uint8_t SHIFT_ITEM_COUNT = sizeof(shiftItems) / sizeof(shiftItems[0]);

void UI_RenderShift(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < SHIFT_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(shiftItems[itemIdx]);
    }
  }
}

// ==========================
// REPRINT
// ==========================

const char *reprintItems[] = {
  "1.Last TRN",
  "2.Current Shift"
};

uint8_t REPRINT_ITEM_COUNT = sizeof(reprintItems) / sizeof(reprintItems[0]);

void UI_RenderReprint(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < REPRINT_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(reprintItems[itemIdx]);
    }
  }
}

void handle_UserIdInput(const char* typed, char preview)
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

    UI_RenderShiftInputUserId(display);
}

void handle_PasswordInput(const char* typed, char preview)
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

    UI_RenderShiftInputPassword(display);
}

void handle_CalibrationPasswordInput(const char* typed, char preview)
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

    UI_RenderCalibrationPasswordInput(display);
}

void handle_UnitNoInput(const char* typed, char preview)
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

    UI_RenderInputUnitNo(display);
}

bool handle_OdometerInput(char key, char* buffer, uint8_t &len, uint8_t maxLen)
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


    UI_RenderInputOdometer(buffer);
    return false;
  
}

bool handle_PresetInput(char key, char* buffer, uint8_t &len, uint8_t maxLen)
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
         // jika kosong -> default unlimited
        if (len == 0) {

            strcpy(buffer, "999999");

            len = strlen(buffer);
        }

        return true;
    }

    UI_RenderInputPreset(buffer);
    return false;

  
}

bool handle_PulserDisplay(PulserEncoder &pulser, uint32_t preset)
{   
    double volume = pulser.getVolume();
    double flow   = pulser.getLPM();
    uint32_t now  = millis();

    static uint32_t lastFlowChangeTime = 0;
    static double lastFlow = 0;
    static bool started = false;

    const uint32_t flowTimeout = 20000;

    // 🔥 deteksi mulai flow
    if (flow > 0.1) {
        started = true;
    }

    // 🔥 deteksi perubahan flow
    if (abs(flow - lastFlow) > 0.01) {
        lastFlowChangeTime = now;
        lastFlow = flow;
    }

    bool selesai = false;

    if (preset != 999999) {

        if (!started) {
            selesai = false;
        }
        else {
            selesai = (volume >= preset) ||
                      (now - lastFlowChangeTime >= flowTimeout);
        }

    } else {
        selesai = started && (now - lastFlowChangeTime >= flowTimeout);
    }

    return selesai;
}

void UI_RenderPulserDisplay(float volume, float flow) {

    LCD_SetCursor(0, 0);
    LCD_Print("Vol:");
    LCD_PrintFloat(volume, 1);
    LCD_Print("L     ");

    LCD_SetCursor(0, 2);
    LCD_Print("Flow:");
    LCD_PrintFloat(flow, 1);
    LCD_Print(" LPM  ");

}

void UI_RenderShiftInputUserId(const char* input) {

    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("User Id :");

    LCD_SetCursor(0, 1);
    LCD_Print("                    "); // clear line
    LCD_SetCursor(0, 1);
    LCD_Print(input);
}

void UI_RenderShiftInputPassword(const char* input) {

    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Password :");
    LCD_SetCursor(0, 1);
    LCD_Print("                    "); // clear line
    LCD_SetCursor(0, 1);
    LCD_Print(input);
}

void UI_RenderCalibrationPasswordInput(const char* input) {
   
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Super Password :");
    LCD_SetCursor(0, 1);
    LCD_Print("                    "); // clear line
    LCD_SetCursor(0, 1);
    LCD_Print(input);
}

void UI_RenderInputUnitNo(const char* input) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Unit No : ");
    LCD_SetCursor(0, 3);
    LCD_Print(input); // 
}

void UI_RenderCheckInputUnitNo() {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Unit No : ");
    LCD_SetCursor(0, 3);
    LCD_Print(RuntimeStorage::Transaction_GetUnit()); // 
    LCD_SetCursor(18, 3);
    LCD_Print("OK"); // 
}

void UI_RenderFinish() {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Transaction Complete");
    LCD_SetCursor(0, 3);
    LCD_Print("Press D to Continue"); 
}

void UI_RenderInputOdometer(const char* input) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("HM/KM : ");
    LCD_SetCursor(0, 3);
    LCD_Print(input); // 
}

void UI_RenderCheckInputOdometer() {

    char buffer[16]; // cukup untuk uint32
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("HM/KM : ");
    LCD_SetCursor(0, 3);
     // convert uint32 → string
    sprintf(buffer, "%lu", RuntimeStorage::Transaction_GetOdometer());

    LCD_Print(buffer); // 
    LCD_SetCursor(18, 3);
    LCD_Print("OK"); // 
}

void UI_RenderInputPreset(const char* input) {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Preset Volume : ");
  LCD_SetCursor(0, 3);
  LCD_Print(input); 
}

void UI_RenderCheckInputPreset() {

    char buffer[16]; // cukup untuk uint32
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("Preset : ");
    LCD_SetCursor(0, 3);
     // convert uint32 → string
    sprintf(buffer, "%lu", RuntimeStorage::Transaction_GetPresetVolume());

    LCD_Print(buffer); // 
    LCD_SetCursor(18, 3);
    LCD_Print("OK"); // 
}

// =========================
// CONFIG SCREEN BLE
// ==========================
void UI_RenderConfig() {

    LCD_SetCursor(0, 0);
    LCD_Print("=== CONFIG MODE ===");

    LCD_SetCursor(0, 1);
    LCD_Print("BLE: ACTIVE");

    LCD_SetCursor(0, 2);
    LCD_Print("App:");

    LCD_SetCursor(5, 2);
    LCD_Print(Config::GetAppName());

    LCD_SetCursor(0, 3);
    LCD_Print("Wait BLE / Save");
}

// ==========================
// UPDATE CONFIG (no flicker)
// ==========================
void UI_UpdateConfigDynamic() {

    LCD_SetCursor(5, 2);
    LCD_Print("               "); // clear area

    LCD_SetCursor(5, 2);
    LCD_Print(Config::GetAppName());
}

void showError(const char* message) {
  LCD_SetCursor(0, 0);
  LCD_Print(message);
}

void showCorrect(const char* message) {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print(message);
}

void UI_RenderTechnitionMenu() {
    LCD_Clear();
    LCD_SetCursor(0,0);
    LCD_Print("1.Set Conf");
    LCD_SetCursor(0,1);
    LCD_Print("2.Show Conf");
    LCD_SetCursor(0,2);
    LCD_Print("*.Back");;
}


void UI_RenderTransactionMenu() {
    LCD_Clear();
    LCD_SetCursor(0,0);
    LCD_Print("1.Input Unit No");
    LCD_SetCursor(0,1);
    LCD_Print("2.Log Out");
    LCD_SetCursor(0,2);
    LCD_Print("3.Reprint");
}