#include "io/PulserEncoder.h"
#include "config/Config.h"

#define DEBOUNCE_DELAY_MS 5
#define FLOW_PIN_1 16
#define FLOW_PIN_2 17

PulserEncoder pulser;

// ===== STATIC =====
volatile int32_t PulserEncoder::_pulseCount = 0;
volatile uint32_t PulserEncoder::_lastInterruptTime = 0;

uint8_t PulserEncoder::_clkPin = 0;
uint8_t PulserEncoder::_dtPin = 0;

bool state1 = false;

bool state2 = false;

// ===== BEGIN =====
void PulserEncoder::begin(uint8_t clkPin, uint8_t dtPin) {

    _clkPin = clkPin;
    _dtPin = dtPin;

    pinMode(_clkPin, INPUT_PULLUP);
    pinMode(_dtPin, INPUT_PULLUP);

    uint8_t pulserChannel = Config::GetPulserChannel();

    if (pulserChannel == 1) {
        pinMode(_clkPin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(_clkPin), isrSingle, RISING);
    } else if (pulserChannel == 2) {
        pinMode(_clkPin, INPUT_PULLUP);
        pinMode(_dtPin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(_clkPin), isrDual, RISING);
    }

    _pulseCount = 0;
    _lastInterruptTime = micros();

    _lastUpdate = millis();

    //Serial.println("PulserEncoder initialized");
}

// ===== ISR =====
void IRAM_ATTR PulserEncoder::isrHandler() {

    uint32_t now = micros();

    // debounce (anti noise)
    if (now - _lastInterruptTime > 300) {
        _pulseCount++;
        _lastInterruptTime = now;
    }
}

void IRAM_ATTR PulserEncoder::isrSingle() {

    uint32_t now = micros();

    if (now - _lastInterruptTime < DEBOUNCE_DELAY_MS) {
        return;
    }

    _pulseCount++;  // Single channel: naik saja
    _lastInterruptTime = now;
}

void IRAM_ATTR PulserEncoder::isrDual() {

    uint32_t now = micros();
    if (now - _lastInterruptTime < DEBOUNCE_DELAY_MS) {
        return;
    }

    int clkState = digitalRead(_clkPin);  // global atau diatur via object encoder
    int dtState  = digitalRead(_dtPin);

    if (clkState == dtState) {
        _pulseCount++;
    } else {
        _pulseCount--;
    }

    _lastInterruptTime = now;
}

// ===== UPDATE (dipanggil di Task) =====
void PulserEncoder::update(uint32_t preset) {

    uint32_t nowMicros = micros();
    uint32_t nowMillis = millis();
    static uint32_t lastLpmAboveZeroTime = millis();

    _kFactor = Config::GetKFactor();
    _cFactor = Config::GetCFactor();
    _slowDownThreshold = Config::GetSlowDownThreshold();

   
    if (nowMicros - _lastUpdate >= 1000000) {  // Update setiap 1 detik
        int32_t deltaPulse = _pulseCount - _lastPulse;
        _lastPulse = _pulseCount;
        _lastUpdate = nowMicros;

    
        // Moving average window
        deltaWindow[index] = deltaPulse;
        index = (index + 1) % WINDOW_SIZE;
    
        if (index == 0) windowFull = true;

        int32_t sum = 0;
        int count = windowFull ? WINDOW_SIZE : index;
        
        for (int i = 0; i < count; i++) {
            sum += deltaWindow[i];
        }

        double avgDelta = (double)sum / count;
        _lpm = (avgDelta / _kFactor) * 60.0;

        if (_lpm > 0) {
            lastLpmAboveZeroTime = nowMillis;
            _running = true;
        }

        double correction = _cFactor / 100;
        double volRaw = getVolumeRaw();
        double volCorrection = volRaw * _cFactor;
        _volume = volRaw - volCorrection;
        //double delta = preset - _volume;

        if (_volume != _lastVolume) {
            _lastVolume = _volume;
            _lastVolumeChangeTime = nowMillis;
        }

    }
}

double PulserEncoder::getVolumeRaw() {
    return (double)_pulseCount / _kFactor;
}

// ===== GETTER =====
double PulserEncoder::getVolume() {
    return _volume;
}

double PulserEncoder::getLPM() {
    return _lpm;
}

void PulserEncoder::setPreset(uint32_t preset) {
     _preset = preset;
}

uint32_t PulserEncoder::getPreset() {
    return _preset;
}

// ===== CONFIG =====
void PulserEncoder::setKFactor(double k) {
    _kFactor = k;
}

void PulserEncoder::setCFactor(double c) {
    _cFactor = c;
}

void PulserEncoder::setSlowDownThreshold(uint16_t threshold) {
    _slowDownThreshold = threshold;
}


// ===== CONTROL =====
void PulserEncoder::start() {
    _running = true;
}

void PulserEncoder::stop() {
    _running = false;
    //Serial.println("Pulser STOP");
}

// ===== RESET =====
void PulserEncoder::reset() {

    _running = false;
    _lastVolumeChangeTime = millis();
    _lastVolume = 0.0;
    _lpm = 0.0;
    _volume = 0.0;
    _pulseCount = 0;  // <<< Reset pulse count agar volume baru dari 0

      // Reset flow window
    for (int i = 0; i < WINDOW_SIZE; i++) {
        deltaWindow[i] = 0;
    }
    index = 0;
    windowFull = false;

    _lastPulse = _pulseCount;      // ✅ ini penting agar deltaPulse = 0
    _lastUpdate = micros();       // ✅ supaya tidak langsung dihitung
}

void PulserEncoder::startSelenoidValve() {
     pinMode(FLOW_PIN_1, OUTPUT);
     pinMode(FLOW_PIN_2, OUTPUT); 
     digitalWrite(FLOW_PIN_1, HIGH);
     digitalWrite(FLOW_PIN_2, HIGH);
}

void PulserEncoder::stopSelenoidValve() {
    pinMode(FLOW_PIN_1, INPUT);
    pinMode(FLOW_PIN_2, INPUT);
}

void PulserEncoder::open1() {

        if (state1) return;

        pinMode(FLOW_PIN_1, OUTPUT);
        digitalWrite(FLOW_PIN_1, HIGH);

        state1 = true;
    }

    void PulserEncoder::close1() {

        if (!state1) return;

         digitalWrite(FLOW_PIN_1, LOW);
         pinMode(FLOW_PIN_1, OUTPUT);

        state1 = false;
    }

    void PulserEncoder::open2() {

        if (state2) return;

        pinMode(FLOW_PIN_2, OUTPUT);
        digitalWrite(FLOW_PIN_2, HIGH);

        state2 = true;
    }

    void PulserEncoder::close2() {

        if (!state2) return;

    
        digitalWrite(FLOW_PIN_2, LOW);
        pinMode(FLOW_PIN_2, INPUT);

        state2 = false;
    }

    void PulserEncoder::openAll() {

        open1();
        open2();
    }

    void PulserEncoder::closeAll() {

        close1();
        close2();
    }

