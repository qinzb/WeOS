/*******************************************************************************************************
 *                                                                                                     *
 *         *********                                                                                   *
 *        ***********                                                                                  *
 *       ***       ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                         CHIPCON CC2420 INTEGRATED 802.15.4 MAC AND PHY                 *
 *      ***   + +   ***                            MAC parameter setup file                            *
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

#ifndef MAC_SETUP_H
#define MAC_SETUP_H


// Maximum number of indirect packets stored in the coordinator. MAX_INDIRECT_PACKETS must
// be set to a maximum value of 7. If a higher value is requested, changes must be made to the
// beacon transmission function to ensure that the beacon only indicates a maximum of 7 pending data frames.
// MAC_OPT_MAX_INDIRECT_PACKETS is ignored if MAC_OPT_FFD is 0
#ifndef MAC_OPT_MAX_INDIRECT_PACKETS
//#define MAC_OPT_MAX_INDIRECT_PACKETS 7
#define MAC_OPT_MAX_INDIRECT_PACKETS 2
#endif


// Is the current device a FFD or RFD?
// Currently only FFD has been tested.
// Setting MAC_OPT_FFD to 0 will reduce code and RAM sizes
#ifndef MAC_OPT_FFD
    #define MAC_OPT_FFD 1
#endif


// The maximum number of devices in the ACL
// If set to 0, ACL is not implemented.
// If MAC_OPT_SECURITY is set, MAC_OPT_ACL_SIZE must be >= 1
#ifndef MAC_OPT_ACL_SIZE
//    #define MAC_OPT_ACL_SIZE 8
    #define MAC_OPT_ACL_SIZE 0
#endif

// Is security implemented?
// 0 : No
// 1 : Yes
#ifndef MAC_OPT_SECURITY
//    #define MAC_OPT_SECURITY 1
    #define MAC_OPT_SECURITY 0
#endif

// If security is implemented, are optional sequeintial freshness included?
#ifndef MAC_OPT_SEQUENTIAL_FRESHNESS
//    #define MAC_OPT_SEQUENTIAL_FRESHNESS 1
    #define MAC_OPT_SEQUENTIAL_FRESHNESS 0
#endif

// The maximum number of PAN descriptors returned from a passive or active scan. This memory space
// is set up by a layer above the MAC
#ifndef MAC_OPT_MAX_PAN_DESCRIPTORS
//    #define MAC_OPT_MAX_PAN_DESCRIPTORS 4
     #define MAC_OPT_MAX_PAN_DESCRIPTORS 2
#endif


#endif
