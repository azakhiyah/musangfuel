#pragma once
#include <stdint.h>

typedef enum {
    EVT_NONE,

    EVT_ENTER_CONFIG,     //(dari keypad secret)
    EVT_CONFIG_UPDATE,        //(dari BLE, bawa data appName)
    EVT_CONFIG_SAVE,          //(dari BLE tombol save)
    EVT_CONFIG_EXIT,          //(opsional: cancel)

    EVT_UI_REFRESH,

    EVT_KEYPAD,
    EVT_RFID,
    EVT_PULSE,

    EVT_GPS_UPDATE,
    EVT_WIFI_CONNECTED,
    EVT_PRINT_DONE

} EventType;

typedef struct {
    EventType type;
    char payload[21];   // 🔥 langsung bawa data
} Event;