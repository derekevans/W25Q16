/*This sketch shows the basics of using the W25Q16 library.*/

/*W25Q16 Connections:
 pin 1 to Arduino pin 10 or Atmega328 pin 15 (Chip Select)
 pin 2 to Arduino pin 12 or Atmega328 pin 18 (Master In Slave Out)
 pin 3 to 3.3V (Vdd)
 pin 4 to GND (Vss)
 pin 5 to Arduino pin 11 or Atmega328 pin 17 (Slave in Master Out)
 pin 6 to Arduino pin 13 or Atmega328 pin 19 (Clock)
 pin 7 to 3.3V (Vdd)
 pin 8 to 3.3V (Vdd)
 */
 
#include <SPI.h> //You must include the SPI library along with the W25Q16 library!
#include <W25Q16.h>

W25Q16 flash;
unsigned int startPage = 0;
unsigned int startPageAddress = 0;
unsigned int endPage = 0;
unsigned int endPageAddress = 0;

void setup() {
  Serial.begin(9600);
  
  //initialize the pcf2127
  flash.init(10); 
  
  //erase the entire chips contents
  flash.chipErase();
   
  //read the manufacturer ID to make sure communications are OK
  //should output 0xEF
  byte manID = flash.manufacturerID(); 
  Serial.print("Manufacturer ID: ");
  Serial.println(manID,HEX);
  //put the flash in lowest power state
  flash.powerDown(); 
}

void loop() {
  //must take the flash out of low power for operation
  flash.releasePowerDown(); 
  //Write three random bytes to the flash and increment the page/address
  int i = 0;
  while(i<3) {
    flash.write(endPage,(byte)endPageAddress,(byte)random(0,255));
    
    //increment the page address
    endPageAddress++; 
    
    //check to see if end of page is reached.  If so, address = 0 and increment page
    endOfPage(); 
    i++;
  }

  //reset the end page and byte variables to where the started for read
  endPageAddress = startPageAddress; 
  endPage = startPage;
  
  //Read those three bytes back
  i = 0;
  while(i<3) {
    consoleOutput(endPage,endPageAddress,flash.read(endPage,(byte)endPageAddress));
    endPageAddress++;
    endOfPage();
    i++;
  }
  
  //Put the flash in its lowest power state
  flash.powerDown();
  
  //set the start page to the end page to utilize other address memory
  startPage = endPage;
  startPageAddress = endPageAddress;
  delay(500);
}

//Checks if the flash page address is full
void endOfPage() {
  if (endPageAddress>255) {
    endPage++;
    if (endPage>8191) {
      endPage = 0;
    } 
    endPageAddress = 0;
  }
}

//formats the output for the console
void consoleOutput(unsigned int page, unsigned int address, byte val) {
   Serial.print("Page ");
   Serial.print(page);
   Serial.print(" Address ");
   Serial.print(address);
   Serial.print(": ");
   Serial.println(val);
} 