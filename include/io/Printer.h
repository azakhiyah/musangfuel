#ifndef PRINTER_H
#define PRINTER_H

#include "Arduino.h"
#include "core/RuntimeStorage.h"

class Printer {
public:

    Printer(HardwareSerial& serial,
                   uint32_t baudRate = 9600,
                   int8_t rx = -1,
                   int8_t tx = -1);

    // =========================
    // INIT
    // =========================
    void begin();

    // =========================
    // LOW LEVEL
    // =========================
    void reset();
    void feed(uint8_t lines = 3);
    void cut();

    // =========================
    // BASIC PRINT
    // =========================
    void printLine(const char* text);
    void printBoldLine(const char* text);

    void printCentered(const char* text);
    void printBoldCentered(const char* text);

    void printSeparator(char separator = '=');

    // =========================
    // MAIN PRINT
    // =========================
    void printTransactionSlip(const TransactionData& trx);

    void printCurrentShift(const ShiftData& shift);
    void printCloseShift(const ShiftData& shift);

    // =========================
    // REPRINT
    // =========================
    void reprintLastTransaction(const TransactionData& trx);

    // =========================
    // LOGO
    // =========================
    void printBitmap80x80_P(const uint8_t *bitmap);
    void printBitmap100x100_P(const uint8_t *bitmap);

    void printHeaderWithLogo100(
        const uint8_t *logo100x100,
        const char *header,
        const char *header2
    );

    void printHeaderWithLogo80(
        const uint8_t *logo80x80,
        const char *header,
        const char *header2
    );

    void printLogo100x100_Centered(const uint8_t *bitmap);

private:

    HardwareSerial& _serial;

    uint32_t _baud;
    int8_t _rxPin;
    int8_t _txPin;

    // =========================
    // INTERNAL
    // =========================
    void sendCommand(const uint8_t* cmd, size_t length);

    void setAlign(char align);
    void setBold(bool enabled);

    void setX(uint16_t xDots);

    void printSlipHeader();
    void printSlipFooter();

    void formatTimestamp(uint32_t ts,char* out,size_t len);
};

extern Printer printer;

#endif