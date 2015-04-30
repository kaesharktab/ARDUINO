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
#include "MCP4725.h"

// Include hardware-specific functions for the correct MCU
#if defined(__AVR__)
	#include "hardware/avr/HW_AVR.h"
#elif defined(__PIC32MX__)
  #include "hardware/pic32/HW_PIC32.h"
#elif defined(__arm__)
	#include "hardware/arm/HW_ARM.h"
#endif

/* Public */

MCP4725::MCP4725(uint8_t data_pin, uint8_t sclk_pin)
{
	_sda_pin = data_pin;
	_scl_pin = sclk_pin;
	_device  = MCP4725A0;
}

void MCP4725::setDevice(uint8_t dev)
{
	_device = dev;
}

MCP4725_Status MCP4725::getStatus()
{
	MCP4725_Status	_s;

	_readStatus();

	_s.currentValue = (_burstArray[1]<<4) | (_burstArray[2]>>4);
	_s.currentVoltage = _s.currentValue * LSb_STEP;
	_s.currentPowerState = (_burstArray[0] & 0x06)>>1;
	_s.startupValue = ((_burstArray[3] & 0x0f)<<8) | (_burstArray[4]);
	_s.startupPowerState = (_burstArray[3] & 0x60)>>5;

	return _s;
}

void MCP4725::setValue(uint16_t value)
{
	_fastWriteValue(value & 0x0fff);
}

void MCP4725::setVoltage(float value)
{
	uint16_t out_value;

	if (value > MAX_VOLTAGE)
		value = MAX_VOLTAGE;

	out_value = uint16_t(float(value) / LSb_STEP);
	if (out_value > 4095)
		out_value = 4095;
	setValue(out_value);
}

void MCP4725::setPowerDown(uint8_t value)
{
	_fastWriteValue(uint16_t(value & 0x03)<<12);
}

void MCP4725::storeValue(uint16_t value)
{
	_writeValue(value & 0x0fff, 0, true);
	delay(50);							// Max EEPROM Twrite according to the MCP4725 datasheet
}

void MCP4725::storeVoltage(float value)
{
	uint16_t out_value;

	if (value > MAX_VOLTAGE)
		value = MAX_VOLTAGE;

	out_value = uint16_t(float(value) / LSb_STEP);
	if (out_value > 4095)
		out_value = 4095;
	storeValue(out_value);
}

void MCP4725::storePowerDown(uint8_t value)
{
	_writeValue(0, (value & 0x03), true);
	delay(50);							// Max EEPROM Twrite according to the MCP4725 datasheet
}

/* Private */

void	MCP4725::_sendStart(uint8_t addr)
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, LOW);
	shiftOut(_sda_pin, _scl_pin, MSBFIRST, addr);
}

void	MCP4725::_sendStop()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, HIGH);
	pinMode(_sda_pin, INPUT);
}

void	MCP4725::_sendNack()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void	MCP4725::_sendAck()
{
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void	MCP4725::_waitForAck()
{
	pinMode(_sda_pin, INPUT);
	digitalWrite(_scl_pin, HIGH);
	while (digitalRead(_sda_pin)==HIGH) {}
	digitalWrite(_scl_pin, LOW);
}

uint8_t MCP4725::_readByte()
{
	pinMode(_sda_pin, INPUT);

	uint8_t value = 0;
	uint8_t currentBit = 0;

	for (int i = 0; i < 8; ++i)
	{
		digitalWrite(_scl_pin, HIGH);
		currentBit = digitalRead(_sda_pin);
		value |= (currentBit << 7-i);
		delayMicroseconds(1);
		digitalWrite(_scl_pin, LOW);
	}
	return value;
}

void MCP4725::_writeByte(uint8_t value)
{
	pinMode(_sda_pin, OUTPUT);
	shiftOut(_sda_pin, _scl_pin, MSBFIRST, value);
}

