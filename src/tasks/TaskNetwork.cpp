#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>
#include "tasks/TaskNetwork.h"
#include "services/NetworkService.h"
#include "config/Config.h"
#include "core/DeviceManager.h"
#include "core/NetworkJob.h"
#include "core/Queue.h"

 void TaskNetwork(void *pv) {
      bool initialized = false;
      bool lastState = false;
      uint32_t lastMqttSend = 0;
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

       if (millis() - lastMqttSend > 1000) {
        lastMqttSend = millis();

        network.sendPendingTransaction();
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
                // case MQTT:
                // {
                //     Serial.print(
                //         "MQTT SEND FILE: "
                //     );

                //     Serial.println(
                //         job.mqttPayload.filename
                //     );

                //     File file =
                //     LittleFS.open(
                //         job.mqttPayload.filename,
                //         "r"
                //     );

                //     if (!file)
                //     {
                //         Serial.println(
                //             "FAILED OPEN MQTT FILE"
                //         );

                //         break;
                //     }

                //     String json =
                //     file.readString();
                //     file.close();
                //     bool ok =
                //     network.publishMQTT(
                //         json.c_str()
                //     );

                //     if (ok)
                //     {
                //         LittleFS.remove(
                //             job.mqttPayload.filename
                //         );

                //         Serial.println(
                //             "JSON FILE DELETED"
                //         );
                //     }

                //     break;
                // }

                default:
                    break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }

 }
    

 