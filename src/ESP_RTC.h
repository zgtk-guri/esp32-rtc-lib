#ifndef _ESP_RTC_H_
#define _ESP_RTC_H_

#include <Arduino.h>
#include <time.h>

class RTC{
public:
    RTC(uint8_t address, xSemaphoreHandle semaphore = NULL)
        : m_address(address), m_sem(semaphore) {}
    virtual ~RTC();

    virtual bool isAvailable() = 0;
    inline void writeTimeToRTC() { writeTimeToRTC(time(NULL)); }
    virtual void writeTimeToRTC(time_t t) = 0;
    virtual time_t readTimeFromRTC() = 0;
    void syncTimeFromRTC();

protected:
    void writeReg(uint8_t regAddr, uint8_t value);
    void writeRegBurst(uint8_t startRegAddr, const uint8_t *buf, uint8_t size);
    uint8_t readReg(uint8_t regAddr);
    void readRegBurst(uint8_t startRegAddr, uint8_t *buf, uint8_t size);

    void takeSemaphore();
    void giveSemaphore();

    inline uint8_t dec2bcd(int value) { return ((value / 10) << 4) | (value % 10) & 0x0F; }
    inline int bcd2dec(uint8_t value) { return (value >> 4) * 10 + (value & 0x0F); }
private:
    uint8_t m_address;
    xSemaphoreHandle m_sem;
};

#endif