#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

#include <Arduino.h>
#include <RTClib.h>

typedef struct {
    uint8_t day;
    uint8_t month;
    uint16_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} DateTimeSimple;

// =========================
// INIT
// =========================
void TimeService_Init();

// =========================
// GET TIME
// =========================
DateTimeSimple Time_GetNow();

// =========================
// UNIX
// =========================

uint32_t Time_NowUnix();

DateTimeSimple Unix_ToDateTime(uint32_t unixTime);

void setDateTime(const DateTime& dt);


#endif