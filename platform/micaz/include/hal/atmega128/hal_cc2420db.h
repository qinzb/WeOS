/*******************************************************************************************************
 *                                                                                                     *
 *        **********                                                                                   *
 *       ************                                                                                  *
 *      ***        ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                        CHIPCON HARDWARE ABSTRACTION LIBRARY FOR THE CC2420             *
 *      ***   + +   ***                             CC2420DB defintion file                            *
 *      ***   +++   ***                                                                                *
 *      ***        ***                                                                                 *
 *       ************                                                                                  *
 *        **********                                                                                   *
 *                                                                                                     *
 *******************************************************************************************************
 * The Chipcon Hardware Abstraction Library is a collection of functions, macros and constants, which  *
 * can be used to ease access to the hardware on the CC2420 and the target microcontroller.            *
 *                                                                                                     *
 * This file contains all definitions that are specific for the CC2420DB development platform.         *
 *******************************************************************************************************
 * Compiler: AVR-GCC                                                                                   *
 * Target platform: CC2420DB                                                                           *
 *******************************************************************************************************
 * Revision history:                                                                                   *
 *  $Log: hal_cc2420db.h,v $
 *  Revision 1.1.1.1  2005/06/23 05:11:39  simonhan
 *  initial import
 *
 *  Revision 1.1.1.1  2005/06/23 04:44:19  simonhan
 *  initial import
 *
 *  Revision 1.1.1.1  2005/06/23 04:11:50  simonhan
 *  initial import
 *
 *  Revision 1.1  2005/04/25 08:02:28  simonhan
 *  check in micaz directory
 *
 *  Revision 1.1.1.1  2005/01/14 22:30:25  simonhan
 *  sos micaz
 *
 *  Revision 1.1  2004/08/26 20:06:39  ram
 *  MicaZ IEEE 802.15.4 MAC
 *
 *  Revision 1.11  2004/04/05 14:52:58  jol
 *  The red and yellow LEDs in the SET_LED_MASK(n) macro have been switched (wrong in the previous version)
 *
 *  Revision 1.10  2004/03/31 11:07:29  oyj
 *  Added postfix _UART1 to pins CTS and RTS throughout the file
 *  (error in previous revision)
 *
 *  Revision 1.9  2004/03/30 14:59:03  mbr
 *  Release for web
 *  
 *
 *
 *
 *******************************************************************************************************/
#ifndef HAL_CC2400DB_H
#define HAL_CC2400DB_H




/*******************************************************************************************************
 *******************************************************************************************************
 **************************                   AVR I/O PORTS                   **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// Port B
#define CSN             0  // PB.0 - Output: SPI Chip Select (CS_N)
#define SCK             1  // PB.1 - Output: SPI Serial Clock (SCLK)
#define MOSI            2  // PB.2 - Output: SPI Master out - slave in (MOSI)
#define MISO            3  // PB.3 - Input:  SPI Master in - slave out (MISO)
#define OLED            4  // PB.4 - Output: Orange LED
#define VREG_EN         5  // PB.5 - Output: VREG_EN to CC2420
#define RESET_N         6  // PB.6 - Output: RESET_N to CC2420
#define GLED            7  // PB.7 - Output: Green LED
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Port D
#define FIFOP           0 // PD.0 - Input:  FIFOP from CC2420
#define FIFO            1 // PD.1 - Input:  FIFO from CC2420
#define UART1_RXD       2 // PD.2 - Input:  UART1 RXD
#define UART1_TXD       3 // PD.3 - Output: UART1 TXD
#define SFD             4 // PD.4 - Input:  SFD from CC2420
#define UART1_RTS       5 // PD.5 - Output: UART HW handshaking: RTS
#define CCA             6 // PD.6 - Input:  CCA from CC2420
#define UART1_CTS       7 // PD.7 - Input:  UART HW handshaking: CTS
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Port E
#define JOYSTICK_UP     0 // PE.0 - Input : Joystick up button
#define JOYSTICK_RIGHT  1 // PE.1 - Input : Joystick right
#define JOYSTICK_CENTER 2 // PE.2 - Input : Joystick center
#define RLED            3 // PE.3 - Output: Red LED
#define YLED            4 // PE.4 - Output: Yellow LED
#define BUTTON          5 // PE.5 - Input : Push button S2
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Port F
#define JOYSTICK_LEFT   1 // PF.1 - Input: Joystick left
#define JOYSTICK_DOWN   2 // PF.2 - Input: Joystick down

// ADC inputs
#define ADC_INPUT_0_POT_METER       0 // PF.0 - ADC0 with potentiometer
#define ADC_INPUT_1                 1 // PF.1 - ADC1
#define ADC_INPUT_2                 2 // PF.2 - ADC2
#define ADC_INPUT_3_TEMP_SENSOR     3 // PF.3 - ADC3 with temp sensor

// JTAG interface:
//     PF.4 - Input : JTAG TCK serial clock
//     PF.5 - Input : JTAG TMS strobe enable
//     PF.6 - Output: JTAG TDO serial data output
//     PF.7 - Input : JTAG TDI serial data input
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Port G
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// External RAM interface:
//     PA and PC - Multiplexed address/data
//     PG.0 - Output: Write enable: WR_N
//     PG.1 - Output: Read enable: RD_N
//     PG.2 - Output: Address Latch Enable: ALE
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Port setup macros

// Port initialization
// Disables pull-up on all inputs!!!
#define PORT_INIT() \
    do { \
        SFIOR |= BM(PUD); \
        DDRB  = BM(RESET_N) | BM(VREG_EN) | BM(MOSI) | BM(SCK) | BM(CSN) | BM(OLED) | BM(GLED); \
        PORTB = BM(RESET_N) | BM(MOSI) | BM(SCK) | BM(CSN); \
        DDRD  = BM(UART1_RTS); \
        PORTD = BM(UART1_RTS) | BM(UART1_CTS);\
        DDRE  = BM(RLED) | BM(YLED); \
        PORTE = 0x00; \
    } while (0)

// Enables the external SRAM
#define ENABLE_EXT_RAM() (MCUCR |= BM(SRE))

// Enables/disables the SPI interface
#define SPI_ENABLE()                (PORTB &= ~BM(CSN))
#define SPI_DISABLE()               (PORTB |= BM(CSN))
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************                 CC2420 PIN ACCESS                 **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// CC2420 pin access

// Pin status
#define FIFO_IS_1       (!!(PIND & BM(FIFO)))
#define CCA_IS_1        (!!(PIND & BM(CCA)))
#define RESET_IS_1      (!!(PINB & BM(RESET_N)))
#define VREG_IS_1       (!!(PINB & BM(VREG_EN)))
#define FIFOP_IS_1      (!!(PIND & BM(FIFOP)))
#define SFD_IS_1        (!!(PIND & BM(SFD)))

// The CC2420 reset pin
#define SET_RESET_ACTIVE()    PORTB &= ~BM(RESET_N)
#define SET_RESET_INACTIVE()  PORTB |= BM(RESET_N)

// CC2420 voltage regulator enable pin
#define SET_VREG_ACTIVE()     PORTB |= BM(VREG_EN)
#define SET_VREG_INACTIVE()   PORTB &= ~BM(VREG_EN)
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************                   SERIAL PORTS                    **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// UART hardware flow control
#define CLR_RTS()                  (PORTD &= ~BM(UART1_RTS))
#define SET_RTS()                  (PORTD |= BM(UART1_RTS))

#define SET_CTS()                  (PORTD |= BM(UART1_CTS))
#define CLR_CTS()                  (PORTD &= ~BM(UART1_CTS))
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************               EXTERNAL INTERRUPTS                 **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// Rising edge trigger for external interrupt 0 (FIFOP)
#define FIFOP_INT_INIT()            do { EICRA |= 0x03; CLEAR_FIFOP_INT(); } while (0)

// FIFOP on external interrupt 0
#define ENABLE_FIFOP_INT()          do { EIMSK |= 0x01; } while (0)
#define DISABLE_FIFOP_INT()         do { EIMSK &= ~0x01; } while (0)
#define CLEAR_FIFOP_INT()           do { EIFR = 0x01; } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// SFD interrupt on timer 1 capture pin
#define ENABLE_SFD_CAPTURE_INT()    do { TIMSK |= BM(TICIE1); } while (0)
#define DISABLE_SFD_CAPTURE_INT()   do { TIMSK &= ~BM(TICIE1); } while (0)
#define CLEAR_SFD_CAPTURE_INT()     do { TIFR = BM(ICF1); } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Falling edge trigger for external interrupt 6 (joystick)
#define JOYSTICK_INT_INIT()         do { EICRB |= 0x20; } while (0)

// Joystick on external interrupt 6
#define ENABLE_JOYSTICK_INT()       do { EIMSK |= 0x40; } while (0)
#define DISABLE_JOYSTICK_INT()      do { EIMSK &= ~0x40; } while (0)
#define CLEAR_JOYSTICK_INT()        do { EIFR = 0x40; } while (0)
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************                      BUTTONS                      **************************
 *******************************************************************************************************
 *******************************************************************************************************/

//-------------------------------------------------------------------------------------------------------
// S2 button
#define BUTTON_PRESSED()           !(PINE & BM(BUTTON))

// Joystick
#define JOYSTICK_UP_PRESSED()       (PINE & BM(JOYSTICK_UP))
#define JOYSTICK_DOWN_PRESSED()     (PINF & BM(JOYSTICK_DOWN))
#define JOYSTICK_LEFT_PRESSED()     (PINF & BM(JOYSTICK_LEFT))
#define JOYSTICK_RIGHT_PRESSED()    (PINE & BM(JOYSTICK_RIGHT))
#define JOYSTICK_CENTER_PRESSED()   (PINE & BM(JOYSTICK_CENTER))
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************                        LEDS                       **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// Clears LED
#define CLR_GLED()                  (PORTB &= ~BM(GLED))
#define CLR_OLED()                  (PORTB &= ~BM(OLED))
#define CLR_YLED()                  (PORTE &= ~BM(YLED))
#define CLR_RLED()                  (PORTE &= ~BM(RLED))

// Set LED
#define SET_GLED()                  (PORTB |= BM(GLED))
#define SET_OLED()                  (PORTB |= BM(OLED))
#define SET_YLED()                  (PORTE |= BM(YLED))
#define SET_RLED()                  (PORTE |= BM(RLED))

// Sets all leds simultaneously to display a 4-bit binary code
#define SET_LED_MASK(n) \
    do { \
        if (n & 0x01) SET_GLED(); else CLR_GLED(); \
        if (n & 0x02) SET_OLED(); else CLR_OLED(); \
        if (n & 0x04) SET_RLED(); else CLR_RLED(); \
        if (n & 0x08) SET_YLED(); else CLR_YLED(); \
    } while (0)
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************               APPLICATION DEBUGGING               **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// Controlled application crash (flashes the LEDs forever to indicate an error code)
#define EXCEPTION(n) \
    do { \
        DISABLE_GLOBAL_INT(); \
        SET_LED_MASK(n); \
        halWait(50000); \
        SET_LED_MASK(0); \
        halWait(50000); \
    } while (TRUE)
//-------------------------------------------------------------------------------------------------------


#endif
