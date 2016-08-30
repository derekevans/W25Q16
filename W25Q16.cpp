/*
  W25Q16.cpp - Arduino library for communicating with the Winbond W25Q16 Serial Flash.
  Created by Derek Evans, July 17, 2016.
*/

#include "Arduino.h"
#include "W25Q16.h"

/********************************Public methods****************************/

/*****************************************************************************
 * 
 *	Function name : init
 *  
 *	Returns :		None
 *  
 *	Parameters :	int FLASS_SS	->	The Slave Select or Chip Select pin used by 
 *										Arduino to select W25Q16.
 * 
 * 
 *	Purpose :	Initializes the W25Q16 by setting the input slave select pin
 *				as OUTPUT and writing it HIGH.  Also initializes the SPI bus,
 *				sets the SPI bit order to MSBFIRST and the SPI data mode to 
 *				SPI_MODE3, ensures the flash is not in low power mode, and
 *				that flash write is disabled.
 * 					
 * 
 ******************************************************************************/
void W25Q16::init(int FLASH_SS)
{
  pinMode(FLASH_SS, OUTPUT);
  digitalWrite(FLASH_SS, HIGH);
  _FLASH_SS = FLASH_SS;
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  releasePowerDown();
  writeDisable();
}

/*****************************************************************************
 * 
 *	Function name : read
 *  
 *	Returns :		byte
 *  
 *	Parameters :	unsigned int page ->  The page to read from.
 *					byte pageAddress -> the page address from which a byte
 												will be read					
 * 
 * 
 *	Purpose :	Reads a byte from the flash page and page address.  The W25Q16 has
 *				8192 pages with 256 bytes in a page.  Both page and byte addresses
 *				start at 0. Page ends at address 8191 and page address ends at 255.
 * 					
 * 
 ******************************************************************************/
byte W25Q16::read(unsigned int page, byte pageAddress) {
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(READ_DATA);
  SPI.transfer((page >> 8) & 0xFF);
  SPI.transfer((page >> 0) & 0xFF);
  SPI.transfer(pageAddress);
  byte val = SPI.transfer(0);
  digitalWrite(_FLASH_SS, HIGH);
  notBusy();
  return val;
}

/*****************************************************************************
 * 
 *	Function name : write
 *  
 *	Returns :		None
 *  
 *	Parameters :	unsigned int page ->  The page to write to.
 *					byte pageAddress -> The page address to which a byte
 *												will be written.					
 * 					byte val -> the byte to write to the page and page address.
 * 
 *	Purpose :	Writes a byte to the flash page and page address.  The W25Q16 has
 *				8192 pages with 256 bytes in a page.  Both page and byte addresses
 *				start at 0. Page ends at address 8191 and page address ends at 255.
 * 					
 * 
 ******************************************************************************/
void W25Q16::write(unsigned int page, byte pageAddress, byte val) {
    writeEnable();
    digitalWrite(_FLASH_SS, LOW);
    SPI.transfer(PAGE_PROGRAM);
    SPI.transfer((page >> 8) & 0xFF);
    SPI.transfer((page >> 0) & 0xFF);
    SPI.transfer(pageAddress);
    SPI.transfer(val);
    digitalWrite(_FLASH_SS, HIGH);
    notBusy();
    writeDisable();
}

/*****************************************************************************
 * 
 *	Function name : initStreamWrite
 *  
 *	Returns :		None
 *  
 *	Parameters :	unsigned int page ->  The page to begin writing.
 *					byte pageAddress -> The page address to to begin
 *												writing.					
 * 
 *	Purpose :	Initializes flash for stream write, e.g. write more than one byte
 *				consecutively.  Both page and byte addresses start at 0. Page 
 *				ends at address 8191 and page address ends at 255.
 * 					
 ******************************************************************************/
void W25Q16::initStreamWrite(unsigned int page, byte pageAddress) {
	writeEnable();
    digitalWrite(_FLASH_SS, LOW);
    SPI.transfer(PAGE_PROGRAM);
    SPI.transfer((page >> 8) & 0xFF);
    SPI.transfer((page >> 0) & 0xFF);
    SPI.transfer(pageAddress);
}

/*****************************************************************************
 * 
 *	Function name : streamWrite
 *  
 *	Returns :		None
 *  
 *	Parameters :	byte val -> the byte to write to the flash.  					
 * 
 *	Purpose :	Writes a byte to the W25Q16.  Must be first called after 
 *				initStreamWrite and then consecutively to write multiple bytes.
 * 					
 ******************************************************************************/
void W25Q16::streamWrite(byte val) {
	SPI.transfer(val);
}

/*****************************************************************************
 * 
 *	Function name : closeStreamWrite
 *  
 *	Returns :		None
 *  
 *	Parameters :	None  					
 * 
 *	Purpose :	Close the stream write. Must be called after the last call to 
 *				streamWrite.
 * 					
 ******************************************************************************/
void W25Q16::closeStreamWrite() {
	digitalWrite(_FLASH_SS, HIGH);
    notBusy();
    writeDisable();
}

/*****************************************************************************
 * 
 *	Function name : initStreamRead
 *  
 *	Returns :		None
 *  
 *	Parameters :	unsigned int page ->  The page to begin reading.
 *					byte pageAddress -> The page address to to begin
 *										reading.					
 * 
 *	Purpose :	Initializes flash for stream read, e.g. read more than one byte
 *				consecutively.  Both page and byte addresses start at 0. Page 
 *				ends at address 8191 and page address ends at 255.
 * 					
 ******************************************************************************/
void W25Q16::initStreamRead(unsigned int page, byte pageAddress) {
	digitalWrite(_FLASH_SS, LOW);
  	SPI.transfer(READ_DATA);
  	SPI.transfer((page >> 8) & 0xFF);
  	SPI.transfer((page >> 0) & 0xFF);
  	SPI.transfer(pageAddress);
}

/*****************************************************************************
 * 
 *	Function name : streamRead
 *  
 *	Returns :		byte
 *  
 *	Parameters :	None					
 * 
 *	Purpose :	Reads a byte from the W25Q16.  Must be first called after 
 *				initStreamRead and then consecutively to write multiple bytes.
 * 					
 ******************************************************************************/
byte W25Q16::streamRead() {
	return SPI.transfer(0);
}

/*****************************************************************************
 * 
 *	Function name : closeStreamRead
 *  
 *	Returns :		None
 *  
 *	Parameters :	None  					
 * 
 *	Purpose :	Close the stream read. Must be called after the last call to 
 *				streamRead.
 * 					
 ******************************************************************************/
void W25Q16::closeStreamRead() {
	digitalWrite(_FLASH_SS, HIGH);
    notBusy();
}

/*****************************************************************************
 * 
 *	Function name : powerDown
 *  
 *	Returns :		None
 *  
 *	Parameters :	None
 * 
 *	Purpose :	Puts the flash in its low power mode.
 * 					
 * 
 ******************************************************************************/
void W25Q16::powerDown() {
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(POWER_DOWN);
  digitalWrite(_FLASH_SS, HIGH);
  notBusy();
}

/*****************************************************************************
 * 
 *	Function name : powerDown
 *  
 *	Returns :		None
 *  
 *	Parameters :	None
 * 
 *	Purpose :	Releases the flash from its low power mode.  Flash cannot be in 
 *				low power mode to perform read/write operations.
 * 					
 * 
 ******************************************************************************/
void W25Q16::releasePowerDown() {
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(RELEASE_POWER_DOWN);
  digitalWrite(_FLASH_SS, HIGH);
  notBusy();
}

/*****************************************************************************
 * 
 *	Function name : chipErase
 *  
 *	Returns :		None
 *  
 *	Parameters :	None
 * 
 *	Purpose :	Erases all data from the flash.
 * 					
 * 
 ******************************************************************************/
void W25Q16::chipErase() {
  writeEnable();
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(CHIP_ERASE);
  digitalWrite(_FLASH_SS, HIGH);
  notBusy();
  writeDisable();
}

/*****************************************************************************
 * 
 *	Function name : manufacturerID
 *  
 *	Returns :		byte
 *  
 *	Parameters :	None
 * 
 *	Purpose :	Reads the manufacturer ID from the W25Q16.  Should return 0xEF.
 * 					
 * 
 ******************************************************************************/
byte W25Q16::manufacturerID() {
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(MANUFACTURER_ID);
  SPI.transfer(0);
  SPI.transfer(0);
  SPI.transfer(0);
  byte val = SPI.transfer(0);
  digitalWrite(_FLASH_SS, HIGH);
  notBusy();
  return val;
}

/*******************************Private Methods******************************/

/*****************************************************************************
 * 
 *	Function name : notBusy
 *  
 *	Returns :		byte
 *  
 *	Parameters :	None
 * 
 *	Purpose :	Halts operation until the flash is finished with its 
 *				write/erase operation. Bit 0 of Status Register 1 of the 
 *				W25Q16 is 1 if the chip is busy with a write/erase operation. 
 * 					
 * 
 ******************************************************************************/
void W25Q16::notBusy() {
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(READ_STATUS_REGISTER_1);
  while (bitRead(SPI.transfer(0),0) & 1) {
  }
  digitalWrite(_FLASH_SS, HIGH);
}

/*****************************************************************************
 * 
 *	Function name : writeEnable
 *  
 *	Returns :		byte
 *  
 *	Parameters :	None
 * 
 *	Purpose :	Sets Bit 1 of Status Register 1.  Bit 1 is the write enable 
 *				latch bit of the status register. This bit must be set prior 
 *				to every write/erase operation.  
 * 					
 * 
 ******************************************************************************/
void W25Q16::writeEnable() {
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(WRITE_ENABLE);
  digitalWrite(_FLASH_SS, HIGH);
  notBusy();
}

/*****************************************************************************
 * 
 *	Function name : writeDisable
 *  
 *	Returns :		byte
 *  
 *	Parameters :	None
 * 
 *	Purpose :	Clears Bit 1 of Status Register 1.  Bit 1 is the write enable 
 *				latch bit of the status register.  Clearing this bit prevents 
 *				the flash from being written or erased.  
 *				
 * 					
 * 
 ******************************************************************************/
void W25Q16::writeDisable() {
  digitalWrite(_FLASH_SS, LOW);
  SPI.transfer(WRITE_DISABLE);
  digitalWrite(_FLASH_SS, HIGH);
  notBusy();
}
