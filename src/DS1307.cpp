#include "DS1307.h"
#include <Arduino.h>
#include <time.h>
#include <unistd.h> // for settimeofday()
#include <sys/time.h>   // for struct timeval
#include <Wire.h>

DS1307::~DS1307()
{
}


bool DS1307::isAvailable(){
    auto reg = readReg(0);
    return !(reg & 0x80);   // Read Clock Halt bit
}

void DS1307::writeTimeToRTC(time_t t){
    struct tm local;
    localtime_r(&t, &local);
    uint8_t buf[7] = {0};
    buf[0] = dec2bcd(local.tm_sec) & 0x7F;
    buf[1] = dec2bcd(local.tm_min) & 0x7F;
    buf[2] = dec2bcd(local.tm_hour) & 0x3F;
    buf[3] = (local.tm_wday + 1) & 0x07;
    buf[4] = dec2bcd(local.tm_mday) & 0x3F;  
    buf[5] = dec2bcd(local.tm_mon + 1) & 0x1F;
    buf[6] = dec2bcd(local.tm_year % 100) & 0x3F;      
    writeRegBurst(0, buf, 7);
}

time_t DS1307::readTimeFromRTC(){
    if(!isAvailable()) return -1;
    uint8_t buf[7];
    readRegBurst(0, buf, 7);

    struct tm local;
    local.tm_sec = bcd2dec(buf[0]);
    local.tm_min = bcd2dec(buf[1]);
    if(buf[2] & 0x40){
        //12 hour mode
        local.tm_hour = bcd2dec(buf[2] & 0x1F) + (buf[2] & 0x20) ? 12 : 0;
    }else{
        local.tm_hour = bcd2dec(buf[2]);
    }
    local.tm_mday = bcd2dec(buf[4]);
    local.tm_mon = bcd2dec(buf[5]) - 1;
    local.tm_year = 2000 - 1900 + bcd2dec(buf[6]);

    return mktime(&local);
}