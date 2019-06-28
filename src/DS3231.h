#ifndef _DS3231_H_
#define _DS3231_H_

#include "ESP_RTC.h"

class DS3231 : public RTC{
public:
    using RTC::RTC;
    DS3231() : RTC(0x68) {}
    ~DS3231();

    bool isAvailable();
    using RTC::writeTimeToRTC;
    void writeTimeToRTC(time_t t);
    time_t readTimeFromRTC();
};

#endif