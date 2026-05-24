#include "io/GPS.h"


GPS gps(14, 27);

GPS::GPS(uint8_t rxPin, uint8_t txPin, uint32_t baudRate)
    : _rxPin(rxPin), _txPin(txPin), _baudRate(baudRate), _gpsSerial(1) {}

void GPS::begin() {
    _gpsSerial.begin(_baudRate, SERIAL_8N1, _rxPin, _txPin);
}

void GPS::update() {

    _updated = false;

    while (_gpsSerial.available()) {

        char c = _gpsSerial.read();

        if (_gps.encode(c)) {

            if (_gps.location.isUpdated()) {

                _lat = _gps.location.lat();
                _lng = _gps.location.lng();

                _updated = true;
            }
        }
    }
}

bool GPS::isLocationValid() {
    return _gps.location.isValid();
}

bool GPS::isLocationUpdated() {
    return _updated;
}

double GPS::getLatitude() {
    return _lat;
}

double GPS::getLongitude() {
    return _lng;
}

double GPS::getSpeedKmph() {
    return _gps.speed.kmph();
}

double GPS::getAltitude() {
    return _gps.altitude.meters();
}

double GPS  ::getHdop() {
    return _gps.hdop.hdop();
}

uint32_t GPS::getSatellites() {
    return _gps.satellites.value();
}