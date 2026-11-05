
#include <Arduino.h>
#include <string.h>

#include "ui/tft/UIOperation.h"
#include "config/Config.h"
#include "services/TimeService.h"

// =====================================================
// LOGO FUEL PUMP
// =====================================================

static void DrawHomeLogo(SPITFT& tft)
{
    // Body pompa
    tft.drawRect(30, 22, 55, 72, TFT_YELLOW);

    // Display pompa
    tft.drawRect(39, 31, 37, 18, TFT_YELLOW);

    // Detail indikator
    tft.fillRect(44, 36, 27, 3, TFT_YELLOW);

    // Bagian bawah pompa
    tft.fillRect(24, 94, 67, 7, TFT_YELLOW);

    // Selang pompa
    tft.drawLine(85, 32, 99, 42, TFT_YELLOW);
    tft.drawLine(99, 42, 99, 78, TFT_YELLOW);
    tft.drawLine(99, 78, 88, 88, TFT_YELLOW);

    // Nozzle
    tft.drawLine(85, 32, 94, 24, TFT_YELLOW);
    tft.drawLine(94, 24, 103, 32, TFT_YELLOW);

    // Garis pemisah logo dan nama aplikasi
    tft.fillRect(115, 20, 3, 85, TFT_YELLOW);
}

// =====================================================
// HOME PAGE
// =====================================================

void UI_TFT_RenderHome(SPITFT& tft)
{
    tft.clear(TFT_BLACK);

    // ===== LOGO =====
    DrawHomeLogo(tft);

    // ===== APPLICATION NAME =====

    tft.setTextSize(3);

   // Lebar tulisan MUSANGFUEL:
    // 11 karakter x 18 pixel = 198 pixel
    int appNameWidth = 11 * 18;
    int appNameX = 290 - (appNameWidth / 2);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(appNameX, 35);
    tft.print("MUSANG");

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print("FUEL");

    // ===== SUBTITLE =====

    const char* subtitle = "FILLING CONTROLLER";

    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    // Font size 1: perkiraan 6 pixel per karakter
    int subtitleWidth = strlen(subtitle) * 13;
    int subtitleX = 290 - (subtitleWidth / 2);

    tft.setCursor(subtitleX, 78);
    tft.print(subtitle);

    // ===== HEADER DIVIDER =====

    tft.fillRect(25, 115, 430, 2, TFT_YELLOW);

    // ===== DATE & TIME PANEL =====

    tft.fillRect(30, 135, 420, 80, 0x18E3);
    tft.drawRect(30, 135, 420, 80, TFT_YELLOW);

    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW, 0x18E3);
    tft.setCursor(195, 145);
    tft.print("DATE & TIME");

    // Tampilkan waktu pertama kali
    UI_TFT_UpdateHomeTime(tft);

    // ===== COMPANY NAME =====

    const char* company = "PT. MUSANG TEKNOLOGI NUSANTARA";

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    int companyWidth = strlen(company) * 6;
    int companyX = (tft.width() - companyWidth) / 2;

    tft.setCursor(companyX, 235);
    tft.print(company);

    // ===== FOOTER =====

    tft.fillRect(60, 270, 360, 35, TFT_YELLOW);

    const char* footer = "PRESS D TO CONTINUE";

    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK, TFT_YELLOW);

    int footerWidth = strlen(footer) * 12;
    int footerX = (tft.width() - footerWidth) / 2;

    tft.setCursor(footerX, 280);
    tft.print(footer);
}

// =====================================================
// UPDATE DATE & TIME
// =====================================================

void UI_TFT_UpdateHomeTime(SPITFT& tft)
{
    // Gunakan TimeService yang sudah ada
    DateTimeSimple now = Time_GetNow();

    char buffer[24];

    snprintf(
        buffer,
        sizeof(buffer),
        "%02d-%02d-%04d %02d:%02d",
        now.day,
        now.month,
        now.year,
        now.hour,
        now.minute
    );

    // Bersihkan area waktu saja
    tft.fillRect(35, 165, 410, 35, 0x18E3);

    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, 0x18E3);

    int textWidth = strlen(buffer) * 12;
    int x = (tft.width() - textWidth) / 2;

    tft.setCursor(x, 175);
    tft.print(buffer);
}

// =====================================================
// SHIFT Management
// =====================================================

static const char* shiftItems[] = {
    "1. Open Shift",
    "2. Close Shift"
};

static const uint8_t SHIFT_ITEM_COUNT =
    sizeof(shiftItems) / sizeof(shiftItems[0]);

void UI_TFT_RenderShift(SPITFT& tft, uint8_t page)
{
    tft.clear(TFT_BLACK);

    // ===== HEADER =====

    tft.setTextSize(2);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(20, 20);
    tft.print("MUSANGFUEL");

    tft.fillRect(20, 55, 440, 2, TFT_YELLOW);

    // ===== PAGE TITLE =====

    const char* title = "SHIFT MANAGEMENT";

    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    int titleWidth = strlen(title) * 12;
    int titleX = (tft.width() - titleWidth) / 2;

    tft.setCursor(titleX, 85);
    tft.print(title);

    // ===== MENU ITEMS =====

    const uint8_t itemsPerPage = 4;
    uint8_t startIdx = page * itemsPerPage;

    for (uint8_t i = 0; i < itemsPerPage; i++)
    {
        uint8_t itemIdx = startIdx + i;

        if (itemIdx < SHIFT_ITEM_COUNT)
        {
            int y = 145 + (i * 55);

            tft.drawRect(55, y, 370, 42, TFT_YELLOW);

            tft.setTextSize(2);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setCursor(80, y + 12);

            tft.print(shiftItems[itemIdx]);
        }
    }

    // ===== FOOTER =====

    tft.setTextSize(1);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(150, 285);
    tft.print("Use keypad to select");
}

// =====================================================
// OPEN SHIFT - USER ID / PASSWORD INPUT
// =====================================================

// void UI_TFT_RenderShiftOpenUser(
//     SPITFT& tft,
//     bool isPassword
// )
// {
//     tft.clear(TFT_BLACK);

//     // HEADER
//     tft.setTextSize(2);
//     tft.setTextColor(TFT_YELLOW, TFT_BLACK);
//     tft.setCursor(20, 20);
//     tft.print("MUSANGFUEL");

//     tft.setTextSize(1);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//     tft.setCursor(350, 27);
//     tft.print("OPEN SHIFT");

//     tft.fillRect(20, 55, 440, 2, TFT_YELLOW);

//     // PAGE TITLE
//     tft.setTextSize(2);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//     tft.setCursor(110, 85);
//     tft.print("SHIFT AUTHENTICATION");

//     // INPUT LABEL
//     tft.setTextSize(2);
//     tft.setTextColor(TFT_YELLOW, TFT_BLACK);
//     tft.setCursor(50, 145);

//     if (isPassword) {
//         tft.print("Password :");
//     } else {
//         tft.print("User DONK :");
//     }

//     // INPUT BOX
//     tft.drawRect(45, 180, 390, 55, TFT_WHITE);

//     tft.setTextSize(2);
//     tft.setTextColor(TFT_GREEN, TFT_BLACK);
//     tft.setCursor(60, 198);

//     // if (input != nullptr) {
//     //     if (isPassword) {
//     //         // Password ditampilkan sebagai karakter tersembunyi
//     //         for (size_t i = 0; input[i] != '\0'; i++) {
//     //             tft.print("*");
//     //         }
//     //     } else {
//     //         tft.print(input);
//     //     }
//     // }

//     // FOOTER
//     tft.setTextSize(1);
//     tft.setTextColor(TFT_YELLOW, TFT_BLACK);
//     tft.setCursor(50, 285);
//     tft.print("ENTER: CONFIRM");

//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//     tft.setCursor(260, 285);
//     tft.print("*: BACK");
// }

// void handle_UserIdInput(const char* typed, char preview)
// {
//     char display[20] = {0};

//     // Copy karakter yang sudah dikonfirmasi
//     strncpy(display, typed, sizeof(display) - 1);

//     // Tambahkan karakter preview multitap
//     if (preview != '\0') {
//         uint8_t len = strlen(display);

//         if (len < sizeof(display) - 1) {
//             display[len] = preview;
//             display[len + 1] = '\0';
//         }
//     }

//     // Simpan hasil untuk dibaca TaskTFT
//     strncpy(
//         inputUserIdDisplay,
//         display,
//         sizeof(inputUserIdDisplay) - 1
//     );

//     inputUserIdDisplay[sizeof(inputUserIdDisplay) - 1] = '\0';
// }
