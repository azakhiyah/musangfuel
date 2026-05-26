#include <cstdint>
#ifndef CONFIG_MENU_H
#define CONFIG_MENU_H

extern uint8_t KALIBRASI_ITEM_COUNT;
extern uint8_t PANEL_ITEM_COUNT; 
extern uint8_t STRUCK_ITEM_COUNT;
extern uint8_t NET_ITEM_COUNT;
extern uint8_t METER_ITEM_COUNT;
extern uint8_t DEVICE_STATUS_ITEM_COUNT;
extern uint8_t DEVICE_ITEM_COUNT;
extern uint8_t USER_ITEM_COUNT;

void UI_RenderCalibMenu(uint8_t page);

//Config Menu
void UI_RenderPanelMenu(uint8_t page);
void UI_RenderStruckMenu(uint8_t page);
void UI_RenderNetMenu(uint8_t page);
void UI_RenderMeterMenu(uint8_t page);
void UI_RenderDeviceStatusrMenu(uint8_t page);
void UI_RenderDeviceMenu(uint8_t page);
void UI_RenderUserMenu(uint8_t page);

#endif