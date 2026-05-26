//#include <Arduino.h>
#include "core/Queue.h"

QueueHandle_t g_eventQueue;
QueueHandle_t g_printQueue;

 #ifdef ENABLE_NETWORK
QueueHandle_t g_networkQueue;
 #endif

void Queue_Init() {
    g_eventQueue = xQueueCreate(30, sizeof(Event));
    g_printQueue =xQueueCreate(5, sizeof(PrintJob));

    #ifdef ENABLE_NETWORK
    g_networkQueue =xQueueCreate(20,sizeof(NetworkJob));
    #endif
   
}

bool Queue_Send(Event *evt) {
       bool ok = xQueueSend(g_eventQueue, evt, 0);

        if (!ok) {
            //Serial.println("QUEUE FULL / SEND FAILED");
        } else {
           // Serial.println("QUEUE SEND OK");
        }

        return ok;

 
}

bool Queue_Receive(Event *evt, TickType_t timeout) {
    return xQueueReceive(g_eventQueue, evt, timeout) == pdTRUE;
}


bool PrintQueue_Send(PrintJob *job) {
       bool ok = xQueueSend(g_printQueue,job, 0);

        if (!ok) {
            //Serial.println("QUEUE FULL / SEND FAILED");
        } else {
           // Serial.println("QUEUE SEND OK");
        }

        return ok;
}

bool PrintQueue_Receive(PrintJob *job, TickType_t timeout) {
    return xQueueReceive(g_printQueue, job, timeout) == pdTRUE;
}

 #ifdef ENABLE_NETWORK
bool NetworkQueue_Send(NetworkJob *job) {
       bool ok = xQueueSend(g_networkQueue,job, 0);

        if (!ok) {
            //Serial.println("QUEUE FULL / SEND FAILED");
        } else {
           // Serial.println("QUEUE SEND OK");
        }

        return ok;
}

bool NetworkQueue_Receive(NetworkJob *job, TickType_t timeout) {
    return xQueueReceive(g_networkQueue, job, timeout) == pdTRUE;
}
 #endif