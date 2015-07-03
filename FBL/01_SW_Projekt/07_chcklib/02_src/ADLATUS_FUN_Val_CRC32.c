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
|  Filename:  ADLATUS_FUN_Val_CRC32.c                                          |
|                                                                              |
|  Comment:   This file includes a checksum calculations functions.            |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_FUN_Val_CRC32.c 4408 2008-11-06 17:15:01Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/07_chcklib/02_src/ADLATUS_FUN_Val_CRC32.c $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      G L O B A L   C O M P I L E R   S W I T C H E S                       */
/* ========================================================================== */
/* DONT REMOVE: This file has to be included to all ADLATUS project files     */
/*               and has to be the first include!                             */
#include "ADLATUS_CompilerSwitches_cdf.h"
/* = END:DONT REMOVE ======================================================== */

/* ========================================================================== */
/* (1)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      C O N S T A N T   D E F I N I T I O N   F I L E S  (CDF)              */
/* ========================================================================== */
#include "ADLATUS_Global_cdf.h"              /* global defines                */
#include "ADLATUS_Hal_cdf.h"
#include "ADLATUS_ValHandler_cdf.h"

/* ========================================================================== */
/* (2)  I N C L U D E S                                                       */
/* -------------------------------------------------------------------------- */
/*      T Y P E   D E F I N I T I O N   F I L E S  (TDF)                      */
/* ========================================================================== */
#include "ADLATUS_Types_tdf.h"               /* must be the first TDF include */
#include "ADLATUS_Hal_tdf.h"
#include "ADLATUS_ValHandler_tdf.h"

/* ========================================================================== */
/* (3)  C O M P I L E R   S W I T C H   S E T T I N G S                       */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* Global Compiler switch settings are done at ADLATUS_CompilerSwitches_CDF.h */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* Local compiler switches: No Entry                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/* ========================================================================== */
/* (4)  D A T A   S E C T I O N   D I R E C T I V E S                         */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (5)  L O C A L   D E F I N E S                                             */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
#define  d_CCval_StopScan                       (UBYTE) 0x00u
#define  d_CCval_ScanRunning                    (UBYTE) 0x01u

/*#define d_CCval_Crc32TabInRAM                       1u*/
#define d_CCval_Crc32TabSize                      256u

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


/* ========================================================================== */
/* (6)  L O C A L  T Y P E   D E F I N E S                                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (7)  L O C A L   V A R I A B L E S  / C O N S T A N T S                    */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */


/*----------------------------------------------------------------------------*/
/* CRC32 coding table                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* In order we have to switch Flash Pages where the constant data is stored,  */
/* we allocate some RAM memory to copy the data before swithing pages.        */
/* This saves startup time compared to using a pre declared RAM variable,     */
/* which is initialized by a copy sequence in the startup code.               */
/*----------------------------------------------------------------------------*/

#ifdef d_CCval_Crc32TabInRAM
/*----------------------------------------------------------------------------*/
/* We put data in RAM during runtime                                          */
/*----------------------------------------------------------------------------*/
ULONG crc32Tab[d_CCval_Crc32TabSize];
const ULONG crc32RomTab[d_CCval_Crc32TabSize] =
#endif

#ifndef d_CCval_Crc32TabInRAM
/*----------------------------------------------------------------------------*/
/* We leave data in ROM                                                       */
/*----------------------------------------------------------------------------*/
const ULONG crc32Tab[d_CCval_Crc32TabSize] =
#endif

{
 /*   0 */     0x00000000ul, 0x77073096ul, 0xee0e612cul, 0x990951baul,
 /*   4 */     0x076dc419ul, 0x706af48ful, 0xe963a535ul, 0x9e6495a3ul,
 /*   8 */     0x0edb8832ul, 0x79dcb8a4ul, 0xe0d5e91eul, 0x97d2d988ul,
 /*  12 */     0x09b64c2bul, 0x7eb17cbdul, 0xe7b82d07ul, 0x90bf1d91ul,
 /*  16 */     0x1db71064ul, 0x6ab020f2ul, 0xf3b97148ul, 0x84be41deul,
 /*  20 */     0x1adad47dul, 0x6ddde4ebul, 0xf4d4b551ul, 0x83d385c7ul,
 /*  24 */     0x136c9856ul, 0x646ba8c0ul, 0xfd62f97aul, 0x8a65c9ecul,
 /*  28 */     0x14015c4ful, 0x63066cd9ul, 0xfa0f3d63ul, 0x8d080df5ul,
 /*  32 */     0x3b6e20c8ul, 0x4c69105eul, 0xd56041e4ul, 0xa2677172ul,
 /*  36 */     0x3c03e4d1ul, 0x4b04d447ul, 0xd20d85fdul, 0xa50ab56bul,
 /*  40 */     0x35b5a8faul, 0x42b2986cul, 0xdbbbc9d6ul, 0xacbcf940ul,
 /*  44 */     0x32d86ce3ul, 0x45df5c75ul, 0xdcd60dcful, 0xabd13d59ul,
 /*  48 */     0x26d930acul, 0x51de003aul, 0xc8d75180ul, 0xbfd06116ul,
 /*  52 */     0x21b4f4b5ul, 0x56b3c423ul, 0xcfba9599ul, 0xb8bda50ful,
 /*  56 */     0x2802b89eul, 0x5f058808ul, 0xc60cd9b2ul, 0xb10be924ul,
 /*  60 */     0x2f6f7c87ul, 0x58684c11ul, 0xc1611dabul, 0xb6662d3dul,
 /*  64 */     0x76dc4190ul, 0x01db7106ul, 0x98d220bcul, 0xefd5102aul,
 /*  68 */     0x71b18589ul, 0x06b6b51ful, 0x9fbfe4a5ul, 0xe8b8d433ul,
 /*  72 */     0x7807c9a2ul, 0x0f00f934ul, 0x9609a88eul, 0xe10e9818ul,
 /*  76 */     0x7f6a0dbbul, 0x086d3d2dul, 0x91646c97ul, 0xe6635c01ul,
 /*  80 */     0x6b6b51f4ul, 0x1c6c6162ul, 0x856530d8ul, 0xf262004eul,
 /*  84 */     0x6c0695edul, 0x1b01a57bul, 0x8208f4c1ul, 0xf50fc457ul,
 /*  88 */     0x65b0d9c6ul, 0x12b7e950ul, 0x8bbeb8eaul, 0xfcb9887cul,
 /*  92 */     0x62dd1ddful, 0x15da2d49ul, 0x8cd37cf3ul, 0xfbd44c65ul,
 /*  96 */     0x4db26158ul, 0x3ab551ceul, 0xa3bc0074ul, 0xd4bb30e2ul,
 /* 100 */     0x4adfa541ul, 0x3dd895d7ul, 0xa4d1c46dul, 0xd3d6f4fbul,
 /* 104 */     0x4369e96aul, 0x346ed9fcul, 0xad678846ul, 0xda60b8d0ul,
 /* 108 */     0x44042d73ul, 0x33031de5ul, 0xaa0a4c5ful, 0xdd0d7cc9ul,
 /* 112 */     0x5005713cul, 0x270241aaul, 0xbe0b1010ul, 0xc90c2086ul,
 /* 116 */     0x5768b525ul, 0x206f85b3ul, 0xb966d409ul, 0xce61e49ful,
 /* 120 */     0x5edef90eul, 0x29d9c998ul, 0xb0d09822ul, 0xc7d7a8b4ul,
 /* 124 */     0x59b33d17ul, 0x2eb40d81ul, 0xb7bd5c3bul, 0xc0ba6cadul,
 /* 128 */     0xedb88320ul, 0x9abfb3b6ul, 0x03b6e20cul, 0x74b1d29aul,
 /* 132 */     0xead54739ul, 0x9dd277aful, 0x04db2615ul, 0x73dc1683ul,
 /* 136 */     0xe3630b12ul, 0x94643b84ul, 0x0d6d6a3eul, 0x7a6a5aa8ul,
 /* 140 */     0xe40ecf0bul, 0x9309ff9dul, 0x0a00ae27ul, 0x7d079eb1ul,
 /* 144 */     0xf00f9344ul, 0x8708a3d2ul, 0x1e01f268ul, 0x6906c2feul,
 /* 148 */     0xf762575dul, 0x806567cbul, 0x196c3671ul, 0x6e6b06e7ul,
 /* 152 */     0xfed41b76ul, 0x89d32be0ul, 0x10da7a5aul, 0x67dd4accul,
 /* 156 */     0xf9b9df6ful, 0x8ebeeff9ul, 0x17b7be43ul, 0x60b08ed5ul,
 /* 160 */     0xd6d6a3e8ul, 0xa1d1937eul, 0x38d8c2c4ul, 0x4fdff252ul,
 /* 164 */     0xd1bb67f1ul, 0xa6bc5767ul, 0x3fb506ddul, 0x48b2364bul,
 /* 168 */     0xd80d2bdaul, 0xaf0a1b4cul, 0x36034af6ul, 0x41047a60ul,
 /* 172 */     0xdf60efc3ul, 0xa867df55ul, 0x316e8eeful, 0x4669be79ul,
 /* 176 */     0xcb61b38cul, 0xbc66831aul, 0x256fd2a0ul, 0x5268e236ul,
 /* 180 */     0xcc0c7795ul, 0xbb0b4703ul, 0x220216b9ul, 0x5505262ful,
 /* 184 */     0xc5ba3bbeul, 0xb2bd0b28ul, 0x2bb45a92ul, 0x5cb36a04ul,
 /* 188 */     0xc2d7ffa7ul, 0xb5d0cf31ul, 0x2cd99e8bul, 0x5bdeae1dul,
 /* 192 */     0x9b64c2b0ul, 0xec63f226ul, 0x756aa39cul, 0x026d930aul,
 /* 196 */     0x9c0906a9ul, 0xeb0e363ful, 0x72076785ul, 0x05005713ul,
 /* 200 */     0x95bf4a82ul, 0xe2b87a14ul, 0x7bb12baeul, 0x0cb61b38ul,
 /* 204 */     0x92d28e9bul, 0xe5d5be0dul, 0x7cdcefb7ul, 0x0bdbdf21ul,
 /* 208 */     0x86d3d2d4ul, 0xf1d4e242ul, 0x68ddb3f8ul, 0x1fda836eul,
 /* 212 */     0x81be16cdul, 0xf6b9265bul, 0x6fb077e1ul, 0x18b74777ul,
 /* 216 */     0x88085ae6ul, 0xff0f6a70ul, 0x66063bcaul, 0x11010b5cul,
 /* 220 */     0x8f659efful, 0xf862ae69ul, 0x616bffd3ul, 0x166ccf45ul,
 /* 224 */     0xa00ae278ul, 0xd70dd2eeul, 0x4e048354ul, 0x3903b3c2ul,
 /* 228 */     0xa7672661ul, 0xd06016f7ul, 0x4969474dul, 0x3e6e77dbul,
 /* 232 */     0xaed16a4aul, 0xd9d65adcul, 0x40df0b66ul, 0x37d83bf0ul,
 /* 236 */     0xa9bcae53ul, 0xdebb9ec5ul, 0x47b2cf7ful, 0x30b5ffe9ul,
 /* 240 */     0xbdbdf21cul, 0xcabac28aul, 0x53b39330ul, 0x24b4a3a6ul,
 /* 244 */     0xbad03605ul, 0xcdd70693ul, 0x54de5729ul, 0x23d967bful,
 /* 248 */     0xb3667a2eul, 0xc4614ab8ul, 0x5d681b02ul, 0x2a6f2b94ul,
 /* 252 */     0xb40bbe37ul, 0xc30c8ea1ul, 0x5a05df1bul, 0x2d02ef8dul
};


/* ========================================================================== */
/* (8)  L O C A L   F U N C T I O N   P R O T O T Y P E S                     */
/* -------------------------------------------------------------------------- */
/*      O N L Y   F O R   T H I S   M O D U L                                 */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (9)  G L O B A L   V A R I A B L E S                                       */
/* -------------------------------------------------------------------------- */
/*      D E F I N E D   W I T H I N   T H I S   M O D U L                     */
/* ========================================================================== */
/* NO ENTRY                                                                   */

/* ========================================================================== */
/* (10) F U N C T I O N   D E F I N I T I O N S                               */
/* -------------------------------------------------------------------------- */
/*      F O R   T H I S   M O D U L                                           */
/* ========================================================================== */

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCval_CRC32_UB                                          |
|                                                                              |
|  Prototype at:   ADLATUS_Validation_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates a checksum by using the            |
|                  algorithm of CCITT.                                         |
|                                                                              |
|  Parameter:      ULONG t_StartAddress_UL    .. calculation start address     |
|                  ULONG t_StopAddress_UL     .. calculation stop address      |
|                  ULONG t_ChecksumAddress_UL .. checksum address              |
|                                                                              |
|  Return Value:   UBYTE t_Result_UB .. 0x00 - No Error                        |
|                                       0x01 - Checksum Error                  |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCval_CRC32_UB (VALCONTROL_ST  *Parameters )
{
  UBYTE   t_Result_UB;           /* Result of check                 */

  UBYTE   ucData;                /* Current data byte               */

  ULONG   t_TempAddress_UL;       /* temp address */
  
  ADL_HAL_VALIDATION_INIT
  ADL_HAL_VALIDATION_ENTRY( Parameters->StartAddress_PBUF )
  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/
  t_Result_UB            = d_CCVhnd_Ready;
  t_TempAddress_UL       = (ULONG)(Parameters->StartAddress_PBUF);  /*l_int !e923 */
  
#ifdef d_CCval_Crc32TabInRAM
  /*--------------------------------------------------------------------------*/
  /* This is initializing the CRC-tab at runtime to minimize overhead.        */
  /* We only have to do this once, so we check if its already done.           */
  /*--------------------------------------------------------------------------*/
  if(!((crc32Tab[0x01] == crc32RomTab[0x01]) && \
    (crc32Tab[d_CCval_Crc32TabSize / 2] == crc32RomTab[d_CCval_Crc32TabSize / 2]) && \
    (crc32Tab[d_CCval_Crc32TabSize-0x01] == crc32RomTab[d_CCval_Crc32TabSize-0x01]))) 
  {
    FUN_CCglo_PMemCopy_V( \
      (VHND_PTR_BUF_UBYTE)&crc32Tab[0x00],\
      (VHND_PTR_BUF_UBYTE)&crc32RomTab[0x00],\
      (ULONG) d_CCval_Crc32TabSize);
  }
#endif

  /*==========================================================================*/
  /* -1- check result                                                         */
  /*==========================================================================*/
  if ( d_CCVhnd_CheckForResult == (d_CCVhnd_CheckForResult & Parameters->Control_UB) )
  {
    if ( Parameters->WorkingBuffer_UL != *((VHND_PTR_BUF_ULONG)(Parameters->ChecksumAddress_PBUF)))
    {
      t_Result_UB = (d_CCVhnd_CheckNOK | d_CCVhnd_Ready);
    }
    else 
    {
      t_Result_UB = (d_CCVhnd_CheckOK | d_CCVhnd_Ready);
    }
  }
  else 
  {
    /*==========================================================================*/
    /* -1- Calculate Checksum                                                   */
    /*==========================================================================*/
    while ((((VHND_PTR_BUF_UBYTE)t_TempAddress_UL) <=  Parameters->EndAddress_PBUF) )  /*l_int !e923 !e946 */
    {
      /*------------------------------------------------------------------------*/
      /* Calculate Checksum                                                     */
      /*------------------------------------------------------------------------*/
      ucData = *((VHND_PTR_BUF_UBYTE)(t_TempAddress_UL));  /*l_int !e923 */      
      
      Parameters->WorkingBuffer_UL = \
        ( Parameters->WorkingBuffer_UL >> 8ul )^ \
          crc32Tab[ ( (Parameters->WorkingBuffer_UL ^ (ULONG)ucData ) & 0xFFul)];
      
      /* Increment Address and counter                                          */
      t_TempAddress_UL++;
      
    } /* ---1- END OF while (..) -----------------------------------------------*/
    t_Result_UB = (d_CCVhnd_Ready);
  }
  
  ADL_HAL_VALIDATION_EXIT( Parameters->StartAddress_PBUF )
  return ( t_Result_UB );
} /*-+- END OF FUNCTION 'FUN_CCval_CRC32_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/*-----------------------------------------------------------------------------+
|    F U N C T I O N   I N F O R M A T I O N                                   |
+------------------------------------------------------------------------------+
|                                                                              |
|  Function Name:  FUN_CCval_CRC32_onFly_UB                                          |
|                                                                              |
|  Prototype at:   ADLATUS_Validation_tdf.h                                    |
|                                                                              |
|  Description:    This function calculates a checksum by using the            |
|                  algorithm of CCITT.                                         |
|                                                                              |
|  Parameter:      ULONG t_StartAddress_UL    .. calculation start address     |
|                  ULONG t_StopAddress_UL     .. calculation stop address      |
|                  ULONG t_ChecksumAddress_UL .. checksum address              |
|                                                                              |
|  Return Value:   UBYTE t_Result_UB .. 0x00 - No Error                        |
|                                       0x01 - Checksum Error                  |
|                                                                              |
+-----------------------------------------------------------------------------*/
UBYTE FUN_CCval_CRC32_onFly_UB (VALCONTROL_ST  *Parameters, ULONG * t_crc_adress_PUL )
{
  UBYTE   t_Result_UB;           /* Result of check                 */

  UBYTE   ucData;                /* Current data byte               */

  ULONG   t_TempAddress_UL;       /* temp address */
  
  ADL_HAL_VALIDATION_INIT
  ADL_HAL_VALIDATION_ENTRY( Parameters->StartAddress_PBUF )
  /*--------------------------------------------------------------------------*/
  /* Init the calculation                                                     */
  /*--------------------------------------------------------------------------*/
  t_Result_UB            = d_CCVhnd_Ready;
  t_TempAddress_UL       = (ULONG)(Parameters->StartAddress_PBUF);  /*l_int !e923 */
  
#ifdef d_CCval_Crc32TabInRAM
  /*--------------------------------------------------------------------------*/
  /* This is initializing the CRC-tab at runtime to minimize overhead.        */
  /* We only have to do this once, so we check if its already done.           */
  /*--------------------------------------------------------------------------*/
  if(!((crc32Tab[0x01] == crc32RomTab[0x01]) && \
    (crc32Tab[d_CCval_Crc32TabSize / 2] == crc32RomTab[d_CCval_Crc32TabSize / 2]) && \
    (crc32Tab[d_CCval_Crc32TabSize-0x01] == crc32RomTab[d_CCval_Crc32TabSize-0x01]))) 
  {
    FUN_CCglo_PMemCopy_V( \
      (VHND_PTR_BUF_UBYTE)&crc32Tab[0x00],\
      (VHND_PTR_BUF_UBYTE)&crc32RomTab[0x00],\
      (ULONG) d_CCval_Crc32TabSize);
  }
#endif


  /*==========================================================================*/
  /* -1- Calculate Checksum                                                   */
  /*==========================================================================*/
  while ((((VHND_PTR_BUF_UBYTE)t_TempAddress_UL) <=  Parameters->EndAddress_PBUF) )  /*l_int !e923 !e946 */
  {
    /*------------------------------------------------------------------------*/
    /* Calculate Checksum                                                     */
    /*------------------------------------------------------------------------*/
    ucData = *((VHND_PTR_BUF_UBYTE)(t_TempAddress_UL));  /*l_int !e923 */      
    
    *(t_crc_adress_PUL) = \
      ( *(t_crc_adress_PUL) >> 8ul )^ \
        crc32Tab[ ( (*(t_crc_adress_PUL) ^ (ULONG)ucData ) & 0xFFul)];
    
    /* Increment Address and counter                                          */
    t_TempAddress_UL++;
    
  } /* ---1- END OF while (..) -----------------------------------------------*/
  t_Result_UB = (d_CCVhnd_Ready);

  
  ADL_HAL_VALIDATION_EXIT( Parameters->StartAddress_PBUF )
  return ( t_Result_UB );
} /*-+- END OF FUNCTION 'FUN_CCval_CRC32_UB' -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
