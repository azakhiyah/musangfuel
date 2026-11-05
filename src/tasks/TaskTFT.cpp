#include <Arduino.h>

#include "tasks/TaskTFT.h"
#include "io/SPITFT.h"
#include "core/StateMachine.h"
#include "ui/tft/UIOperation.h"
#include "services/TimeService.h"

static SPITFT tft;

void TaskTFT(void *pv)
{
    tft.begin(1);

    AppState lastState = static_cast<AppState>(-1);
    int lastMinute = -1;

    while (true)
    {
        AppState currentState = StateMachine_GetState();

        if (currentState != lastState)
        {
            switch (currentState)
            {
                case STATE_HOME:
                    UI_TFT_RenderHome(tft);
                    lastMinute = -1;
                    break;

                case STATE_SHIFT:
                    UI_TFT_RenderShift(tft,0);
                    break;

                // case STATE_SHIFT_OPEN_USERID:
                //     // Akan dihubungkan dengan buffer User ID
                //     UI_TFT_RenderShiftOpenUser(tft, false);
                    
                //     break;

                // case STATE_SHIFT_OPEN_PASSWORD:
                //     // Akan dihubungkan dengan buffer Password
                //     UI_TFT_RenderShiftOpenUser(tft, true);
                //     break;

                default:
                    tft.clear(TFT_BLACK);
                    break;
            }

            lastState = currentState;

            Serial.printf(
                "[TaskTFT] State changed: %d\n",
                (int)currentState
            );
        }

        // HOME: update waktu setiap pergantian menit
        if (currentState == STATE_HOME)
        {
            DateTimeSimple now = Time_GetNow();

            if (now.minute != lastMinute)
            {
                UI_TFT_UpdateHomeTime(tft);
                lastMinute = now.minute;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(250));
    }
}