#include "io/LCD.h"
#include "config/Config.h"
#include <cstring>
#include <cstdio>

void UI_RenderIpInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Ip :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}


void UI_RenderSsidInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Ssid :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderSsidUserInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Ssid User :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderSsidPasswordInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Ssid Password :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}


void UI_RenderHostPortInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Host Port :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderMqttUserInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Mqtt User :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderMqttPasswordInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Mqtt Password :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void UI_RenderMqttTopicInput(const char* input)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_Print("Set Mqtt Topic :");

    LCD_SetCursor(0, 3);

    char buffer[21] = {0};

    if (input != nullptr) {
        strncpy(buffer, input, sizeof(buffer) - 1);
        LCD_Print(buffer);
    }
}

void handle_IpInput(const char* typed, char preview, const char* output)
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

    UI_RenderIpInput(display);
}

void handle_SsidInput(const char* typed, char preview, const char* output)
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

    UI_RenderSsidInput(display);
}


void handle_SsidPasswordInput(const char* typed, char preview, const char* output)
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

    UI_RenderSsidPasswordInput(display);
}


void handle_SsidUserInput(const char* typed, char preview, const char* output)
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

    UI_RenderSsidUserInput(display);
}


void handle_HostPortInput(const char* typed, char preview, const char* output)
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

    UI_RenderHostPortInput(display);
}


void handle_MqttUserInput(const char* typed, char preview, const char* output)
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

    UI_RenderMqttUserInput(display);
}


void handle_MqttPasswordInput(const char* typed, char preview, const char* output)
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

    UI_RenderMqttPasswordInput(display);
}


void handle_MqttTopicInput(const char* typed, char preview, const char* output)
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

    UI_RenderMqttTopicInput(display);
}

void UI_RenderIpShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Ip : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetIP()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderSsidShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Ssid : ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetSSID()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderSsidUserShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Ssid User: ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetSSIDUser()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}


void UI_RenderSsidPasswordShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Ssid Password: ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetSSIDPassword()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}


void UI_RenderHostPortShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Host Port: ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%d",
        Config::GetHostPort()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}


void UI_RenderMqttUserShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Mqtt User: ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetMQTTUser()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderMqttPasswordShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Mqqt Password: ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetMQTTPassword()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

void UI_RenderMqttTopicShow() {
  LCD_Clear();
  LCD_SetCursor(0, 0);
  LCD_Print("Mqqt Topic: ");

  char buffer[21];

    snprintf(
        buffer,
        sizeof(buffer),
        "%s",
        Config::GetMQTTTopic()
    );

  LCD_SetCursor(0, 3);
  LCD_Print(buffer);
}

