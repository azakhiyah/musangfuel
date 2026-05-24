#include <Arduino.h>
#include "io/LCD.h"
#include "ui/ConfigMenu.h"

const char *kalibrasiItems[] = {
  "1.Panel Config",
  "2.Struck Config",
  "3.Net Config",
  "4.Meter Config",
  "5.Device Status",
  "6.User",
};

uint8_t KALIBRASI_ITEM_COUNT = sizeof(kalibrasiItems) / sizeof(kalibrasiItems[0]);

void UI_RenderCalibMenu(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < KALIBRASI_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(kalibrasiItems[itemIdx]);
    }
  }
}

const char *panelItems[] = {
  "1.Date Time",
  "2.App Name",
  "3.Id Panel",
  "4.Country Code",
  "5.Company Code",
  "6.Location"
};

uint8_t PANEL_ITEM_COUNT = sizeof(panelItems) / sizeof(panelItems[0]);

void UI_RenderPanelMenu(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < PANEL_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(panelItems[itemIdx]);
    }
  }
}

const char *struckItems[] = {
  "1.Header1",
  "2.Header2",
  "3.Footer1",
  "4.Footer2",
  "5.Slip Number"
};

uint8_t STRUCK_ITEM_COUNT = sizeof(struckItems) / sizeof(struckItems[0]);

void UI_RenderStruckMenu(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < STRUCK_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(struckItems[itemIdx]);
    }
  }
}

const char *netItems[] = {
  "1.Ip",
  "2.Ssid",
  "3.Ssid Usernamme",
  "4.Ssid Password",
  "5.Server Port",
  "6.Mqqt User",
  "7.Mqqt Password",
  "8.Mqqt Topic"
};

uint8_t NET_ITEM_COUNT = sizeof(netItems) / sizeof(netItems[0]);

void UI_RenderNetMenu(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < NET_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(netItems[itemIdx]);
    }
  }
}

const char *meterItems[] = {
  "1.K Factor",
  "2.C Factor",
  "3.SD Threshold",
  "4.Pulser Channel",
  "5.Totaliser"
};

uint8_t METER_ITEM_COUNT = sizeof(meterItems) / sizeof(meterItems[0]);

void UI_RenderMeterMenu(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < METER_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(meterItems[itemIdx]);
    }
  }
}

const char *deviceStatusItems[] = {
  "1.Rfid Status",
  "2.Modem Status",
  "3.Gps Status"
};

uint8_t DEVICE_STATUS_ITEM_COUNT = sizeof(deviceStatusItems) / sizeof(deviceStatusItems[0]);

void UI_RenderDeviceStatusrMenu(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < DEVICE_STATUS_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(deviceStatusItems[itemIdx]);
    }
  }
}

const char *userItems[] = {
  "1.Add",
  "2.Remove"
};

uint8_t USER_ITEM_COUNT = sizeof(userItems) / sizeof(userItems[0]);

void UI_RenderUserMenu(uint8_t page)
{
  LCD_Clear();

  uint8_t itemsPerPage = 4;
  uint8_t startIdx = page * itemsPerPage;

  for (uint8_t i = 0; i < itemsPerPage; i++)
  {
    uint8_t itemIdx = startIdx + i;

    if (itemIdx < USER_ITEM_COUNT)
    {
      LCD_SetCursor(0, i);
      LCD_Print(userItems[itemIdx]);
    }
  }
}