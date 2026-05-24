#include <cstdint>
#ifndef OPERATION_MENU_H
#define OPERATION_MENU_H
#include "io/PulserEncoder.h"


void UI_RenderHome();
void UI_RenderTechnitionMenu();

//Menu Operational
void UI_UpdateConfigDynamic();
void UI_RenderShift(u_int8_t page);
void UI_RenderReprint(u_int8_t page);

//Menu Credential
void UI_RenderShiftInputUserId(const char* input);
void UI_RenderShiftInputPassword(const char* input);
void UI_RenderCalibrationPasswordInput(const char* input);

void handle_UserIdInput(const char* typed, char preview);
void handle_PasswordInput(const char* typed, char preview);
void handle_CalibrationPasswordInput(const char* typed, char preview);


//Menu Transaksi
void UI_RenderTransactionMenu();
void UI_RenderInputUnitNo(const char* input);
void UI_RenderCheckInputUnitNo();
void UI_RenderInputOdometer(const char* input);
void UI_RenderCheckInputOdometer();
void UI_RenderInputPreset(const char* input);
void UI_RenderCheckInputPreset();
void UI_RenderPulserDisplay(float volume, float flow);
void UI_RenderFinish();

void handle_UnitNoInput(const char* typed, char preview);
bool handle_OdometerInput(char key, char* buffer, uint8_t &len, uint8_t maxLen);
bool handle_PresetInput(char key, char* buffer, uint8_t &len, uint8_t maxLen);
bool handle_PulserDisplay(PulserEncoder &pulser, uint32_t preset);

//General Message
void showError(const char* input);
void showCorrect(const char* input);

//BLE
void UI_RenderConfig();




#endif