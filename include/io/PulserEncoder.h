#ifndef PULSER_ENCODER_H
#define PULSER_ENCODER_H

#include <Arduino.h>

class PulserEncoder {
public:
    void begin(uint8_t clkPin, uint8_t dtPin);

    // dipanggil di TaskPulser
    void update(uint32_t preset = 0);

    // getter
    double getVolumeRaw();
    double getVolume();
    double getLPM();
    void setPreset(uint32_t preset);
    uint32_t getPreset();

    // config
    void setSlowDownThreshold(uint16_t threshold);
    void setKFactor(double k);
    void setCFactor(double c);


    void stopSelenoidValve();
    void startSelenoidValve();

   

    // control
    void reset();
    void start();
    void stop();

    void open1();
    void open2();
    void openAll();
    void close1();
    void close2();
    void closeAll();

    // ISR
    static void IRAM_ATTR isrHandler();
    static void IRAM_ATTR isrSingle();
    static void IRAM_ATTR isrDual();

private:
    // ===== PIN =====
    static uint8_t _clkPin;
    static uint8_t _dtPin;

    // ===== DATA ISR =====
    static volatile int32_t _pulseCount;
    static volatile uint32_t _lastInterruptTime;

    // ===== DATA RUNTIME =====
    double _volume = 0;
    double _lpm = 0;
    float _lastVolume = 0;

    float _kFactor = 0.0f;
    float _cFactor = 0.0f;
    uint32_t _preset = 0;

    float _slowDownThreshold = 0;

    bool _running = false;

    uint32_t _lastUpdate = 0;
    int32_t _lastPulse = 0;
    uint32_t _lastVolumeChangeTime = 0;

    static const int WINDOW_SIZE = 5;
    int32_t deltaWindow[WINDOW_SIZE] = {0};
    int index = 0;
    bool windowFull = false;

};

extern PulserEncoder pulser;

#endif