#ifndef __LEDS_H__
#define __LEDS_H__

#include <os_cpu.h>

#define ON 1
#define OFF 0
#define TOGGLE 2
#define leds_port PORTC //PORTC 4:red, 5:green, 6:yellow
enum{
	RED=1,
	GREEN,
	YELLOW
};

MCU_ASSIGN_PIN(LED1, C, 4);
MCU_ASSIGN_PIN(LED2, C, 5);
MCU_ASSIGN_PIN(LED3, C, 6);

void leds_init(void);
void leds(unsigned char led,  unsigned char state);

#endif

