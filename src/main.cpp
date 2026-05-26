#include <Arduino.h>
//#include <EEPROM.h>
#include <LittleFS.h>

#include "config/Config.h"
//#include "config/ConfigHandler.h"
#include "config/UserHandler.h"

#include "core/Queue.h"
#include "core/StateMachine.h"
#include "core/RuntimeStorage.h"
#include "core/DeviceManager.h"

#include "services/TimeService.h"

#include "tasks/TaskUI.h"
#include "tasks/TaskKeypad.h"
#include "tasks/TaskGPS.h"
#include "tasks/TaskPulser.h"
#include "tasks/TaskStateMachine.h"
#include "tasks/TaskPrinter.h"

#ifdef ENABLE_NETWORK
#include "tasks/TaskNetwork.h"
#endif

void setup() {
    Serial.begin(115200);
    delay(1000); // wait for Serial to initialize
    LittleFS.begin();
    Config::Init();
    DeviceManager::begin();
    delay(5);
    UserHandler_LoadFromFS();
    Queue_Init();
    StateMachine_Init();
    TimeService_Init();  //

    RuntimeStorage::begin();


    //RuntimeStorage::Shift_Reset(); // 🔥 reset shift saat boot (untuk development, nanti dihapus)
    
    RuntimeStorage::loadShift(g_Shift);
    RuntimeStorage::Transaction_LoadLastFromFS();

    RuntimeStorage::ListFiles("/",0);
    
    char buffer[21];
    /*
    1.tambah menu input tgl DAN WAKTU *
    2 fix reprint transaksi saat restart hilang *
    3.tambah task RFID
    4.tambah task Send Data
    - tambah logo
    5.Cari cara upload file user.db dan config.txt (dah ga perlu insert via keypad)*
    - menu insert user  baru 
    6.bikin menu untuk melihat data konfigurasi *
    7.Preset bisa dikosongkan jika kosong maka nilai maks 999999 *
    8.unit no bisa kosong()
    9.Menu Input COnfigurasi *
  
    */


    // CORE 1 (default 4096)
    xTaskCreatePinnedToCore(TaskUI, "UI", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(TaskKeypad, "Keypad", 2048, NULL, 3, NULL, 1);
    xTaskCreatePinnedToCore(TaskStateMachine,"StateMachine",8192,NULL,2, NULL,1);
    xTaskCreatePinnedToCore(TaskPulser,"TaskPulser",2048,NULL,2,NULL,1);
    
    // CORE 0 
    xTaskCreatePinnedToCore(TaskGPS,"TaskGPS",2048,NULL, 1, NULL,0);
    xTaskCreatePinnedToCore(TaskPrinter,"TaskPrinter",4096,NULL,1,NULL,0);
    #ifdef ENABLE_NETWORK
    xTaskCreatePinnedToCore(TaskNetwork,"TaskNetwork",4096,NULL,1,NULL,0);
    #endif
    
}

void loop() {

    RuntimeStorage::update();
}

