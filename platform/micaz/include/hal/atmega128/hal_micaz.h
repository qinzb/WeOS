/*******************************************************************************************************
 *                                                                                                     *
 *        **********                                                                                   *
 *       ************                                                                                  *
 *      ***        ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                        CHIPCON HARDWARE ABSTRACTION LIBRARY FOR THE CC2420             *
 *      ***   + +   ***                CC2420DK + STK500 + STK501 platform defintion file              *
 *      ***   +++   ***                                                                                *
 *      ***        ***                                                                                 *
 *       ************                                                                                  *
 *        **********                                                                                   *
 *                                                                                                     *
 *******************************************************************************************************
 * The Chipcon Hardware Abstraction Library is a collection of functions, macros and constants, which  *
 * can be used to ease access to the hardware on the CC2420 and the target microcontroller.            *
 *                                                                                                     *
 * This file contains all definitions that are specific for the CC2420DK + STK500 + STK501 development *
 * platform.                                                                                           *
 *******************************************************************************************************
 * Compiler: AVR-GCC                                                                                   *
 * Target platform: CC2420DK + STK500 + STK501                                                         *
 *******************************************************************************************************
 * Revision history:                                                                                   *
 *******************************************************************************************************/
#ifndef HAL_MICAZ_H
#define HAL_MICAZ_H
#include <sos.h>

/*******************************************************************************************************
 *******************************************************************************************************
 **************************                   AVR I/O PORTS                   **************************
 *******************************************************************************************************
 *******************************************************************************************************/

// The following definitions have been set by Ram from TinyOS-1.1.7 MicaZ pin map (tinyos-1.x/tos/platform/micaz/hardware.h)

//-------------------------------------------------------------------------------------------------------
// Port A:
#define RESET_N		6  // PA.6 - Output: RESET_N to CC2420 *
#define VREG_EN 	5  // PA.5 - Output: VREG_EN to CC2420 *

//-------------------------------------------------------------------------------------------------------
// Port B:
#define CSN		0  // PB.0 - Output: SPI Chip Select (CS_N)
#define SCK		1  // PB.1 - Output: SPI Serial Clock (SCLK)
#define MOSI		2  // PB.2 - Output: SPI Master out - slave in (MOSI)
#define MISO		3  // PB.3 - Input:  SPI Master in - slave out (MISO)
#define FIFO		7  // PB.7 - Input:  FIFO from CC2420 *
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Port D:
#define SFD		4  // PD.4 - Input:  SFD from CC2420
#define CCA		6  // PD.6 - Input:  CCA from CC2420 *
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
// Port E:
#define FIFOP		6  // PE.6 - Input:  FIFOP from CC2420 *
// Ram: The above is the external interrupt 6

//-------------------------------------------------------------------------------------------------------
// Port setup macros

// Port initialization
// Debug signals on PORTD 1-3
// Disables pull-up on all inputs!!!
/* #define PORT_INIT() \ */
/* 	do { \ */
/*     	SFIOR |= BM(PUD); \ */
/*     	DDRB  = BM(RESET_N) | BM(VREG_EN) | BM(MOSI) | BM(SCK) | BM(CSN); \ */
/*     	PORTB = BM(RESET_N) | BM(MOSI) | BM(SCK) | BM(CSN); \ */
/*     	DDRD  = 0x0E; \ */
/*     	SW_INIT(); \ */
/*     	LED_INIT(); \ */
/*        } while (0) */
// Ram: I have removed the Pull Up Disable Flag .... 
#define PORT_INIT() \
        do { \
		DDRA |= (_BV(RESET_N) | _BV(VREG_EN));      \
		DDRB |= (_BV(SCK) | _BV(CSN) | _BV(MOSI));  \
		DDRB &= ~(_BV(MISO) | _BV(FIFO));           \
		DDRD &= ~(_BV(SFD) | _BV(CCA));             \
		DDRE &= ~(_BV(FIFOP));                      \
        SPI_DISABLE();\
        } while (0) 

// Enables/disables the SPI interface
#define SPI_ENABLE()                PORTB &= ~(_BV(CSN))
#define SPI_DISABLE()               PORTB |= (_BV(CSN))

//-------------------------------------------------------------------------------------------------------
 



/*******************************************************************************************************
 *******************************************************************************************************
 **************************                 CC2420 PIN ACCESS                 **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// CC2420 pin access
#define FIFO_IS_1		(!!(bit_is_set(PINB, FIFO)))
#define CCA_IS_1		(!!(bit_is_set(PIND, CCA)))
#define RESET_IS_1		(!!(bit_is_set(PINA, RESET_N)))
#define VREG_IS_1		(!!(bit_is_set(PINA, VREG_EN)))
#define FIFOP_IS_1		(!!(bit_is_set(PINE ,FIFOP)))
#define SFD_IS_1		(!!(bit_is_set(PIND ,SFD)))

// CC2420 reset_n pin
#define SET_RESET_ACTIVE()    PORTA &= ~_BV(RESET_N)
#define SET_RESET_INACTIVE()  PORTA |= _BV(RESET_N)

// CC2420 voltage regulator enable pin
#define SET_VREG_ACTIVE()    PORTA |= _BV(VREG_EN);
#define SET_VREG_INACTIVE()  PORTA &= ~_BV(VREG_EN);
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************               EXTERNAL INTERRUPTS                 **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// FIFOP on external interrupt 6
#define ENABLE_FIFOP_INT()          do { EIMSK |= _BV(FIFOP); } while (0)
#define DISABLE_FIFOP_INT()         do { EIMSK &= ~_BV(FIFOP); } while (0)
#define CLEAR_FIFOP_INT()           do { EIFR |= _BV(FIFOP); } while (0)

// Rising edge trigger for external interrupt 6 (FIFOP)
#define FIFOP_INT_INIT()            do { EICRB |= _BV(ISC61); EICRB |= _BV(ISC60); CLEAR_FIFOP_INT(); } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// SFD interrupt on timer 1 capture pin
#define ENABLE_SFD_CAPTURE_INT()    do { TIMSK |= _BV(TICIE1); } while (0)
#define DISABLE_SFD_CAPTURE_INT()   do { TIMSK &= ~_BV(TICIE1); } while (0)
#define CLEAR_SFD_CAPTURE_INT()     do { TIFR |= _BV(ICF1); } while (0)
//-------------------------------------------------------------------------------------------------------

#endif
