/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  ADLATUS_HALflash.h                                               |
|                                                                              |
|  Comment:   Header File for the HIS conform Flashdriver.                     |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   CW         Christian Wehebrink       SMART Electronic Development GmbH     |
|   MB         Michael Becker            SMART Electronic Development GmbH     |
|   JS	       Jürgen Schade             SMART Electronic Development GmbH     |
|   SN	       Sven Neuer 		 SMART Electronic Development GmbH     |                                                                      |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_HISflash.h 4429 2008-11-13 16:01:25Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_HISflash.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __HISFF_
#define __HISFF_
/* ========================================================================== */

/* ========================================================================== */
/*                                                                            */
/*                                                                            */
/*          Variables for the HIS flash driver                                */
/*       -----------------------------------------------------------          */
/*         ( Functional Specification of a Flash Driver Version 1.3)          */
/*                                                                            */
/* ========================================================================== */
typedef unsigned char tMinorNumber;    /* Flash driver major version number   */
typedef unsigned char tMajorNumber;    /* Flash driver minor version number   */
typedef unsigned char tBugfixVersion;  /* Bugfix patchlevel                   */
typedef unsigned short tFlashResult;   /* result of flash driver routines     */
typedef unsigned long tAddress;        /* logical address                     */
typedef unsigned long tLength;         /* length in bytes                     */
typedef unsigned char tData;           /* ambiguous data                      */
typedef void (*tWDTriggerFct) (void);  /* watchdog trigger routine            */
/* vendor defines */
typedef unsigned char tCommand;        /* command byte */
typedef unsigned char tDirection;      /* direction byte */
typedef unsigned char tFormat;         /* format byte */
typedef unsigned char tReserve;        /* alignment byte */
typedef unsigned short tSector;        /* sector type definition word */
typedef unsigned char tMilTime;         /* format byte */
typedef unsigned long t_Delaytime;

typedef struct
{
   /* initialisation : in-out Parameters  */
   tBugfixVersion patchlevel;          /* flash driver patch level version     */
   tMinorNumber minornumber;           /* Flash driver minor version number    */
   tMajorNumber majornumber;           /* Flash driver major version number    */
   unsigned char reserved1;            /* reserved for future use, set to 0x00 */

   /* return value  */
   tFlashResult errorcode;
   unsigned short reserved2;

   /* erase / write . input parameters  */
   tAddress address;                   /* logical target address              */
   tLength length;                     /* length information in bytes         */
   tData *data;                        /* pointer to data buffer              */

   /* additional input parameters */
   tWDTriggerFct wdTriggerFct;         /* pointer to watchdog trigger routine */

   /* erase/write : optional output parameters : debugging information        */
   tData intendedData[2];              /* intented data at error address      */
   tData actualData[2];                /* actual data at error address        */
   tAddress errorAddress;              /* address of error                    */

   tCommand Command_UB;               /* command byte */
   tDirection Direction_UB;           /* direction byte */
   tFormat Format_UB;                 /* format byte */
   tMilTime Millisec_UB;               /* alignment byte */

   tSector StartSector_US;       /* command byte */
   tSector EndSector_US;         /* command byte */
   tSector NbrOfSectors_US;     /* command byte */
   tSector ActualSector_US;     /* command byte */

   t_Delaytime Delaytime_UL;
} tFlashParam;


typedef void (*tFlashFct) (tFlashParam*);

typedef struct
{
    UBYTE ubInterfaceVersion;
    UBYTE ubReserverd;
    UBYTE ubMask;
    UBYTE ubMCU;
    tFlashFct pfnFlashInit;
    tFlashFct pfnFlashDeinit;
    tFlashFct pfnFlashErase;
    tFlashFct pfnFlashWrite;
} tHis_Header;

/* ========================================================================== */
/*      external variables and constants                                      */
/* ========================================================================== */
/* In ADLATUS_FUN_CChal_HISCheck.c */
extern tFlashParam FlashParam;
UBYTE FUN_CChal_HISDriverCheck_UB ( tHis_Header *);

extern const ULONG C_FlashDelay_UL;
/* ========================================================================== */
/*      Function Prototypes                                                   */
/* ========================================================================== */
void FlashInit (tFlashParam *flashParam);        /* initialization function   */
void FlashDeinit (tFlashParam *flashParam);      /* deinitialization function */
void FlashErase (tFlashParam *flashParam); 	 /* Flash erase function      */
void FlashWrite (tFlashParam *flashParam); 	 /* Flash write function      */

void PARROT_Init (tFlashParam *flashParam);        /* initialization function   */
void PARROT_Deinit (tFlashParam *flashParam);      /* deinitialization function */
void PARROT_Erase (tFlashParam *flashParam); 	 /* Flash erase function      */
void PARROT_Write (tFlashParam *flashParam); 	 /* Flash write function      */

/* ========================================================================== */
/*      Defines                                                               */
/* ========================================================================== */
#define FLASH_DRIVER_INIT_OFFSET      0x0004u
#define FLASH_DRIVER_DEINIT_OFFSET    0x0008u
#define FLASH_DRIVER_ERASE_OFFSET     0x000Cu
#define FLASH_DRIVER_WRITE_OFFSET     0x0010u

/* sa holds the flash driver Startaddress, pp contains pointer to the para. Struct */
#define FLASH_DRIVER_INIT(sa,pp)      ((tFlashFct)&((sa)+FLASH_DRIVER_INIT_OFFSET)(pp))
#define FLASH_DRIVER_DEINIT(sa,pp)    ((tFlashFct)&((sa)+FLASH_DRIVER_DEINIT_OFFSET)(pp))
#define FLASH_DRIVER_ERASE(sa,pp)     ((tFlashFct)&((sa)+FLASH_DRIVER_ERASE_OFFSET)(pp))
#define FLASH_DRIVER_WRITE(sa,pp)     ((tFlashFct)&((sa)+FLASH_DRIVER_WRITE_OFFSET)(pp))

#define FLASH_DRIVER_MCUTYPE(sa)      ((sa)+0u)
#define FLASH_DRIVER_MASKTYPE(sa)     ((sa)+1u)
#define FLASH_DRIVER_INTERFACE(sa)    ((sa)+3u)

/* header identification for internal TMS470R FlashFunctions */
#define FLASH_DRIVER_VERSION_MCUTYPE   (UBYTE)  0x01u
#define FLASH_DRIVER_VERSION_MASKTYPE  (UBYTE)  0x81u
#define FLASH_DRIVER_VERSION_INTERFACE (UBYTE)  0x01u

/* header identification for PARROT FlashFunctions */
#define FLASH_DRIVER_VERSION_MCUTYPE_PARROT (UBYTE) 0x01u
#define FLASH_DRIVER_VERSION_MASKTYPE_PARROT (UBYTE) 0x81u
#define FLASH_DRIVER_VERSION_INTERFACE_PARROT (UBYTE) 0x02u

#define FLASH_DRIVER_VERSION_MAJOR     (UBYTE)  0x01u
#define FLASH_DRIVER_VERSION_MINOR     (UBYTE)  0x30u
#define FLASH_DRIVER_VERSION_PATCH     (UBYTE)  0x00u

/* Error code internal flash programming */
#define kFlashOK                          0x0000u
#define kFlashFailed                      0x0001u
#define kAddressOutOfRange                0x0002u
#define kWrongAddressAlignment            0x0004u
#define kWrongLengthAlignment             0x0008u
#define kVerificationError                0x0010u
#define kFlashEraseFailed                 0x0020u
#define kFlashEraseBusy                   0x0040u
#define kFlashSecured                     0x0080u
#define kProtectionViolation              0x0100u
#define kAccessError                      0x0101u

/* error code for PARROT update */
#define d_CCpar_FlashOK                   0x0000u
#define d_CCpar_VerificationError         0x0001u
#define d_CCpar_NotEnoughData             0x0002u
#define d_CCpar_FlashFailed               0x0004u
#define d_CCpar_DeviceOutOfRange          0x0008u
#define d_CCpar_MemoryOverflow            0x0010u
#define d_CCpar_ToMuchData                0x0020u
#define d_CCpar_AccessError               0x0040u
#define d_CCpar_ProtectionViolation       0x0080u
#define d_CCpar_DefaultError              0x00FFu

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
