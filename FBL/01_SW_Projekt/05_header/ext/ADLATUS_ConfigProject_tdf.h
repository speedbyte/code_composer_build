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
|  Filename:  ADLATUS_ConfigProject_tdf.h                                      |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the current SMART ADLATUS project configuration.                 |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ConfigProject_tdf.h 4464 2008-11-24 14:54:35Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_ConfigProject_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_CONFIGPROJECT_
#define __TDF_CONFIGPROJECT_
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*+--------------------------------------------------------------------------+*/
/*| Common SW-Timer                                                          |*/
/*+--------------------------------------------------------------------------+*/

/*----------------------------------------------------------------------------*/
/* System Entry Time                                                          */
/*----------------------------------------------------------------------------*/
extern const UWORD c_CCconprj_IntWdtTimeout_UW;

/*+--------------------------------------------------------------------------+*/
/*| END OF Common SW-Timer                                                   |*/
/*+--------------------------------------------------------------------------+*/
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
/*+--------------------------------------------------------------------------+*/
/*| VW-specific SW-Timer                                                     |*/
/*+--------------------------------------------------------------------------+*/

/*----------------------------------------------------------------------------*/
/* timeout after successful programming or communication break                */
/*----------------------------------------------------------------------------*/
extern const TIMER_16 c_CCconprj_StopCommResetTime_T16;

extern const TIMER_16 c_CCconprj_EraseResetTime_T16;

#ifdef cs_CCTP20
  extern const TIMER_16 c_CCconprj_StopCommResetTime_T16;
#endif

/*----------------------------------------------------------------------------*/
/* time in ms for a running timelock after an unsuccessful seed&key attempt   */
/*----------------------------------------------------------------------------*/
extern const TIMER_16 c_CCconprj_Timelock_T16;

/*+--------------------------------------------------------------------------+*/
/*| END OF VW-specific SW-Timer                                              |*/
/*+--------------------------------------------------------------------------+*/
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


/*----------------------------------------------------------------------------*/
/* Description:   Default CAN Baudrate                                        */
/*----------------------------------------------------------------------------*/
extern const UWORD c_CCconprj_DefaultCanBaudrate_UW;

/*----------------------------------------------------------------------------*/
/* Description:   Can Controller Selection                                    */
/*----------------------------------------------------------------------------*/
extern const UWORD c_CCconprj_CanController_UW;

#ifdef cs_CChal_variable_CANID
/*----------------------------------------------------------------------------*/
/* Description:   CAN additional RX and TX Ids                                */
/*----------------------------------------------------------------------------*/
extern ULONG   c_CCconprj_CanAdditionalRxIds[];
extern ULONG   c_CCconprj_CanAdditionalTxIds[];
#endif
/*----------------------------------------------------------------------------*/
/* Description:   CAN Message Object Configuration Table                      */
/*----------------------------------------------------------------------------*/
extern CANMSGOBJCONFIG_ST   c_CCconprj_CanMsgObjConfig_AST[];

/*----------------------------------------------------------------------------*/
/* Description:   CAN Channel Configuration Table                             */
/*----------------------------------------------------------------------------*/
extern const CANMSGOBJINFO_ST     c_CCconprj_CanChannelTable_AST[];

#ifdef cs_CCUDS_Audi

/*----------------------------------------------------------------------------*/
/* Description:   CAN Address Mode Configuration                              */
/*----------------------------------------------------------------------------*/
extern const ADDRESSMODECONFIG_ST c_CCconprj_AddrModeCfg_AST[];

/*----------------------------------------------------------------------------*/
/* Description:   Timing definitions for Diagnostic Session Control           */
/*----------------------------------------------------------------------------*/
extern const DSCTIMING_ST c_CCconprj_DscServerTiming_ST;

#endif
/*----------------------------------------------------------------------------*/
/* Description:   Internal CPU Frequency                                      */
/*----------------------------------------------------------------------------*/
extern const ULONG                c_CCconprj_ClockFrequency_UL;

/*----------------------------------------------------------------------------*/
/* Description:   CAN module input Frequency                                  */
/*----------------------------------------------------------------------------*/
extern const ULONG                c_CCconprj_CanClockFrequency_UL;

/*----------------------------------------------------------------------------*/
/* Description:   Timer module input Frequency                                */
/*----------------------------------------------------------------------------*/
extern const ULONG                c_CCconprj_TimerFrequency_UL;

/*----------------------------------------------------------------------------*/
/* Description:   Session Selection Table                                     */
/*----------------------------------------------------------------------------*/
extern const COMMAND_ST*          c_CCconprj_SessionSelectionTable_APST[];

/*----------------------------------------------------------------------------*/
/* Description:   Session Initialisation Table                                */
/*----------------------------------------------------------------------------*/
extern const COMMANDINIT_ST*      c_CCconprj_SessionInitTable_APST[];

/*----------------------------------------------------------------------------*/
/* Description:   Memory Map Table for system check                           */
/*----------------------------------------------------------------------------*/
extern const VALIDATIONMAP_ST     c_CCconprj_ValidationMap_AST[];

/*----------------------------------------------------------------------------*/
/* Description:  Length of checksum and signature                             */
/*----------------------------------------------------------------------------*/
extern const UWORD c_CCconprj_SignatureLength_UW;
extern const UWORD c_CCconprj_ChecksumLength_UW;

/*----------------------------------------------------------------------------*/
/* Description:   Memory Map Table for download check                         */
/*----------------------------------------------------------------------------*/
extern const DOWNLOADMAP_ST       c_CCconprj_DownloadMap_AST[];

/*----------------------------------------------------------------------------*/
/* Description:   Memory Map Table for EEPROM Access                          */
/*----------------------------------------------------------------------------*/
extern const NVMENTRY_ST          c_CCconprj_EEPROM_AST[];

#ifdef cs_CCUDS_Audi
/*----------------------------------------------------------------------------*/
/* Description:   CAN diagnostic response telegrams                           */
/*----------------------------------------------------------------------------*/
extern const DIAGRESPONSETG_ST c_CCconprj_DiagResponseTG_AST[];

/*----------------------------------------------------------------------------*/
/* Description:   service 0x22 EEPROM access externals                        */
/*----------------------------------------------------------------------------*/
extern const RDBIDENTRY_ST c_CCconprj_ReadDataByID_AST[];
extern const UBYTE c_CCconprj_MaxNbrRecordDataIDs_UB;
extern const UWORD c_CCconprj_ECUProgramInfo_UW;
extern const UWORD c_CCconprj_AsamOdxFileID_UW;
extern const UBYTE c_CCconprj_LengthRepairShopCode_UB;
extern const UBYTE c_CCconprj_LengthProgDate_UB;
extern const UBYTE c_CCconprj_LengthProgState_UB;
extern const UBYTE c_CCconprj_LengthFingerprint_UB;
extern const UBYTE c_CCconprj_LengthBootSWIdent_UB;

extern const UBYTE c_CCconprj_LogBlockEEPROM_AUB[3][7];

extern const UWORD c_CCconprj_WriteFingerprint_UW;
extern const UWORD c_CCconprj_ReadFingerprint_UW;

#endif

/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
/* No ENTRY                                                                   */

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

