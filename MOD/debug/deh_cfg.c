/******************************************************************************/
/*****************  Copyright (c) 2011 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file 	deh_cfg.c
 * \brief	this module manage is the application part of DEH
 * Detailed description found below.
 * \brief	Detailed description of the file.
*/
/*-----------------------------------------------------------------------------
* $Date: 2010-10-08 11:11:48 +0200 (Fr, 08 Okt 2010) $
* $Rev: 15681 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_DTC/trunk/src/APP_DTC_C1.c $
*-----------------------------------------------------------------------------
*/
/*==============================================================================
                      Diagnostic Event Handler  (DEH)
                     ---------------------------------
                      Version 1.2.5
                      Author: Ingo Gensert/ IAV GmbH
                      Date:   29.01.2008
                      Modul:  deh_cfg.c

Revison History:
----------------
Rev.                        Comments
------------------------------------------------------------------------------
V1.2.5   Change StatusOfDTC Flags after DEH_ClearEvent()
=======  =====================================================================
  KWP2000: clear event within the error memory BUT NO change of readiness bit
           and set to PASSIVE (NOT STORED)
  UDS:     bit7 - MIL ........................ => RESET to OFF
           bit6 - Test Not Compl This Op Cycle => reset to NOT TESTED THIS OP.
                                                  CYCLE
           bit5 - Test Failed Since Last Clear => NO CHANGE 
           bit4 - Test Not Complete SLC ...... => NO CHANGE 
           bit3 - Conf DTC ................... => Reset to NO CONFIRMED DTC
           bit2 - Pending DTC ................ => NO CHANGE
           bit1 - Test Failed This OP Cycle .. => Reset to TEST NOT FAILED
           bit0 - Test Current State ......... => reset to PASSIVE
        

V1.2.4   Bugfix DEH_ReadDTCExtDataRecord()
=======  =====================================================================
  1. priority test for the current event with correct event index
==============================================================================

V1.2.3   NOT ERASABLE EVENTS handling
=======  =====================================================================
  1. All state bits (StatusOfDTC) for NOT erasable events are not reset or 
     cleared with the function DEH_ClearDiagnosticInformation() if the current 
     diagnostic session NOT equal to Developper Session OR Programmer Session 
==============================================================================

V1.2.2   Extend Prio 9 / Change Locked DTC handling / Extend DEFECT ECU errors
=======  =====================================================================
  1. Change state from ACTIVE - NOT STORED events after call 
     DEH_ControlDTCSetting(ON) back to PASSIVE state. This is necesarry to 
     start a normal debouncing and storage after new error detection.
  2. Priority 9 events are only erasable within the developer diagnosis 
     session.
  3. DEH_ClearDiagnosticInformation() in all other diagnosis session
     (NOT developer session) has no influence to the Status Of DTC flags for 
     Prio 9 Events.
  4. ECU defect events are now erasable within the diagnosis programmer session.
  5. Bugfix:: DEH_ReadDTCExtDataRecord()-> StatusOfDTC return value now correct
     (former only for event with the number 0!)
  6. clear unused variable DEH_stActiveEvents_mb
  7. support the variable DEH_StEV_chgState_mu8 only if compiler switch 
    (DEH_SUP_DETECT_CHANGE_EV_STATE) is set
  8. new state for unlearn algorithm state machine (to avoid unlearn blocking 
     after a new DEH_Init(..))
==============================================================================

V1.2.1   PRIORITY 9 Event support
=======  ========================
  events with priority 9 have no influence to the CAN event state bit
  (macro DEH_GET_CAN_EVENT_BIT (variable DEH_stCANEventBit_mb)) and no
  influence to the warning lamp state(macro DEH_GET_LAMP_INFO
  (variable DEH_stWarnLamp_mu8)

  KWP2000:
  --------
  The function DEH_ReadIdentifiedEvents(..) and DEH_GetNumberOfIdentEvents(..)
  read out the events with priority 9 only in diagnosis session
  DEH_ENTW_DIAG_SESSION (0x86 => developer session) in all other diagnosis session
  this events will be ignored.

  UDS:
  ----
  The function DEH_ReadDTCByStatus(..) and DEH_ReadDTCExtDataRecord(..)
  read out the events with priority 9 only in diagnosis session
  DEH_ENTW_DIAG_SESSION (0x4F => developer session) in all other diagnosis session
  this events will be ignored.
==============================================================================

V1.2.0
======

KWP2000
-------
  1) new Readyness Bit Handling to fullfill OBD requirements
  ----------------------------------------------------------
  The readiness bit is permanent set to tested (value 0) after
  the first received test report for an special DTC. The service
  DEH_ClearDiagnosticInformation(clear error memory) reset all
  readiness bits to NOT tested (value 1).

     - I)   DEH_StoreStatusOfDTCInfos()
              function to inform the DEH about system shutt off and to store the
              internal Readiness Bit into a EEPROM Byte - array
     - II)  APL_DEH_WriteStatusOfDTC(..)
              function from application for the DEH to write readiness information
              status bytes in the EEPROM byte array
     - III) APL_DEH_ReadStatusOfDTC(..)
              function from application for the DEH to read readiness information
              status bytes from the EEPROM byte array

UDS/KWP2000
-----------
  2) new unlearn counter handling if no valid unlearn counter available
  ---------------------------------------------------------------------
  - store 0xFF as valid value to the error memory if the application delivered this value
  - NO unlearn of a PASSIVE error with unlearn counter 0xFF in the error memory

  3) DEH_E_INIT for all SDS service function if DEH NOT initialized

UDS
---
  4) function DEH_ReadDTCExtDataRecord
  ------------------------------------
  - return value DEH_OK, if DTC NOT stored
  - return value numOfBytes = 0, if DTC NOT stored

  5) ControllDTC and Pending Bits
  -------------------------------
  - NO support of the Pending Status Bits after ControlDTC with 
    parameter DTC_OFF (0x02)
    
  6) DEH_ReadDTCExtDataRecord new data layout
  -------------------------------------------
  - no central aging counter included in the service answer
================================================================================
V1.1.3
1) new feature -> deactivate all blocked DTCs after diagnostic session change
                  with function DEH_SetDiagnosticSession(..)

2) new feature -> Control DTC NOT possible if UDS support and diagnostic session
                  is OBDII&VWDefaultDiagnosticSession (0x01) in function 
                  DEH_ControlDTCSetting(..)

3) bugfix      -> change logical reaction for the parameter DEH_SET_DTC_ON and 
                  DEH_SET_DTC_OFF within function DEH_ControlDTCSetting(..)

4) bugfix      -> variables DEH_stCANEventBit_mb/DEH_stWarnLamp_mu8 NOT static
                  
5) bugfix      -> change Freeze Frame Structure for option without reading
                  AddFreezeFrames 
6) feature     -> change datatypes for STOFDTC_STORAGE_BLOCK_SIZE to uint16_t
================================================================================
V1.1.2  
1) new Test Failed Last State Specification
  - Test Failed Last State is NOT debounced error state which are stored into 
    the EEPROM for next driving cycle
    
2) new include structure

3) aprove CAN-Event State Bit and Warning Lamp State after aging

4) read FREEZE FRAMES if error entry with activated Freeze Frame Update Bit 
    - for error with Event Debounced 1 
      (and Freeze Frame Update Period is not reached)
=================================================================================  

V1.1.1   First Revision with full UDS-Support 
         ===================================
if UDS and OR OBD supported (dependant on defines):::

1) new application function to read and store the StatusOfDTC flags in buffer
   from the application
    - APL_DEH_ReadStatusOfDTC()
    - APL_DEH_WriteStatusOfDTC()

2) new function to read back all StatusOfTDC flags to application buffer if the
   application going to sleep
    - DEH_StoreStatusOfDTCInfos()

3) update funciton APL_DEH_ReadAddFF() with new data defines 

4) support APL_DEH_ReadDTCSnapshotRecord() to read Snap shot data from application

5) inform the application for DEH state canges APL_DEH_SnapShotDatenInit()
   
6) extend the service DEH_ReadDTCExtDataRecord() with Record Numbers::
  - 0x70-0x7F : Extended Record Datas
  - 0xFF      : AllDTCExtendedDataRecords
============================================================================*/

/* ========================================================================== */
/* Project Includes                                                           */
/* ========================================================================== */

#include "deh.h"
#include "i2s_types.h"
#include "sds_ext.h"
#include "DEH_DTC_CFG.h"
#include "APP_CAN.h"
#include "OSEK_HostTask_H1.h"
#include "CSHDL.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
#include "deh_cfg.id"

/* ========================================================================== */
/* Global Variables                                                           */
/* ========================================================================== */
Union_EEPEventDataBlock EE_EventMemoryBlockArray;

/* =========== EVENT DATAS ================================================== */
const tDEH_EventPathParameter DEH_EventPathParameter[DEH_MAX_NUMBER_OF_EVENTPATHS];

/* =========== FREEZE FRAME TABLE =========================================== */

#ifndef  DEH_TEST
#if (DEH_MAX_NUMBER_FF>0)
  uint8_t DEH_FreezeFrameTable[DEH_MAX_NUMBER_FF_IDX][DEH_MAX_NUMBER_FF]; 
#endif
#endif

#define DEH_TIME_DEBOUNCED               0x00
#define DEH_EVENT_DEBOUNCED              0x01

#define DEH_UNLEARN_NOT_LOCKED           0x00
#define DEH_UNLEARN_LOCKED               0x02

#define DEH_STATE_CHANGE_REPORT_ACTIVE   0x00
#define DEH_STATE_CHANGE_REPORT_DEACTIVE 0x04

#define DEH_DELETE_EVENT_POSSIBLE        0x00
#define DEH_DELETE_EVENT_IMPOSSIBLE      0x08

#define MAX_EEPROM_WRITE_RETRIES_DEH     1000 //used for Diagnose 2E Write E2P direct, numbers of retries to write in EEPROM
static TriStateType_DEH_EE  SucessfulRegisterDataToEEprom = DEH_EE_NOTHING_TODO;
static uint16_t _APL_DEH_u16EEpromWriting_W_Control= 0;

static uint8_t EE_UnlearnCouner_u8 = 0xFF;

/* -------------------------------------------------------------------------- */
/* Example for an event dataset with two events                               */
/* -------------------------------------------------------------------------- */
/*  desc:              "event 1";                                                      */   
/*  DTC:               0x1234;                                                         */
/*  EventParameter:    0b000; -> time debounced                                        */
/*  EventSymPrio:      0x12;  -> symptom = 2; priority = 1;                            */
/*  QualEvent:         2.0;   -> 2 seconds (100 ms cyclic task -> 20 dez => 14 hex)    */
/*  DeQualEvent:       1.0;   -> 2 seconds (100 ms cyclic task -> 10 dez => 0A hex)    */
/*  LampParameter:     0b00000011; -> event is connected to lamp 0; flashing is active */
/*  IndexFFrameTable:  0;          -> freeze frames from line 0 in freeze frame table  */

/*  desc:              "event 2";                                                       */   
/*  DTC:               0x5678;                                                          */
/*  EventParameter:    0b001; -> event debounced                                        */
/*  EventSymPrio:      0x34;  -> symptom = 4; priority = 3;                             */
/*  QualEvent:         7.0;   -> 07 events for defect detection                         */
/*  DeQualEvent:       3.0;   -> 03 events to change to PASSIVE state                   */
/*  LampParameter:     0b00001000; -> event is connected to lamp 1; flashing NOT active */
/*  IndexFFrameTable:  0xFF        -> read no freeze frames                             */

/* further conditions:                                                        */
/*----------------------------------------------------------------------------*/
/* I)  freeze frame table with one line                                       */
/* II) only one freeze frame (only standard FF)                               */

/* #define DEH_MAX_NUMBER_OF_EVENTPATHS 2                                     */
/* #define DEH_MAX_NUMBER_FF            2                                     */
/* #define DEH_MAX_NUMBER_FF_IDX        1                                     */

uint8_t DEH_FreezeFrameTable[DEH_MAX_NUMBER_FF_IDX][DEH_MAX_NUMBER_FF] =
                                                          {STD_FF_NUMBER};

 const tDEH_EventPathParameter DEH_EventPathParameter[DEH_MAX_NUMBER_OF_EVENTPATHS]=
    {{            
      /****************************************************************************/
      /*       DEH_EVENT_0.DTC                                                    */
      0x904811, /* DTC_904811_MIKROTELEFON_KURZSCHLUSSNACHMASSE  */
      /*       DEH_EVENT_0.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_0.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_0.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_0.DeQualEvent                                            */
      0x28,    /* No Dequalification  */
      /*       DEH_EVENT_0.LampParameter                                          */
      0x00,      /* Lamp1 with no blinking */
      /*       DEH_EVENT_0.IndexFFrameTable                                       */
      0x00},     /* only 1 freezeframetabel-line -> 0x00 */
     /****************************************************************************/
     /*       DEH_EVENT_1.DTC                                                    */
     {0x904812, /* DTC_904812_MIKROTELEFON_KURZSCHLUSSNACHPLUS */	
      /*       DEH_EVENT_1.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_1.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_1.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_1.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_1.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_1.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_2.DTC                                                    */
     {0x904813, /* DTC_904813_MIKROTELEFON_UNTERBRECHUNG  */
      /*       DEH_EVENT_2.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_2.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_2.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_2.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_2.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_2.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_3.DTC                                                    */
     {0x904F11,  /* DTC_904F11_AUDIOKANALLINKS_KURZSCHLUSSNACHMASSE */
      /*       DEH_EVENT_3.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_3.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_3.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_3.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_3.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_3.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_4.DTC                                                    */
     {0x904F12,  /* DTC_904F12_AUDIOKANALLINKS_KURZSCHLUSSNACHPLUS */
      /*       DEH_EVENT_4.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_4.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_4.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_4.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_4.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_4.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_5.DTC                                                    */
     {0x904FF0,  /* DTC_904FF0_AUDIOKANALLINKS_KURZSCHLUSSUNTEREINANDER */
      /*       DEH_EVENT_5.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_5.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_5.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_5.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_5.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_5.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_6.DTC                                                    */
     {0x904F13,  /* DTC_904F13_AUDIOKANALLINKS_UNTERBRECHUNG */
      /*       DEH_EVENT_6.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_6.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_6.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_6.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_6.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_6.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_7.DTC                                                    */
     {0x904E11,  /* DTC_904E11_AUDIOKANALRECHTS_KURZSCHLUSSNACHMASSE */
      /*       DEH_EVENT_7.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_7.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_7.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_7.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_7.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_7.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_8.DTC                                                   */
     {0x904E12,    /* DTC_904E12_AUDIOKANALRECHTS_KURZSCHLUSSNACHPLUS */
      /*       DEH_EVENT_8.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_8.EventSymPrio                                          */
      0x40,      /* UDS  */
      /*       DEH_EVENT_8.QualEvent                                             */
      0x50,
      /*       DEH_EVENT_8.DeQualEvent                                           */
      0x28,
      /*       DEH_EVENT_8.LampParameter                                         */
      0x00,
      /*       DEH_EVENT_8.IndexFFrameTable                                      */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_9.DTC                                                   */
     {0x904EF0,    /* DTC_904EF0_AUDIOKANALRECHTS_KURZSCHLUSSUNTEREINANDER */
      /*       DEH_EVENT_9.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_9.EventSymPrio                                          */
      0x40,      /* UDS  */
      /*       DEH_EVENT_9.QualEvent                                             */
      0x50,
      /*       DEH_EVENT_9.DeQualEvent                                           */
      0x28,
      /*       DEH_EVENT_9.LampParameter                                         */
      0x00,
      /*       DEH_EVENT_9.IndexFFrameTable                                      */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_10.DTC                                                    */
     {0x904E13,  /* DTC_904E13_AUDIOKANALRECHTS_UNTERBRECHUNG */
      /*       DEH_EVENT_10.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_10.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_10.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_10.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_10.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_10.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_11.DTC                                                    */
     {0x905000,  /* DTC_905000_FUNKTIONEINSCHRAENKUNGDURCHUEBERTEMPERATUR */
      /*       DEH_EVENT_11.EventParameter                                         */
      0x01,
      /*       DEH_EVENT_11.EventSymPrio                                           */
      0x30,      /* UDS  */
      /*       DEH_EVENT_11.QualEvent                                              */
      0x001E,
      /*       DEH_EVENT_11.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_11.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_11.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_12.DTC                                                    */
     {0x905207,  /* DTC_905207_DREITASTENMODUL_MECHANISCHER_FEHLER */
      /*       DEH_EVENT_12.EventParameter                                        */
         DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
         DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_12.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_12.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_12.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_12.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_12.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_13.DTC                                                    */
     {0x905211,  /* DTC_905211_DREITASTENMODUL_KURZSCHLUSSNACHMASSE */
      /*       DEH_EVENT_13.EventParameter                                         */
         DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
         DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_13.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_13.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_13.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_13.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_13.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_14.DTC                                                    */
     {0x905212,  /* DTC_905212_DREITASTENMODUL_KURZSCHLUSSNACHPLUS */
      /*       DEH_EVENT_14.EventParameter                                        */
         DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
         DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_14.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_14.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_14.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_14.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_14.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_15.DTC                                                    */
     {0x905213,  /* DTC_905213_DREITASTENMODUL_UNTERBRECHUNG */
      /*       DEH_EVENT_15.EventParameter                                         */
         DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
         DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_15.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_15.QualEvent                                              */
      0x50,
      /*       DEH_EVENT_15.DeQualEvent                                            */
      0x28,
      /*       DEH_EVENT_15.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_15.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_16.DTC                                                    */
     {0x000021,  /* DTC_000021_DATENBUSKOMFORT_DEFEKT */
      /*       DEH_EVENT_16.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_16.EventSymPrio                                           */
      0x20,      /* UDS  */
      /*       DEH_EVENT_16.QualEvent                                              */
      0x0002,
      /*       DEH_EVENT_16.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_16.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_16.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_17.DTC                                                    */
     {0x000022,  /* DTC_000022_DATENBUSKOMFORT_KEINEKOMMUNIKATION */
      /*       DEH_EVENT_17.EventParameter                                         */
      DEH_TIME_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_17.EventSymPrio                                           */
      0x20,      /* UDS  */
      /*       DEH_EVENT_17.QualEvent                                              */
      0x0001,
      /*       DEH_EVENT_17.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_17.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_17.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_18.DTC                                                    */
     {0xC06400,  /* DTC_C06400_INFOTAIMENTCAN_DEFEKT */
      /*       DEH_EVENT_18.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_18.EventSymPrio                                           */
      0x20,      /* UDS  */
      /*       DEH_EVENT_18.QualEvent                                              */
      0x0002,
      /*       DEH_EVENT_18.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_18.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_18.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_19.DTC                                                    */
     {0xC06500,  /* DTC_C06500_INFOTAIMENTCAN_KEINEKOMMUNIKATION */
      /*       DEH_EVENT_19.EventParameter                                         */
      DEH_TIME_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_19.EventSymPrio                                           */
      0x20,      /* UDS  */
      /*       DEH_EVENT_19.QualEvent                                              */
      0x0001,
      /*       DEH_EVENT_19.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_19.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_19.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_20.DTC                                                    */
     {0xC14000,  /* DTC_C14000_BODYCOMPUTER1_KEINEKOMMUNIKATION */ 
      /*       DEH_EVENT_20.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_20.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_20.QualEvent                                              */
      0x0005,
      /*       DEH_EVENT_20.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_20.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_20.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_21.DTC                                                    */
      {0xC15500, /* DTC_C15500_SG_SCHALTTAFELEINSATZ_KEINEKOMMUNIKATION */
      /*       DEH_EVENT_21.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_21.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_21.QualEvent                                              */
      0x0019,
      /*       DEH_EVENT_21.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_21.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_21.IndexFFrameTable                                       */
      0x00},     
     /****************************************************************************/
     /*       DEH_EVENT_22.DTC                                                    */
    
     {0xC21200,   /* DTC_C21200_SG_LENKSAEULENELEKTRONIK_KEINEKOMMUNIKATION */
      /*       DEH_EVENT_22.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_22.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_22.QualEvent                                              */
      0x0005,
      /*       DEH_EVENT_22.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_22.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_22.IndexFFrameTable                                       */
      0x00},    
     /****************************************************************************/
     /*       DEH_EVENT_23.DTC                                                    */
     {0xD00900,  /* DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL */
      /*       DEH_EVENT_23.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_23.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_23.QualEvent                                              */
      0x0005,
      /*       DEH_EVENT_23.DeQualEvent                                            */
      0x0001,  
      /*       DEH_EVENT_23.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_23.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_24.DTC                                                    */
     {0xD01100,  /* DTC_D01100_VERSORGUNGSSPANNUNG_ZUNIEDRIG */
      /*       DEH_EVENT_24.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_24.EventSymPrio                                           */
      0x30,      /* UDS  */
      /*       DEH_EVENT_24.QualEvent                                              */
      0x2D,
      /*       DEH_EVENT_24.DeQualEvent                                            */
      0x5,
      /*       DEH_EVENT_24.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_24.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_25.DTC                                                    */
     {0xD01200,  /* DTC_D01200_VERSORGUNGSSPANNUNG_ZUHOCH */
      /*       DEH_EVENT_25.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_25.EventSymPrio                                           */
      0x30,      /* UDS  */
      /*       DEH_EVENT_25.QualEvent                                              */
      0x2D,
      /*       DEH_EVENT_25.DeQualEvent                                            */
      0x5,
      /*       DEH_EVENT_25.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_25.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_26.DTC                                                    */
     {0xD01300,    /* DTC_D01300_SG_NICHTCODIERT */
      /*       DEH_EVENT_26.EventParameter                                        */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_26.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_26.QualEvent                                              */
      0x0001,
      /*       DEH_EVENT_26.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_26.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_26.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_27.DTC                                                    */
     {0xD01400,    /* DTC_D01400_SG_FALSCHCODIERT  */
      /*       DEH_EVENT_27.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_27.EventSymPrio                                           */
      0x40,      /* UDS  */
      /*       DEH_EVENT_27.QualEvent                                              */
      0x0001,
      /*       DEH_EVENT_27.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_27.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_27.IndexFFrameTable                                       */
      0x00},
     /****************************************************************************/
     /*       DEH_EVENT_28.DTC                                                    */
     {0x000016,  /* DTC_000016_SG_RADIO_KEINSIGNAL */
      /*       DEH_EVENT_28.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_28.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_28.QualEvent                                              */
      0x0005,
      /*       DEH_EVENT_28.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_28.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_28.IndexFFrameTable                                       */
      0x00},      
     /****************************************************************************/
     /*       DEH_EVENT_29.DTC                                                    */
     {0xD40000,    /* DTC_D40000_FUNKTIONSEINSCHRAENKUNG_UNTERSPANNUNG */
      /*       DEH_EVENT_29.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_29.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_29.QualEvent                                              */
      0xA,
      /*       DEH_EVENT_29.DeQualEvent                                            */
      0x1,
      /*       DEH_EVENT_29.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_29.IndexFFrameTable                                       */
      0x00},      
     /****************************************************************************/
     /*       DEH_EVENT_30.DTC                                                    */
     {0xD40100,  /* DTC_D40100_FUNKTIONSEINSCHRAENKUNG_UEBERSPANNUNG */
      /*       DEH_EVENT_30.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_30.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_30.QualEvent                                              */
      0xA,
      /*       DEH_EVENT_30.DeQualEvent                                            */
      0x1,
      /*       DEH_EVENT_30.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_30.IndexFFrameTable                                       */
      0x00},      
     /****************************************************************************/
     /*       DEH_EVENT_31.DTC                                                    */
     {0xA00045,  /* DTC_A00045_STEUERGERAETDEFEKT_PROGRAMMSPEICHERFEHLER */
      /*       DEH_EVENT_31.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_IMPOSSIBLE,
      /*       DEH_EVENT_31.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_31.QualEvent                                              */
      0x0064,
      /*       DEH_EVENT_31.DeQualEvent                                            */
      0x0032,
      /*       DEH_EVENT_31.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_31.IndexFFrameTable                                       */
      0x00},      
     /****************************************************************************/
     /*       DEH_EVENT_32.DTC                                                    */
     {0xA00046,  /* DTC_A00046_STEUERGERAETDEFEKT_EEPROMFEHLER */
      /*       DEH_EVENT_32.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_IMPOSSIBLE,
      /*       DEH_EVENT_32.EventSymPrio                                           */
      0x60,      /* UDS  */
      /*       DEH_EVENT_32.QualEvent                                              */
      0x0001,
      /*       DEH_EVENT_32.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_32.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_32.IndexFFrameTable                                       */
      0x00},      
     /****************************************************************************/
     /*       DEH_EVENT_33.DTC                                                    */
     {0xA00047,  /* DTC_A00047_STEUERGERAETDEFEKT_WATCHDOGFEHLER */
      /*       DEH_EVENT_33.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_IMPOSSIBLE,
      /*       DEH_EVENT_33.EventSymPrio                                           */
      0x30,      /* UDS  */
      /*       DEH_EVENT_33.QualEvent                                              */
      0x0001,
      /*       DEH_EVENT_33.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_33.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_33.IndexFFrameTable                                       */
      0x00},      
     /****************************************************************************/
     /*       DEH_EVENT_34.DTC                                                    */
     {0xA00500,  /* DTC_A00500_UNGUELTIGER_DATENSATZ */
      /*       DEH_EVENT_34.EventParameter                                         */
      DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
      DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
      /*       DEH_EVENT_34.EventSymPrio                                           */
      0x30,      /* UDS  */
      /*       DEH_EVENT_34.QualEvent                                              */
      0x0001,
      /*       DEH_EVENT_34.DeQualEvent                                            */
      0x0001,
      /*       DEH_EVENT_34.LampParameter                                          */
      0x00,
      /*       DEH_EVENT_34.IndexFFrameTable                                       */
      0x00},   
	 /****************************************************************************/
	 /*       DEH_EVENT_35.DTC                                                    */
	 {0x905111,  /* DTC_905111_BASEPLATE_KURZSCHLUSSNACHMASSE */
	  /*       DEH_EVENT_35.EventParameter                                        */
	     DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
	     DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
	  /*       DEH_EVENT_35.EventSymPrio                                           */
	  0x40,      /* UDS  */
	  /*       DEH_EVENT_35.QualEvent                                              */
	  0x50,
	  /*       DEH_EVENT_35.DeQualEvent                                            */
	  0x28,
	  /*       DEH_EVENT_35.LampParameter                                          */
	  0x00,
	  /*       DEH_EVENT_35.IndexFFrameTable                                       */
	  0x00},
	 /****************************************************************************/
	 /*       DEH_EVENT_36.DTC                                                    */
	 {0x905112,  /* DTC_905112_BASEPLATE_KURZSCHLUSSNACHPLUS */
	  /*       DEH_EVENT_36.EventParameter                                         */
	     DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
	     DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
	  /*       DEH_EVENT_36.EventSymPrio                                           */
	  0x40,      /* UDS  */
	  /*       DEH_EVENT_36.QualEvent                                              */
	  0x50,
	  /*       DEH_EVENT_36.DeQualEvent                                            */
	  0x28,
	  /*       DEH_EVENT_36.LampParameter                                          */
	  0x00,
	  /*       DEH_EVENT_36.IndexFFrameTable                                       */
	  0x00},
	 /****************************************************************************/
	 /*       DEH_EVENT_37.DTC                                                    */
	 {0x905113,  /* DTC_905113_BASEPLATE_UNTERBRECHNUNG */
	  /*       DEH_EVENT_37.EventParameter                                        */
	     DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
	     DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
	  /*       DEH_EVENT_37.EventSymPrio                                           */
	  0x40,      /* UDS  */
	  /*       DEH_EVENT_37.QualEvent                                              */
	  0x50,
	  /*       DEH_EVENT_37.DeQualEvent                                            */
	  0x28,
	  /*       DEH_EVENT_37.LampParameter                                          */
	  0x00,
	  /*       DEH_EVENT_37.IndexFFrameTable                                       */
	  0x00},
	 /****************************************************************************/
	 /*       DEH_EVENT_38.DTC                                                    */
	 {0x905311,  /* DTC_905311_GSMANTENNA_KURZSCHLUSSNACHMASSE */
	  /*       DEH_EVENT_38.EventParameter                                         */
	     DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
	     DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
	  /*       DEH_EVENT_38.EventSymPrio                                           */
	  0x40,      /* UDS  */
	  /*       DEH_EVENT_38.QualEvent                                              */
	  0x50,
	  /*       DEH_EVENT_38.DeQualEvent                                            */
	  0x28,
	  /*       DEH_EVENT_38.LampParameter                                          */
	  0x00,
	  /*       DEH_EVENT_38.IndexFFrameTable                                       */
	  0x00},
	 /****************************************************************************/
	 /*       DEH_EVENT_39.DTC                                                    */
	 {0x905315,  /* DTC_905312_GSMANTENNA_KURZSCHLUSSNACHPLUS_UNTERBRECHNUNG */
	  /*       DEH_EVENT_39.EventParameter                                         */
	     DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
	     DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
	  /*       DEH_EVENT_39.EventSymPrio                                           */
	  0x40,      /* UDS  */
	  /*       DEH_EVENT_39.QualEvent                                              */
	  0x50,
	  /*       DEH_EVENT_39.DeQualEvent                                            */
	  0x28,
	  /*       DEH_EVENT_39.LampParameter                                          */
	  0x00,
	  /*       DEH_EVENT_39.IndexFFrameTable                                       */
	  0x00},
	 /****************************************************************************/
	 /*       DEH_EVENT_40.DTC                                                    */
	 {0x905707,  /* DTC_905707_HANDYHALTER_MECHANISCHER_FEHLER */
	  /*       DEH_EVENT_40.EventParameter                                         */
	     DEH_EVENT_DEBOUNCED | DEH_UNLEARN_NOT_LOCKED |
	     DEH_STATE_CHANGE_REPORT_DEACTIVE | DEH_DELETE_EVENT_POSSIBLE,
	  /*       DEH_EVENT_40.EventSymPrio                                           */
	  0x40,      /* UDS  */
	  /*       DEH_EVENT_40.QualEvent                                              */
	  0x50,
	  /*       DEH_EVENT_40.DeQualEvent                                            */
	  0x28,
	  /*       DEH_EVENT_40.LampParameter                                          */
	  0x00,
	  /*       DEH_EVENT_40.IndexFFrameTable                                       */
	  0x00}};
	 /****************************************************************************/

  

/* ========================================================================= */


/* ========================================================================== */
/* Prototypes local functions                                                 */
/* ========================================================================== */
tDEH_Return APL_DEH_EEPWriteEventData ( uint16_t  BlockPosition_u16, uint16_t BlockSize_u16, uint8_t *Buffer_u8);
tDEH_Return APL_DEH_EEPReadEventData  ( uint16_t  BlockPosition_u16, uint16_t BlockSize_u16, uint8_t *Buffer_u8);
tDEH_Return APL_DEH_ReadStdFF(tDEH_StdFreezeFrame *pStdDestination);

#if defined(DEH_SUP_KWP2000)
  #ifdef DEH_READ_ADD_FF 
    tDEH_Return APL_DEH_ReadAddFF(tDEH_FreezeFrameNumber FF_Number, tDEH_AddFreezeFrame *pAddDestination);
  #endif
#elif defined(DEH_SUP_UDS)
  #ifdef DEH_READ_ADD_FF 
    tDEH_Return APL_DEH_ReadAddFF(tDEH_FreezeFrameNumber FF_Number, uint8_t BuffSize, uint8_t *pNumOfBytes, uint8_t *pAddFFrame);
  #endif
  #ifdef DEH_SUP_SNAP_SHOT_DATA
    tDEH_Return APL_DEH_ReadDTCSnapshotRecord(tDEH_DTC DTC, uint16_t BuffSize_u16, uint16_t *pNumOfBytes_u16, uint8_t *pDestBuffer);
    void        APL_DEH_SnapShotDatenInit(uint8_t mode);
  #endif
  tDEH_Return APL_DEH_ReadStatusOfDTC(uint16_t BlockSize_u16, uint8_t *Buffer_u8);
  tDEH_Return APL_DEH_WriteStatusOfDTC(uint16_t BlockSize_u16, uint8_t *Buffer_u8);

#endif
uint8_t     APL_DEH_ReadUnlearnCounter(void);


/* ========================================================================== */

/* ========================================================================== */

  /* --------------------------------------------------------------------- */
  /* if the flag DEH_SUP_DETECT_CHANGE_EV_STATE == TRUE then should be     */
  /* all event states cahnges reportetd via this function.                 */
  /* The application is responsible for the relevant actions and responses */
  /* The DEH user is responsible for the content of this function.         */
  /* Here is only the empty function body for programming purpose          */
  /* --------------------------------------------------------------------- */

#ifndef  DEH_TEST
#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
/* ============================================================================
function name    :   APL_DEH_ReportChgEventState(tDEH_EventNumber EventNum_uT,
                                                 uint8_t          state_u8)
input parameter  :   tDEH_EventNumber EventNum_uT : current event number
                     uint8_t          state_u8    : current event state
output parameter :   
globals          :   
return value     :   no
description      :   Callback which is called if a event state has been changed 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 
============================================================================ */
  void APL_DEH_ReportChgEventState(tDEH_EventNumber EventNum_uT, uint8_t state_u8)
  {

    switch (EventNum_uT)
    {
      default:
      break;
    }
    return;
  }

#endif
#endif


/* ============================================================================
function name    :   APL_DEH_SnapShotDatenInit()
input parameter  :   mode  : current special mode of DEH
output parameter :   
globals          :   
return value     :   no
description      :   Function to inform the application about special DEH states 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
27.11.06    GIGE    built base version 
============================================================================ */
#ifdef DEH_SUP_SNAP_SHOT_DATA
void APL_DEH_SnapShotDatenInit(uint8_t mode)
{
    /* ==================================================================== */
    /* DEH is started with function DEH_Init()                              */
    /* ==================================================================== */
    if (mode == DEH_INIT)
  {
                
  }
    /* ==================================================================== */
    /* Event Memory is cleared with function DEH_ClearDiagnsotciInformation */
    /* ==================================================================== */
  else if (mode == DEH_CLEAR_EVENT_MEMORY)
  {
  
  }
return;
}
#endif


/* ============================================================================
function name    :   DEH_InitEEPROM()
input parameter  :   
output parameter :   
globals          :   
return value     :   no
description      :   Function for initializing the DTC EEPROM area once 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
27.11.06    GIGE    built base version 
============================================================================ */
#define MAX_DTC_EEPROM_INIT_VALUES  130

/**--------------------------------------------------------------------------*
* Function:       InitDEHFaultData                                           *
* Description:    Load the Faults from the EEprom                            *
* Parameters:     void                                                       *
* Returns:        void                                                       *
*                                                                            *
*---------------------------------------------------------------------------**/
void InitDEHFaultData(void)
{
	EEDeviceControlBlockType *pEEDcb = (EEDeviceControlBlockType *)getEEHostControl();
	CommonResponseCodeType retVal_E2P = COMMON_RSP_OK;

	/* first UnlearnCounter from RAM Mirror */
	(void)EEPROM_enGetSingleByteFromRamMirror(&EE_UnlearnCouner_u8, EEPROM_LAYOUT_ENUM_DEH_UnclearCounter);
    DEBUG_VALUE1(DEH_SWI_TRC, MOD_DEH, "Read DEH_UnlearnCounter from eeprom = %d",EE_UnlearnCouner_u8);
    /* DEH manager of VW standard software init event memory block with data  - from EEProm */
	retVal_E2P = EEPROM_sfRead(pEEDcb,
				VWDB_EE_ADDDRESS_DTCs,
				(Uint8Type *)EE_EventMemoryBlockArray.AllString,
				MAX_DTC_EEPROM_INIT_VALUES);
	if(retVal_E2P != COMMON_RSP_OK)
	{
		DEBUG_TEXT(DEH_SWI_ERR, MOD_DEH,"E2P Read FAIL");
		DEH_InitEEPROM();
	}

    return;
}

void DEH_InitEEPROM(void)     /* damit wird der DTC-EEPROM-Bereich einmalig mit 0xFF vorbelegt */
{
    Uint16Type i;

    for(i = 0; i <= MAX_DTC_EEPROM_INIT_VALUES; i++)
    {
        EE_EventMemoryBlockArray.AllString[i] = 0xFF;
    }
}

/*============================================================================
function name    :  APL_DEH_EEPReadEventData()
input parameter  :  BlockPosition : read position in Byte-Array 
                    BlockSize     : size of bytes to read from memeory 
                    Buffer        : pointer to write all  read values

output parameter :   -
globals          :   
return value     :  tDEH_Return
description      :  Function to read stored information in the event memory 
                    buffer. The Caller informs the function about the position 
                    where the needed data are located. From this position will 
                    be read the wanted bytes (blocksize) into the buffer.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 

============================================================================*/
tDEH_Return APL_DEH_EEPReadEventData ( uint16_t  BlockPosition_u16, uint16_t BlockSize_u16, uint8_t *Buffer_u8)
{
    tDEH_Return retVal = DEH_OK;

    sdsMemCpy(Buffer_u8, &EE_EventMemoryBlockArray.AllString[BlockPosition_u16],BlockSize_u16);
    
    return (retVal);
} /* end function APL_DEH_EEPReadEventData */


/*============================================================================
function name    :  APL_DEH_EEPWriteEventData()
input parameter  :  BlockPosition : write position in Byte-Array 
                    BlockSize     : size of bytes to read from memeory 
                    Buffer        : pointer where the source bytes are standing.

output parameter :   -
globals          :   
return value     :  tDEH_Return
description      :  Function to write new information in the event memory 
                    buffer. The Caller informs the function about the position 
                    where the needed data are located. From this position will 
                    be read the wanted bytes (blocksize) into the event memory.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 

============================================================================*/
tDEH_Return APL_DEH_EEPWriteEventData( uint16_t  BlockPosition_u16, uint16_t BlockSize_u16, uint8_t *Buffer_u8)
{
	#define DTC_Nbr     ((Buffer_u8[1] << 8) + Buffer_u8[0])
    #define DTC_Status  (Buffer_u8[2] & 1)
	tDEH_Return retVal = DEH_OK;
	CommonResponseCodeType rC = COMMON_RSP_OK;
	EEDeviceControlBlockType    *pEEDcb = (EEDeviceControlBlockType *)getEEHostControl();

	DEBUG_VALUE2(DEH_SWI_DBG, MOD_DEH, "Set Failure Memory at Position: %d,Size %d",BlockPosition_u16,BlockSize_u16);
	sdsMemCpy(&EE_EventMemoryBlockArray.AllString[BlockPosition_u16],Buffer_u8,BlockSize_u16);
	DEBUG_VALUE2(DEH_SWI_TRC, MOD_DEH, "Internal DTC-NBR %d, Status %d",DTC_Nbr, DTC_Status);
	/* (rC_E2P == COMMON_RSP_OK) - successful register to write E2P but write is not complete done !! */
	/* Complete DEH Ram Array write to eeprom 130 Bytes                                               */
	rC = EEPROM_LAYOUT_rcWrite(pEEDcb, VWDB_EE_ADDDRESS_DTCs, (uint8_t *) EE_EventMemoryBlockArray.AllString, MAX_DTC_EEPROM_INIT_VALUES,NULL);
	if (COMMON_RSP_OK == rC)
	{
		SucessfulRegisterDataToEEprom = DEH_EE_REGISTER;
		_APL_DEH_u16EEpromWriting_W_Control= 0;
		return (retVal);
	}
	else
	{
		DEBUG_TEXT(DEH_SWI_TRC, MOD_DEH, "EEPROM Write for DEH is busy for Error Memory Entries");
		/* DEH not handle a complete retry mechanisms in all cases to write data to eeprom          */
		/* Set flag that data from DEH was not register to write data to eeprom                     */
		/* The data of DEH is now write outside the DEH context to eeprom if above write was not OK */
		SucessfulRegisterDataToEEprom = DEH_EE_NOT_REGISTER;
		return (retVal);
	}




} /* end function APL_DEH_EEPWriteEventData */

/*----------------------------------------------------------------------------*/
/* Function    :  APL_DEH_EEPWriteHandleRetries                               */
/**
    \brief      E2P handle retry mechanism for DEH outside DEH context

    \param      void

	\return

*/
/*----------------------------------------------------------------------------*/
void APL_DEH_EEPWriteHandleRetries(void)
{
	CommonResponseCodeType rC = COMMON_RSP_OK;
	EEDeviceControlBlockType    *pEEDcb = (EEDeviceControlBlockType *)getEEHostControl();

	if(DEH_EE_NOT_REGISTER == SucessfulRegisterDataToEEprom)
	{
		/* (rC_E2P == COMMON_RSP_OK) - successful register to write E2P but write is not complete done !! */
		/* Complete DEH Ram Array write to eeprom  130 Bytes                                             */
		rC = EEPROM_LAYOUT_rcWrite(pEEDcb, VWDB_EE_ADDDRESS_DTCs, (uint8_t *) EE_EventMemoryBlockArray.AllString, MAX_DTC_EEPROM_INIT_VALUES,NULL);
		if (COMMON_RSP_OK == rC)
		{
			SucessfulRegisterDataToEEprom = DEH_EE_REGISTER;
			_APL_DEH_u16EEpromWriting_W_Control= 0;
			DEBUG_TEXT(DEH_SWI_TRC, MOD_DEH, "EEPROM DEH Write register was OK in retry part");
		}
		else
		{
			_APL_DEH_u16EEpromWriting_W_Control++;
			SucessfulRegisterDataToEEprom = DEH_EE_NOT_REGISTER;
			DEBUG_VALUE1(DEH_SWI_TRC, MOD_DEH, "EEPROM DEH Write not register. Try: %d",_APL_DEH_u16EEpromWriting_W_Control);

		}
		if (MAX_EEPROM_WRITE_RETRIES_DEH < _APL_DEH_u16EEpromWriting_W_Control)
		{
			SucessfulRegisterDataToEEprom = DEH_EE_NOTHING_TODO;
			_APL_DEH_u16EEpromWriting_W_Control= 0;
			DEBUG_TEXT(DEH_SWI_TRC, MOD_DEH, "MAX EEPROM WRITE RETRIES reached for DEH - Stop.");
			/* MAX EEPROM WRITE RETRIES - write not work -> Stop */
			/* No additional error handling available here       */
		}

	}
	else if (DEH_EE_REGISTER == SucessfulRegisterDataToEEprom)
	{
		SucessfulRegisterDataToEEprom = DEH_EE_NOTHING_TODO;
		_APL_DEH_u16EEpromWriting_W_Control= 0;
	}
	else  /* DEH_EE_NOTHING_TODO  */
	{
		;
	}
}  /* end function APL_DEH_EEPWriteHandleRetries */

/*----------------------------------------------------------------------------*/
/* Function    :  APL_DEH_IsDEHData_SuccessfulRegisterToEEprom                 */
/**
    \brief      Is DEH data successful register to EEprom

    \param      void

	\return     SucessfulRegisterDataToEEprom

*/
/*----------------------------------------------------------------------------*/
TriStateType_DEH_EE APL_DEH_IsDEHData_SuccessfulRegisterToEEprom(void)
{
   return(SucessfulRegisterDataToEEprom);
}

/*============================================================================
function name    :  APL_DEH_ReadStdFF()
input parameter  :  pStdDestination : pointer to struct for Standard Freeze 
                    Frame with type tDEH_StdFreezeFrame.


output parameter :  -
globals          :  -
return value     :  tDEH_Return
description      :  Read Standard Freeze Frames from system.

------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 

============================================================================*/
#ifdef DEH_READ_STD_FF
tDEH_Return APL_DEH_ReadStdFF(tDEH_StdFreezeFrame *pStdDestination)
{
    tDEH_Return retVal = DEH_OK;
    Uint8Type Convert= 0;
    
    if(TRUE == APP_CAN_sfDiagnose_1_MessageReceived())
    {
        pStdDestination->StdFF[0] = DIAGNOSE_KM_STAND_2;
        pStdDestination->StdFF[1] = DIAGNOSE_KM_STAND_1;
        pStdDestination->StdFF[2] = DIAGNOSE_KM_STAND_0;
        Convert = (DIAGNOSE_JAHR_1 << 4) | DIAGNOSE_JAHR_0;
        pStdDestination->StdFF[3] = Convert >> 6; 
        pStdDestination->StdFF[4] = (Convert << 2) | (DIAGNOSE_MONAT >> 2);   
        pStdDestination->StdFF[5] = (DIAGNOSE_MONAT << 6) | (DIAGNOSE_TAG_1 << 2) | (DIAGNOSE_TAG_0 << 1) | (DIAGNOSE_STUNDEN_1);
        pStdDestination->StdFF[6] = (DIAGNOSE_STUNDEN_0 << 4) | (DIAGNOSE_MINUTE >> 2);
        pStdDestination->StdFF[7] = (DIAGNOSE_MINUTE << 6) | (DIAGNOSE_SEKUNDE_1 << 1) | DIAGNOSE_SEKUNDE_0;
    }
    else
    {
        pStdDestination->StdFF[0] = 0xFF;
        pStdDestination->StdFF[1] = 0xFF;
        pStdDestination->StdFF[2] = 0xFF;
        pStdDestination->StdFF[3] = 0xFF;
        pStdDestination->StdFF[4] = 0xFF;
        pStdDestination->StdFF[5] = 0xFF;
        pStdDestination->StdFF[6] = 0xFF;
        pStdDestination->StdFF[7] = 0xFF;
    }
    return (retVal);
}
#endif


/*============================================================================
function name    :  APL_DEH_ReadAddFF()
input parameter  :  FF_Number       : additional freeze frame number
                    pAddDestination : pointer to struct for Additional Freeze 
                    Frame with type tDEH_AddFreezeFrame (only in KWP2000 variant).
                    
                    BuffSize        : size of destination buffer for for an  
                    Additional Freeze Frame (only in UDS variant).
                    pNumOfBytes     : pointer to a value for a number of copied bytes
                    pAddFFrame      : pointer to destination buffer

output parameter :  -
globals          :  -
return value     :  tDEH_Return
description      :  read addional freeze frames from system.

------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 

============================================================================*/
#ifdef DEH_READ_ADD_FF
tDEH_Return APL_DEH_ReadAddFF
(
  tDEH_FreezeFrameNumber  FF_Number,
#if defined(DEH_SUP_KWP2000)
  tDEH_AddFreezeFrame     *pAddDestination
#elif defined(DEH_SUP_UDS)
  uint8_t                 BuffSize,
  uint8_t                 *pNumOfBytes,
  uint8_t                 *pAddFFrame
#endif
)
{
  tDEH_Return retVal = DEH_OK;
  
#ifdef DEH_SUP_UDS
  *pNumOfBytes = (uint8_t) 0;
#endif  

  return (retVal);
}
#endif


/*============================================================================
function name    :  APL_DEH_ReadUnlearnCounter()
input parameter  : -
                   

output parameter :  -
globals          :  -
return value     :  tDEH_Return
description      :  read addional freeze frames from system.

------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 

============================================================================*/
uint8_t APL_DEH_ReadUnlearnCounter(void)
{
    static Diagnose1MessageType dehDiagInfo;

    if(TRUE == APP_CAN_sfDiagnose_1_MessageReceived())
    {
    	/* Gateway ZVZ  get with Diagnose 1 message */
       dehDiagInfo = APP_CAN_vDiagnose_1_Data();
       /* gatway ZVZ not available */ 
       if (0xFF == dehDiagInfo.verlernzaehler)
       {  
           dehDiagInfo.verlernzaehler = EE_UnlearnCouner_u8;    	  
       } 
       // Test to change verlernzähler
       //(void)EEPROM_enGetSingleByteFromRamMirror(&dehDiagInfo.verlernzaehler, EEPROM_LAYOUT_ENUM_SMSWriting);
       if (dehDiagInfo.verlernzaehler != EE_UnlearnCouner_u8)
       {
    	   DEBUG_VALUE1(DEH_SWI_TRC, MOD_DEH, "Verlernzaehler %d",dehDiagInfo.verlernzaehler);
    	   EE_UnlearnCouner_u8 = dehDiagInfo.verlernzaehler;
           (void)EEPROM_sfRamWriteOnly(
                                   (Uint8Type*)&EE_UnlearnCouner_u8,
                                   EEPROM_LAYOUT_ENUM_DEH_UnclearCounter);
       }
    }
    else
    {        
       /* Gateway ZVZ not available */
       dehDiagInfo.verlernzaehler = EE_UnlearnCouner_u8;
       DEBUG_VALUE1(DEH_SWI_WNG, MOD_DEH, "No Diagnose 1 Nachricht, Last saved Verlernzaehler %d",dehDiagInfo.verlernzaehler);
    }

    return (dehDiagInfo.verlernzaehler);
}



#ifdef DEH_SUP_SNAP_SHOT_DATA
/*============================================================================
function name    :   APL_DEH_ReadDTCSnapshotRecord()
input parameter  :   DTC         : DTC to read the snapshot records
                     BuffSize    : size of destination buffer for snapshot records
                     pNumOfBytes : pointer to a value for a number of copied bytes
                     pDestBuffer : pointer to destination buffer
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read stored snapshot records for a DTC. 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
18.10.06    GAFR    built base version 
============================================================================*/
tDEH_Return APL_DEH_ReadDTCSnapshotRecord(tDEH_DTC DTC, uint16_t BuffSize, uint16_t *pNumOfBytes, uint8_t *pDestBuffer)
{
  tDEH_Return retVal = DEH_OK;
  
  return (retVal);
}
#endif

/*============================================================================
function name    :   APL_DEH_ReadStatusOfDTC()
input parameter  :   BlockSize_u16  : number of bytes to read
                     Buffer_u8     : pointer to destination buffer
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read stored DTC status information
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
14.11.06    GIGE    built base version 
============================================================================*/
tDEH_Return APL_DEH_ReadStatusOfDTC(uint16_t BlockSize_u16, uint8_t *Buffer_u8)
{

  /* Only call in Init Part of DEH - we use an other mechanism to init DEH*/
  tDEH_Return retVal = DEH_OK;

  return (retVal);

}

/*============================================================================
function name    :   APL_DEH_WriteStatusOfDTC()
input parameter  :   BlockSize_u16  : number of bytes to write
                     Buffer_u8     : pointer to source buffer
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to write the DTC status information into EEPROM
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
14.11.06    GIGE    built base version 
============================================================================*/
tDEH_Return APL_DEH_WriteStatusOfDTC(uint16_t BlockSize_u16, uint8_t *Buffer_u8)  
{
    /* Only call in Init Part of DEH - we use an other mechanism to init DEH*/
	tDEH_Return retVal = DEH_OK;

  return (retVal);
}

/*============================================================================
function name    :   DEH_AdditionalReportEvent()
input parameter  :   tDEH_EventNumber EventNum_uT              : current event number
                     _Bool            DEH_StEV_stRepEvent_mu8  : current event state
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to write the DTC Event information into EEPROM
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
14.11.06    GIGE    built base version 
============================================================================*/
tDEH_Return DEH_AdditionalReportEvent (tDEH_EventNumber DEH_EventNumber_muT, _Bool DEH_StEV_stRepEvent_mu8)
{
  tDEH_Return  retVal_u8 =  DEH_OK;

#ifdef SUPPORT_BT_UPDATE
  if (BT_FALSE == CSHDL_bGetBuetoothUpdateActive())
    retVal_u8 = DEH_ReportEvent(DEH_EventNumber_muT, DEH_StEV_stRepEvent_mu8);
#else
  retVal_u8 = DEH_ReportEvent(DEH_EventNumber_muT, DEH_StEV_stRepEvent_mu8);
#endif
  if (DEH_StEV_stRepEvent_mu8 == TRUE)
  {
	  DEBUG_VALUE2(DEH_SWI_DBG, MOD_DEH, "DEH_EventNumber_muT: %d, DEH_StEV_stRepEvent_mu8: %d",DEH_EventNumber_muT, DEH_StEV_stRepEvent_mu8);
  }
  if (retVal_u8 != DEH_OK)
  {
	  DEBUG_VALUE1(DEH_SWI_ERR, MOD_DEH, "Attention: DTC %d isn#t stored !",DEH_EventNumber_muT);
  }
  return (retVal_u8);
}


