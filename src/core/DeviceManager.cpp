#include "core/DeviceManager.h"
#include "config/Config.h"

namespace DeviceManager {

    // 🔒 gunakan volatile karena diakses lintas task
    static volatile bool gpsEnabled   = false;
    static volatile bool rfidEnabled  = false;
    static volatile bool modemEnabled = false;

    void begin() {
        gpsEnabled   = (Config::GetGPSStatus() == 1);
        rfidEnabled  = (Config::GetRFIDStatus() == 1);
        modemEnabled = (Config::GetModemStatus() == 1);
    }

    bool isGpsEnabled()   { return gpsEnabled; }
    bool isRfidEnabled() { return rfidEnabled; }
    bool isModemEnabled(){ return modemEnabled; }

    void setGpsEnabled(bool en)   { gpsEnabled = en; }
    void setRfidEnabled(bool en)  { rfidEnabled = en; }
    void setModemEnabled(bool en) { modemEnabled = en; }
}