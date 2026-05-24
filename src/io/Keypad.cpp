#include "io/Keypad.h"
#include <Wire.h>
#include <I2CKeyPad.h>

#define KEYPAD_ADDR 0x20

static I2CKeyPad keypad(KEYPAD_ADDR);
static uint8_t lastIndex = 0xFF;

static bool repeatMode = false;

static unsigned long lastPressTime = 0;
static unsigned long lastRepeatTime = 0;

static const uint16_t debounceDelay = 50;
static const uint16_t longPressDelay = 400;   // delay sebelum repeat mulai
static const uint16_t repeatInterval = 120;   // kecepatan repeat



// static unsigned long lastTime = 0;

// if (millis() - lastTime < 150) return;

// lastTime = millis();

// mapping index → karakter
static const char keyMap[16] = {
    '1','2','3','A',
    '4','5','6','B',
    '7','8','9','C',
    '*','0','#','D'
};

static char lastKey = 0;

void Keypad_Init() {
    Wire.begin();
    keypad.begin();
}

void Keypad_SetRepeatMode(bool enable) {
    repeatMode = enable;
}

bool Keypad_Available() {

    if (!keypad.isPressed()) {
        lastIndex = 0xFF;
        return false;
    }

    uint8_t index = keypad.getKey();

    if (index == 0xFF || index >= 16) {
        return false;
    }

    unsigned long now = millis();

    // ===== FIRST PRESS =====
    if (index != lastIndex) {
        lastIndex = index;
        lastPressTime = now;
        lastRepeatTime = now;
        lastKey = keyMap[index];
        return true;
    }

    // ===== HOLD / REPEAT MODE =====
    if (repeatMode) {

        // tunggu sampai long press aktif
        if (now - lastPressTime < longPressDelay) {
            return false;
        }

        // repeat tiap interval
        if (now - lastRepeatTime >= repeatInterval) {
            lastRepeatTime = now;
            lastKey = keyMap[index];
            return true;
        }
    }

    return false;
}

char Keypad_GetKey() {
    return lastKey;
}

