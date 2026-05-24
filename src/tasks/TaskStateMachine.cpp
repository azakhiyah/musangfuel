//#include <Arduino.h>

#include "core/Event.h"
#include "core/Queue.h"
#include "core/StateMachine.h"
#include "config/Config.h"
#include "core/RuntimeStorage.h"
#include "io/PulserEncoder.h"
#include "services/NetworkService.h"
//#include "tasks/TaskUI.h"
#include "ui/OperationMenu.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void TaskStateMachine(void *pv) {

    Event evt;
    bool done = false;
    bool fastvalveOpened = false;
    bool slowMode = false;
    uint8_t fastValveStatus = 0;
    uint8_t slowValveStatus = 0;


    while (1) {

        if (xQueueReceive(g_eventQueue, &evt,pdMS_TO_TICKS(100)) == pdTRUE) {

            // 🔥 DEBUG
            // Serial.printf("Event diterima: type=%d, data=%c\n",
            //               evt.type,
            //               (char)evt.data);

            //Serial.printf("Queue addr SM: %p\n", g_eventQueue);
            //Serial.printf("Event diterima: type=%d\n", evt.type);
            //Serial.println("EVENT RECEIVED IN STATE MACHINE");

            StateMachine_HandleEvent(&evt);
          
        }

        AppState state = StateMachine_GetState();
/*
        if (state == STATE_EXECUTING_TRANSACTION) {
       
            uint32_t preset = RuntimeStorage::Transaction_GetPresetVolume();
            done = handle_PulserDisplay(pulser, preset);

                if (!done) {
                    //Serial.print(fastvalveOpened);
                    double vol = pulser.getVolume();
                    double delta = preset - vol;
                    float  _slowDownThreshold = Config::GetSlowDownThreshold();

               
                    if (!fastvalveOpened) {
                          pulser.open1();
                          pulser.open2();
                          fastvalveOpened = true;
                    } 
                        // Jika mendekati preset, FLOW_PIN_1 dimatikan
                    if (!slowMode && delta <= _slowDownThreshold) { //masalah
                        pulser.close1();
                         slowMode = true;
                    } 
                    
                    if (vol >= preset) {
                        // Jika volume sudah tercapai, FLOW_PIN_2 dimatikan
                        pulser.close2(); 
                    }
                    
                } else if  (done) {
                    pulser.close1();
                    pulser.close2();
                    fastvalveOpened = false;
                    slowMode = false;
                }
        }
*/        

         if (state == STATE_EXECUTING_TRANSACTION)
    {
        NetworkJob job;
        static uint32_t lastUdpSend = 0;

        uint32_t preset =
            RuntimeStorage::
                Transaction_GetPresetVolume();

        done =
            handle_PulserDisplay(
                pulser,
                preset
            );

        if (!done)
        {
            double vol =
                pulser.getVolume();

            double delta =
                preset - vol;

            float slowDownThreshold =
                Config::
                    GetSlowDownThreshold();

            // =====================================
            // OPEN FAST + SLOW VALVE
            // =====================================
            if (!fastvalveOpened)
            {
                pulser.open1();
                pulser.open2();

                 fastValveStatus = 1;
                 slowValveStatus = 1;

                fastvalveOpened = true;
            }

            // =====================================
            // SLOW MODE
            // =====================================
            if (!slowMode &&
                delta <= slowDownThreshold)
            {
                pulser.close1();

                 fastValveStatus = 0;
                 slowValveStatus = 1;

                slowMode = true;
            }

            // =====================================
            // CLOSE FINAL VALVE
            // =====================================
            if (vol >= preset)
            {
                pulser.close2();
            }

            // =====================================
            // SEND LIVE UDP
            // EVERY 500 ms
            // =====================================
            if (millis() - lastUdpSend >= 500)
            {
                lastUdpSend = millis();

                // char udpPayload[128];

                // snprintf(
                //     udpPayload,
                //     sizeof(udpPayload),

                //     "P,%lu,%u,%u,%u,%.2f,%.2f,;",

                //     millis(),

                //     Config::GetPanelId(),

                //     fastValveStatus,

                //     slowValveStatus,

                //     vol,

                //     pulser.getLPM()
                // );

                // network.sendUDP(udpPayload);

                job.type = TELEMTRY;
                job.telemetryPayload.timestamp = millis();
                job.telemetryPayload.panelId = Config::GetPanelId();
                job.telemetryPayload.fastValve = fastValveStatus;
                job.telemetryPayload.slowValve = slowValveStatus;
                job.telemetryPayload.volume = vol;
                job.telemetryPayload.flowRate = pulser.getLPM();
                NetworkQueue_Send(&job);

            }
        }
        else
        {
            pulser.close1();
            pulser.close2();

            fastvalveOpened = false;
            slowMode = false;

            fastValveStatus = 0;
            slowValveStatus = 0;

        }
    }

        vTaskDelay(pdMS_TO_TICKS(5)); // loop cepat

    }

}