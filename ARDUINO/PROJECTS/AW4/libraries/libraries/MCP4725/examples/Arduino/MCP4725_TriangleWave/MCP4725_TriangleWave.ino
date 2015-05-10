// MCP4725_TriangleWave 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my MCP4725-library to 
// make the MCP4725 DAC chip output a triangle wave
//
// To use the hardware I2C (TWI) interface of the Arduino you must connect
// the pins as follows:
//
// Arduino Uno/2009:
// ----------------------
// MCP4725:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//           SCL pin   -> Arduino Analog 5 or the dedicated SCL pin
//
// Arduino Leonardo:
// ----------------------
// MCP4725:  SDA pin   -> Arduino Digital 2 or the dedicated SDA pin
//           SCL pin   -> Arduino Digital 3 or the dedicated SCL pin
//
// Arduino Mega:
// ----------------------
// MCP4725:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA pin
//           SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL pin
//
// Arduino Due:
// ----------------------
// MCP4725:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA1 (Digital 70) pin
//           SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL1 (Digital 71) pin
//
// The internal pull-up resistors will be activated when using the 
// hardware I2C interfaces.
//
// You can connect the MCP4725 to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access 
// to the pins used, and you will also have to use appropriate, external
// pull-up resistors on the data and clock signals.
//
#include <MCP4725.h>

MCP4725 dac(SDA, SCL);

// Change the number below to set the desired resolution
//
// Resolution | Value
// -----------+------
// 12 bit     |   1
// 11 bit     |   2
// 10 bit     |   4
//  9 bit     |   8
//  8 bit     |  16
//  7 bit     |  32
//  6 bit     |  64
//
#define RESOLUTION  (1)

uint16_t counter;

void setup(void) 
{
  dac.begin();
}

void loop(void) 
{
  // Ramp UP
  for (counter = 0; counter < 4096; counter+=RESOLUTION)
    dac.setValue(counter);
  // Ramp DOWN
  for (counter = 4095; (counter >= 0) and (counter < 4096); counter-=RESOLUTION)
    dac.setValue(counter);
}
