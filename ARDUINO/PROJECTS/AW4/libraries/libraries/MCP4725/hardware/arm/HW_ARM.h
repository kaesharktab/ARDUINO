void MCP4725::begin(uint8_t channel)
{
	_use_hw = false;
	if ((_sda_pin == SDA) and (_scl_pin == SCL))
	{
		_use_hw = true;
		twi = TWI1;
		pmc_enable_periph_clk(WIRE_INTERFACE_ID);
		PIO_Configure(g_APinDescription[PIN_WIRE_SDA].pPort, g_APinDescription[PIN_WIRE_SDA].ulPinType, g_APinDescription[PIN_WIRE_SDA].ulPin, g_APinDescription[PIN_WIRE_SDA].ulPinConfiguration);
		PIO_Configure(g_APinDescription[PIN_WIRE_SCL].pPort, g_APinDescription[PIN_WIRE_SCL].ulPinType, g_APinDescription[PIN_WIRE_SCL].ulPin, g_APinDescription[PIN_WIRE_SCL].ulPinConfiguration);
		NVIC_DisableIRQ(TWI1_IRQn);
		NVIC_ClearPendingIRQ(TWI1_IRQn);
		NVIC_SetPriority(TWI1_IRQn, 0);
		NVIC_EnableIRQ(TWI1_IRQn);

	}
	else if ((_sda_pin == SDA1) and (_scl_pin == SCL1))
	{
		_use_hw = true;
		twi = TWI0;
		pmc_enable_periph_clk(WIRE1_INTERFACE_ID);
		PIO_Configure(g_APinDescription[PIN_WIRE1_SDA].pPort, g_APinDescription[PIN_WIRE1_SDA].ulPinType, g_APinDescription[PIN_WIRE1_SDA].ulPin, g_APinDescription[PIN_WIRE1_SDA].ulPinConfiguration);
		PIO_Configure(g_APinDescription[PIN_WIRE1_SCL].pPort, g_APinDescription[PIN_WIRE1_SCL].ulPinType, g_APinDescription[PIN_WIRE1_SCL].ulPin, g_APinDescription[PIN_WIRE1_SCL].ulPinConfiguration);
		NVIC_DisableIRQ(TWI0_IRQn);
		NVIC_ClearPendingIRQ(TWI0_IRQn);
		NVIC_SetPriority(TWI0_IRQn, 0);
		NVIC_EnableIRQ(TWI0_IRQn);
	}

	if (_use_hw)
	{
		// activate internal pullups for twi.
		digitalWrite(SDA, 1);
		digitalWrite(SCL, 1);

		// Reset the TWI
		twi->TWI_CR = TWI_CR_SWRST;
		// TWI Slave Mode Disabled, TWI Master Mode Disabled.
		twi->TWI_CR = TWI_CR_SVDIS;
		twi->TWI_CR = TWI_CR_MSDIS;
		// Set TWI Speed
		twi->TWI_CWGR = (TWI_DIV << 16) | (TWI_SPEED << 8) | TWI_SPEED;
		// Set master mode
		twi->TWI_CR = TWI_CR_MSEN;
	}
	else
	{
		pinMode(_scl_pin, OUTPUT);
	}
	_device = _device + channel;
}

void MCP4725::_readStatus()
{
	if (_use_hw)
	{
		// Store current TWI speed
		uint32_t cwgrback = twi->TWI_CWGR;
		// Set our TWI speed
		twi->TWI_CWGR = (TWI_DIV << 16) | (TWI_SPEED << 8) | TWI_SPEED;

		// Set slave address and number of internal address bytes.
		twi->TWI_MMR = TWI_MMR_MREAD | (_device << 16);
		// Send START condition
		twi->TWI_CR = TWI_CR_START;

		for (int i=0; i<4; i++)
		{
			while ((twi->TWI_SR & TWI_SR_RXRDY) != TWI_SR_RXRDY) {};
			_burstArray[i] = twi->TWI_RHR;
		}

		twi->TWI_CR = TWI_CR_STOP;
		while ((twi->TWI_SR & TWI_SR_RXRDY) != TWI_SR_RXRDY) {};
		_burstArray[5] = twi->TWI_RHR;
		while ((twi->TWI_SR & TWI_SR_TXCOMP) != TWI_SR_TXCOMP) {};
		
		// Restore current TWI speed
		twi->TWI_CWGR = cwgrback;
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
		uint32_t cwgrback = twi->TWI_CWGR;
		// Set our TWI speed
		twi->TWI_CWGR = (TWI_DIV << 16) | (TWI_SPEED << 8) | TWI_SPEED;
		// Set slave address and number of internal address bytes.
		twi->TWI_MMR = (_device << 16);
		// Send first byte to start transfer
		if (updateEEPROM)
			twi->TWI_THR = MCP4725_CMD_STORE | (pd<<1);
		else
			twi->TWI_THR = MCP4725_CMD_WRITE | (pd<<1);
		while ((twi->TWI_SR & TWI_SR_TXRDY) != TWI_SR_TXRDY) {};
		// Send second byte
		twi->TWI_THR = value>>4;
		while ((twi->TWI_SR & TWI_SR_TXRDY) != TWI_SR_TXRDY) {};
		// Send third byte
		twi->TWI_THR = (value & 0x0f)<<4;
		while ((twi->TWI_SR & TWI_SR_TXRDY) != TWI_SR_TXRDY) {};
		// Send STOP condition
		twi->TWI_CR = TWI_CR_STOP;
		while ((twi->TWI_SR & TWI_SR_TXCOMP) != TWI_SR_TXCOMP) {};
		// Restore current TWI speed
		twi->TWI_CWGR = cwgrback;
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
		uint32_t cwgrback = twi->TWI_CWGR;
		// Set our TWI speed
		twi->TWI_CWGR = (TWI_DIV << 16) | (TWI_SPEED << 8) | TWI_SPEED;
		// Set slave address and number of internal address bytes.
		twi->TWI_MMR = (_device << 16);
		// Send first byte to start transfer
		twi->TWI_THR = value>>8;
		while ((twi->TWI_SR & TWI_SR_TXRDY) != TWI_SR_TXRDY) {};
		// Send second byte
		twi->TWI_THR = value & 0xff;
		while ((twi->TWI_SR & TWI_SR_TXRDY) != TWI_SR_TXRDY) {};
		// Send STOP condition
		twi->TWI_CR = TWI_CR_STOP;
		while ((twi->TWI_SR & TWI_SR_TXCOMP) != TWI_SR_TXCOMP) {};
		// Restore current TWI speed
		twi->TWI_CWGR = cwgrback;
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

