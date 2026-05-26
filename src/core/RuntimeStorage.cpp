#include "core/RuntimeStorage.h"
#include "core/DeviceManager.h"
#include "config/Config.h"
#include <LittleFS.h>
#include <FS.h>
#include "io/GPS.h"
#include "services/TimeService.h"

Preferences RuntimeStorage::prefs;

// namespace NVS
#define NVS_NAMESPACE "runtime"

// keys
#define KEY_SHIFT      "shift"
#define KEY_SLIP       "slip"
#define KEY_TOTALISER  "tot"

// ===== GLOBAL SHIFT =====
ShiftData g_Shift;

TransactionData g_transaction;
TransactionData g_lastTransaction;

bool g_hasLastTransaction = false;

// ===== STATIC VAR =====
uint32_t RuntimeStorage::g_slip = 0;
double RuntimeStorage::g_totaliser = 0.0;

bool RuntimeStorage::dirtySlip = false;
bool RuntimeStorage::dirtyTotaliser = false;
bool RuntimeStorage::g_transactionDirty = false;


unsigned long RuntimeStorage::lastSaveTime = 0;


// =========================
// GET OBJECT
// =========================
TransactionData& RuntimeStorage::Transaction_Get()
{
    return g_transaction;
}

// =========================
// COPY OBJECT
// =========================
void RuntimeStorage::Transaction_GetData(TransactionData &out)
{
    memcpy(&out, &g_transaction, sizeof(TransactionData));

}

void RuntimeStorage::Shift_GetData(ShiftData &out)
{
    memcpy(&out, &g_Shift, sizeof(ShiftData));

}


// ===== INIT =====
void RuntimeStorage::begin() {
    prefs.begin(NVS_NAMESPACE, false);

    // load ke RAM saat boot
    g_slip = prefs.getUInt(KEY_SLIP, 0);
    g_totaliser = prefs.getDouble(KEY_TOTALISER, 0.0);

    memset(&g_transaction, 0, sizeof(TransactionData));
    g_transaction.isActive = true;

    g_transactionDirty = true;

    //Serial.println("RuntimeStorage INIT");
    //Serial.printf("SLIP: %u\n", g_slip);
    //Serial.printf("TOTALISER: %.3f\n", g_totaliser);
}

// ===== PERIODIC SAVE =====
void RuntimeStorage::update() {

    unsigned long now = millis();

    if (now - lastSaveTime < SAVE_INTERVAL_MS) return;

    if (dirtySlip) {
        prefs.putUInt(KEY_SLIP, g_slip);
        dirtySlip = false;
        //Serial.println("SLIP SAVED");
    }

    if (dirtyTotaliser) {
        prefs.putDouble(KEY_TOTALISER, g_totaliser);
        dirtyTotaliser = false;
        //Serial.println("TOTALISER SAVED");
    }

    lastSaveTime = now;
}

// ===== FORCE SAVE =====
void RuntimeStorage::forceSave() {

    prefs.putUInt(KEY_SLIP, g_slip);
    prefs.putDouble(KEY_TOTALISER, g_totaliser);

    dirtySlip = false;
    dirtyTotaliser = false;

    //Serial.println("FORCE SAVE DONE");
}

// ===== SHIFT =====
bool RuntimeStorage::saveShift(const ShiftData &data) {
    size_t written = prefs.putBytes(KEY_SHIFT, &data, sizeof(ShiftData));
    return (written == sizeof(ShiftData));
}

bool RuntimeStorage::loadShift(ShiftData &data) {

    if (!prefs.isKey(KEY_SHIFT)) {
        memset(&data, 0, sizeof(ShiftData));
        return false;
    }

    size_t read = prefs.getBytes(KEY_SHIFT, &data, sizeof(ShiftData));
    return (read == sizeof(ShiftData));
}

void RuntimeStorage::Shift_Reset() {

    memset(&g_Shift, 0, sizeof(ShiftData));
    g_Shift.isActive = false;

    saveShift(g_Shift);

    //Serial.println("SHIFT RESET (FORCED)");
}

// ===== SLIP =====
void RuntimeStorage::setSlip(uint32_t value) {
    g_slip = value;
    dirtySlip = true;
}

uint32_t RuntimeStorage::getSlip() {
    return g_slip;
}

// ===== TOTALISER =====
void RuntimeStorage::setTotaliser(double value) {
    g_totaliser = value;
    dirtyTotaliser = true;
}

double RuntimeStorage::getTotaliser() {
    return g_totaliser;
}


void RuntimeStorage::Transaction_SetUserId(const char* userId)
{
    strncpy(g_transaction.userId, userId, sizeof(g_transaction.userId)-1);

    g_transactionDirty = true;
}

const char* RuntimeStorage::Transaction_GetUserId()
{
    return g_transaction.userId;
}

void RuntimeStorage::Transaction_SetUserName(const char* userName) 
{
    strncpy(g_transaction.userName, userName, sizeof(g_transaction.userName)-1);
    g_transactionDirty = true;
}

const char* RuntimeStorage::Transaction_GetUserName()
{
    return g_transaction.userName;
}

    // ===== PANEL ID =====
void RuntimeStorage::Transaction_SetPanelId(uint8_t panelId) 
{
    g_transaction.panelId = panelId;
    g_transactionDirty = true;
}
      
uint8_t RuntimeStorage::Transaction_GetPanelId() 
{
    return g_transaction.panelId;
}

    // ===== COMPANY CODE =====
void RuntimeStorage::Transaction_SetCompanyCode(const char* companyCode)
{
    strncpy(g_transaction.companyCode, companyCode, sizeof(g_transaction.companyCode)-1);
    g_transactionDirty = true;
}

const char* RuntimeStorage::Transaction_GetCompanyCode()
{
    return g_transaction.companyCode;
}

    // ===== COUNTRY CODE =====
void RuntimeStorage::Transaction_SetCountryCode(const char* countryCode)
{
    strncpy(g_transaction.countryCode, countryCode, sizeof(g_transaction.countryCode)-1);
    g_transactionDirty = true;
}

const char* RuntimeStorage::Transaction_GetCountryCode()
{
    return g_transaction.countryCode;
}

void RuntimeStorage::Transaction_SetLocation(const char* location) 
{
    strncpy(g_transaction.location, location, sizeof(g_transaction.location)-1);
}

const char* RuntimeStorage::Transaction_GetLocation()
{
    return g_transaction.location;
}


void RuntimeStorage::Transaction_SetUnit(const char* unit)
{
    strncpy(g_transaction.unitNumber, unit, sizeof(g_transaction.unitNumber)-1);
    g_transactionDirty = true;
}

const char* RuntimeStorage::Transaction_GetUnit()
{
    return g_transaction.unitNumber;
}


void RuntimeStorage::Transaction_SetOdometer(uint32_t odometer)
{
    g_transaction.odometer = odometer;
    g_transactionDirty = true;
}

uint32_t RuntimeStorage::Transaction_GetOdometer()
{
    return g_transaction.odometer;
}

void RuntimeStorage::Transaction_SetPresetVolume(uint32_t preset)
{
    g_transaction.presetVolume = preset;
    g_transactionDirty = true;
}

uint32_t RuntimeStorage::Transaction_GetPresetVolume()
{
    return g_transaction.presetVolume;
}


void RuntimeStorage::Transaction_SetStart(double totaliser)
{
    g_transaction.startTotaliser = totaliser;
    g_transaction.startTime = Time_NowUnix(); // atau RTC

    g_transactionDirty = true;
}

double RuntimeStorage::Transaction_GetStart()
{
    return g_transaction.startTotaliser;
}

void RuntimeStorage::Transaction_SetEnd(double totaliser)
{
    g_transaction.endTotaliser = totaliser;
    g_transaction.endTime = Time_NowUnix();// atau RTC

    g_transactionDirty = true;
}

double RuntimeStorage::Transaction_GetEnd()
{
    return g_transaction.endTotaliser;
}

void RuntimeStorage::Transaction_SetLatitude(float latitude)
{
    g_transaction.latitude = latitude;
    g_transactionDirty = true;
}

float RuntimeStorage::Transaction_GetLatitude()
{
    return g_transaction.latitude;
}

void RuntimeStorage::Transaction_SetLongitude(float longitude)
{
    g_transaction.longitude = longitude;
    g_transactionDirty = true;
}

float RuntimeStorage::Transaction_GetLongitude()
{
    return g_transaction.longitude;
}

void RuntimeStorage::Transaction_SetVolume(double volume)
{
    g_transaction.volume = volume;
    g_transactionDirty = true;
}

double RuntimeStorage::Transaction_GetVolume()
{
    return g_transaction.volume;
}

  // ===== START TIME (masalah) =====
void RuntimeStorage::Transaction_SetStartTime(uint32_t startTime) {
    g_transaction.startTime = startTime;
    g_transactionDirty = true;

}
    
uint32_t RuntimeStorage::Transaction_GetStartTime(){
    return g_transaction.startTime;

}

    // ===== END TIME (masalah)=====
void RuntimeStorage::Transaction_SetEndTime(uint32_t endTime){
    g_transaction.endTime = endTime;
    g_transactionDirty = true;
}

uint32_t RuntimeStorage::Transaction_GetEndTime(){
    return g_transaction.endTime;
}

    // ===== SLIP NUMBER =====
void RuntimeStorage::Transaction_SetSlipNumber(uint32_t slipNumber){
    g_transaction.slipNumber = slipNumber;
    g_transactionDirty = true;
}       

uint32_t RuntimeStorage::Transaction_GetSlipNumber(){
    return g_transaction.slipNumber;
}

void RuntimeStorage::Transaction_Reset() {

  g_transaction.presetVolume = 0;
  g_transaction.odometer = 0;
  g_transaction.latitude = 0;
  g_transaction.longitude = 0;
  g_transaction.startTime =0;
  g_transaction.endTime =0;
  g_transaction.startTotaliser =0;
  g_transaction.endTotaliser =0;
  g_transaction.volume =0;
  g_transaction.slipNumber =0;

}

void RuntimeStorage::Transaction_SaveLast()
{
    memcpy(
        &g_lastTransaction,
        &g_transaction,
        sizeof(TransactionData)
    );

     g_hasLastTransaction = true;
}

void RuntimeStorage::Transaction_GetLast(TransactionData &out)
{
    // memcpy(
    //     &out,
    //     &g_lastTransaction,
    //     sizeof(TransactionData)
    // );

     File file =
        LittleFS.open("/lasttrx.dat", "r");

    if (!file) {

        Serial.println(
            "FAILED OPEN lasttrx.dat"
        );

        memset(
            &out,
            0,
            sizeof(TransactionData)
        );

        return;
    }

    // validasi ukuran file
    if (file.size() != sizeof(TransactionData)) {

        Serial.println(
            "INVALID lasttrx.dat SIZE"
        );

        memset(
            &out,
            0,
            sizeof(TransactionData)
        );

        file.close();

        return;
    }

    size_t readBytes =
        file.read(
            (uint8_t*)&out,
            sizeof(TransactionData)
        );

    file.close();

    if (readBytes != sizeof(TransactionData)) {

        Serial.println(
            "FAILED READ lasttrx.dat"
        );

        memset(
            &out,
            0,
            sizeof(TransactionData)
        );

        return;
    }

}

bool RuntimeStorage::Transaction_HasLast()
{
    return g_hasLastTransaction;
}



bool RuntimeStorage::Transaction_SaveLastToFS()
{
    // =========================================
    // ALWAYS SAVE LAST TRANSACTION
    // FOR REPRINT / RECOVERY
    // =========================================
    File lastFile =
        LittleFS.open("/lasttrx.dat", "w");

    if (!lastFile) {

        Serial.println(
            "FAILED OPEN lasttrx.dat"
        );

        return false;
    }
    g_lastTransaction = g_transaction;
    size_t writtenLast =
        lastFile.write(
            (uint8_t*)&g_lastTransaction,
            sizeof(TransactionData)
        );

    lastFile.close();

    if (writtenLast != sizeof(TransactionData)) {

        Serial.println(
            "FAILED WRITE lasttrx.dat"
        );

        return false;
    }

    // =========================================
    // SAVE JSON ONLY IF MODEM ENABLED
    // =========================================
    if (Config::GetModemStatus() == 1) {

        // -------------------------------------
        // BUILD FILE NAME
        // -------------------------------------
        char filename[32];

        snprintf(
            filename,
            sizeof(filename),
            "/trx_%06lu.json",
            g_transaction.slipNumber
        );

        // -------------------------------------
        // OPEN JSON FILE
        // -------------------------------------
        File jsonFile =
            LittleFS.open(filename, "w");

        if (!jsonFile) {

            Serial.println(
                "FAILED OPEN JSON FILE"
            );

            return false;
        }

        // -------------------------------------
        // BUILD JSON
        // -------------------------------------
        char json[1024];

        snprintf(
            json,
            sizeof(json),

            "{"
            "\"slipNumber\":%lu,"
            "\"userId\":\"%s\","
            "\"userName\":\"%s\","
            "\"panelId\":%u,"
            "\"location\":\"%s\","
            "\"companyCode\":\"%s\","
            "\"countryCode\":\"%s\","
            "\"unitNumber\":\"%s\","

            "\"startTime\":%lu,"
            "\"endTime\":%lu,"

            "\"startTotaliser\":%.3f,"
            "\"endTotaliser\":%.3f,"
            "\"volume\":%.3f,"

            "\"presetVolume\":%lu,"
            "\"odometer\":%lu,"

            "\"latitude\":%.6f,"
            "\"longitude\":%.6f,"

            "\"isActive\":%s"
            "}",

            g_lastTransaction.slipNumber,
            g_lastTransaction.userId,
            g_lastTransaction.userName,
            g_lastTransaction.panelId,
            g_lastTransaction.location,
            g_lastTransaction.companyCode,
            g_lastTransaction.countryCode,
            g_lastTransaction.unitNumber,

            g_lastTransaction.startTime,
            g_lastTransaction.endTime,

            g_lastTransaction.startTotaliser,
            g_lastTransaction.endTotaliser,
            g_lastTransaction.volume,

            g_lastTransaction.presetVolume,
            g_lastTransaction.odometer,

            g_lastTransaction.latitude,
            g_lastTransaction.longitude,

            g_lastTransaction.isActive
                ? "true"
                : "false"
        );

        // -------------------------------------
        // WRITE JSON
        // -------------------------------------
        size_t writtenJson =
            jsonFile.print(json);

        jsonFile.close();

        if (writtenJson == 0) {

            Serial.println(
                "FAILED WRITE JSON FILE"
            );

            return false;
        }

        Serial.printf(
            "JSON SAVED: %s\n",
            filename
        );
    }

    Serial.println(
        "LAST TRANSACTION SAVED"
    );

    return true;
}


void RuntimeStorage::ListFiles(const char* directory, uint8_t levels)
{
    File root = LittleFS.open(directory);

    if (!root) {

        Serial.printf(
            "FAILED OPEN DIR: %s\n",
            directory
        );

        return;
    }

    if (!root.isDirectory()) {

        Serial.printf(
            "NOT DIRECTORY: %s\n",
            directory
        );

        root.close();

        return;
    }

    File file = root.openNextFile();

    while (file) {

        if (file.isDirectory()) {

            Serial.print("[DIR ] ");
            Serial.println(file.name());

            // =====================================
            // RECURSIVE
            // =====================================
            if (levels > 0) {

                char subDir[64];

                strncpy(
                    subDir,
                    file.name(),
                    sizeof(subDir) - 1
                );

                subDir[sizeof(subDir) - 1] = '\0';

                ListFiles(subDir, levels - 1);
            }

        } else {

            Serial.print("[FILE] ");
            Serial.print(file.name());

            Serial.print("  SIZE: ");
            Serial.print(file.size());

            Serial.println(" bytes");
        }

        file.close();

        file = root.openNextFile();
    }

    root.close();
}


bool RuntimeStorage::Transaction_LoadLastFromFS()
{
    File file = LittleFS.open("/lasttrx.dat", "r");

    if (!file) {

        //Serial.println("NO LAST TRANSACTION FILE");

        return false;
    }

    size_t readed =
        file.read(
            (uint8_t*)&g_lastTransaction,
            sizeof(TransactionData)
        );

    file.close();

    if (readed != sizeof(TransactionData)) {

        //Serial.println("FAILED READ LAST TRX");

        return false;
    }

    g_hasLastTransaction = true;

    //Serial.println("LAST TRANSACTION LOADED");

    return true;
}



