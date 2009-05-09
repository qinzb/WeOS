/*******************************************************************************************************
 *                                                                                                     *
 *         *********                                                                                   *
 *        ***********                                                                                  *
 *       ***       ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                         CHIPCON CC2420 INTEGRATED 802.15.4 MAC AND PHY                 *
 *      ***   + +   ***                           Transmission using CSMA-CA                           *
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
 * This file contains the interface to the CSMA-CA transmission mechanism.                             *
 *******************************************************************************************************/

#ifndef MAC_CSMACA_H
#define MAC_CSMACA_H
#include <sos.h>
//-------------------------------------------------------------------------------------------------------
// Function prototypes

// Starts a transmission using CSMA-CA
void csmaTxInit(MAC_PACKET *pPacket);

// This prototype is used to cancel the timeout (upon reception of an acknowledgment)
void csmaTxTimeout(void);

// Called after beacon reception to resume the CSMA-CA mechanism
BOOL csmaHandleTxAfterBeacon(void);

// Calculates the duration of a packet, including ack. and IFS
UINT8 csmaCalcPacketDuration(UINT8 length, BOOL ackRequest);

// Generate callback on command frames
void csmaCommandCallback(MAC_ENUM status, MAC_COMMAND_TYPE command);

//-------------------------------------------------------------------------------------------------------




#endif
