#include "ESP_RTC.h"
#include <Arduino.h>
#include <time.h>
#include <unistd.h> // for settimeofday()
#include <sys/time.h>   // for struct timeval
#include <Wire.h>

RTC::~RTC() {}

void RTC::syncTimeFromRTC(){
    struct timeval tv;
    tv.tv_sec = readTimeFromRTC();
    tv.tv_usec = 0;
    if(tv.tv_sec > 0) settimeofday(&tv, NULL);
}

void RTC::writeReg(uint8_t regAddr, uint8_t value){
    takeSemaphore();
    Wire.beginTransmission(m_address);
    Wire.write(regAddr);
    Wire.write(value);
    Wire.endTransmission();
    giveSemaphore();
}

void RTC::writeRegBurst(uint8_t startRegAddr, const uint8_t *buf, uint8_t size){
    takeSemaphore();
    Wire.beginTransmission(m_address);
    Wire.write(startRegAddr);
    for (size_t i = 0; i < size; i++)
    {
        Wire.write(buf[i]);
    }
    Wire.endTransmission();
    giveSemaphore();
}

uint8_t RTC::readReg(uint8_t regAddr){
    takeSemaphore();
    Wire.beginTransmission(m_address);
    Wire.write(regAddr);
    Wire.endTransmission();

    Wire.requestFrom(m_address, 1);
    while(Wire.available() == 0);
    auto reg = Wire.read();
    giveSemaphore();
    return reg;
}

void RTC::readRegBurst(uint8_t startRegAddr, uint8_t *buf, uint8_t size){
    takeSemaphore();
    Wire.beginTransmission(m_address);
    Wire.write(startRegAddr);
    Wire.endTransmission();

    Wire.requestFrom(m_address, size);
    for (size_t i = 0; i < size; i++)
    {
        while(!Wire.available());
        buf[i] = Wire.read();
    }
    giveSemaphore();
}

void RTC::takeSemaphore(){
    if(m_sem != NULL){
        while(xSemaphoreTake(m_sem, portMAX_DELAY) != pdTRUE);
    }
}

void RTC::giveSemaphore(){
    if(m_sem != NULL){
        xSemaphoreGive(m_sem);
    }
}