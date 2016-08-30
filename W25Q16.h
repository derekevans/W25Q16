/*
  W25Q16.h - Arduino library for communicating with the Winbond W25Q16 Serial Flash.
  Created by Derek Evans, July 17, 2016.
*/

#ifndef W25Q16_h
#define W25Q16_h

#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define PAGE_PROGRAM 0x02
#define READ_STATUS_REGISTER_1 0x05
#define READ_DATA 0x03
#define CHIP_ERASE 0xC7
#define POWER_DOWN 0xB9
#define RELEASE_POWER_DOWN 0xAB
#define MANUFACTURER_ID 0x90

#include "Arduino.h"
#include "SPI.h"

class W25Q16
{
  public:
	void init(int FLASH_SS);
    void write(unsigned int page, byte pageAddress, byte val);
    byte read(unsigned int page, byte pageAddress);
    void initStreamWrite(unsigned int page, byte pageAddress);
    void streamWrite(byte val);
    void closeStreamWrite();
    void initStreamRead(unsigned int page, byte pageAddress);
    byte streamRead();
    void closeStreamRead();
	void powerDown();
	void releasePowerDown();
    void chipErase();
    byte manufacturerID();
  private:
  	void writeEnable();
  	void writeDisable();
  	void notBusy();
    int _FLASH_SS;
};

#endif
