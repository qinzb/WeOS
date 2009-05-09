/*******************************************************************************************************
 *                                                                                                     *
 *         *********                                                                                   *
 *        ***********                                                                                  *
 *       ***       ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                         CHIPCON CC2420 INTEGRATED 802.15.4 MAC AND PHY                 *
 *      ***   + +   ***                            MAC LAYER SECURITY FUNCTIONS                        *
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
 * This file contains the MAC setup parameters which may be set differently for different applications *
 * or devices. These parameters can also be set in the make file.                                      *
 *******************************************************************************************************/

#ifndef MAC_SECURITY_H
#define MAC_SECURITY_H
#include <sos.h>
//-------------------------------------------------------------------------------------------------------
// Function prototypes
//void msupFindSecurityMaterial(MAC_PACKET* pPacket)

BYTE msupFindSecurityMaterial(BOOL isTx, BOOL securityEnable, BYTE addrMode, WORD panId, ADDRESS *pAddr, SECURITY_MATERIAL **ppSecurityMaterial, QWORD **ppExtendedAddress);
BYTE msupSetupCC2420Security(BOOL isTx, BYTE securityMode, KEY pKey, QWORD* pExtendedAddress, BYTE* pCounters, BYTE clearTextLength);

#endif
