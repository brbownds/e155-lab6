// DS1722.h
// Broderick Bownds
// brbownds@hmc.edu
// 10/20/2025

// This is the header file for DS1722 temperature sensor

#ifndef DS1722_H
#define DS1722_H

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////


// This gets and returns the current temperature
float readTemp();

// Set the resolution of the temperature sensor (8-12 bits)
void setprecision(int precision);

#endif