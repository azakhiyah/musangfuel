#include <Arduino.h>

#include "tasks/TaskKeypad.h"

#include "io/Keypad.h"
#include "core/Event.h"
#include "core/Queue.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void TaskKeypad(void *pv) {

    Keypad_Init();

    Event evt;

    while (1) {

        if (Keypad_Available()) {

            evt.type = EVT_KEYPAD;
            evt.payload[0] = Keypad_GetKey(); // kirim ASCII

            Queue_Send(&evt);
        }

        vTaskDelay(pdMS_TO_TICKS(20)); // cukup cepat
    }


}