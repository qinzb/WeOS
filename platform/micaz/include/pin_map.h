/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4: */

#ifndef _PIN_MAP_H_
#define _PIN_MAP_H_

#include <pin_defs.h>
#include <pin_alt_func.h>

// LED assignments
ALIAS_IO_PIN( RED_LED, PINA2);
ALIAS_IO_PIN( GREEN_LED, PINA1);
ALIAS_IO_PIN( YELLOW_LED, PINA0);

ALIAS_IO_PIN( SERIAL_ID, PINA4);
ALIAS_IO_PIN( BAT_MON, PINA5);
ALIAS_IO_PIN( THERM_PWR, PINA7);

// ChipCon control assignments

#define CC_FIFOP_INT SIG_INTERRUPT6
ALIAS_IO_PIN( CC_RSTN, PINA6); // chipcon reset
ALIAS_IO_PIN( CC_VREN, PINA5); // chipcon power enable
//ALIAS_IO_PIN( CC_FIFOP1, PIND7);  // fifo interrupt
ALIAS_IO_PIN( CC_FIFOP1, PINE6);  // fifo interrupt

ALIAS_IO_PIN( CC_CCA, PIND6);	  // 
ALIAS_IO_PIN( CC_SFD, PIND4);	  // chipcon packet arrival
ALIAS_IO_PIN( CC_CS, PINB0);	  // chipcon enable
ALIAS_IO_PIN( CC_FIFO, PINB7);	  // chipcon fifo

ALIAS_IO_PIN( RADIO_CCA, PIND6);	  // 

// Flash assignments
ALIAS_IO_PIN( FLASH_SELECT, PINA3);
ALIAS_IO_PIN( FLASH_CLK, PIND5);
ALIAS_IO_PIN( FLASH_OUT, PIND3);
ALIAS_IO_PIN( FLASH_IN, PIND2);

// interrupt assignments
ALIAS_IO_PIN( MICAZ_INT0, PINE4);
ALIAS_IO_PIN( MICAZ_INT1, PINE5);
ALIAS_IO_PIN( MICAZ_INT2, PINE6);
ALIAS_IO_PIN( MICAZ_INT3, PINE7);

// spibus assignments 
//ALIAS_IO_PIN( SPI_OC1C, PINB7);

// power control assignments
ALIAS_IO_PIN( PW0, PINC0);
ALIAS_IO_PIN( PW1, PINC1);
ALIAS_IO_PIN( PW2, PINC2);
ALIAS_IO_PIN( PW3, PINC3);
ALIAS_IO_PIN( PW4, PINC4);
ALIAS_IO_PIN( PW5, PINC5);
ALIAS_IO_PIN( PW6, PINC6);
ALIAS_IO_PIN( PW7, PINC7);

// Function implemented in hardware.c
void init_IO(void);

#endif //_PIN_MAP_H_




