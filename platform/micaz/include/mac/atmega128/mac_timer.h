/*******************************************************************************************************
 *                                                                                                     *
 *         *********                                                                                   *
 *        ***********                                                                                  *
 *       ***       ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                         CHIPCON CC2420 INTEGRATED 802.15.4 MAC AND PHY                 *
 *      ***   + +   ***                           Low-level timing functions                           *
 *      ***   +++   ***                                                                                *
 *       ***       ***                                                                                 *
 *        ***********                                                                                  *
 *         *********                                                                                   *
 *                                                                                                     *
 *******************************************************************************************************
 * CONFIDENTIAL                                                                                        *
 *******************************************************************************************************
 * The use of this file is restricted by the signed MAC software license agreement.                    *
 *******************************************************************************************************
 * This file contains function prototypes for the MAC callback timer engine. The callback timer        *
 * generates a T1.COMPA interrupt at every 320 usecs = 20 symbols = 1 backoff slot. Command strobes    *
 * that need to be aligned with backoff slots must be sent immediately after TCNT1 = 0, using the      *
 * WAIT_FOR_BOUNADRY() macro.                                                                          *
 *                                                                                                     *
 * NOTE: These functions are meant to be used with an 8 MHz crystal oscillator!                        *
 *******************************************************************************************************/
 
#ifndef MAC_TIMER_H
#define MAC_TIMER_H
#include <sos.h>

//-------------------------------------------------------------------------------------------------------
// Callback table
#define MAC_TIMER_COUNT 12

typedef struct {
   	VFPTR pFunc;
	INT32 timeout;
    UINT8 nextTimer;
    UINT8 prevTimer;
    BOOL active;
} MAC_TIMER_INFO;
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Function prototypes

// Callback timer setup (timeout in backoff slots (rounded downwards)
void msupInitCallbackTimer(void);
BOOL msupSetCallbackTimer(VFPTR pFunc, INT32 timeout);
BOOL msupCancelCallbackTimer(void *pFunc);

// Wait for the next backoff slot boandary (global interrupts must be turned off when the timer wraps!)
#define WAIT_FOR_BOUNDARY() while (!(TIFR & BM(OCF1A)))

// Callback timer adjustments (align this node's timer with the last received beacon)
void msupTimerBeaconAlignment(void);

// Idle looping (timeout in msecs)
void msupWait(UINT16 timeout);

// Clock/time stamp functions
UINT32 msupGetTime(void);
UINT32 msupGetBeaconRxTime(void);
//-------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------
// Backoff slot counter
extern volatile UINT32 macBosCounter;
//-------------------------------------------------------------------------------------------------------

#endif
