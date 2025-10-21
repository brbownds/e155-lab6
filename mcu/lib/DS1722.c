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


float Temp(void)
{
    uint8_t lsb = 0, msb = 0;

    // Read LSB 
    digitalWrite(SPI_CE, 1);
    spiSendReceive(DS1722_READ_TEMP_LSB);
    lsb = spiSendReceive(0x00);  // dummy byte to clock data out
    digitalWrite(SPI_CE, 0);

    // Read MSB 
    digitalWrite(SPI_CE, 1);
    spiSendReceive(DS1722_READ_TEMP_MSB);
    msb = spiSendReceive(0x00);
    digitalWrite(SPI_CE, 0);

    // Combine bytes into signed 12-bit temperature (datasheet format)
    float temp = (float)((int8_t)msb) + ((float)lsb / 256.0f);
    return temp;
}

// Sets DS1722 resolution (8–12 bits) by writing configuration register.
// precision: valid options are 8, 9, 10, 11, or 12.
void setprecision(int precision)
{
    uint8_t byte;

    switch (precision) {
        case 8:  byte = 0xF0; break;
        case 9:  byte = 0xF2; break;
        case 10: byte = 0xF4; break;
        case 11: byte = 0xF5; break;
        case 12: byte = 0xF8; break;
        default: byte = 0xF8; break;  // default to 12-bit
    }

    digitalWrite(SPI_CE, 1);
    spiSendReceive(DS1722_WRITE_CONFIG);
    spiSendReceive(byte);
    digitalWrite(SPI_CE, 0);

    // Small delay for configuration to latch (10–20 ms is plenty)
    delay_millis(TIM15, 20);
}
