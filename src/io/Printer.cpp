#include "io/Printer.h"

#include "config/Config.h"
#include "services/TimeService.h"
#include "core/RuntimeStorage.h"
#include "config/UserHandler.h"

Printer printer(Serial, 9600, 1, 3);

// =========================
// CONSTRUCTOR
// =========================
Printer::Printer(
    HardwareSerial& serial,
    uint32_t baudRate,
    int8_t rx,
    int8_t tx
)
: _serial(serial),
  _baud(baudRate),
  _rxPin(rx),
  _txPin(tx)
{
}

// =========================
// BEGIN
// =========================
void Printer::begin()
{
    _serial.begin(_baud, SERIAL_8N1, _rxPin, _txPin);

    delay(300);

    reset();

    //Serial.println("Printer Ready");
}

// =========================
// RESET
// =========================
void Printer::reset()
{
    uint8_t cmd[] = {0x1B, 0x40};
    sendCommand(cmd, sizeof(cmd));
}

// =========================
// SEND COMMAND
// =========================
void Printer::sendCommand(const uint8_t* cmd,
                                 size_t length)
{
    _serial.write(cmd, length);
}

// =========================
// ALIGN
// =========================
void Printer::setAlign(char align)
{
    uint8_t value = 0;

    switch (align) {
        case 'C': value = 1; break;
        case 'R': value = 2; break;
        default:  value = 0; break;
    }

    uint8_t cmd[] = {0x1B, 0x61, value};

    sendCommand(cmd, sizeof(cmd));
}

// =========================
// BOLD
// =========================
void Printer::setBold(bool enabled)
{
    uint8_t cmd[] = {0x1B, 0x45, static_cast<uint8_t>(enabled)};

    sendCommand(cmd, sizeof(cmd));
}

// =========================
// TEXT
// =========================
void Printer::printLine(const char* text)
{
    _serial.println(text);
}

void Printer::printBoldLine(const char* text)
{
    setBold(true);
    _serial.println(text);
    setBold(false);
}

void Printer::printCentered(const char* text)
{
    setAlign('C');
    _serial.println(text);
    setAlign('L');
}

void Printer::printBoldCentered(const char* text)
{
    setAlign('C');
    setBold(true);

    _serial.println(text);

    setBold(false);
    setAlign('L');
}

void Printer::printSeparator(char separator)
{
    char line[33];

    for (int i = 0; i < 32; i++) {
        line[i] = separator;
    }

    line[32] = '\0';

    _serial.println(line);
}

// =========================
// FEED
// =========================
void Printer::feed(uint8_t lines)
{
    for (uint8_t i = 0; i < lines; i++) {
        _serial.println();
    }
}

// =========================
// CUT
// =========================
void Printer::cut()
{
    uint8_t cmd[] = {0x1D, 0x56, 0x00};

    sendCommand(cmd, sizeof(cmd));
}

// =========================
// HEADER
// =========================
void Printer::printSlipHeader()
{
    printCentered(Config::GetHeader());
    printCentered(Config::GetHeader2());

    printSeparator('=');
}

// =========================
// FOOTER
// =========================
void Printer::printSlipFooter()
{
    printSeparator('=');

    printCentered(Config::GetFooter());
    printCentered(Config::GetFooter2());

    feed(4);
}

// =========================
// FORMAT TIMESTAMP
// =========================
void Printer::formatTimestamp(uint32_t ts,
                                     char* out,
                                     size_t len)
{

    DateTimeSimple dt = Unix_ToDateTime(ts);

    snprintf(out,
             len,
             "%02d-%02d-%04d %02d:%02d:%02d",
             dt.day,
             dt.month,
             dt.year,
             dt.hour,
             dt.minute,
             dt.second);
}

// =========================
// PRINT TRANSACTION
// =========================
void Printer::printTransactionSlip(
    const TransactionData& trx
)
{

    char startTime[32];
    char endTime[32];

    formatTimestamp(trx.startTime,
                    startTime,
                    sizeof(startTime));

    formatTimestamp(trx.endTime,
                    endTime,
                    sizeof(endTime));

    printSlipHeader();
    _serial.printf("Operator   : %s\n", trx.userName);
    _serial.printf("Start Time : %s\n", startTime);
    _serial.printf("End Time   : %s\n", endTime);   
    _serial.printf("Slip No    : %lu\n", trx.slipNumber);
    _serial.printf("Unit No    : %s\n", trx.unitNumber);
    _serial.printf("HM/KM      : %lu\n",trx.odometer);
    _serial.printf("Volume     : %.f L\n",trx.volume);
    // _serial.printf("Volume     : %.2f L\n",trx.volume);

    printSlipFooter();
}

void Printer::reprintLastTransaction(
    const TransactionData& trx
)
{

    char startTime[32];
    char endTime[32];

    formatTimestamp(trx.startTime,
                    startTime,
                    sizeof(startTime));

    formatTimestamp(trx.endTime,
                    endTime,
                    sizeof(endTime));
    printCentered("REPRINT");    
    printSlipHeader();
    _serial.printf("Operator   : %s\n", trx.userName);
    _serial.printf("Start Time : %s\n", startTime);
    _serial.printf("End Time   : %s\n", endTime);   
    _serial.printf("Slip No    : %lu\n", trx.slipNumber);
    _serial.printf("Unit No    : %s\n", trx.unitNumber);
    _serial.printf("HM/KM      : %lu\n",trx.odometer);
     _serial.printf("Volume     : %.f L\n",trx.volume);
    //_serial.printf("Volume     : %.2f L\n",trx.volume);

    printSlipFooter();
}


// =========================
// PRINT SHIFT
// =========================
void Printer::printCurrentShift(const ShiftData& shift) {
   
    char startTimeShift[32];
    char endTimeShift[32];

    formatTimestamp(g_Shift.openTimestamp,
                    startTimeShift,
                    sizeof(startTimeShift));

    formatTimestamp(g_Shift.closeTimestamp,
                    endTimeShift,
                    sizeof(endTimeShift));
        double currentTotaliser = RuntimeStorage::getTotaliser();
        double currentVolume = currentTotaliser - g_Shift.startTotaliser;
    User* user = User_FindById(g_Shift.userId);

    printCentered("SHIFT SUMMARY");
    printSlipHeader();
    _serial.printf("Operator   : %s\n",  user->username);
    _serial.printf("Pump Id    : %lu\n", g_Shift.panelId);
    _serial.printf("Open Time  : %s\n", startTimeShift);   
    _serial.printf("Close Time : - \n");
    _serial.printf("Start Total: %.2f \n", g_Shift.startTotaliser);
    _serial.printf("Curr Total : %.2f \n",currentTotaliser);
    _serial.printf("Curr Volume: %.f L\n",currentVolume);
    // _serial.printf("Curr Volume: %.2f L\n",currentVolume);
    printSlipFooter();

}

void Printer::printCloseShift(const ShiftData& shift) {
   
    char startTimeShift[32];
    char endTimeShift[32];

    User* user = User_FindById(g_Shift.userId);

    formatTimestamp(g_Shift.openTimestamp,
                    startTimeShift,
                    sizeof(startTimeShift));

    formatTimestamp(g_Shift.closeTimestamp,
                    endTimeShift,
                    sizeof(endTimeShift));
        double Volume = g_Shift.endTotaliser - g_Shift.startTotaliser;


    printCentered("SHIFT REPORT");
    printSlipHeader();
    _serial.printf("Operator   : %s\n", user->username);
    _serial.printf("Pump Id    : %lu\n", g_Shift.panelId);
    _serial.printf("Open Time  : %s\n", startTimeShift);   
    _serial.printf("Close Time : %s\n", endTimeShift);
    _serial.printf("Start Total: %.2f \n", g_Shift.startTotaliser);
    _serial.printf("End Total  : %.2f \n",g_Shift.endTotaliser);
    _serial.printf("Volume     : %.f L\n", Volume);
    //_serial.printf("Volume     : %.2f L\n", Volume);
    printSlipFooter();
}