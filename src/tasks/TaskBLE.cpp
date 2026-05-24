// #include <Arduino.h>

// #include "tasks/TaskBLE.h"

// #include "core/Event.h"
// #include "core/Queue.h"
// #include "core/StateMachine.h"

// #include "config/Config.h"

// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// // ==========================
// // UUID (bebas, tapi konsisten)
// // ==========================
// #define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
// #define CHAR_APP_UUID       "12345678-1234-1234-1234-1234567890ac"
// #define CHAR_SAVE_UUID      "12345678-1234-1234-1234-1234567890ad"

// // ==========================
// static BLEServer* pServer = nullptr;
// static BLECharacteristic* pCharApp = nullptr;
// static BLECharacteristic* pCharSave = nullptr;

// static bool bleActive = false;

// // ==========================
// // Callback APP NAME
// // ==========================
// class AppNameCallback : public BLECharacteristicCallbacks {
//     void onWrite(BLECharacteristic *pChar) override {
//         Serial.println("BLE WRITE CALLBACK TRIGGERED");  // 🔥 DEBUG 1
//         std::string value = pChar->getValue();

//         if (value.length() == 0) return;
//         Serial.print("RAW VALUE: ");                    // 🔥 DEBUG 2
//         Serial.println(value.c_str());

//         // batasi max 20 char
//         char buffer[21];
//         size_t len = value.length() > 20 ? 20 : value.length();

//         memcpy(buffer, value.data(), len);
//         buffer[len] = '\0';

//         Event evt;
//         evt.type = EVT_CONFIG_UPDATE;
//         strncpy(evt.payload, buffer, 20);
//         evt.payload[20] = '\0';

//         Serial.println("SEND EVT_CONFIG_UPDATE");       // 🔥 DEBUG 3
//         Serial.printf("Queue addr BLE: %p\n", g_eventQueue);

//         Queue_Send(&evt);
//     }
// };

// // ==========================
// // Callback SAVE
// // ==========================
// class SaveCallback : public BLECharacteristicCallbacks {
//     void onWrite(BLECharacteristic *pChar) override {

//         Serial.println("BLE SAVE TRIGGERED");

//         Event evt;
//         evt.type = EVT_CONFIG_SAVE;

//         Queue_Send(&evt);
//     }
// };

// // ==========================
// // INIT BLE
// // ==========================
// static void BLE_Start() {

//     BLEDevice::init("FuelDevice_Config");

//     pServer = BLEDevice::createServer();
//     BLEService *pService = pServer->createService(SERVICE_UUID);

//     // APP NAME
//     pCharApp = pService->createCharacteristic(
//         CHAR_APP_UUID,
//         BLECharacteristic::PROPERTY_READ |
//         BLECharacteristic::PROPERTY_WRITE
//     );

//     pCharApp->setCallbacks(new AppNameCallback());
//     pCharApp->setValue(Config::GetAppName());

//     // SAVE
//     pCharSave = pService->createCharacteristic(
//         CHAR_SAVE_UUID,
//         BLECharacteristic::PROPERTY_WRITE
//     );

//     pCharSave->setCallbacks(new SaveCallback());

//     pService->start();

//     BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
//     pAdvertising->start();

//     bleActive = true;
// }

// // ==========================
// // STOP BLE
// // ==========================
// static void BLE_Stop() {

//     if (bleActive) {
//         BLEDevice::deinit(true);
//         bleActive = false;
//     }
// }

// // ==========================
// // TASK
// // ==========================
// void TaskBLE(void *pv) {

//     while (1) {

//         AppState state = StateMachine_GetState();

//         if (state == STATE_CONFIG) {

//             if (!bleActive) {
//                 BLE_Start();
//             }

//             // update value (biar UI sync)
//             if (pCharApp) {
//                 pCharApp->setValue(Config::GetAppName());
//             }

//         } else {

//             if (bleActive) {
//                 BLE_Stop();
//             }
//         }

//         vTaskDelay(pdMS_TO_TICKS(200));
//     }
// }