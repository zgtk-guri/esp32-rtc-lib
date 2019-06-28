#ifndef _DS1307_H_
#define _DS1307_H_

#include "ESP_RTC.h"

class DS1307 : public RTC{
public:
    using RTC::RTC;
    DS1307() : RTC(0x68) {}
    ~DS1307();

    bool isAvailable();
    using RTC::writeTimeToRTC;
    void writeTimeToRTC(time_t t);
    time_t readTimeFromRTC();
};

#endif