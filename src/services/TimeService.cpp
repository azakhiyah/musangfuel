#include "services/TimeService.h"
#include <Wire.h>

RTC_DS3231 rtc;

// =========================
// INIT
// =========================
void TimeService_Init()
{
    Wire.begin();

    if (!rtc.begin()) {

        Serial.println("RTC NOT FOUND");
        return;
    }

    //Serial.println("RTC READY");

    if (rtc.lostPower()) {

        Serial.println("RTC LOST POWER");

        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//         rtc.adjust(DateTime(
//             2026, // year
//             5,    // month
//             12,    // day
//             14,   // hour
//             32,   // minute
//             0     // second
// ));

        Serial.println("RTC AUTO SET");
    }

    DateTime now = rtc.now();

    // Serial.printf(
    //     "%02d-%02d-%04d %02d:%02d:%02d\n",
    //     now.day(),
    //     now.month(),
    //     now.year(),
    //     now.hour(),
    //     now.minute(),
    //     now.second()
    // );

    // Serial.print(rtc.now().unixtime());
}

// DateTime getDateTime() {
//     return rtc.now();
// }

// =========================
// GET NOW
// =========================
DateTimeSimple Time_GetNow()
{
    DateTime now = rtc.now();

    DateTimeSimple t;

    t.day    = now.day();
    t.month  = now.month();
    t.year   = now.year();
    t.hour   = now.hour();
    t.minute = now.minute();
    t.second = now.second();

    return t;
}

// // =========================
// // TO UNIX
// // =========================
// uint32_t DateTime_ToUnix(const DateTimeSimple &t)
// {
//     DateTime dt(
//         t.year,
//         t.month,
//         t.day,
//         t.hour,
//         t.minute,
//         t.second
//     );

//     return dt.unixtime();
// }

// =========================
// UNIX TO DATETIME
// =========================
DateTimeSimple Unix_ToDateTime(uint32_t unixTime)
{
    DateTime dt(unixTime);

    DateTimeSimple t;

    t.day    = dt.day();
    t.month  = dt.month();
    t.year   = dt.year();
    t.hour   = dt.hour();
    t.minute = dt.minute();
    t.second = dt.second();

    return t;
}

uint32_t Time_NowUnix()
{
    return rtc.now().unixtime();
}

void setDateTime(const DateTime& dt) {
  rtc.adjust(dt);
}

