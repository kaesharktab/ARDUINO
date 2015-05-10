// MCP4725_setVoltage 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my MCP4725-library to set the MCP4725 DAC
// chip output to a specific voltage.
//
// To use the hardware I2C (TWI) interface of the chipKit you must connect
// the pins as follows:
//
// Arduino Uno32/uC32:
// ----------------------
// MCP4725:  SDA pin   -> Arduino Analog 4 (Digital 18)
//           SCL pin   -> Arduino Analog 5 (Digital 19)
// *** Remember that you will have to set JP6 and JP8 in the correct position
//
// Arduino Max32:
// ----------------------
// MCP4725:  SDA pin   -> Arduino Digital 20 (SDA)
//           SCL pin   -> Arduino Digital 21 (SCL)
//
// The PIC32 microcontroller does not have internal pull-up resistors so you 
// must use appropriate, external pull-up resistors on the data and clock 
// signals.
//
// You can connect the MCP4725 to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access 
// to the pins used, and you will also have to use appropriate, external
// pull-up resistors on the data and clock signals.
//

#include <MCP4725.h>

MCP4725 dac(SDA, SCL);

void setup(void) 
{
  dac.begin();
}

void loop(void) 
{
    for (float v = 0; v <= MAX_VOLTAGE ; v+=(MAX_VOLTAGE / 10))
    {
      dac.setVoltage(v);
      delay(3000);
    }
}
