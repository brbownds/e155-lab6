// DS1722.c
// Broderick Bownds (rewritten using Quinn Miyamoto's tactics)
// brbownds@hmc.edu
// 10/22/2025
// Source code for DS1722 temperature sensor via SPI1


#include "DS1722.h"
#include <stdint.h>
#include <stdio.h>
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_TIM.h"

int precisiondefault = 12;

// Command constants (per DS1722 datasheet)
#define DS1722_WRITE_CONFIG   0x80
#define DS1722_READ_TEMP_LSB  0x01
#define DS1722_READ_TEMP_MSB  0x02


// Function: setprecision
// Sets the DS1722 temperature resolution (8–12 bits)
// Writes configuration byte over SPI


void setprecision(int precision)
{
   char config; // default continuous conversion mode
  
  precisiondefault = precision;

   if (precision == 8)   config = 0xE0;
   else if (precision == 9)  config = 0xE2;
   else if (precision == 10) config = 0xE4;
   else if (precision == 11) config = 0xE6;
   else if (precision == 12) config = 0xEE; // full 12-bit continuous conversion
  

   // Begin SPI write
   digitalWrite(SPI_CE, 1);                 // Enable CE
   spiSendReceive(DS1722_WRITE_CONFIG);  // Command: write configuration register
   spiSendReceive(config);               // Write configuration byte
   digitalWrite(SPI_CE, 0);                 // Disable CE


   // Wait for configuration to apply and conversions to stabilize
   delay_millis(TIM15, 1000);
}


///////////////////////////////////////////////////////////////////////////////
// Function: Temp
// Reads the current temperature from the DS1722 and converts to degrees
///////////////////////////////////////////////////////////////////////////////
float readTemp(void)
{
    uint8_t msb = 0;
    uint8_t lsb = 0;

    // Read MSB
    digitalWrite(SPI_CE, 1);
    spiSendReceive(0x02);
    msb = spiSendReceive(0x00);
    digitalWrite(SPI_CE, 0);
    
    // Read LSB
    digitalWrite(SPI_CE, 1);
    spiSendReceive(0x01);
    lsb = spiSendReceive(0x00);
    digitalWrite(SPI_CE, 0);

    uint16_t raw = ((uint16_t)msb << 8) | lsb;

    int16_t signraw = (int16_t)raw;

    // Convert using two’s complement and scaling
    float temp;
        temp = (double)signraw/256.0;  // negative temps

    printf("MSB= 0x%02X LSB=0x%02X temp = %.4f C" "(Resolution =%d-bit)\n", msb, lsb, temp, precisiondefault);
    return temp;
}

