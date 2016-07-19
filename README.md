# W25Q16
Arduino library for the Winbond W25Q16 Serial Flash.

## Connections for W25Q16 to Arduno Uno or Atmega328

pin 1 to Arduino pin 10 or Atmega328 pin 16 (Chip Select)

pin 2 to Arduino pin 12 or Atmega328 pin 18 (Master In Slave Out)

pin 3 to 3.3V (Vdd)

pin 4 to GND (Vss)

pin 5 to Arduino pin 11 or Atmega328 pin 17 (Slave in Master Out)

pin 6 to Arduino pin 13 or Atmega328 pin 19 (Clock)

pin 7 to 3.3V (Vdd)

pin 8 to 3.3V (Vdd)

NOTE: The Winbond W25Q16 requires 2.7V to 3.6V for operation.  The Arduino Uno operates at 5V so I had converted an Arduino Uno from 5V to 3.3V to utilize with the W25Q16.  I used the technique described at https://learn.adafruit.com/arduino-tips-tricks-and-techniques/3-3v-conversion to convert an Arduino Uno to 3.3V.
