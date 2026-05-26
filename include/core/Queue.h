#pragma once
#include "Event.h"
#include "PrintJob.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#ifdef ENABLE_NETWORK
#include "NetworkJob.h"
#endif

extern QueueHandle_t g_eventQueue;
extern QueueHandle_t g_printQueue;
#ifdef ENABLE_NETWORK
extern QueueHandle_t g_networkQueue;
#endif

void Queue_Init();

bool Queue_Send(Event *evt);
bool Queue_Receive(Event *evt, TickType_t timeout);

bool PrintQueue_Send(PrintJob *job);
bool PrintQueue_Receive(PrintJob *job, TickType_t timeout);

#ifdef ENABLE_NETWORK
bool NetworkQueue_Send(NetworkJob *job);
bool NetworkQueue_Receive(NetworkJob *job, TickType_t timeout);
#endif