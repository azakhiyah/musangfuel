#pragma once
#include <Arduino.h>

namespace DeviceManager {

    void begin();   // load dari Config saat boot

    // getter
    bool isGpsEnabled();
    bool isRfidEnabled();
    bool isModemEnabled();

    // setter (kalau nanti mau dynamic update)
    void setGpsEnabled(bool en);
    void setRfidEnabled(bool en);
    void setModemEnabled(bool en);
}