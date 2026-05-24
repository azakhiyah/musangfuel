
#include "io/KeypadMultiKey.h"
#include <Wire.h>
#include <I2CKeyPad.h>
#include <string.h>

#define KEYPAD_ADDR 0x20

static I2CKeyPad keypad(KEYPAD_ADDR);

// ===== KEY MAP =====
static const char keyMap[16] = {
    '1','2','3','A',
    '4','5','6','B',
    '7','8','9','C',
    '*','0','#','D'
};

// ===== STATE =====
static char lastKey = '\0';
static uint8_t charIndex = 0;
static unsigned long lastPressTime = 0;
static bool keypadUpperCase = true;
static uint8_t lastIndex = 0xFF;

static String typedText = "";

// ===== CONFIG =====
static const uint16_t pressDelay = 800;

// ===== INIT =====
void KeypadMultiKey_Init() {
    Wire.begin();
    keypad.begin();
}

// ===== AVAILABILITY =====
    
bool KeypadMultiKey_Available() {

    uint8_t index = keypad.getKey();

    // jika tidak ada tombol
    if (index == 0xFF) {
        lastIndex = 0xFF;
        return false;
    }

    // hanya trigger saat perubahan (edge)
    if (index != lastIndex && index < 16) {
        lastKey = keyMap[index];
        lastIndex = index;
        return true;
    }

    return false;
}

// ===== CHAR GROUP =====
static const char* getCharGroup(char key) {

    if (keypadUpperCase) {
        switch (key) {
            case '1': return ".,?!-_/@\\1";
            case '2': return "ABC2";
            case '3': return "DEF3";
            case '4': return "GHI4";
            case '5': return "JKL5";
            case '6': return "MNO6";
            case '7': return "PQRS7";
            case '8': return "TUV8";
            case '9': return "WXYZ9";
            case '0': return " 0";
            default: return "";
        }
    } else {
        switch (key) {
            case '1': return ".,?!-_/@\\1";
            case '2': return "abc2";
            case '3': return "def3";
            case '4': return "ghi4";
            case '5': return "jkl5";
            case '6': return "mno6";
            case '7': return "pqrs7";
            case '8': return "tuv8";
            case '9': return "wxyz9";
            case '0': return " 0";
            default: return "";
        }
    }
}

// ===== COMMIT =====
static void commitLastChar() {
    if (lastKey != '\0') {
        const char* group = getCharGroup(lastKey);
        if (strlen(group) > 0) {
            typedText += group[charIndex];
        }
        lastKey = '\0';
        charIndex = 0;
    }
}

// ===== MAIN PROCESS =====
// char KeypadMultiKey_GetChar(char key) {
//     unsigned long now = millis();

//     // ===== SPECIAL KEY =====

//     // ENTER / COMMIT
//     if (key == 'A') {
//         commitLastChar();
//         return '\0';
//     }

//     // BACKSPACE
//     if (key == 'B') {
//         commitLastChar();
//         if (typedText.length() > 0) {
//             typedText.remove(typedText.length() - 1);
//         }
//         return '\0';
//     }

//     // TOGGLE CASE
//     if (key == 'D') {
//         keypadUpperCase = !keypadUpperCase;
//         return '\0';
//     }

//     // ===== MULTI TAP =====

//     if (key == lastKey) {

//         const char* group = getCharGroup(key);

//         if (strlen(group) > 0) {
//             charIndex = (charIndex + 1) % strlen(group);
//         }

//     } else {

//         // tombol baru → commit sebelumnya
//         commitLastChar();

//         lastKey = key;
//         charIndex = 0;
//     }

//     lastPressTime = now;
//     return '\0';
// }


// ===== PREVIEW =====
char KeypadMultiKey_GetPreviewChar() {
    if (lastKey != '\0') {
        const char* group = getCharGroup(lastKey);
        return group[charIndex];
    }
    return '\0';
}

// ===== TEXT =====
const char* KeypadMultiKey_GetTypedText() {
    return typedText.c_str();
}



// ===== CLEAR =====
void KeypadMultiKey_Clear() {
    typedText = "";
    lastKey = '\0';
    charIndex = 0;
}

void KeypadMultiKey_Update() {
    if (lastKey != '\0') {
        if (millis() - lastPressTime > pressDelay) {
            commitLastChar();
        }
    }
}

void KeypadMultiKey_Commit()
{
    commitLastChar(); // pakai fungsi internal kamu
}

void KeypadMultiKey_ProcessKey(char key)
{
    unsigned long now = millis();

    // ===== SPECIAL KEY =====

    // ENTER / COMMIT
    if (key == 'A') {
        commitLastChar();
        return;
    }

    // BACKSPACE
    if (key == 'B') {
        commitLastChar();
        if (typedText.length() > 0) {
            typedText.remove(typedText.length() - 1);
        }
        return;
    }

    // TOGGLE CASE
    if (key == 'D') {
        keypadUpperCase = !keypadUpperCase;
        return;
    }

    // ===== MULTI TAP =====

    if (key == lastKey && (now - lastPressTime < pressDelay)) {

        const char* group = getCharGroup(key);

        if (strlen(group) > 0) {
            charIndex = (charIndex + 1) % strlen(group);
        }

    } else {

        // tombol baru → commit sebelumnya
        commitLastChar();

        lastKey = key;
        charIndex = 0;
    }

    lastPressTime = now;
}