void MCP4725::begin(uint8_t channel)
{
	if ((_sda_pin == SDA) and (_scl_pin == SCL))
	{
		_use_hw = true;
		// activate internal pullups for twi.
		digitalWrite(SDA, HIGH);
		digitalWrite(SCL, HIGH);
		//delay(1);  // Workaround for a linker bug

		// initialize twi prescaler and bit rate
		cbi(TWSR, TWPS0);
		cbi(TWSR, TWPS1);

		// enable twi module
		TWCR = _BV(TWEN) | _BV(TWIE)/* | _BV(TWEA)*/;
	}
	else
	{
		_use_hw = false;
		pinMode(_scl_pin, OUTPUT);
	}
	_device = _device + channel;
}

void MCP4725::_readStatus()
{
	if (_use_hw)
	{
		// Store current TWI speed
		uint8_t twbrback = TWBR;
		// Set our TWI speed
		TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
		// Send start address
		TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);						// Send START
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = (_device<<1) | 1;
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		// Read data starting from start address
		for (int i=0; i<5; i++)
		{
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);								// Send ACK and clear TWINT to proceed
			while ((TWCR & _BV(TWINT)) == 0) {};									// Wait for TWI to be ready
			_burstArray[i] = TWDR;
		}

		TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);									// Send STOP
		// Restore current TWI speed
		TWBR = twbrback;
	}
	else
	{
		_sendStart((_device<<1) | 1);
		_waitForAck();

		for (int i=0; i<5; i++)
		{
			_burstArray[i] = _readByte();
			_sendAck();
		}
		_sendStop();
	}
}

void MCP4725::_writeValue(uint16_t value, uint8_t pd, boolean updateEEPROM)
{
	if (_use_hw)
	{
		// Store current TWI speed
		uint8_t twbrback = TWBR;
		// Set our TWI speed
		TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
		// Send start address
		TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);						// Send START
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = (_device<<1);
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		if (updateEEPROM)
			TWDR = MCP4725_CMD_STORE | (pd<<1);
		else
			TWDR = MCP4725_CMD_WRITE | (pd<<1);
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = (value>>4);
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = ((value & 0x0f)<<4);
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready

		TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);									// Send STOP
		// Restore current TWI speed
		TWBR = twbrback;
	}
	else
	{
		_sendStart((_device<<1));
		_waitForAck();
		if (updateEEPROM)
			_writeByte(MCP4725_CMD_STORE | (pd<<1));
		else
			_writeByte(MCP4725_CMD_WRITE | (pd<<1));
		_waitForAck();
		_writeByte(value>>4);
		_waitForAck();
		_writeByte((value & 0x0f)<<4);
		_waitForAck();
		_sendStop();
	}
}

void MCP4725::_fastWriteValue(uint16_t value)
{
	if (_use_hw)
	{
		// Store current TWI speed
		uint8_t twbrback = TWBR;
		// Set our TWI speed
		TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
		// Send start address
		TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);						// Send START
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = (_device<<1);
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = (value>>8);
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = (value & 0xff);
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready

		TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);									// Send STOP
		// Restore current TWI speed
		TWBR = twbrback;
	}
	else
	{
		_sendStart((_device<<1));
		_waitForAck();
		_writeByte(value>>8);
		_waitForAck();
		_writeByte(value & 0xff);
		_waitForAck();
		_sendStop();
	}
}
