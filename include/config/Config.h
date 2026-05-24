#pragma once
#include <stdint.h>

namespace Config {

    void Init();

    const char* GetAppName();
    uint8_t GetPanelId();
    const char* GetCountryCode();
    const char* GetLocation();
    const char* GetCompanyCode();

    const char* GetHeader();
    const char* GetHeader2();
    const char* GetFooter();
    const char* GetFooter2();

    const char* GetIP();
    const char* GetSSID();
    const char* GetSSIDUser();
    const char* GetSSIDPassword();

    uint16_t GetHostPort();
    const char* GetMQTTUser();
    const char* GetMQTTPassword();
    const char* GetMQTTTopic();

    double GetKFactor();
    double GetCFactor();
    uint16_t GetSlowDownThreshold();
    uint8_t GetPulserChannel();

    bool GetRFIDStatus();
    bool GetModemStatus();
    bool GetGPSStatus();

    

    void SetAppName(const char* v);
    void SetPanelId(uint8_t v);
    void SetCountryCode(const char* v);
    void SetLocation(const char* v);
    void SetCompanyCode(const char* v);

    void SetHeader(const char* v);
    void SetHeader2(const char* v);
    void SetFooter(const char* v);
    void SetFooter2(const char* v);

    void SetIP(const char* v);
    void SetSSID(const char* v);
    void SetSSIDUser(const char* v);
    void SetSSIDPassword(const char* v);

    void SetHostPort(uint16_t v);
    void SetMQTTUser(const char* v);
    void SetMQTTPassword(const char* v);
    void SetMQTTTopic(const char* v);

    void SetKFactor(double v);
    void SetCFactor(double v);
    void SetSlowDownThreshold(uint16_t v);
    void SetPulserChannel(uint8_t v);

    void SetRFIDStatus(bool v);
    void SetModemStatus(bool v);
    void SetGPSStatus(bool v);

}