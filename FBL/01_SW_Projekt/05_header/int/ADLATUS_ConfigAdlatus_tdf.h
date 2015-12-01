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
|  Filename:  ADLATUS_ConfigAdlatus_tdf.h                                      |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the SMART ADLATUS configuration.                                 |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_ConfigAdlatus_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/int/ADLATUS_ConfigAdlatus_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_CONFIGADLATUS_
#define __TDF_CONFIGADLATUS_
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: System                                 */
/*----------------------------------------------------------------------------*/
extern const SYSTEMINTERFACE_ST    c_CCconsys_SystemInterface_ST;

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: CAN                                    */
/*----------------------------------------------------------------------------*/
extern const CANINTERFACE_ST       c_CCconsys_CanInterface_ST;

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: SessionHandler                         */
/*----------------------------------------------------------------------------*/
extern const SESSIONINTERFACE_ST       c_CCconsys_SessionInterface_ST;

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: ServiceHandler                         */
/*----------------------------------------------------------------------------*/
extern const SRVHANDLERINTERFACE_ST    c_CCconsys_SrvHandlerInterface_ST;

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: Application KWP2000                    */
/*----------------------------------------------------------------------------*/
extern const KWP2000INTERFACE_ST       c_CCconsys_Kwp2000Interface_ST[];

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: HAL                                    */
/*----------------------------------------------------------------------------*/
extern const HALINTERFACE_ST           c_CCconsys_HalInterface_ST;

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: NVM Handler                            */
/*----------------------------------------------------------------------------*/
extern const NVMINTERFACE_ST           c_CCconsys_NvmInterface_ST;

/*----------------------------------------------------------------------------*/
/* Initialization interface for modul: Timer                                  */
/*----------------------------------------------------------------------------*/
extern const SWTIMERHANDLERINTERFACE_ST c_CCconsys_SWTimerHandlerInterface_ST;


/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */
void FUN_CCconsys_InitAllSystemBuffer_V (void);


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */


/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */

