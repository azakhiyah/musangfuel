#include "config/ConfigHandler.h"
#include "config/Config.h"
#include <Wire.h>
#include <string.h>
#include <LittleFS.h>


void ConfigHandler::Init() {
    Wire.begin();
}

// void ConfigHandler::ConfigHandler_SaveToFS() {

//     if (!LittleFS.begin()) {
//         Serial.println("LittleFS Mount Failed");
//         return;
//     }

//     File file = LittleFS.open("/config.txt", "w");

//     if (!file) {
//         Serial.println("Failed to open config.txt for writing");
//         return;
//     }

//     // tulis semua config ke file
//     file.println("APP_NAME=" + String(Config::GetAppName()));
//     file.println("ID_PANEL=" + String(Config::GetPanelId()));
//     file.println("COUNTRY_CODE=" + String(Config::GetCountryCode()));
//     file.println("LOCATION=" + String(Config::GetLocation()));
//     file.println("CODE_COMPANY=" + String(Config::GetCompanyCode()));

//     file.println("HEADER=" + String(Config::GetHeader()));
//     file.println("HEADER2=" + String(Config::GetHeader2()));
//     file.println("FOOTER=" + String(Config::GetFooter()));
//     file.println("FOOTER2=" + String(Config::GetFooter2()));

//     file.println("IP=" + String(Config::GetIP()));
//     file.println("SSID=" + String(Config::GetSSID()));
//     file.println("SSID_USER=" + String(Config::GetSSIDUser()));
//     file.println("SSID_PASSWORD=" + String(Config::GetSSIDPassword()));

//     file.println("HOST_PORT=" + String(Config::GetHostPort()));
//     file.println("MQTT_USER=" + String(Config::GetMQTTUser()));
//     file.println("MQTT_PASSWORD=" + String(Config::GetMQTTPassword()));
//     file.println("MQTT_TOPIC=" + String(Config::GetMQTTTopic()));

//     file.println("K_FAKTOR=" + String(Config::GetKFactor()));
//     file.println("C_FAKTOR=" + String(Config::GetCFactor()));
//     file.println("SLOW_DOWN_THRESHOLD=" + String(Config::GetSlowDownThreshold()));
//     file.println("PULSER_CHANNEL=" + String(Config::GetPulserChannel()));

//     file.println("RFID_STATUS=" + String(Config::GetRFIDStatus()));
//     file.println("MODEM_STATUS=" + String(Config::GetModemStatus()));
//     file.println("GPS_STATUS=" + String(Config::GetGPSStatus()));

//     file.close();

//     Serial.println("CONFIG SAVED SUCCESS");
// }

/*
void ConfigHandler::ConfigHandler_LoadFromFS() {

    if (!LittleFS.begin()) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    File file = LittleFS.open("/config.txt", "r");

    if (!file) {
        Serial.println("config.txt not found");
        return;
    }

    while (file.available()) {

        String line = file.readStringUntil('\n');
        line.trim();

        if (line.length() == 0) continue;

        int idx = line.indexOf('=');
        if (idx == -1) continue;

        String key = line.substring(0, idx);
        String value = line.substring(idx + 1);

        key.trim();
        value.trim();

        // ===== MAPPING =====
        if (key == "APP_NAME") Config::SetAppName(value.c_str());
        else if (key == "ID_PANEL") Config::SetPanelId(value.toInt());
        else if (key == "COUNTRY_CODE") Config::SetCountryCode(value.c_str());
        else if (key == "LOCATION") Config::SetLocation(value.c_str());
        else if (key == "COMPANY_CODE") Config::SetCompanyCode(value.c_str());

        else if (key == "HEADER") Config::SetHeader(value.c_str());
        else if (key == "HEADER2") Config::SetHeader2(value.c_str());
        else if (key == "FOOTER") Config::SetFooter(value.c_str());
        else if (key == "FOOTER2") Config::SetFooter2(value.c_str());

        else if (key == "IP") Config::SetIP(value.c_str());
        else if (key == "SSID") Config::SetSSID(value.c_str());
        else if (key == "SSID_USER") Config::SetSSIDUser(value.c_str());
        else if (key == "SSID_PASSWORD") Config::SetSSIDPassword(value.c_str());

        else if (key == "HOST_PORT") Config::SetHostPort(value.toInt());
        else if (key == "MQTT_USER") Config::SetMQTTUser(value.c_str());
        else if (key == "MQTT_PASSWORD") Config::SetMQTTPassword(value.c_str());
        else if (key == "MQTT_TOPIC") Config::SetMQTTTopic(value.c_str());

        else if (key == "K_FAKTOR") Config::SetKFactor(value.toDouble());
        else if (key == "C_FAKTOR") Config::SetCFactor(value.toDouble());
        else if (key == "SLOW_DOWN_THRESHOLD") Config::SetSlowDownThreshold(value.toInt());
        else if (key == "PULSER_CHANNEL") Config::SetPulserChannel(value.toInt());

        else if (key == "RFID_STATUS") Config::SetRFIDStatus(value.toInt());
        else if (key == "MODEM_STATUS") Config::SetModemStatus(value.toInt());
        else if (key == "GPS_STATUS") Config::SetGPSStatus(value.toInt());
    }

    file.close();

    // Serial.println("CONFIG LOADED SUCCESS");
    // Serial.printf("APP_NAME: %s\n", Config::GetAppName());
    // Serial.printf("ID_PANEL: %d\n", Config::GetPanelId());
    // Serial.printf("COUNTRY_CODE: %s\n", Config::GetCountryCode());
    // Serial.printf("LOCATION: %s\n", Config::GetLocation());
    // Serial.printf("CODE_COMPANY: %s\n", Config::GetCompanyCode());
    // Serial.printf("HEADER: %s\n", Config::GetHeader());
    // Serial.printf("HEADER2: %s\n", Config::GetHeader2());
    // Serial.printf("FOOTER: %s\n", Config::GetFooter());
    // Serial.printf("FOOTER2: %s\n", Config::GetFooter2());
    // Serial.printf("IP: %s\n", Config::GetIP());
    // Serial.printf("SSID: %s\n", Config::GetSSID());
    // Serial.printf("SSID_USER: %s\n", Config::GetSSIDUser());
    // Serial.printf("SSID_PASSWORD: %s\n", Config::GetSSIDPassword());
    // Serial.printf("HOST_PORT: %d\n", Config::GetHostPort());
    // Serial.printf("MQTT_USER: %s\n", Config::GetMQTTUser());
    // Serial.printf("MQTT_PASSWORD: %s\n", Config::GetMQTTPassword());
    // Serial.printf("MQTT_TOPIC: %s\n", Config::GetMQTTTopic());
    
    // Serial.printf("K_FAKTOR: %.9f\n",Config::GetKFactor());
    // Serial.printf("C_FAKTOR: %.9f\n", Config::GetCFactor());
    // Serial.printf("SLOW_DOWN_THRESHOLD: %d\n", Config::GetSlowDownThreshold());
    // Serial.printf("PULSER_CHANNEL: %d\n", Config::GetPulserChannel());
    // Serial.printf("RFID_STATUS: %d\n", Config::GetRFIDStatus());
    // Serial.printf("MODEM_STATUS: %d\n", Config::GetModemStatus());
    // Serial.printf("GPS_STATUS: %d\n", Config::GetGPSStatus());
}
*/

void ConfigHandler::ConfigHandler_LoadFromFS()
{
    if (!LittleFS.begin()) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    File file = LittleFS.open("/config.txt", "r");

    if (!file) {
        Serial.println("config.txt not found");
        return;
    }

    char line[128];

    while (file.available()) {

        size_t len = file.readBytesUntil('\n',
                                         line,
                                         sizeof(line) - 1);

        line[len] = '\0';

        // ===== REMOVE CR/LF =====
        while (len > 0 &&
              (line[len - 1] == '\r' ||
               line[len - 1] == '\n'))
        {
            line[--len] = '\0';
        }

        // ===== SKIP EMPTY =====
        if (len == 0) {
            continue;
        }

        // ===== FIND '=' =====
        char* eq = strchr(line, '=');

        if (!eq) {
            continue;
        }

        // split key=value
        *eq = '\0';

        char* key   = line;
        char* value = eq + 1;

        // ===== MAPPING =====
        if (strcmp(key, "APP_NAME") == 0)
            Config::SetAppName(value);

        else if (strcmp(key, "ID_PANEL") == 0)
            Config::SetPanelId(atoi(value));

        else if (strcmp(key, "COUNTRY_CODE") == 0)
            Config::SetCountryCode(value);

        else if (strcmp(key, "LOCATION") == 0)
            Config::SetLocation(value);

        else if (strcmp(key, "COMPANY_CODE") == 0)
            Config::SetCompanyCode(value);

        else if (strcmp(key, "HEADER") == 0)
            Config::SetHeader(value);

        else if (strcmp(key, "HEADER2") == 0)
            Config::SetHeader2(value);

        else if (strcmp(key, "FOOTER") == 0)
            Config::SetFooter(value);

        else if (strcmp(key, "FOOTER2") == 0)
            Config::SetFooter2(value);

        else if (strcmp(key, "IP") == 0)
            Config::SetIP(value);

        else if (strcmp(key, "SSID") == 0)
            Config::SetSSID(value);

        else if (strcmp(key, "SSID_USER") == 0)
            Config::SetSSIDUser(value);

        else if (strcmp(key, "SSID_PASSWORD") == 0)
            Config::SetSSIDPassword(value);

        else if (strcmp(key, "HOST_PORT") == 0)
            Config::SetHostPort(atoi(value));

        else if (strcmp(key, "MQTT_USER") == 0)
            Config::SetMQTTUser(value);

        else if (strcmp(key, "MQTT_PASSWORD") == 0)
            Config::SetMQTTPassword(value);

        else if (strcmp(key, "MQTT_TOPIC") == 0)
            Config::SetMQTTTopic(value);

        else if (strcmp(key, "K_FAKTOR") == 0)
            Config::SetKFactor(atof(value));

        else if (strcmp(key, "C_FAKTOR") == 0)
            Config::SetCFactor(atof(value));

        else if (strcmp(key, "SLOW_DOWN_THRESHOLD") == 0)
            Config::SetSlowDownThreshold(atoi(value));

        else if (strcmp(key, "PULSER_CHANNEL") == 0)
            Config::SetPulserChannel(atoi(value));

        else if (strcmp(key, "RFID_STATUS") == 0)
            Config::SetRFIDStatus(atoi(value));

        else if (strcmp(key, "MODEM_STATUS") == 0)
            Config::SetModemStatus(atoi(value));

        else if (strcmp(key, "GPS_STATUS") == 0)
            Config::SetGPSStatus(atoi(value));
    }

    file.close();
}


bool ConfigHandler::ConfigHandler_UpdateValue(
    const char* targetKey,
    const char* newValue
) {

    if (!LittleFS.begin()) {

        Serial.println("LittleFS Mount Failed");
        return false;
    }

    File readFile = LittleFS.open("/config.txt", "r");

    if (!readFile) {

        Serial.println("Failed open config");
        return false;
    }

    File tempFile = LittleFS.open("/temp.txt", "w");

    if (!tempFile) {

        Serial.println("Failed open temp");
        readFile.close();
        return false;
    }

    bool keyFound = false;

    while (readFile.available()) {

        char line[128] = {0};

        size_t len = readFile.readBytesUntil(
            '\n',
            line,
            sizeof(line) - 1
        );

        line[len] = '\0';

        if (strlen(line) == 0) {
            continue;
        }

        char workingLine[128];
        strncpy(
            workingLine,
            line,
            sizeof(workingLine)
        );

        char* equalPos = strchr(workingLine, '=');

        if (equalPos == nullptr) {

            tempFile.println(line);
            continue;
        }

        *equalPos = '\0';

        char* key = workingLine;

        if (strcmp(key, targetKey) == 0) {

            tempFile.print(targetKey);
            tempFile.print("=");
            tempFile.println(newValue);

            keyFound = true;

        } else {

            tempFile.println(line);
        }
    }

    // Tambah jika belum ada
    if (!keyFound) {

        tempFile.print(targetKey);
        tempFile.print("=");
        tempFile.println(newValue);
    }

    readFile.close();
    tempFile.close();

    LittleFS.remove("/config.txt");
    LittleFS.rename("/temp.txt", "/config.txt");

    Serial.print(targetKey);
    Serial.println(" SAVED SUCCESS");

    return true;
}