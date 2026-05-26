#include "config/Config.h"
#include "config/ConfigHandler.h"

#include <string.h>

namespace {

    struct {
        char appName[21];
        uint8_t panelId;

        char countryCode[21];
        char location[21];
        char companyCode[16];

        char header[21];
        char header2[21];
        char footer[21];
        char footer2[21];

        char ip[16];    
        char ssid[21];
        char ssidUser[21];
        char ssidPassword[21];

        uint16_t hostPort;
        char mqttUser[21];
        char mqttPassword[21];
        char mqttTopic[64];

        double kFactor;
        double cFactor;
        uint16_t slowDownThreshold;
        uint8_t pulserChannel;

        bool rfidStatus;
        bool modemStatus;
        bool gpsStatus;
    } g_config;

}

// ==========================
// INIT (dipanggil saat boot)
// ==========================
void Config::Init() {
     ConfigHandler::ConfigHandler_LoadFromFS();
   
}

// ==========================
// GETTER
// ==========================

const char* Config::GetAppName() { return g_config.appName; }
uint8_t Config::GetPanelId() { return g_config.panelId; }
const char* Config::GetCountryCode() { return g_config.countryCode; }
const char* Config::GetLocation() { return g_config.location; }
const char* Config::GetCompanyCode() { return g_config.companyCode; }

const char* Config::GetHeader() { return g_config.header; }
const char* Config::GetHeader2() { return g_config.header2; }
const char* Config::GetFooter() { return g_config.footer; }
const char* Config::GetFooter2() { return g_config.footer2; }

const char* Config::GetIP() { return g_config.ip; }
const char* Config::GetSSID() { return g_config.ssid; }
const char* Config::GetSSIDUser() { return g_config.ssidUser; }
const char* Config::GetSSIDPassword() { return g_config.ssidPassword; }

uint16_t Config::GetHostPort() { return g_config.hostPort; }
const char* Config::GetMQTTUser() { return g_config.mqttUser; }
const char* Config::GetMQTTPassword() { return g_config.mqttPassword; }
const char* Config::GetMQTTTopic() { return g_config.mqttTopic; }

double Config::GetKFactor() { return g_config.kFactor; }
double Config::GetCFactor() { return g_config.cFactor; }
uint16_t Config::GetSlowDownThreshold() { return g_config.slowDownThreshold; }
uint8_t Config::GetPulserChannel() { return g_config.pulserChannel; }

bool Config::GetRFIDStatus() { return g_config.rfidStatus; }
bool Config::GetModemStatus() { return g_config.modemStatus; }
bool Config::GetGPSStatus() { return g_config.gpsStatus; }

static void trimRight(char* str)
{
    int len = strlen(str);

    while (len > 0)
    {
        char c = str[len - 1];

        if (c == ' '  ||
            c == '\r' ||
            c == '\n' ||
            c == '\t')
        {
            str[len - 1] = '\0';
            len--;
        }
        else
        {
            break;
        }
    }
}



// ===== HELPER COPY =====
static void copyStr(char* dst, const char* src, size_t size) {
     if (!dst || !src || size == 0) {
        return;
    }

    //memset(dst, 0, size);

    //strncpy(dst, src, size - 1);

    snprintf(dst, size, "%s", src);

    trimRight(dst);
}


// ==========================
// SETTER (RAM ONLY)
// ==========================

void Config::SetAppName(const char* v) { copyStr(g_config.appName, v, sizeof(g_config.appName)); }
void Config::SetPanelId(uint8_t v) { g_config.panelId = v; }
void Config::SetCountryCode(const char* v) { copyStr(g_config.countryCode, v, sizeof(g_config.countryCode)); }
void Config::SetLocation(const char* v) { copyStr(g_config.location, v, sizeof(g_config.location)); }
void Config::SetCompanyCode(const char* v) { copyStr(g_config.companyCode, v, sizeof(g_config.companyCode)); }

void Config::SetHeader(const char* v) { copyStr(g_config.header, v, sizeof(g_config.header)); }
void Config::SetHeader2(const char* v) { copyStr(g_config.header2, v, sizeof(g_config.header2)); }
void Config::SetFooter(const char* v) { copyStr(g_config.footer, v, sizeof(g_config.footer)); }
void Config::SetFooter2(const char* v) { copyStr(g_config.footer2, v, sizeof(g_config.footer2)); }

void Config::SetIP(const char* v) { copyStr(g_config.ip, v, sizeof(g_config.ip)); }
void Config::SetSSID(const char* v) { copyStr(g_config.ssid, v, sizeof(g_config.ssid)); }
void Config::SetSSIDUser(const char* v) { copyStr(g_config.ssidUser, v, sizeof(g_config.ssidUser)); }
void Config::SetSSIDPassword(const char* v) { copyStr(g_config.ssidPassword, v, sizeof(g_config.ssidPassword)); }

void Config::SetHostPort(uint16_t v) { g_config.hostPort = v; }
void Config::SetMQTTUser(const char* v) { copyStr(g_config.mqttUser, v, sizeof(g_config.mqttUser)); }
void Config::SetMQTTPassword(const char* v) { copyStr(g_config.mqttPassword, v, sizeof(g_config.mqttPassword)); }
void Config::SetMQTTTopic(const char* v) { copyStr(g_config.mqttTopic, v, sizeof(g_config.mqttTopic)); }

void Config::SetKFactor(double v) { g_config.kFactor = v; }
void Config::SetCFactor(double v) { g_config.cFactor = v; }
void Config::SetSlowDownThreshold(uint16_t v) { g_config.slowDownThreshold = v; }
void Config::SetPulserChannel(uint8_t v) { g_config.pulserChannel = v; }

void Config::SetRFIDStatus(bool v) { g_config.rfidStatus = v; }
void Config::SetModemStatus(bool v) { g_config.modemStatus = v; }
void Config::SetGPSStatus(bool v) { g_config.gpsStatus = v; }



