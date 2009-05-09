/*******************************************************************************************************
 *                                                                                                     *
 *        **********                                                                                   *
 *       ************                                                                                  *
 *      ***        ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                        CHIPCON HARDWARE ABSTRACTION LIBRARY FOR THE CC2420             *
 *      ***   + +   ***                               Library header file                              *
 *      ***   +++   ***                                                                                *
 *      ***        ***                                                                                 *
 *       ************                                                                                  *
 *        **********                                                                                   *
 *                                                                                                     *
 *******************************************************************************************************
 * The Chipcon Hardware Abstraction Library is a collection of functions, macros and constants, which  *
 * can be used to ease access to the hardware on the CC2420 and the target microcontroller.            *
 *                                                                                                     *
 * All HAL library macros and constants are defined here.                                              *
 *******************************************************************************************************
 * Compiler: AVR-GCC                                                                                   *
 * Target platform: CC2420DB, CC2420 + any ATMEGA MCU                                                  *
 *******************************************************************************************************
 * Revision history:                                                                                   *
 *  $Log: hal.h,v $
 *  Revision 1.2  2005/10/25 17:51:26  simonhan
 *  remove for loop bug
 *
 *  Revision 1.1.1.1  2005/06/23 05:11:39  simonhan
 *  initial import
 *
 *  Revision 1.1.1.1  2005/06/23 04:44:18  simonhan
 *  initial import
 *
 *  Revision 1.1.1.1  2005/06/23 04:11:49  simonhan
 *  initial import
 *
 *  Revision 1.1  2005/04/25 08:02:28  simonhan
 *  check in micaz directory
 *
 *  Revision 1.3  2005/03/29 00:54:58  simonhan
 *  remove SOS_DEV_SECTION
 *
 *  Revision 1.2  2005/02/24 01:33:32  roy
 *  Added function to change TX power on node.
 *
 *  Revision 1.1.1.1  2005/01/14 22:30:25  simonhan
 *  sos micaz
 *
 *  Revision 1.3  2004/11/15 04:06:58  ram
 *  Moved the Zigbee MAC to the
 *
 *  Revision 1.2  2004/09/07 21:32:27  ram
 *  Debugged the RX of the IEEE 802.15.4 MAC
 *
 *  Revision 1.1  2004/08/26 20:06:39  ram
 *  MicaZ IEEE 802.15.4 MAC
 *
 *  Revision 1.8  2004/03/30 14:59:03  mbr
 *  Release for web
 *  
 *
 *
 *******************************************************************************************************/
#ifndef HAL_H
#define HAL_H
#include <sos.h>


/*******************************************************************************************************
 *******************************************************************************************************
 **************************            AVR<->CC2420 SPI INTERFACE             **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// Initialization

// Enables SPI, selects "master", clock rate FCK / 2, and SPI mode 0
#define SPI_INIT() \
    do { \
        SPCR = BM(SPE) | BM(MSTR); \
        SPSR = BM(SPI2X); \
    } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// FAST SPI: Low level functions
//      x = value (BYTE or WORD)
//      p = pointer to the byte array to operate on
//      c = the byte count
//
// SPI_ENABLE() and SPI_DISABLE() are located in the devboard header file (CS_N uses GPIO)

#define FASTSPI_WAIT() \
    do { \
        while (!(SPSR & BM(SPIF))); \
    } while (0) 

#define FASTSPI_TX(x) \
    do { \
        SPDR = x; \
        FASTSPI_WAIT(); \
    } while (0)

#define FASTSPI_RX(x) \
    do { \
        SPDR = 0; \
        FASTSPI_WAIT(); \
        x = SPDR; \
    } while (0)

#define FASTSPI_RX_GARBAGE() \
    do { \
        SPDR = 0; \
        FASTSPI_WAIT(); \
    } while (0)

#define FASTSPI_TX_WORD_LE(x) \
    do { \
        FASTSPI_TX(x); \
        FASTSPI_TX((x) >> 8); \
    } while (0)
    
#define FASTSPI_TX_WORD(x) \
    do { \
        FASTSPI_TX(((WORD)(x)) >> 8); \
        FASTSPI_TX((BYTE)(x)); \
    } while (0)
    
#define FASTSPI_TX_MANY(p,c) \
    do { \
		UINT8 spiCnt = 0;    \
        for (spiCnt = 0; spiCnt < (c); spiCnt++) { \
            FASTSPI_TX(((BYTE*)(p))[spiCnt]); \
        } \
    } while (0)
        
#define FASTSPI_RX_WORD_LE(x) \
    do { \
        SPDR = 0; \
        FASTSPI_WAIT(); \
        x = SPDR; \
        SPDR = 0; \
        FASTSPI_WAIT(); \
        x |= SPDR << 8; \
    } while (0)

#define FASTSPI_RX_WORD(x) \
    do { \
        SPDR = 0; \
        FASTSPI_WAIT(); \
        x = SPDR << 8; \
        SPDR = 0; \
        FASTSPI_WAIT(); \
        x |= SPDR; \
    } while (0)
    
#define FASTSPI_RX_MANY(p,c) \
    do { \
		UINT8 spiCnt = 0;    \
        for (spiCnt = 0; spiCnt < (c); spiCnt++) { \
            FASTSPI_RX((p)[spiCnt]); \
        } \
    } while (0)
        
// Register address:
#define FASTSPI_TX_ADDR(a) \
    do { \
        SPDR = a; \
        FASTSPI_WAIT(); \
    } while (0)

// Register address:
#define FASTSPI_RX_ADDR(a) \
    do { \
        SPDR = (a) | 0x40; \
        FASTSPI_WAIT(); \
    } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
//  FAST SPI: Register access
//      s = command strobe
//      a = register address
//      v = register value

#define FASTSPI_STROBE(s) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_TX_ADDR(s); \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_SETREG(a,v) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_TX_ADDR(a); \
        FASTSPI_TX((BYTE) ((v) >> 8)); \
        FASTSPI_TX((BYTE) (v)); \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_GETREG(a,v) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_RX_ADDR(a); \
        FASTSPI_RX_WORD(v); \
        SPI_DISABLE(); \
    } while (0)

// Updates the SPI status byte
#define FASTSPI_UPD_STATUS(s) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_TX_ADDR(CC2420_SNOP); \
        s = SPDR; \
	SPI_DISABLE();\
    } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
//  FAST SPI: FIFO access
//      p = pointer to the byte array to be read/written
//      c = the number of bytes to read/write
//      b = single data byte

#define FASTSPI_WRITE_FIFO(p,c) \
    do { \
		UINT8 spiCnt = 0;       \
        SPI_ENABLE(); \
        FASTSPI_TX_ADDR(CC2420_TXFIFO); \
        for (spiCnt = 0; spiCnt < (c); spiCnt++) { \
            FASTSPI_TX(((BYTE*)(p))[spiCnt]); \
        } \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_READ_FIFO(p,c) \
    do { \
		UINT8 spiCnt = 0;      \
        SPI_ENABLE(); \
        FASTSPI_RX_ADDR(CC2420_RXFIFO); \
        for (spiCnt = 0; spiCnt < (c); spiCnt++) { \
            while (!FIFO_IS_1); \
            FASTSPI_RX(((BYTE*)(p))[spiCnt]); \
        } \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_READ_FIFO_BYTE(b) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_RX_ADDR(CC2420_RXFIFO); \
        FASTSPI_RX(b); \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_READ_FIFO_NO_WAIT(p,c) \
    do { \
		UINT8 spiCnt = 0;              \
        SPI_ENABLE(); \
        FASTSPI_RX_ADDR(CC2420_RXFIFO); \
        for (spiCnt = 0; spiCnt < (c); spiCnt++) { \
            FASTSPI_RX(((BYTE*)(p))[spiCnt]); \
        } \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_READ_FIFO_GARBAGE(c) \
    do { \
		UINT8 spiCnt = 0;            \
        SPI_ENABLE(); \
        FASTSPI_RX_ADDR(CC2420_RXFIFO); \
        for (spiCnt = 0; ((spiCnt < (c)) && (FIFO_IS_1)); spiCnt++) { \
            FASTSPI_RX_GARBAGE(); \
        } \
        SPI_DISABLE(); \
    } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
//  FAST SPI: CC2420 RAM access (big or little-endian order)
//      p = pointer to the variable to be written
//      a = the CC2420 RAM address
//      c = the number of bytes to write
//      n = counter variable which is used in for/while loops (UINT8)
//
//  Example of usage:
//      UINT8 n;
//      UINT16 shortAddress = 0xBEEF;
//      FASTSPI_WRITE_RAM_LE(&shortAddress, CC2420RAM_SHORTADDR, 2);

#define FASTSPI_WRITE_RAM_LE(p,a,c,n) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_TX(0x80 | (a & 0x7F)); \
        FASTSPI_TX((a >> 1) & 0xC0); \
        for (n = 0; n < (c); n++) { \
            FASTSPI_TX(((BYTE*)(p))[n]); \
        } \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_READ_RAM_LE(p,a,c,n) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_TX(0x80 | (a & 0x7F)); \
        FASTSPI_TX(((a >> 1) & 0xC0) | 0x20); \
        for (n = 0; n < (c); n++) { \
            FASTSPI_RX(((BYTE*)(p))[n]); \
        } \
        SPI_DISABLE(); \
    } while (0)
    
#define FASTSPI_WRITE_RAM(p,a,c,n) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_TX(0x80 | (a & 0x7F)); \
        FASTSPI_TX((a >> 1) & 0xC0); \
        n = c; \
        do { \
            FASTSPI_TX(((BYTE*)(p))[--n]); \
        } while (n); \
        SPI_DISABLE(); \
    } while (0)

#define FASTSPI_READ_RAM(p,a,c,n) \
    do { \
        SPI_ENABLE(); \
        FASTSPI_TX(0x80 | (a & 0x7F)); \
        FASTSPI_TX(((a >> 1) & 0xC0) | 0x20); \
        n = c; \
        do { \
            FASTSPI_RX(((BYTE*)(p))[--n]); \
        } while (n); \
        SPI_DISABLE(); \
    } while (0)
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Other useful SPI macros
#define FASTSPI_RESET_CC2420() \
    do { \
        FASTSPI_SETREG(CC2420_MAIN, 0x0000); \
        FASTSPI_SETREG(CC2420_MAIN, 0xF800); \
    } while (0)
//-------------------------------------------------------------------------------------------------------




/*******************************************************************************************************
 *******************************************************************************************************
 **************************                    INTERRUPTS                     **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// General
#define ENABLE_GLOBAL_INT()         do { asm ("sei\n\t" ::); } while (0)
#define DISABLE_GLOBAL_INT()        do { asm ("cli\n\t" ::); } while (0)
//-------------------------------------------------------------------------------------------------------


/*******************************************************************************************************
 *******************************************************************************************************
 **************************                   USEFUL STUFF                    **************************
 *******************************************************************************************************
 *******************************************************************************************************/


//-------------------------------------------------------------------------------------------------------
// Useful stuff
#define NOP() asm volatile ("nop\n\t" ::)
//-------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------
//  void halWait(UINT16 timeout)
//
//  DESCRIPTION:
//      Runs an idle loop for [timeout] microseconds.
//
//  ARGUMENTS:
//      UINT16 timeout
//          The timeout in microseconds
//-------------------------------------------------------------------------------------------------------
void halWait(UINT16 timeout);




/*******************************************************************************************************
 *******************************************************************************************************
 **************************              SIMPLE CC2420 FUNCTIONS              **************************
 *******************************************************************************************************
 *******************************************************************************************************/

//-------------------------------------------------------------------------------------------------------
//  Example of usage: Starts RX on channel 14 after reset
//      FASTSPI_RESET_CC2420();
//      FASTSPI_STROBE(CC2420_SXOSCON);
//      halRfSetChannel(14);
//      ... other registers can for instance be initialized here ...
//      halRfWaitForCrystalOscillator();
//      ... RAM access can be done here, since the crystal oscillator must be on and stable ...
//      FASTSPI_STROBE(CC2420_SRXON);
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
//  void rfWaitForCrystalOscillator(void)
//
//  DESCRIPTION:
//      Waits for the crystal oscillator to become stable. The flag is polled via the SPI status byte.
//
//      Note that this function will lock up if the SXOSCON command strobe has not been given before the
//      function call. Also note that global interrupts will always be enabled when this function
//      returns.
//-------------------------------------------------------------------------------------------------------
void halRfWaitForCrystalOscillator(void);


//-------------------------------------------------------------------------------------------------------
//  void halRfSetChannel(UINT8 Channel)
//
//  DESCRIPTION:
//      Programs CC2420 for a given IEEE 802.15.4 channel.
//      Note that SRXON, STXON or STXONCCA must be run for the new channel selection to take full effect.
//
//  PARAMETERS:
//      UINT8 channel
//          The channel number (11-26)
//-------------------------------------------------------------------------------------------------------
void halRfSetChannel(UINT8 channel);


//
//  void halTxSetPower(UINT8 power)
//
//  DESCRIPTION:
//      Function to control tx power of the CC2420
//
//  PARAMETERS:
//      UINT8 power
//          Power level (31-0) with 31 high power
void halTxSetPower(UINT8 power);

#endif
