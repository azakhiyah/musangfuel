#ifndef GPS_H
#define GPS_H

#include "TinyGPS++.h"
#include "HardwareSerial.h"

class GPS {
public:
    GPS(uint8_t rxPin, uint8_t txPin, uint32_t baudRate = 9600);

    void begin();
    void update();

    bool isLocationValid();
    bool isLocationUpdated();

    double getLatitude();
    double getLongitude();
    double getSpeedKmph();
    double getAltitude();
    double getHdop();
    uint32_t getSatellites();

private:
    uint8_t _rxPin, _txPin;
    uint32_t _baudRate;

    HardwareSerial _gpsSerial;
    TinyGPSPlus _gps;

    // cache (biar tidak ambil langsung dari TinyGPS tiap saat)
    double _lat = 0;
    double _lng = 0;
    bool _updated = false;
};

extern GPS gps;
#endif