#pragma once

void KeypadMultiKey_Init();
bool KeypadMultiKey_Available();
//char KeypadMultiKey_GetChar(char key);          // proses input
char KeypadMultiKey_GetPreviewChar();   // preview char
const char* KeypadMultiKey_GetTypedText();

void KeypadMultiKey_Clear();

void KeypadMultiKey_Update();

void KeypadMultiKey_ProcessKey(char key);

void KeypadMultiKey_Commit();