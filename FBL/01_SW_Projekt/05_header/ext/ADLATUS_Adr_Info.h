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
|  Filename:  ADLATUS_Adr_Info.h                                               |
|                                                                              |
|  Comment:   This file includes all address definitions of the adlatus        |
|             and of the apllication software                                  |
|                                                                              |
+------------------------------------------------------------------------------+
| A U T H O R   I D E N T I T Y                                                |
+------------------------------------------------------------------------------+
|                                                                              |
| Initials     Name                      Company                               |
| --------     ---------------------     ----------------------------------    |
|   KA         Markus Karas              SMART Electronic Development GmbH     |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Adr_Info.h 5771 2010-07-06 06:49:42Z werner $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/S1NN-ADLATUS-TMS470R1SF55B-MucSuc/20100706_S1NN_TMS470S_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Adr_Info.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/*==========================================================================*/
#ifndef __ADR_INFO_
#define __ADR_INFO_
/*==========================================================================*/




/******************************************************************************/
/*                                                                            */
/* ADLATUS Address Information                                                */
/*                                                                            */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Start Address of the ADLATUS                                               */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_AdlatusStartAdr                 0x00000000ul
/*----------------------------------------------------------------------------*/
/* End Address of the ADLATUS                                                 */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_AdlatusEndAdr                   0x00007FFFul
/*----------------------------------------------------------------------------*/
/* Checksum Calculation Start Address of the ADLATUS                          */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_AdlatusChksumCalcStartAdr       0x00000000ul
/*----------------------------------------------------------------------------*/
/* Checksum Calculation End Address of the ADLATUS                            */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_AdlatusChksumCalcEndAdr         0x00007FFDul
/*----------------------------------------------------------------------------*/
/* Checksum Start Address of the ADLATUS                                      */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_AdlatusChksumStartAdr           0x00007FFEul



/******************************************************************************/
/*                                                                            */
/* ASW specific address information                                           */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/**                                                                          **/
/**     APPLICATION AREA   ASW.01                                            **/
/**     ----------------------------------------------------------------     **/
/**     A S W  E N T R Y  C O N F I G U R A T I O N                          **/
/**     ----------------------------------------------------------------     **/
/**     Application software entry adress                                    **/
/**                                                                          **/
/******************************************************************************/
/******************************************************************************/
#define d_CCadrinfo_AswEntryAddress                 0x00008000ul

/*----------------------------------------------------------------------------*/
/* Interrupt Vector Table of ASW                                              */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_FswIntvecttable                 0x00008000ul

/*----------------------------------------------------------------------------*/
/* ASW Area 01                                                                */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_Asw1StartAdr                    0x00008000ul
#define d_CCadrinfo_Asw1EndAdr                      0x000BFFFFul
#define d_CCadrinfo_Asw1ChksumStartAdr              0x00008000ul
#define d_CCadrinfo_Asw1ChksumEndAdr                0x000BFFFFul
#define d_CCadrinfo_Asw1ChksumAdr                   0x000BFFFEul

/*----------------------------------------------------------------------------*/
/* Parrot Boot Area  128KByte                                                 */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_ParrotBootStartAdr              0x80000000ul
#define d_CCadrinfo_ParrotBootEndAdr                0x8002FFFFul
/*----------------------------------------------------------------------------*/
/* Parrot Applikation Area  128MByte - 128KByte                               */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_ParrotStartAdr                  0x80030000ul
#define d_CCadrinfo_ParrotEndAdr                    0x87FFFFFFul



/*----------------------------------------------------------------------------*/
/* ASW Area 02                                                                */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_Asw2StartAdr                    0x00000000ul
#define d_CCadrinfo_Asw2EndAdr                      0x00000000ul
#define d_CCadrinfo_Asw2ChksumStartAdr              0x00000000ul
#define d_CCadrinfo_Asw2ChksumEndAdr                0x00000000ul

#define d_CCadrinfo_Asw2ChksumAdr                   0x00000000ul

/*----------------------------------------------------------------------------*/
/* ASW Area 03                                                                */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_Asw3StartAdr                    0x00000000ul
#define d_CCadrinfo_Asw3EndAdr                      0x00000000ul
#define d_CCadrinfo_Asw3ChksumStartAdr              0x00000000ul
#define d_CCadrinfo_Asw3ChksumEndAdr                0x00000000ul

#define d_CCadrinfo_Asw3ChksumAdr                   0x00000000ul

/*----------------------------------------------------------------------------*/
/* EEPROM CHECKSUM                                                            */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_EepromStartAdr                 0x00000100ul
#define d_CCadrinfo_EepromEndAdr                   0x00000119ul

/******************************************************************************/
/*                                                                            */
/* FlashFunctions Address Information                                         */
/*                                                                            */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Start Address of the FlashFunctions on RAM                                 */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_FlashFctRamStartAdr             0x00401000ul
/*----------------------------------------------------------------------------*/
/* End Address of the FlashFunctions on RAM                                   */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_FlashFctRamEndAdr               0x004018FFul

#define d_CCadrinfo_Ram1StartAdr                    0x00400000ul
#define d_CCadrinfo_Ram1EndAdr                      0x00401FFFul

/******************************************************************************/
/*                                                                            */
/* Watchdog Address Information                                               */
/*                                                                            */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Start Address of the External Watchdog in Flash                            */
/*------+------------------------------+--------------------------------------*/
#define d_CCadrinfo_WatchdogFlashStartAdr       ((ULONG) &ROMWDT)

/*----------------------------------------------------------------------------*/
/* Start Address of the External Watchdog in RAM                              */
/*------+------------------------------+--------------------------------------*/
#define d_CCadrinfo_WatchdogRamStartAdr         ((ULONG) &RAMWDT)

/*----------------------------------------------------------------------------*/
/* Size of the External Watchdog                                              */
/*----------------------------------------------------------------------------*/
#define d_CCadrinfo_WatchdogSize                   0x0370u


/******************************************************************************/
/*                                                                            */
/* Address of Flash Request Pattern                                           */
/*                                                                            */
/******************************************************************************/
#define d_CCadrinfo_FlashRequestAddress             0x00402000ul

/******************************************************************************/
/*                                                                            */
/* reprogADLATUS specific address information                                 */
/*                                                                            */
/******************************************************************************/
#define d_CCadrinfo_oldAdlatusStartAdr              0x00000000ul
#define d_CCadrinfo_oldAdlatusEndAdr                0x00000000ul

#define d_CCadrinfo_newAdlatusStartAdr              0x00000000ul
#define d_CCadrinfo_newAdlatusEndAdr                0x00000000ul

#define d_CCadrinfo_FlashFctFlashStartAdr           0x00000000ul
#define d_CCadrinfo_FlashFctFlashEndAdr             0x00000000ul


/******************************************************************************/
/*                                                                            */
/* address information for default data for 1a 9b service when eeprom access  */
/* was not successful and the application software is valid                   */
/* Only necessary for TP20 and not 0x22 commands                              */
/******************************************************************************/
#define d_CCadrinfo_ASWDefaultPartnumber1a9b        0x00007000ul
#define d_CCadrinfo_ASWDefaultProgramStatus1a9b     0x00007100ul
#define d_CCadrinfo_ASWDefaultDataStatus1a9b        0x00007200ul

/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
