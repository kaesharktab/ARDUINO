/*
  MCP4725.cpp - Arduino/chipKit library support for the MCP4725 I2C DAC
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library has been made to easily interface and use the MCP4725 DAC with
  an Arduino or chipKit.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#ifndef MCP4725_h
#define MCP4725_h

#if defined(__AVR__)
	#include "Arduino.h"
	#include "hardware/avr/HW_AVR_defines.h"
#elif defined(__PIC32MX__)
	#include "WProgram.h"
	#include "hardware/pic32/HW_PIC32_defines.h"
#elif defined(__arm__)
	#include "Arduino.h"
	#include "hardware/arm/HW_ARM_defines.h"
#endif

#define MCP4725A0	0x60
#define MCP4725A1	0x62
#define MCP4725A2	0x64
#define MCP4725A3	0x66

#define MCP4725_CMD_WRITE	(0x40)  // Writes data to the DAC only
#define MCP4725_CMD_STORE	(0x60)  // Writes data to the DAC and the EEPROM

#define	MCP4725_POWERDOWN_OFF	0x00
#define	MCP4725_POWERDOWN_1K	0x01
#define	MCP4725_POWERDOWN_100K	0x02
#define	MCP4725_POWERDOWN_500K	0x03
#define	MCP4725_POWERDOWN_UNK	0xFF

class MCP4725_Status
{
public:
	uint16_t	currentValue;
	float		currentVoltage;
	uint8_t		currentPowerState;
	uint16_t	startupValue;
	uint8_t		startupPowerState;

	MCP4725_Status()
	{
		currentValue = 0xffff;
		currentVoltage = 100;
		currentPowerState = MCP4725_POWERDOWN_UNK;
		startupValue = 0xffff;
		startupPowerState = MCP4725_POWERDOWN_UNK;
	}
};


class MCP4725
{
	public:
		MCP4725(uint8_t data_pin, uint8_t sclk_pin);

		void			setDevice(uint8_t dev = MCP4725A0);
		void			begin(uint8_t channel = 0);
		MCP4725_Status	getStatus();
		void			setValue(uint16_t value);
		void			setVoltage(float value);
		void			setPowerDown(uint8_t value);
		void			storeValue(uint16_t value);
		void			storeVoltage(float value);
		void			storePowerDown(uint8_t value);

	private:
		uint8_t			_scl_pin;
		uint8_t			_sda_pin;
		boolean			_use_hw;
		uint8_t			_device;
		uint8_t			_burstArray[6];

		void			_sendStart(uint8_t addr);
		void			_sendStop();
		void			_sendAck();
		void			_sendNack();
		void			_waitForAck();
		uint8_t			_readByte();
		void			_writeByte(uint8_t value);
		void			_readStatus();
		void			_writeValue(uint16_t value, uint8_t pd, boolean updateEEPROM = false);
		void			_fastWriteValue(uint16_t value);
#if defined(__arm__)
		Twi				*twi;
#endif
};
#endif
