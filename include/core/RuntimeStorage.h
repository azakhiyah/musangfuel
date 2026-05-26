#pragma once

#include <Arduino.h>
#include <Preferences.h>

typedef struct {
    uint32_t openTimestamp;
    uint8_t panelId;
    char userId[20];
    double startTotaliser;
    double endTotaliser;
    uint32_t closeTimestamp;
    uint32_t volume;
    bool isActive;
} ShiftData;

typedef struct {
    double startTotaliser;
    double endTotaliser;
    double volume;

    float latitude;
    float longitude;

    uint32_t startTime;
    uint32_t endTime;
    uint32_t presetVolume;
    uint32_t odometer;
    uint32_t slipNumber;

    uint8_t panelId;
    bool isActive;

    char userId[20];
    char userName[32];
    char location[32];
    char companyCode[16];
    char countryCode[8];
    char unitNumber[16];

    //bool isActive;   // penting untuk recovery kalau mati listrik
} TransactionData;

extern ShiftData g_Shift;
extern TransactionData g_transaction;

class RuntimeStorage {
public:

    static void Transaction_Reset();

    static TransactionData& Transaction_Get();

    static void Transaction_GetData(TransactionData &out);
    static void Shift_GetData(ShiftData &out);

    static void Transaction_SaveLast();
    static void Transaction_GetLast(TransactionData &out);

    static bool Transaction_SaveLastToFS();
    static bool Transaction_LoadLastFromFS();


    static bool Transaction_HasLast();
    
    // ===== INIT =====
    static void begin();

    // ===== PERIODIC (WAJIB DIPANGGIL DI LOOP / TASK) =====
    static void update();

    // ===== FORCE SAVE (event penting) =====
    static void forceSave();

    // ===== SHIFT =====
    static bool saveShift(const ShiftData &data);
    static bool loadShift(ShiftData &data);
    static void Shift_Reset();
 

    // ===== SLIP =====
    static void setSlip(uint32_t value);
    static uint32_t getSlip();

    // ===== TOTALISER =====
    static void setTotaliser(double value);
    static double getTotaliser();


    // ======== TRANSACTION ========
    
    // ===== USER ID =====
    static void Transaction_SetUserId(const char* userId);
    static const char* Transaction_GetUserId();

    // ===== USER NAME=====
    static void Transaction_SetUserName(const char* userName);
    static const char* Transaction_GetUserName();

    // ===== PANEL ID =====
    static  void Transaction_SetPanelId(uint8_t panelId);
    static  uint8_t Transaction_GetPanelId();

    // ===== COMPANY CODE =====
    static void Transaction_SetCompanyCode(const char* companyCode);
    static const char* Transaction_GetCompanyCode();
    
    // ===== COUNTRY CODE =====
    static void Transaction_SetCountryCode(const char* countryCode);
    static const char* Transaction_GetCountryCode();

    // ===== LOCATION =====
    static void Transaction_SetLocation(const char* location);
    static const char* Transaction_GetLocation();
    
    // ===== UNIT NUMBER ===
    static void Transaction_SetUnit(const char* unit);
    static const char* Transaction_GetUnit();

    // ===== ODOMETER =====
    static void Transaction_SetOdometer(uint32_t odometer);
    static uint32_t Transaction_GetOdometer();

    // ===== PRESET VOLUME =====
    static void Transaction_SetPresetVolume(uint32_t preset);
    static uint32_t Transaction_GetPresetVolume();

    //===== VOLUME =====
    static void Transaction_SetVolume(double volume);
    static double Transaction_GetVolume();

    // ===== Latitude =====
    static void Transaction_SetLatitude(float latitude);
    static float Transaction_GetLatitude(); 

    // ===== Longitude =====
    static void Transaction_SetLongitude(float longitude);
    static float Transaction_GetLongitude(); 

    // ===== START TIME =====
    static void Transaction_SetStartTime(uint32_t startTime);
    static uint32_t Transaction_GetStartTime();

    // ===== END TIME =====
    static void Transaction_SetEndTime(uint32_t endTime);
    static uint32_t Transaction_GetEndTime();

    // ===== START TOTALISER =====
    static void Transaction_SetStart(double totaliser);
    static double Transaction_GetStart();

    // ===== END TOTALISER =====
    static void Transaction_SetEnd(double totaliser);
    static double Transaction_GetEnd();    

    // ===== SLIP NUMBER =====
    static void Transaction_SetSlipNumber(uint32_t slipNumber);
    static uint32_t Transaction_GetSlipNumber();


    static void ListFiles(const char* directory, uint8_t levels);
    



private:
     static Preferences prefs;

    // ===== RUNTIME CACHE (RAM) =====
    static uint32_t g_slip;
    static double g_totaliser;


    //static TransactionData g_transaction;

    // ===== DIRTY FLAG =====
    static bool dirtySlip;
    static bool dirtyTotaliser;
    static bool g_transactionDirty;

    // ===== TIMER =====
    static unsigned long lastSaveTime;

    // ===== CONFIG =====
    static const uint32_t SAVE_INTERVAL_MS = 5000; // 5 detik

};