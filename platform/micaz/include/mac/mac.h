/*******************************************************************************************************
 *                                                                                                     *
 *         *********                                                                                   *
 *        ***********                                                                                  *
 *       ***       ***                                                                                 *
 *      ***   +++   ***                                                                                *
 *      ***   + +   ***                                                                                *
 *      ***   +                         CHIPCON CC2420 INTEGRATED 802.15.4 MAC AND PHY                 *
 *      ***   + +   ***                               CC2420 definitions                               *
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
 * This file contains PIB and contant definitions, and prototypes for the MAC SAPs. Please note that   *
 * some of the MAC functionality requires buffer pointers to be set (read the function headers in      *
 * mac.c).                                                                                             *
 *                                                                                                     *
 * TBD: Move internal definitions to a separate file?                                                  *
 *******************************************************************************************************/

#ifndef MAC_H
#define MAC_H
#include <sos.h>

//----------------------------------------------------------------------------------------------------------
// PHY constants
#define aMaxPHYPacketSize       127
#define aTurnaroundTime         12 // symbol periods
#define aUnitBackoffPeriod      20

// MAC constants
extern QWORD aExtendedAddress;
#define aBaseSlotDuration       60
#define aNumSuperframeSlots     16
#define aBaseSuperframeDuration (aBaseSlotDuration * aNumSuperframeSlots)
#define aMaxBE                  5
#define aMaxBeaconOverhead      75
#define aMaxBeaconPayloadLength (aMaxPHYPacketSize - aMaxBeaconOverhead)
#define aGTSDescPersistenceTime 4
#define aMaxFrameOverhead       25
#define aMaxFrameResponseTime   1220
#define aMaxFrameRetries        3
#define aMaxLostBeacons         4
#define aMaxMACFrameSize        (aMaxPHYPacketSize - aMaxFrameOverhead)
#define aMaxSIFSFrameSize       18
#define aMinCAPLength           440
#define aMinLIFSPeriod          40
#define aMinSIFSPeriod          12
#define aResponseWaitTime       (32 * aBaseSuperframeDuration)

// Other MAC-defined constants
#define MAX_PENDING_LIST_SIZE   7
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// MAC ACL and security information bases

#define MAC_SECURITY_NONE               0x00
#define MAC_SECURITY_AES_CTR            0x01
#define MAC_SECURITY_AES_CCM128         0x02
#define MAC_SECURITY_AES_CCM64          0x03
#define MAC_SECURITY_AES_CCM32          0x04
#define MAC_SECURITY_AES_CBC_MAC128     0x05
#define MAC_SECURITY_AES_CBC_MAC64      0x06
#define MAC_SECURITY_AES_CBC_MAC32      0x07
#define MAC_HIGHEST_SECURITY_MODE       MAC_SECURITY_AES_CBC_MAC32

#define MAC_UNSECURED_MODE              0x00
#define MAC_ACL_MODE                    0x01
#define MAC_SECURED_MODE                0x02

//----------------------------------------------------------------------------------------------------------
// MAC Security flags definitions.
// Note that the bits are shifted compared to the actual security flags defined by IEEE 802.15.4,
// please see the CC2420 datasheet for details.
#define MAC_CC2420_CTR_FLAGS            0x42
#define MAC_CC2420_CCM_FLAGS            0x09

typedef BYTE KEY[16];

typedef struct {
    KEY pSymmetricKey;
    DWORD frameCounter;
    BYTE keySequenceCounter;
#if MAC_OPT_SEQUENTIAL_FRESHNESS
    DWORD externalFrameCounter;
    BYTE externalKeySequenceCounter;
#endif
} SECURITY_MATERIAL;

// Access Control List
typedef struct {
    QWORD extendedAddress;
    WORD shortAddress;
    WORD panId;
    BYTE securityMaterialLength;
    SECURITY_MATERIAL* pSecurityMaterial;
    BYTE securitySuite;
} ACL_ENTRY;

typedef ACL_ENTRY ACL_ENTRY_SET[MAC_OPT_ACL_SIZE];

//----------------------------------------------------------------------------------------------------------
// MAC and PHY PAN information bases



// MAC PIB
typedef struct {
    BYTE macAckWaitDuration;
    BOOL macAssociationPermit;
    BOOL macAutoRequest;
    BOOL macBattLifeExt;
    BYTE macBattLifeExtPeriods;
    BYTE* pMacBeaconPayload;
    BYTE macBeaconPayloadLength;
    BYTE macBeaconOrder;
    WORD macBeaconTxTime;
    BYTE macBSN;
    QWORD macCoordExtendedAddress;
    WORD macCoordShortAddress;
    BYTE macDSN;
    BOOL macGTSPermit;
    BYTE macMaxCsmaBackoffs;
    BYTE macMinBE;
    WORD macPANId;
    BOOL macPromiscuousMode;
    BOOL macRxOnWhenIdle;
    WORD macShortAddress;
    BYTE macSuperframeOrder;
    WORD macTransactionPersistenceTime;

#if MAC_OPT_ACL_SIZE>0
    // ACL attributes
    ACL_ENTRY_SET* ppMacACLEntryDescriptorSet;
    BYTE macACLEntryDescriptorSetSize;
#endif

#if MAC_OPT_SECURITY
    // Security attributes
    BOOL macDefaultSecurity;
    BYTE macDefaultSecurityMaterialLength;
    SECURITY_MATERIAL* pMacDefaultSecurityMaterial;
    BYTE macDefaultSecuritySuite;
#endif

#if ((MAC_OPT_SECURITY) || (MAC_OPT_ACL_SIZE>0))
    BYTE macSecurityMode;
#endif

} MAC_PIB;
extern MAC_PIB mpib;

typedef enum {
    MAC_ACK_WAIT_DURATION = 0x40,
    MAC_ASSOCIATION_PERMIT,
    MAC_AUTO_REQUEST,
    MAC_BATT_LIFE_EXT,
    MAC_BATT_LIFE_EXT_PERIODS,
    MAC_BEACON_PAYLOAD,
    MAC_BEACON_PAYLOAD_LENGTH,
    MAC_BEACON_ORDER,
    MAC_BEACON_TX_TIME,
    MAC_BSN,
    MAC_COORD_EXTENDED_ADDRESS,
    MAC_COORD_SHORT_ADDRESS,
    MAC_DSN,
    MAC_GTS_PERMIT,
    MAC_MAX_CSMA_BACKOFFS,
    MAC_MIN_BE,
    MAC_PAN_ID,
    MAC_PROMISCUOUS_MODE,
    MAC_RX_ON_WHEN_IDLE,
    MAC_SHORT_ADDRESS,
    MAC_SUPERFRAME_ORDER,
    MAC_TRANSACTION_PERSISTENCE_TIME,
    MAC_ACL_ENTRY_DESCRIPTOR_SET = 0x70,
    MAC_ACL_ENTRY_DESCRIPTOR_SETSIZE,
    MAC_DEFAULT_SECURITY,
    MAC_DEFAULT_SECURITY_MATERIAL_LENGTH,
    MAC_DEFAULT_SECURITY_MATERIAL,
    MAC_DEFAULT_SECURITY_SUITE,
    MAC_SECURITY_MODE
} MAC_PIB_ATTR;

// PHY PIB
typedef struct {
    UINT8 phyCurrentChannel;
    UINT8 phyTransmitPower;
    UINT8 phyCcaMode;
} PHY_PIB;
extern PHY_PIB ppib;
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// The MAC command state variable
typedef enum {
    MAC_STATE_DEFAULT,
    MAC_STATE_ASSOC_STARTED,
    MAC_STATE_ASSOC_REQ_SENT,
    MAC_STATE_ASSOC_WAIT_FOR_RESPONSE,
    MAC_STATE_ENERGY_SCAN,
    MAC_STATE_ACTIVE_PASSIVE_SCAN,
    MAC_STATE_ORPHAN_SCAN,
    MAC_STATE_ORPHAN_REALIGNED,
    MAC_STATE_POLLING
} MAC_STATE_TYPE;
extern MAC_STATE_TYPE macState;
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// MAC packet types
typedef enum {
    DATA,
    BEACON,
    ACKNOWLEDGE,
    COMMAND 
} MAC_PACKET_TYPE;

typedef enum {
    CMD_ASSOCIATION_REQUEST = 1,
    CMD_ASSOCIATION_RESPONSE,
    CMD_DISASSOCIATION_NOTIFICATION,
    CMD_DATA_REQUEST,
    CMD_PAN_ID_CONFLICT_NOTIFICATION,
    CMD_ORPHAN_NOTIFICATION,
    CMD_BEACON_REQUEST,
    CMD_COORDINATOR_REALIGNMENT,
    CMD_GTS_REQUEST
} MAC_COMMAND_TYPE;
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------

// MAC status values
typedef BYTE MAC_ENUM;

#define SUCCESS                 0
#define BEACON_LOSS             0xE0
#define CHANNEL_ACCESS_FAILURE  0xE1
#define DENIED                  0xE2
#define DISABLE_TRX_FAILURE     0xE3
#define FAILED_SECURITY_CHECK   0xE4
#define FRAME_TOO_LONG          0xE5
#define INVALID_GTS             0xE6
#define INVALID_HANDLE          0xE7
#define INVALID_PARAMETER       0xE8
#define NO_ACK                  0xE9
#define NO_BEACON               0xEA
#define NO_DATA                 0xEB
#define NO_SHORT_ADDRESS        0xEC
#define OUT_OF_CAP              0xED
#define PAN_ID_CONFLICT         0xEE
#define REALIGNMENT             0xEF
#define TRANSACTION_EXPIRED     0xF0
#define TRANSACTION_OVERFLOW    0xF1
#define TX_ACTIVE               0xF2
#define UNAVAILABLE_KEY         0xF3
#define UNSUPPORTED_ATTRIBUTE   0xF4
#define RX_DEFERRED             0xF5

// Association status
#define PAN_AT_CAPACITY 1
#define PAN_ACCESS_DENIED 2
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// Data structures used with the MAC primitives
typedef union {
    WORD  Short;
    QWORD Extended;
} ADDRESS;

typedef struct {
    BYTE    srcAddrMode;
    WORD    srcPanId;
    ADDRESS srcAddr;
    BYTE    dstAddrMode;
    WORD    dstPanId;
    ADDRESS dstAddr;
    UINT8   mpduLinkQuality;
    BOOL    securityUse;
    UINT8   aclEntry;
    INT8    msduLength;
    BYTE    pMsdu[aMaxMACFrameSize];
} MCPS_DATA_INDICATION;

typedef struct {
    UINT8 coordAddrMode;
    WORD coordPanId;
    UINT8 logicalChannel;
    ADDRESS coordAddress;
    WORD superframeSpec;
    BOOL gtsPermit;
    UINT8 linkQuality;
    DWORD timeStamp;
    BOOL securityUse;
    UINT8 aclEntry;
    BOOL securityFailure;
} PAN_DESCRIPTOR;

typedef struct {
    UINT8 bsn;
    PAN_DESCRIPTOR panDescriptor;
    BYTE pendAddrSpec;
    ADDRESS pAddrList[7];
    UINT8 sduLength;
    BYTE pSdu[aMaxBeaconPayloadLength];
} MLME_BEACON_NOTIFY_INDICATION;
extern MLME_BEACON_NOTIFY_INDICATION *pBeaconNotifyIndication;

typedef struct {
    ADDRESS orphanAddress;
    BOOL securityUse;
    UINT8 aclEntry;
} MLME_ORPHAN_INDICATION;

typedef struct {
    WORD panId;
    UINT8 srcAddrMode;
    ADDRESS srcAddr;
    UINT8 dstAddrMode;
    ADDRESS dstAddr;
    MAC_ENUM status;
} MLME_COMM_STATUS_INDICATION;

typedef struct {
    BYTE scanType;
    DWORD unscannedChannels;
    UINT8 resultListSize;
    union{
    UINT8 pEnergyDetectList[16];
    PAN_DESCRIPTOR pPANDescriptorList[MAC_OPT_MAX_PAN_DESCRIPTORS];
    };
} MLME_SCAN_CONFIRM;
extern MLME_SCAN_CONFIRM *pScanConfirm;
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// Address modes - for use with mcpsDataRequest only (all other address mode parameters shall use
// AM_SHORT_16 or AM_EXTENDED_64)
#define SRC_ADDR_BM     0xC0
#define SRC_ADDR_NONE   0x00
#define SRC_ADDR_SHORT  0x80
#define SRC_ADDR_EXT    0xC0

#define DEST_ADDR_BM    0x0C
#define DEST_ADDR_NONE  0x00
#define DEST_ADDR_EXT   0x0C
#define DEST_ADDR_SHORT 0x08

// Internal
#define BOTH_ADDR_USED  0x88
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// TX options - for use with mcpsDataRequest only
#define TX_OPT_SECURITY_ENABLE 0x08
#define TX_OPT_INDIRECT        0x04
#define TX_OPT_GTS             0x02
#define TX_OPT_ACK_REQ         0x01
#define TX_OPT_NONE            0x00
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// MCPS and MLME prototypes (ordered alphabetically)
void mcpsDataRequest(BYTE addrModes, WORD srcPanId, ADDRESS *pSrcAddr, WORD destPanId, ADDRESS *pDestAddr, UINT8 msduLength, BYTE *pMsdu, BYTE msduHandle, BYTE txOptions);
void mcpsDataConfirm(MAC_ENUM status, BYTE msduHandle);
void mcpsDataIndication(void);

MAC_ENUM mcpsPurgeRequest(BYTE msduHandle);

void mlmeAssociateRequest(UINT8 logicalChannel, BYTE coordAddrMode, WORD coordPANId, ADDRESS *pCoordAddress, BYTE capabilityInformation, BOOL securityEnable);
void mlmeAssociateIndication(ADDRESS deviceAddress, BYTE capabilityInformation, BOOL securityUse, BOOL aclEntry);
void mlmeAssociateResponse(ADDRESS *deviceAddress, WORD assocShortAddress, MAC_ENUM status, BOOL securityEnable);
void mlmeAssociateConfirm(WORD AssocShortAddress, BYTE status);

void mlmeBeaconNotifyIndication(void);

void mlmeCommStatusIndication(WORD panId, BYTE srcAddrMode, ADDRESS *pSrcAddr, BYTE dstAddrMode, ADDRESS *pDstAddr, BYTE status);

void mlmeDisassociateRequest(QWORD *pDeviceAddress, BYTE disassociateReason, BOOL securityEnable);
void mlmeDisassociateIndication(QWORD deviceAddress, BYTE disassociateReason, BOOL securityUse, BOOL aclEntry);
void mlmeDisassociateConfirm(MAC_ENUM status);

MAC_ENUM mlmeGetRequest(MAC_PIB_ATTR pibAttribute, void *pPibAttributeValue);

void mlmeOrphanIndication(QWORD orphanAddress, BOOL securityUse, BOOL aclEntry);
void mlmeOrphanResponse(QWORD orphanAddress, WORD shortAddress, BOOL associatedMember, BOOL securityEnable);

void mlmePollRequest(BYTE coordAddrMode, WORD coordPANId, ADDRESS *coordAddress, BOOL securityEnable);
void mlmePollConfirm(MAC_ENUM status);

MAC_ENUM mlmeResetRequest(BOOL setDefaultPIB);

void mlmeRxEnableRequest(BOOL deferPermit, UINT32 rxOnTime, UINT32 rxOnDuration);
void mlmeRxEnableConfirm(MAC_ENUM status);

MAC_ENUM mlmeScanRequest(BYTE scanType, DWORD scanChannels, UINT8 scanDuration);

MAC_ENUM mlmeSetRequest(MAC_PIB_ATTR pibAttribute, void *pPibAttributeValue);

MAC_ENUM mlmeStartRequest(WORD panId, UINT8 logicalChannel, UINT8 beaconOrder, UINT8 superframeOrder, BOOL panCoordinator, BOOL batteryLifeExtension, BOOL coordRealignment, BOOL securityEnable);

void mlmeSyncRequest(UINT8 logicalChannel, BOOL trackBeacon);

void mlmeSyncLossIndication(MAC_ENUM lossReason);
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// Internal support function prototypes

void msupEndReception(void);
void msupBeginReception(void);
void msupSendDataRequest(BYTE coordAddrMode, WORD coordPANId, ADDRESS *pCoordAddress, BYTE securityEnable);
UINT32 msupCalcSuperframeDuration(void);
UINT32 msupCalcBeaconInterval(void);

// Random number generator (TBD: move to HAL?)
void msupInitRandom(void);
BYTE msupGetRandomByte(void);
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// INTERNAL MAC FLAGS
#define SET_MF(n)   intMacFlags |= n
#define CLEAR_MF(n) intMacFlags &= ~n
#define GET_MF(n)   (!!(intMacFlags & n))
#define RESET_MF()  intMacFlags = 0

#define MF_COORDINATOR     0x01
#define MF_PAN_COORDINATOR 0x02
#define MF_BEACON_SECURITY 0x04
#define MF_TRANSMIT_BEACON 0x08

extern BYTE intMacFlags;
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// FRAME CONTROL FIELD

// Indexes
#define FRAME_TYPE_IDX              0
#define SECURITY_ENABLED_IDX        3
#define FRAME_PENDING_IDX           4
#define ACK_REQ_IDX                 5
#define INTRA_PAN_IDX               6
#define DEST_ADDR_MODE_IDX          10
#define SRC_ADDR_MODE_IDX           14

// Bit masks
#define FRAME_TYPE_BM               0x0007
#define SECURITY_ENABLED_BM         0x0008
#define FRAME_PENDING_BM            0x0010
#define ACK_REQ_BM                  0x0020
#define INTRA_PAN_BM                0x0040
#define DEST_ADDR_MODE_BM           0x0C00
#define SRC_ADDR_MODE_BM            0xC000

// FRAME_TYPE
#define FT_BEACON                   0
#define FT_DATA                     1
#define FT_ACKNOWLEDGMENT           2
#define FT_MAC_COMMAND              3
#define FT_100                      4
#define FT_101                      5
#define FT_110                      6
#define FT_111                      7

// ADDR_MODE
#define AM_NONE                     0
#define AM_SHORT_16                 2
#define AM_EXTENDED_64              3
#define AM_EXTENDED_64_BM           (AM_EXTENDED_64 << DEST_ADDR_MODE_IDX)
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// SCAN TYPES
#define ENERGY_SCAN                 0
#define ACTIVE_SCAN                 1
#define PASSIVE_SCAN                2
#define ORPHAN_SCAN                 3
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// DISASSOCIATE REASONS
#define COORD_WISHES_DEVICE_TO_LEAVE 1
#define DEVICE_WISHES_TO_LEAVE       2
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// SUPERFRAME SPECIFICATION FIELD (BEACON)

// Indexes
#define SS_BEACON_ORDER_IDX         0
#define SS_SUPERFRAME_ORDER_IDX     4
#define SS_FINAL_CAP_SLOT_IDX       8
#define SS_BATT_LIFE_EXT_IDX        12
#define SS_PAN_COORDINATOR_IDX      14
#define SS_ASSOCIATION_PERMIT_IDX   15

// Bit masks
#define SS_BEACON_ORDER_BM          0x000F
#define SS_SUPERFRAME_ORDER_BM      0x00F0
#define SS_FINAL_CAP_SLOT_BM        0x0F00
#define SS_BATT_LIFE_EXT_BM         0x1000
#define SS_PAN_COORDINATOR_BM       0x4000
#define SS_ASSOCIATION_PERMIT_BM    0x8000
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// CAPABILITY INFORMATION FIELD (ASSOCIATION REQUEST)

// Indexes
#define CI_ALTERNATE_PAN_COORD_IDX  0
#define CI_DEVICE_TYPE_IS_FFD_IDX   1
#define CI_POWER_SOURCE_IDX         2
#define CI_RX_ON_WHEN_IDLE_IDX      3
#define CI_SECURITY_CAPABILITY_IDX  6
#define CI_ALLOCATE_ADDRESS_IDX     7

// Bit masks
#define CI_ALTERNATE_PAN_COORD_BM   0x01
#define CI_DEVICE_TYPE_IS_FFD_BM    0x02
#define CI_POWER_SOURCE_BM          0x04
#define CI_RX_ON_WHEN_IDLE_BM       0x08
#define CI_SECURITY_CAPABILITY_BM   0x40
#define CI_ALLOCATE_ADDRESS_BM      0x80
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
// Length of Frame Check Sequence (FCS) field (#bytes)
#define MAC_FCS_LENGTH 2

//----------------------------------------------------------------------------------------------------------
// Some parameters from the superframe specification in the last beacon
typedef struct {
    UINT8 finalCap;
    volatile BOOL battLifeExt;
} MAC_SUPERFRAME_SPEC;
extern volatile MAC_SUPERFRAME_SPEC sfSpec;
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// Internal structures used for packet reception and transmission

// Packet TX structure
typedef struct {
    volatile BOOL active;
    UINT8 headerLength;              // Actual length(pHeader)
    UINT8 length;                    // Total packet length
    BYTE pHeader[23+5*MAC_OPT_SECURITY];    // Frame control field (2) | Sequence number (1) | Addressing fields (0 to 20) | Optional security (5 bytes)
    BYTE pPayload[aMaxMACFrameSize];
    BYTE type;                       // (FT_DATA / FT_MAC_COMMAND)
    BYTE txOptions;
    BYTE extraByte;                  // MSDU handle
    BOOL toCoord;                    // pHeader->destination = coordinator
    UINT8 duration;                  // The number of backoff slots required to transmit this packet
#if MAC_OPT_SECURITY
    // Security material, if used
    BYTE securityMode;
    BYTE micLength;
    SECURITY_MATERIAL *pSecurityMaterial;
#endif
} MAC_PACKET;
extern MAC_PACKET directPacket;

// Extra wrapper for indirect packets
typedef struct {
    MAC_PACKET packet;
    UINT16 timeToLive;
    BOOL isRequested;
    UINT8 nextPacket;
    UINT8 prevPacket;
    BOOL isFirstPacket; // Is this the first packet in the queue for this node?
} MAC_INDIRECT_PACKET;
extern MAC_INDIRECT_PACKET pIndirectPackets[MAC_OPT_MAX_INDIRECT_PACKETS];

// Info related to packet transmission
typedef struct {
    // A pointer to the active packet
    MAC_PACKET *pPacket;
    
    // CSMA-CA "state machine"
    UINT8 cw;
    UINT8 be;
    UINT8 nb;
    
    // CSMA-CA status
    BOOL csmaSuccess;
    volatile BOOL csmaActive;
    volatile BOOL csmaWaitForBeacon;
    volatile INT8 csmaRandomBackoff;
    
    // The number of retries left
    UINT8 retryDowncount;
} MAC_TX_INFO;
extern volatile MAC_TX_INFO txInfo;

#define RX_STRUCTURE MCPS_DATA_INDICATION

// Info related to packet reception
typedef struct {
    // Reception "state machine"
    BOOL endReception;
    volatile BOOL parsingError;
    
    // Packet data
    UINT8 length;
    RX_STRUCTURE *pMDI;
    WORD frameControl;
    UINT8 sequenceNumber;
    BYTE pFooter[2];
    
    // The length of the last beacon (BOSs)
    UINT8 beaconDuration;
        
    // RX enable related (TBD)
    UINT32 onDurationBos;
    void *pExtraPtr;
    
    // RX on state (TBD):
    //     On: if (old onCounter == 0) turn RX on
    //     Off: if (old onCounter == 1) turn RX off
    volatile INT8 onCounter;
    BOOL onCounterOverride;
    
} MAC_RX_INFO;
extern volatile MAC_RX_INFO rxInfo;


typedef struct {
    DWORD frameCounter;
    BYTE keySequenceCounter;
    BYTE micLength;
    MAC_ENUM securityStatus;
    BYTE securityMode;
} MAC_RX_SECURITY_INFO;

//----------------------------------------------------------------------------------------------------------


void msupRxOn(void);
void msupRxOnAtBoundary(void);
void msupAutoRxOn(void);
void msupRxOff(void);
void msupRxOffIndirectData(void);
void msupRxOffBeacon(void);
UINT8 msupGetBeaconRxMargin(void);

#endif
