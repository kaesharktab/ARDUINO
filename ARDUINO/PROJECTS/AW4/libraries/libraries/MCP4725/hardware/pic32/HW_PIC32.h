inline void _waitForIdleBus() { while (I2C1CON & 0x1f) {} }

void MCP4725::begin(uint8_t channel)
{
	if ((_sda_pin == SDA) and (_scl_pin == SCL))
	{
		uint32_t	tpgd;

		_use_hw = true;
		pinMode(SDA, OUTPUT);
		digitalWrite(SDA, HIGH);
		IFS0CLR = 0xE0000000;									// Clear Interrupt Flag
		IEC0CLR = 0xE0000000;									// Disable Interrupt
		I2C1CONCLR = (1 << _I2CCON_ON);							// Disable I2C interface
		tpgd = ((F_CPU / 8) * 104) / 125000000;
		I2C1BRG = (F_CPU / (2 * TWI_FREQ) - tpgd) - 2;			// Set I2C Speed
		I2C1ADD = _device + channel;							// Set I2C device address
		I2C1CONSET = (1 << _I2CCON_ON) | (1 << _I2CCON_STREN);	// Enable I2C Interface
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
		_waitForIdleBus();									// Wait for I2C bus to be Idle before starting
		I2C1CONSET = (1 << _I2CCON_SEN);					// Send start condition
		if (I2C1STAT & (1 << _I2CSTAT_BCL)) { return; }		// Check if there is a bus collision
		while (I2C1CON & (1 << _I2CCON_SEN)) {}				// Wait for start condition to finish
		I2C1TRN = (_device<<1) | 1;							// Send device Read address
		while (I2C1STAT & (1 << _I2CSTAT_IWCOL))			// Check if there is a Write collision
		{
			I2C1STATCLR = (1 << _I2CSTAT_IWCOL);			// Clear Write collision flag
			I2C1TRN = (_device<<1) | 1;						// Retry send device Read address
		}
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		byte dummy = I2C1RCV;								// Clear _I2CSTAT_RBF (Receive Buffer Full)
		for (int i=0; i<5; i++)
		{
			_waitForIdleBus();								// Wait for I2C bus to be Idle before continuing
			I2C1CONSET = (1 << _I2CCON_RCEN);				// Set RCEN to start receive
			while (I2C1CON & (1 << _I2CCON_RCEN)) {}		// Wait for Receive operation to finish
			while (!(I2C1STAT & (1 << _I2CSTAT_RBF))) {}	// Wait for Receive Buffer Full
			_burstArray[i] = I2C1RCV;						// Read data
			if (i == 4)
				I2C1CONSET = (1 << _I2CCON_ACKDT);			// Prepare to send NACK
			else
				I2C1CONCLR = (1 << _I2CCON_ACKDT);			// Prepare to send ACK
			I2C1CONSET = (1 << _I2CCON_ACKEN);				// Send ACK/NACK
			while (I2C1CON & (1 << _I2CCON_ACKEN)) {}		// Wait for ACK/NACK send to finish
		}
		I2C1CONSET = (1 << _I2CCON_PEN);					// Send stop condition
		while (I2C1CON & (1 << _I2CCON_PEN)) {}				// Wait for stop condition to finish
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
		_waitForIdleBus();									// Wait for I2C bus to be Idle before starting
		I2C1CONSET = (1 << _I2CCON_SEN);					// Send start condition
		if (I2C1STAT & (1 << _I2CSTAT_BCL)) { return; }		// Check if there is a bus collision
		while (I2C1CON & (1 << _I2CCON_SEN)) {}				// Wait for start condition to finish
		I2C1TRN = (_device<<1);								// Send device Write address
		while (I2C1STAT & (1 << _I2CSTAT_IWCOL))			// Check if there is a Write collision
		{
			I2C1STATCLR = (1 << _I2CSTAT_IWCOL);			// Clear Write collision flag
			I2C1TRN = (_device<<1);							// Retry send device Write address
		}
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		if (updateEEPROM)
			I2C1TRN = (MCP4725_CMD_STORE | (pd<<1));		// Send command CMD_STORE if updateEEPROM is true
		else
			I2C1TRN = (MCP4725_CMD_WRITE | (pd<<1));		// Send command CMD_WRITE if updateEEPROM is false
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1TRN = (value>>4);								// Send the 1st data byte
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1TRN = ((value & 0x0f)<<4);						// Send the 2nd data byte
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1CONSET = (1 << _I2CCON_PEN);					// Send stop condition
		while (I2C1CON & (1 << _I2CCON_PEN)) {}				// Wait for stop condition to finish
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
		_waitForIdleBus();									// Wait for I2C bus to be Idle before starting
		I2C1CONSET = (1 << _I2CCON_SEN);					// Send start condition
		if (I2C1STAT & (1 << _I2CSTAT_BCL)) { return; }		// Check if there is a bus collision
		while (I2C1CON & (1 << _I2CCON_SEN)) {}				// Wait for start condition to finish
		I2C1TRN = (_device<<1);								// Send device Write address
		while (I2C1STAT & (1 << _I2CSTAT_IWCOL))			// Check if there is a Write collision
		{
			I2C1STATCLR = (1 << _I2CSTAT_IWCOL);			// Clear Write collision flag
			I2C1TRN = (_device<<1);							// Retry send device Write address
		}
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1TRN = (value>>8);								// Send the 1st data byte
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1TRN = (value & 0xff);							// Send the 2nd data byte
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1CONSET = (1 << _I2CCON_PEN);					// Send stop condition
		while (I2C1CON & (1 << _I2CCON_PEN)) {}				// Wait for stop condition to finish
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

