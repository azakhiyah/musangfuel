#include "io/GPS.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "core/DeviceManager.h"


// global cache (thread-safe sederhana)
static double g_lat = 0;
static double g_lng = 0;

// void TaskGPS(void *pv) {

//     gps.begin();

//     while (1) {

//         gps.update();

//         if (gps.isLocationUpdated()) {

//             g_lat = gps.getLatitude();
//             g_lng = gps.getLongitude();
//         }

//         vTaskDelay(pdMS_TO_TICKS(10)); // ringan & cepat
//     }
// }

void TaskGPS(void *pv) {

    // gps.begin();

    // Serial.print("GPS Task running on core: ");
    // Serial.println(xPortGetCoreID());

     bool initialized = false;
     bool lastState = false;


    while (1) {

       
       bool enabled = DeviceManager::isGpsEnabled();

        // 🔥 DETEKSI PERUBAHAN STATE
        if (enabled != lastState) {

            if (enabled) {
                //Serial.println("GPS ENABLED → INIT");
                gps.begin();
                initialized = true;
            } else {
                Serial.println("GPS DISABLED");
                initialized = false;  // 🔥 reset
            }

            lastState = enabled;
        }

        if (!enabled) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        // normal operation
        if (initialized) {
            gps.update();

                // 🔥 cek update
                if (gps.isLocationUpdated()) {

                    float lat = gps.getLatitude();
                    float lng = gps.getLongitude();

                    // Serial.print("LAT: ");
                    // Serial.print(lat, 6);

                    // Serial.print(" | LNG: ");
                    // Serial.println(lng, 6);
                }

                // 🔥 cek status (optional)
                if (!gps.isLocationValid()) {
                    //Serial.println("GPS belum fix...");
                }
            }

        vTaskDelay(pdMS_TO_TICKS(500)); // setengah detik
    }
}