// DS1722.c
// Broderick Bownds
// brbownds@hmc.edu
// 10/21/2025
// Source code for DS1722 temperature sensor via SPI1

#include "DS1722.h"
#include <stdint.h>
#include <stdio.h>
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_TIM.h"

#define DS1722_READ_TEMP_LSB   0x01
#define DS1722_READ_TEMP_MSB   0x02
#define DS1722_WRITE_CONFIG    0x80


float readTemp(void) {
   uint8_t LSB = 0;    // Least significant bit
   uint8_t MSB = 0;    // Most significant bit


   // --- Read MSB (Address 0x02) ---
   digitalWrite(SPI_CE, PIO_HIGH);    // Start SPI transaction
   spiSendReceive(0x02);               // Send read command for MSB register
   MSB = spiSendReceive(0x00);         // Read the MSB data (send dummy byte 0x00)
   digitalWrite(SPI_CE, PIO_LOW);     // Pulse CE to end MSB read


   // --- Read LSB (Address 0x01) ---
   digitalWrite(SPI_CE, PIO_HIGH);    // Start new SPI transaction
   spiSendReceive(0x01);               // Send read command for LSB register
   LSB = spiSendReceive(0x00);         // Read the LSB data (send dummy byte 0x00)
   digitalWrite(SPI_CE, PIO_LOW);     // End SPI transaction


   //  Concatenate bytes and calculate temperature
  
   // Combine the MSB and LSB into a single 16-bit word.
   uint16_t rawTemp = ((uint16_t) MSB << 8) | LSB;

   int16_t signedValue = (int16_t)rawTemp;
    printf("MSB: %d, LSB: %d\n", MSB, LSB);
   // The resolution is 1/256 ºC per LSB.
   float temp = (double)signedValue / 256.0;


   return temp;
}


void setprecision(int precision)
{
    char byte;

    if (precision == 8) {
        byte = 0xE0;  // 8-bit resolution, continuous conversion
    } else if (precision == 9) {
        byte = 0xE2;  // 9-bit resolution
    } else if (precision == 10) {
        byte = 0xE4;  // 10-bit resolution
    } else if (precision == 11) {
        byte = 0xE5;  // 11-bit resolution
    } else {
        byte = 0xE8;  // Default to 12-bit resolution
    }

    // Write config byte to DS1722
    digitalWrite(SPI_CE, PIO_HIGH);
    spiSendReceive(DS1722_WRITE_CONFIG);  // 0x80
    spiSendReceive(byte);
    digitalWrite(SPI_CE, PIO_LOW);

    // Allow time for conversion to begin
    delay_millis(TIM15, 1000);
}





