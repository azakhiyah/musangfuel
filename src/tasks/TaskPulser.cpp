#include "io/PulserEncoder.h"
#include "core/StateMachine.h"    
#include "config/Config.h"

extern PulserEncoder pulser;


void TaskPulser(void *pv) {

    pulser.begin(32, 33);
    //pulser.setKFactor(100.0f);
    pulser.setKFactor(Config::GetKFactor());

    bool running = false;


    //Serial.print("Pulser Task Core: ");
    //Serial.println(xPortGetCoreID());

    while (1) {

        bool shouldRun = StateMachine_IsTransactionRunning();

        // 🔥 DETEKSI TRANSISI
        if (shouldRun && !running) {
            //Serial.println("PULSER START");
            pulser.reset();
            pulser.start();
            running = true;
        }

   

        if (!shouldRun && running) {
            //Serial.println("PULSER STOP");
            pulser.stop();
            running = false;
        }


        // 🔥 HANYA UPDATE SAAT RUNNING
        if (running) {
            pulser.update();
        }

        vTaskDelay(pdMS_TO_TICKS(running ? 100 : 500));
    }
}