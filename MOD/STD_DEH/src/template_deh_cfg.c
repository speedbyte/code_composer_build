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

/* ========================================================================== */
/* Global Variables                                                           */
/* ========================================================================== */

/* =========== EVENT DATAS ================================================== */
const tDEH_EventPathParameter DEH_EventPathParameter[DEH_MAX_NUMBER_OF_EVENTPATHS];

/* =========== FREEZE FRAME TABLE =========================================== */

#ifndef  DEH_TEST
#if (DEH_MAX_NUMBER_FF>0)
  uint8_t DEH_FreezeFrameTable[DEH_MAX_NUMBER_FF_IDX][DEH_MAX_NUMBER_FF]; 
#endif
#endif

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

/*   uint8_t DEH_FreezeFrameTable[DEH_MAX_NUMBER_FF_IDX][DEH_MAX_NUMBER_FF] =  */
/*                                                          {{0x6C},{0x0A}};   */

/*  const tDEH_EventPathParameter DEH_EventPathParameter[DEH_MAX_NUMBER_OF_EVENTPATHS] */ 
/*      {                                                                   */
/*       DEH_EVENT_1.DTC                                                    */
/*      0x1234,                                                             */
/*       DEH_EVENT_1.EventParameter                                         */
/*      0x00,                                                               */
/*       DEH_EVENT_1.EventSymPrio                                           */
/*      0x12,                                                               */
/*       DEH_EVENT_1.QualEvent                                              */
/*      0x0014,                                                             */
/*       DEH_EVENT_1.DeQualEvent                                            */
/*      0x000A,                                                             */
/*       DEH_EVENT_1.LampParameter                                          */
/*      0x03,                                                               */
/*       DEH_EVENT_1.IndexFFrameTable                                       */
/*      0x00},                                                              */
/*                                                                          */
/*       DEH_EVENT_2.DTC                                                    */
/*      {0x5678,                                                            */
/*        DEH_EVENT_2.EventParameter                                        */
/*      0x01,                                                               */
/*       DEH_EVENT_2.EventSymPrio                                           */
/*      0x34,                                                               */
/*       DEH_EVENT_2.QualEvent                                              */
/*      0x0007,                                                             */
/*       DEH_EVENT_2.DeQualEvent                                            */
/*      0x0003,                                                             */
/*       DEH_EVENT_2.LampParameter                                          */
/*      0x08,                                                               */
/*       DEH_EVENT_2.IndexFFrameTable                                       */
/*      0xFF}}                                                              */
/* ======================================================================== */

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
  tDEH_Return retVal = DEH_OK;

  return (retVal);
} /* end function APL_DEH_EEPWriteEventData */


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
  uint8_t UnlearnCounter_u8 = (uint8_t) 0;

  return (UnlearnCounter_u8);
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
  tDEH_Return retVal = DEH_OK;

  return (retVal);
}


