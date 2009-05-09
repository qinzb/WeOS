//#include <avr/io.h>
#include <leds.h>


void leds_init()
{
	MCU_MAKE_LED1_OUTPUT();
	MCU_MAKE_LED2_OUTPUT();
	MCU_MAKE_LED3_OUTPUT();
	MCU_SET_LED1_PIN();
	MCU_SET_LED2_PIN();
	MCU_SET_LED3_PIN();
}

void leds(unsigned char led, unsigned char state)
{
	if(led >= RED && led <= YELLOW && state >= OFF && state <= TOGGLE)
	{
		switch(state)
		{
			case OFF:
				leds_port |= _BV(led+3);
				break;
			case ON:
				leds_port &= ~ _BV(led+3);
				break;
			case TOGGLE:
				leds_port ^= _BV(led+3);
				break;
			default:
				break;
		}
	}
	return;
}
