//    #include <Arduino.h>
#include "tasks/TaskNetwork.h"
#include "services/NetworkService.h"
#include "config/Config.h"
#include "core/DeviceManager.h"
#include "core/NetworkJob.h"
#include "core/Queue.h"

// void TaskNetwork(void *pv) {
//     bool initialized = false;
//     bool lastState = false;
//      while (1)
//     {
//        bool enabled = DeviceManager::isModemEnabled();
//        if (enabled != lastState) {
//            if (enabled) {
//             network.begin(
//                 Config::GetSSID(),
//                 Config::GetSSIDPassword(),
//                 Config::GetIP(),
//                 Config::GetHostPort(),
//                 Config::GetMQTTUser(),
//                 Config::GetMQTTPassword(),
//                 Config::GetMQTTTopic(),
//                 IPAddress(10,206,105,186), // UDP SERVER
//                 5000                      // UDP PORT
//             );
//             initialized = true;            
//            } else {
//             Serial.println("Modem disabled");
//             initialized = false;
//            }
//            lastState = enabled;
//        }

//        if (!enabled) {
//             vTaskDelay(pdMS_TO_TICKS(1000));
//             continue;
//        }

//        if (initialized) {
//            network.update();          
//        }

//         vTaskDelay(pdMS_TO_TICKS(100));
//     }
// }

 void TaskNetwork(void *pv) {
      bool initialized = false;
      bool lastState = false;
      NetworkJob job;

     while (1)
    {
       bool enabled = DeviceManager::isModemEnabled();
       if (enabled != lastState) {
           if (enabled) {
            network.begin(
                Config::GetSSID(),
                Config::GetSSIDPassword(),
                Config::GetIP(),
                Config::GetHostPort(),
                Config::GetMQTTUser(),
                Config::GetMQTTPassword(),
                Config::GetMQTTTopic(),
                IPAddress(10,66,167,186), // UDP SERVER
                5000                      // UDP PORT
            );
            initialized = true;            
           } else {
            Serial.println("Modem disabled");
            initialized = false;
           }
           lastState = enabled;
       }

       if (!enabled) {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
       }

       if (initialized) {
           network.update();          
       }

        while (
            NetworkQueue_Receive(
                &job,
                0
            )
        )

        {

            switch (job.type)
            {

                // =================================
                // UDP TELEMETRY
                // =================================
                case TELEMTRY:
                {
                    char udpPayload[128];

                    snprintf(
                        udpPayload,
                        sizeof(udpPayload),

                        "P,%lu,%u,%u,%u,%.1f,%.1f,;",

                        job.telemetryPayload.timestamp,

                        job.telemetryPayload.panelId,

                        job.telemetryPayload.fastValve
                            ? 1
                            : 0,

                        job.telemetryPayload.slowValve
                            ? 1
                            : 0,

                        job.telemetryPayload.volume,

                        job.telemetryPayload.flowRate
                    );

                    network.sendUDP(
                        udpPayload
                    );

                    break;
                }

                // =================================
                // MQTT TRANSACTION
                // =================================
                case MQTT:
                {
                    Serial.print(
                        "MQTT SEND FILE: "
                    );

                    Serial.println(
                        job.payload.filename
                    );

                    // nanti:
                    // load json file
                    // publish mqtt
                    // delete file

                    break;
                }

                default:
                    break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }

 }
    

    /*
void TaskNetwork(void *pv)
{
      bool initialized = false;
      bool lastState = false;
      NetworkJob job;

    while (1)
    {
        // =====================================
        // CHECK MODEM STATUS
        // =====================================
        bool enabled =
            DeviceManager::isModemEnabled();

        // =====================================
        // MODEM STATE CHANGED
        // =====================================
        if (enabled != lastState)
        {
            if (enabled)
            {
                Serial.println(
                    "MODEM ENABLED"
                );

                network.begin(
                    Config::GetSSID(),
                    Config::GetSSIDPassword(),
                    Config::GetIP(),
                    Config::GetHostPort(),
                    Config::GetMQTTUser(),
                    Config::GetMQTTPassword(),
                    Config::GetMQTTTopic(),
                    IPAddress(10,66,167,186),
                    5000
                );

                initialized = true;
            }
            else
            {
                Serial.println(
                    "MODEM DISABLED"
                );

                initialized = false;
            }

            lastState = enabled;
        }

        // =====================================
        // MODEM OFF
        // =====================================
        if (!enabled)
        {
            vTaskDelay(
                pdMS_TO_TICKS(1000)
            );

            continue;
        }

        // =====================================
        // NETWORK MAINTENANCE
        // =====================================
        if (initialized)
        {
            network.update();
        }

        // =====================================
        // RECEIVE SINGLE NETWORK JOB
        // FIFO STYLE
        // =====================================
        if (
            NetworkQueue_Receive(
                &job,
                0
            )
        )
        {

            switch (job.type)
            {

                // =================================
                // UDP TELEMETRY
                // =================================
                case TELEMTRY:
                {
                    char udpPayload[128];

                    snprintf(
                        udpPayload,
                        sizeof(udpPayload),

                        "P,%lu,%u,%u,%u,%.1f,%.1f,;",

                        job.telemetryPayload.timestamp,

                        job.telemetryPayload.panelId,

                        job.telemetryPayload.fastValve
                            ? 1
                            : 0,

                        job.telemetryPayload.slowValve
                            ? 1
                            : 0,

                        job.telemetryPayload.volume,

                        job.telemetryPayload.flowRate
                    );

                    network.sendUDP(
                        udpPayload
                    );

                    break;
                }

                // =================================
                // MQTT JSON FILE
                // =================================
                case MQTT:
                {
                    Serial.print(
                        "MQTT SEND FILE: "
                    );

                    Serial.println(
                        job.payload.filename
                    );

                    // TODO:
                    // read json file
                    // publish mqtt
                    // delete file

                    break;
                }

                default:
                    break;
            }
        }

        // =====================================
        // VERY SMALL DELAY
        // =====================================
        vTaskDelay(
            pdMS_TO_TICKS(1)
        );
    }
}
    */