// *** Hardwarespecific defines ***
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))

#ifndef TWI_FREQ
	#define TWI_FREQ 400000L
#endif

#define MAX_VOLTAGE	5.0f
#define LSb_STEP	0.001220703125f
