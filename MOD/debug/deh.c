/*==============================================================================
                      Diagnostic Event Handler  (DEH)
                     ---------------------------------
                      Version 1.2.5
                      Author: Ingo Gensert/ IAV GmbH
                      Date:   29.01.2008
                      Modul:  deh.c

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
================================================================================
 
V1.2.3   NOT ERASABLE EVENTS handling
=======  =====================================================================
  1. All state bits (StatusOfDTC) for NOT erasable events are not reset or 
     cleared with the function DEH_ClearDiagnosticInformation() if the current 
     diagnostic session NOT equal to Developper Session OR Programmer Session 
================================================================================

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
================================================================================

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
================================================================================
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

/*==========================================================================*/
/* Project Includes                                                         */
/*==========================================================================*/
#include "deh.h"


/*==========================================================================*/
/* Macros                                                                    */
/*==========================================================================*/

/*==========================================================================*/
/* Defines                                                                   */
/*==========================================================================*/

/*==========================================================================*/
/* Typedefs                                                                  */
/*==========================================================================*/

/*==========================================================================*/
/* Global Variables                                                         */
/*==========================================================================*/

#ifdef DEH_TEST
    /* internal event memory buffer */
  uint16_t DEH_EventMemBuffer[DEH_MAX_NUMBER_EV_MEM];
  #ifdef DEH_FF_BUFFER_SIZE
      /*  Freeze Frame Buffer */
    tDEH_FFBuffer DEH_FF_Buffer[DEH_FF_BUFFER_SIZE];
  #endif
    /* buffer to debounce all events and store the actual event state */
  tDEH_QualET DEH_DebounceBuffer[DEH_MAX_NUMBER_OF_EVENTPATHS];

    /* buffer for all event states */ 
  tDEH_EventBufferState DEH_EventBuffer[DEH_MAX_NUMBER_OF_EVENTPATHS];

    /* array for the test not completed since last clear datas */
  uint8_t TestNC[STOFDTC_ONE_BLOCK_SIZE];

  uint8_t DEH_IntitStatusOfDTC_u8;
    /* ======================================================================== */
    /* Data definition for UDS and OBD support                                  */
    /* ======================================================================== */
  #ifdef DEH_SUP_UDS
      /* array for Test Failed Last State info*/
    uint8_t TestFLS[STOFDTC_ONE_BLOCK_SIZE];

    #ifdef DEH_SUP_UDS_OBD_STATUS
        /* array for pending DTC for the last driving cycles */
      uint8_t PendingDTC_lastDC[STOFDTC_ONE_BLOCK_SIZE];
        /* array for pending DTC for the current driving cycles */
      uint8_t PendingDTC_curDC[STOFDTC_ONE_BLOCK_SIZE];
        /* array for the test failed since last clear datas */
      uint8_t TestFailedSLC[STOFDTC_ONE_BLOCK_SIZE];
        /* array for test failed this operation cycle */
      uint8_t TestFailedTOC[STOFDTC_ONE_BLOCK_SIZE]; 
    #endif
  
  #endif

#else   /* END ... #ifdef DEH_TEST */
  static   uint16_t DEH_EventMemBuffer[DEH_MAX_NUMBER_EV_MEM];
  #ifdef DEH_FF_BUFFER_SIZE
      /*  Freeze Frame Buffer */
    static tDEH_FFBuffer DEH_FF_Buffer[DEH_FF_BUFFER_SIZE];
  #endif
    /* buffer to debounce all events and store the actual event state */
  static tDEH_QualET DEH_DebounceBuffer[DEH_MAX_NUMBER_OF_EVENTPATHS];

    /* buffer for all event states */ 
  tDEH_EventBufferState DEH_EventBuffer[DEH_MAX_NUMBER_OF_EVENTPATHS];
  static uint8_t DEH_IntitStatusOfDTC_u8;

    /* array for the test not completed since last clear datas */
  static uint8_t TestNC[STOFDTC_ONE_BLOCK_SIZE];

    /* ======================================================================== */
    /* Data definition for UDS and OBD support                                  */
    /* ======================================================================== */
  #ifdef DEH_SUP_UDS
      /* array for Test Failed Last State info*/
    static uint8_t TestFLS[STOFDTC_ONE_BLOCK_SIZE];    
    #ifdef DEH_SUP_UDS_OBD_STATUS
        /* array for pending DTC for the last driving cycles */
      static uint8_t PendingDTC_lastDC[STOFDTC_ONE_BLOCK_SIZE];
        /* array for pending DTC for the current driving cycles */
      static uint8_t PendingDTC_curDC[STOFDTC_ONE_BLOCK_SIZE];
        /* array for the test failed since last clear datas */
      static uint8_t TestFailedSLC[STOFDTC_ONE_BLOCK_SIZE];
        /* array for test failed this operation cycle */
      static uint8_t TestFailedTOC[STOFDTC_ONE_BLOCK_SIZE]; 
    #endif
  
#endif

#endif

  /* actual diagnostic type */
#ifndef  DEH_TEST
  static tDEH_DiagnosticType DEH_actDiaSes_muT;
#else
  tDEH_DiagnosticType DEH_actDiaSes_muT;
#endif

  /* internal counter to read events (supported, identified or by status) */
#ifdef DEH_TEST
  uint16_t  DEH_cntEvent_mu16;
#else 
  static uint16_t  DEH_cntEvent_mu16;
#endif

  /* message to store the last received service function */
static uint8_t DEH_SRV_lastRecFct_mu8; 

  /* message for Event Memory Fault Indication */ 
static uint8_t DEH_EventMemoryErrors_mu8;

  /* message for CAN-Event Bit for the system */ 
_Bool DEH_stCANEventBit_mb;

  /* global variable for the actual warnlamp state */
uint8_t DEH_stWarnLamp_mu8;

  /* counter for available freeze frame buffer */
static uint8_t DEH_numOfFreeFF_mu8;

  /* counter for read freeze frame period */
static uint8_t DEH_cntReadFF_mu8;

  /* counter for update event memory period */
static uint8_t DEH_cntEvUpd_mu8;

  /* counter for check warning lamp status period*/
#ifndef  DEH_TEST
  static uint8_t DEH_cntWLP_mu8;
#else
  uint8_t DEH_cntWLP_mu8;
#endif

  /* flag for update event memory */
static uint8_t DEH_stUpdateEM_b;

  /* flag read freeze frame necessary */
static uint8_t DEH_stReadFF_b;

  /* flag for event memory init indication */
static uint8_t DEH_stInitEM_b;

  /* state variable for unlearn algorithm */
static uint8_t DEH_StM_UnAlg_mu8;

  /* flag for a full event memory to improve the runtime */
static  _Bool DEH_stFullEvMem_mb;

  /* flags for indication passive/active events are in the event memory */
static _Bool DEH_stPassiveEvents_mb;

  /* smallest priority in event memory if the event memory full */
static uint8_t DEH_EM_smallestPrio_mu8;

  /* first unlearn counter after system start */
static uint8_t DEH_oldUnlearnCnt_mu8;

/*==========================================================================*/
/* Global Prototypes                                                        */
/*==========================================================================*/

/* ========================================================================== */
/* Prototypes local functions                                                 */
/* ========================================================================== */

static tDEH_Return DEH_ESiF_ClearEM(tDEH_EventMemory  MemPos_uT, uint8_t mode_u8);

static tDEH_Return DEH_ESiF_InitEM(void);

static tDEH_Return DEH_ESiF_WriteEM(uint16_t  EventNumber_u16,
                                    uint8_t    eventstate_u8);
static tDEH_Return DEH_ESiF_UpdateEM(uint16_t          EventNumber_u16,
                                     uint8_t          NewEventState_u8,
                                     tDEH_EventMemory  EvMemoryPos_uT);

static tDEH_Return DEH_ESiF_LookForPosEM(uint16_t         Event_u16,
                                         tDEH_EventMemory  *position_uT);

static tDEH_Return DEH_ESiF_LookForPosToOvEM( uint16_t          EventNumber_u16,
                                              tDEH_EventMemory  *Position_uT);

static tDEH_Return DEH_ESiF_TstLUCandClearEM(tDEH_EventMemory EvMemoryPos_uT);

static tDEH_Return DEH_ESiF_UnlearnAlg(void);

#if (!defined(DEH_TEST))
  static uint8_t     DEH_SUiF_StatusOfDTC(uint16_t  EventNumber_u16,uint8_t  mode_u8);
#endif

static void        DEH_FFiF_GetStdFF(uint16_t              EventNumber_u16,
                                     tDEH_StdFreezeFrame  *StdFF );

#ifdef DEH_SUP_KWP2000
static void        DEH_FFiF_GetAddFF(uint16_t             EventNumber_u16,
                                     tDEH_AddFreezeFrame  *AddFF);
#endif
                                     
static tDEH_Return DEH_FFiF_InitFF(void);
static tDEH_Return DEH_FFiF_ReadFF( uint16_t EventNumber_u16);
static tDEH_Return DEH_FFiF_ClearFF(uint16_t EventNumber_u16);
static tDEH_Return DEH_StM_Event_cyclic(void);
static tDEH_Return DEH_StM_Memory(uint8_t   mode_u8, 
                                  uint16_t  EventNumber_u16);
#ifdef DEH_TEST
  tDEH_Return DEH_SUiF_InitStatusOfDTC(void);
#else
  static tDEH_Return DEH_SUiF_InitStatusOfDTC(void);
#endif 


static void DEH_SUiF_ClearStatusOfDTCInfos (void);
#ifdef DEH_SUP_UDS
static void DEH_FFiF_GetExtFF(uint16_t EventNumber_u16, uint8_t *ExtDataRecord_u8);
#endif
/* ========================================================================== */
/* ========================================================================== */


/* ============================================================================
function name    :   DEH_ClearDiagnosticInformation(tDEH_DTCGroup DTCGroup)
input parameter  :   tDEH_DTCGroup DTCGroup
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to clear the Event-Memory (all entries in EEPROM)
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
12.10.03    GIGE    built base version 

============================================================================= */
tDEH_Return DEH_ClearDiagnosticInformation(tDEH_DTCGroup DTCGroup)
{
    /* tmp. Variable for Return value -> init with DEH_OK */
  tDEH_Return       retVal_u8      =                    DEH_OK;
  tDEH_EventMemory  i              = (tDEH_EventMemory) 0;
  uint16_t          k              = (uint16_t)         0;
  _Bool             stClear_b      = (_Bool)            TRUE;
  _Bool             event_locked_b = (_Bool)            FALSE;
    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
    if (DTCGroup == DEH_DTC_ALL_GROUPS)
    {
        /* call function to INIT the freeze frame buffer */
      DEH_FFiF_InitFF();

      while ( (i < ((tDEH_EventMemory )DEH_MAX_NUMBER_EV_MEM )) && (stClear_b) )
      {
      
         /* test if a valid event in the internal event buffer */
        if (    (DEH_EventMemBuffer[i] != DEH_EM_FREE_POS)  
             && (DEH_EventMemBuffer[i] < DEH_MAX_NUMBER_OF_EVENTPATHS) )
        {
            /* clear the external event buffer */
          retVal_u8 = DEH_ESiF_ClearEM(i, DEH_CLEAR_DIAG_INFO);

            /* update the internal information after clear event */
          if ( (retVal_u8 != DEH_OK) &&  (retVal_u8 != DEH_E_CLEAR) )
          {
              /* set flag for abort the clear function */
            stClear_b = (_Bool) FALSE;

              /* set function return code */
            retVal_u8 = DEH_E_BUSY;
          }
        }

          /* increase the internal counter */
        i++;
      }

        /* clear all event states after for all events after successfull clear */
      if (stClear_b)
      {
          /* set the flag for event memory NOT full*/
        DEH_stFullEvMem_mb = (_Bool)FALSE;

          /* clear all actual event states to not tested and PASSIVE */
        for (k = (uint16_t) 0 ; k < ((uint16_t) DEH_MAX_NUMBER_OF_EVENTPATHS ); k++)
        {
            /* update the eventstate parameter if the event erasable        */
            /* there are two conditions for NOT erasable events :           */
            /* I.I   event is marked as NOT erasable AND                    */
            /* I.II  current diagnosis session is NOT the developer session */
            /* OR                                                           */
            /* II.I event with prio 9 AND                                   */
            /* II.II current diagnosis session is NOT the developer session */


              /* test if the event LOCKED (Bit 3 == 1 ) */
          if (DEH_EVENT_BUFFER_STATE(k) & DEH_EB_MSK_LOCK_DTC) 
          {
             event_locked_b = (_Bool) TRUE;
          }

            /* set the actual debounce value to stop value*/
          DEH_EVENT_DEBOUNCE(k) = DEH_DEB_STOP;

            /* special case 1:                                                */
            /* I) Event is NOT erasable  AND                                  */
            /* II)   current diagnostic session is Developper                 */
            /*    OR Programmer ses. BUT NO PRIO 9                            */
            /* YES => event can be cleared                                    */
          if(  (DEH_EVENT_PARAMETER(k) & DEH_EP_MSK_EV_NO_CLEAR)
             &&(   (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION )
                  ||((DEH_actDiaSes_muT == DEH_ECU_PRG_DIAG_SESSION )
                    &&(( DEH_GET_EVENT_PRIORITY(k) != DEH_DEVELOP_EVENT_PRIO)))))
          {
              /* set the actual event state to PASSIVE and DTC NOT TESTED */
            DEH_EVENT_BUFFER_STATE(k) = (uint8_t) (   DEH_EVENT_PASSIVE 
                                                    | DEH_EB_MSK_DTC_RB );
          }
            /* second special case will be tested                   */
            /* I ) Event has Prio 9                                 */
            /* AND                                                  */
            /* II) Current diagnosis session is DEVELOPER session   */
          else if(    (DEH_GET_EVENT_PRIORITY(k)== DEH_DEVELOP_EVENT_PRIO)
                   && (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION ))
          {
              /* set the event to PASSIVE STORED and DTC NOT TESTED*/
            DEH_EVENT_BUFFER_STATE(k) = (uint8_t) (   DEH_EVENT_PASSIVE 
                                                    | DEH_EB_MSK_DTC_RB );
          } 
            /* 1) NO Prio 9 Event        AND */
            /* 2) Event is Erasable          */
          else if(    (DEH_GET_EVENT_PRIORITY(k)!= DEH_DEVELOP_EVENT_PRIO)
                   && (!(DEH_EVENT_PARAMETER(k) & DEH_EP_MSK_EV_NO_CLEAR)))
          {
            /* set the event to PASSIVE and SET Readiness Bit*/
               DEH_EVENT_BUFFER_STATE(k) = (uint8_t) (   DEH_EVENT_PASSIVE 
                                                       | DEH_EB_MSK_DTC_RB ); 
          }

            /* check the old LOCKED state and set again if the event was locked */
          if (event_locked_b)
          {
              /* set the event state to LOCKED DTC */
            DEH_EVENT_BUFFER_STATE(k) |= (uint8_t) DEH_EB_MSK_LOCK_DTC;
          } /* if locked */
        } /* for */
      } /* if stClear_b*/
    } /* end if start Clear */
    else
    {
        /* return value is unknown DTC-Group */
      retVal_u8 = DEH_E_UNKNOWN_DTCGROUP;
    } /* end else wrong DTC Group*/
  
      /* call function to reset all internal state flags */
    DEH_SUiF_ClearStatusOfDTCInfos();

      /* inform the application about event memory clear if */
      /* SNAP SHOT SUPPORT is defined                       */
    #if defined (DEH_SUP_SNAP_SHOT_DATA)
      APL_DEH_SnapShotDatenInit(DEH_CLEAR_EVENT_MEMORY);
    #endif
  }
  else  /* no correct init*/
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }
    /* store the last received service function */
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_CLEAR_DIAG_INF;

  return(retVal_u8);
} /* end of function DEH_ClearDiagnosticInformation () */


/*============================================================================
function name    :  DEH_ControlDTCSetting ()
input parameter  :  tDEH_DTCSetType DTCSetType
                    tDEH_TypeOfDTC  TypeOfDTC
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to control the storage of detected  events
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
12.10.03    GIGE    built base version 

============================================================================*/
tDEH_Return DEH_ControlDTCSetting (tDEH_DTCSetType DTCSetType, tDEH_TypeOfDTC TypeOfDTC)
{
  tDEH_Return       retVal_u8 =  DEH_E_UNKNOWN_DTC;
  tDEH_EventNumber  i;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {

      /* test for correct DTCSeType (ON/OFF) */
    if ( (DTCSetType != DEH_SET_DTC_ON) && (DTCSetType != DEH_SET_DTC_OFF))
    {
       retVal_u8 = DEH_E_MODE;
    }
    else
    {
      switch (DEH_actDiaSes_muT)
      {

  #ifdef DEH_SUP_KWP2000 
        case DEH_VW_DEFAULT_DIAG_SESSION:
  #endif
        case DEH_EXT_DIAG_SESSION:

        case DEH_VW_EOL_DIAG_SESSION:
    
  #ifdef DEH_SUP_KWP2000 
        case DEH_EOBD_DIAG_SESSION:
  #endif
        case DEH_ECU_PRG_DIAG_SESSION:

        case DEH_ENTW_DIAG_SESSION:


              /* test if correct DTC value within range 0x001 - 0xFFF1 OR 0xFFFF */
            if (     ( TypeOfDTC == DEH_DTC_ALL )
  #ifdef DEH_SUP_KWP2000 
                 ||  (    ( TypeOfDTC != DEH_DTC_NO ) 
                       && ( TypeOfDTC <= DEH_DTC_COM ) )
  #endif 
               )
            {
               
                /* analyse all event path parameter */
              for (i =((tDEH_EventNumber) 0); i < ((tDEH_EventNumber) DEH_MAX_NUMBER_OF_EVENTPATHS) ; i++ )
              {
                  /* test if the actual read DTC the correct DTC */

                if (  (TypeOfDTC == DEH_DTC_ALL)
    #ifdef DEH_SUP_KWP2000 
                      || (TypeOfDTC == DEH_EVENT_DTC(i))
    #endif
                   )
                {
                    /* set the correct return value */
                  retVal_u8 = DEH_OK;

                    /* test the DTC Set Mode and set the correct mask */
                  if (DTCSetType == DEH_SET_DTC_OFF )
                  {
                      /* set the DTC storage bit */
                    DEH_EVENT_BUFFER_STATE(i)|= DEH_EB_MSK_LOCK_DTC; 
                  }
                  else if (DTCSetType == DEH_SET_DTC_ON )
                  {
                      /* clear (reset) the DTC storage bit */
                    DEH_EVENT_BUFFER_STATE(i)&= DEH_EB_MSK_FREE_DTC; 
                    
                      /* test the current state                  */
                      /* if the event DEFECT and NOT STORED then */
                      /* change the state back to PASSIVE        */
                    if (    DEH_IS_EVENT_DEFECT(i)
                         && (!(DEH_EVENT_IS_STORED(i))))
                    {
                        /* set the event state to PASSIVE (bit0-2) = 0 */
                        /* and set LOCKED bit (bit3) = 0               */
                      DEH_EVENT_BUFFER_STATE(i)&= 0xF0; 
                    }
                  }
                } /* end if */
              } /* end for */
             }
          break;

          default:
          {
              /* return value is wrong Diagnose-Session */
            retVal_u8 = DEH_E_WRONG_DIAG_SESSION;
          } /* end else wrong DTC Group*/
          break;
      } /* end switch */
  
        /* store the last received service function */
      DEH_SRV_lastRecFct_mu8 = DEH_SRV_CTRL_DTC_SET;

    }     /* else path for correct DTCSeType (ON/OFF) */
  }
  else /* no correct init */
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }

  return(retVal_u8);
} /* end of function DEH_ControlDTCSetting () */



/*============================================================================
function name    :  DEH_ClearEvent(tDEH_EventNumber EventNumber_uT))
input parameter  :  EventNumber_uT : Eventnumber
output parameter :   
globals          :   
return value     :  tDEH_Return
description      :  Function to clear an eventnumber number from the
                    event memory and reset the internal state.

------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
20.04.04    GIGE      built base version 

============================================================================*/
tDEH_Return DEH_ClearEvent(tDEH_EventNumber EventNumber_uT)
{
  tDEH_Return       retVal_u8 = DEH_OK;  
  tDEH_EventMemory  PosEM_uT;

    /* test for a valid event number  */
  if (EventNumber_uT < DEH_MAX_NUMBER_OF_EVENTPATHS)
  {
      /* test if event clear possible or not */
    if (!(DEH_EVENT_PARAMETER(EventNumber_uT) & DEH_EP_MSK_EV_NO_CLEAR))
    {
      
        /* test if event stored or not (bit DEH_EB_MSK_EV_STORED is set) */
      if (DEH_EVENT_BUFFER_STATE(EventNumber_uT) & DEH_EB_MSK_EV_STORED)
      {
      
          /* look for given event number */
        retVal_u8 = DEH_ESiF_LookForPosEM((uint16_t)EventNumber_uT, &PosEM_uT);

          /* check if a valid position found in the event memory buffer */
        if (PosEM_uT != DEH_EM_NO_VALID_POS)
        {
            /* call the function to clear the event memory position */
          retVal_u8 = DEH_ESiF_ClearEM(PosEM_uT, DEH_CLEAR_EVENT);
            /* reset last state to PASSIVE*/
          if(retVal_u8==DEH_OK)
          {
#ifdef DEH_SUP_UDS
              /* reset flag for last test state from ACTIVE to PASSIVE */
            DEH_CLR_TEST_LAST_STATE(EventNumber_uT);
  #ifdef DEH_SUP_UDS_OBD_STATUS
              /* reset flag for Test Failed This Operation Cycle */
            DEH_CLR_TEST_FAILED_TOC(EventNumber_uT);
  #endif
#endif
          }
        }
      }
      else
      {
          /* reset the internal event buffer value to NOT TESTED and PASSIVE */
        DEH_EVENT_BUFFER_STATE(EventNumber_uT) = (uint8_t) 
                                                        (   DEH_EVENT_PASSIVE 
                                                          | DEH_EB_MSK_DTC_RB);
#ifdef DEH_SUP_UDS
          /* reset last state to PASSIVE*/
        DEH_CLR_TEST_LAST_STATE(EventNumber_uT);
#endif
      }
    } 
    else
    {
        /* return code for blocked clear */
      retVal_u8 = DEH_E_CLEAR;
    }
  }

  else
  {
      /* set the return code */
    retVal_u8 = DEH_E_UNKNOWN_EVENTPATH;
  }

  return (retVal_u8);
}


/*============================================================================
function name    :  DEH_ESiF_ClearEM())
input parameter  :  MemPos_uT : position to clear in the event memory
output parameter :   
globals          :   
return value     :  tDEH_Return
description      :  Function to clear a position from the event memory.
                    

------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
20.04.04    GIGE      built base version 

============================================================================*/
static tDEH_Return DEH_ESiF_ClearEM(tDEH_EventMemory  MemPos_uT, uint8_t mode_u8)
{
  tDEH_Return       retVal_u8   = DEH_E_UNKNOWN_EVENTPATH;
  uint8_t           Buffer_u8[2];
  uint16_t          OldEvent_u16 ;

    /* look for correct position and update the event memory state */
  if (MemPos_uT < DEH_MAX_NUMBER_EV_MEM)
  {
      /* read the stored internal event number */
    OldEvent_u16 = DEH_EventMemBuffer[MemPos_uT];

    if (OldEvent_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS)
    {
        /* test if event clear possible or not                                */
        /* I)  current diagnosis session is Developer Session (Entwickler) OR */
        /* II.) Event NOT equal prio 9                                     AND*/
        /* III) Event is erasable                                          OR */
        /* IV)  Event is NOT erasable && diagnosis session == Progr. Session  */
      if (     ( DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION )
            || (( mode_u8 == DEH_CLEAR_EVENT)
            || (( DEH_GET_EVENT_PRIORITY(OldEvent_u16) != DEH_DEVELOP_EVENT_PRIO)
                  &&(  (!(DEH_EVENT_PARAMETER(OldEvent_u16) & DEH_EP_MSK_EV_NO_CLEAR))
                    || ((DEH_EVENT_PARAMETER(OldEvent_u16) & DEH_EP_MSK_EV_NO_CLEAR)
                         && ( DEH_actDiaSes_muT == DEH_ECU_PRG_DIAG_SESSION )))))
         )
      {
          /* mark the memory position as cleared                */
          /* the lowbyte first then the highbyte of EventNumber */
        Buffer_u8[EMB_POS_EV_NUM_LOWBYTE]   = (uint8_t) DEH_EM_FREE_POS;
        Buffer_u8[EMB_POS_EV_NUM_HIGHBYTE]  = (uint8_t)(DEH_EM_FREE_POS >> 8);

          /* clear the external event buffer */
        retVal_u8 = APL_DEH_EEPWriteEventData( (uint16_t)( EMB_BLOCK_SIZE * MemPos_uT),
                                               (uint16_t) EMB_BL_SIZE_EV_NUM,
                                               &Buffer_u8[0]                     );

          /* test the result and set failure flag */
        if (retVal_u8 == DEH_OK)
        {
            /* clear the internal buffer */
          DEH_EventMemBuffer[MemPos_uT] = DEH_EM_FREE_POS;

            /* test if the event LOCKED (Bit 3 == 1 ) */
          if (DEH_EVENT_BUFFER_STATE(OldEvent_u16) & DEH_EB_MSK_LOCK_DTC) 
          {
              /* set the actual event state to PASSIVE and DTC NOT TESTED */
              /* and LOCKED DTC                                           */
            DEH_EVENT_BUFFER_STATE(OldEvent_u16) = (uint8_t) 
                                                    (   DEH_EVENT_PASSIVE 
                                                      | DEH_EB_MSK_DTC_RB
                                                      | DEH_EB_MSK_LOCK_DTC);
          }
          else
         {
              /* set the actual event state to PASSIVE and DTC NOT TESTED */
            DEH_EVENT_BUFFER_STATE(OldEvent_u16) = (uint8_t) 
                                                    (   DEH_EVENT_PASSIVE 
                                                      | DEH_EB_MSK_DTC_RB);
          }
        }
        else if (retVal_u8 != DEH_E_BUSY)
        {
           /* set failure flag in global error message */
         DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_WRITE;
        }
      } 
      else
      {
          /* return code for blocked clear */
        retVal_u8 = DEH_E_CLEAR;
      }
    }
  }  /* end if (parameter_u8 < DEH_MAX_NUMBER_EV_MEM) */

  return (retVal_u8);
}


/*============================================================================
function name    :  DEH_ESiF_InitEM(void))
input parameter  :  - 
output parameter :  - 
globals          :  - 
return value     :  tDEH_Return
description      :  Function to read all event memory entries and update the 
                    event state in the DEH_EventBuffer[x] array.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 

============================================================================*/
static tDEH_Return DEH_ESiF_InitEM(void)
{
    /* internal variables */
  tDEH_Return       retVal_u8       = DEH_OK;
  tDEH_Return       retVal2_u8;
  uint16_t          EventNumber_u16;
  uint8_t           StOfDTC_u8      = (uint8_t) 0;
  tDEH_EventMemory  i               = (tDEH_EventMemory) 0;
  uint8_t           Buffer_u8[EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC];

    /* read all event entries from external memory                       */
    /* finish if a invalid entry in the DEH event memory and the whole   */
    /* memory is cleared                                                 */

  while (    (i         < DEH_MAX_NUMBER_EV_MEM) 
          && (retVal_u8 != DEH_E_UNKNOWN_EVENTPATH)
          && (retVal_u8 != DEH_E_INIT_MEMORY))
  {
      /* read the stored event number  */
    retVal_u8 = APL_DEH_EEPReadEventData( 
                        (uint16_t) ( (EMB_BLOCK_SIZE*i) + EMB_POS_EV_NUM_LOWBYTE),
                        (uint16_t) (EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC),
                                  &Buffer_u8[0]);

      /* test for sucessfull external event buffer read */
    if (retVal_u8 == DEH_OK)
    {
        /* read out the event number information              */
        /* the lowbyte first then the highbyte of EventNumber */
      EventNumber_u16  =   (uint16_t)Buffer_u8[EMB_POS_EV_NUM_LOWBYTE];
      EventNumber_u16 |= (uint16_t)((Buffer_u8[EMB_POS_EV_NUM_HIGHBYTE]) << 8);

        /* read out the status of DTC information*/
      StOfDTC_u8 = Buffer_u8[EMB_POS_STOFDTC];

      /* test for a valid entry in the parameter EventNumber_uT */
      if ( (EventNumber_u16 != DEH_EM_FREE_POS)            &&
           (EventNumber_u16 >= DEH_MAX_NUMBER_OF_EVENTPATHS)  )
      {
          /* set the function return value */
        retVal_u8 = DEH_E_UNKNOWN_EVENTPATH ;

          /* set failure flag in global error message */
        DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EVNUM_EM_INIT;

          /* clear the whole EEPROM Memory because any inconsistence problem */
        for (i = (tDEH_EventMemory) 0 ; i < ((tDEH_EventMemory) DEH_MAX_NUMBER_EV_MEM ); i++)
         {
             /* call the function to clear the act position i in event memory */
           retVal2_u8 = DEH_ESiF_ClearEM(i, DEH_CLEAR_EVENT);

           if (retVal2_u8 != DEH_OK)
           {
              /* set failure flag in global error message */
            DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EM_CLEAR;  

              /* set the return code for the function */
            retVal_u8 = DEH_E_INIT_MEMORY;
           
           } /* end the for loop to clear Event Memory */
         }  /* end for loop */
      }    /* end if test event number */
          /* correct event number is read and now start the system INIT-Fct. */
      else
      {
          /* write the read event number in the internal buffer */
        DEH_EventMemBuffer[i] = EventNumber_u16;

          /* test for a valid event DEH_ERR_MSK_EM_CLEAR entry in the array */
          /* and update DTC state                                           */

        if (EventNumber_u16 != DEH_EM_FREE_POS)
        {
            /* set the flag for event memory stored indication */
          DEH_EVENT_BUFFER_STATE(EventNumber_u16) |= DEH_EB_MSK_EV_STORED;

            /* test for ACTIVE STORED EVENTS and start with special init state */
          if ((StOfDTC_u8 & DEH_EM_ACTIVE_STORED) == DEH_EM_ACTIVE_STORED)
          {
              /* check if the event erasable because NOT set of NO CLEAR bit */
            if (!(DEH_EVENT_PARAMETER( EventNumber_u16) & DEH_EP_MSK_EV_NO_CLEAR))
            {
                /* set the init state for this event in event state buffer */
              DEH_EVENT_BUFFER_STATE(EventNumber_u16) |= (uint8_t) 
                                                       DEH_EVENT_DEB_ACT_STO;
            }
              /* event can NOT be cleared from event memory -> start state */
              /* is ACTIVE                                                 */
            else
            {
                /* set the init state for this event in event state buffer */
              DEH_EVENT_BUFFER_STATE(EventNumber_u16) |= (uint8_t) 
                                                       DEH_EVENT_ACTIVE;
            }
          }
        } /* end if (EventNumber_uT <> DEH_EM_FREE_POS) */
      } /* end else */
    }
    else
    {
         /* set failure flag for EEP Read problems */
      DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_READ; 

        /* for all read error codes ->clear the actual memory position */
      if (retVal_u8 != DEH_E_INIT_MEMORY)
      {
          /* clear the actual memory position */
        DEH_EventMemBuffer[i] = DEH_EM_FREE_POS;

          /* set the return code */
        retVal_u8 = DEH_E_READ_MEMORY;
      }

    } /* end else (retVal_u8 == DEH_OK) */

      /* increment the counter to read the external event memory entries */
    i++;
  } /* end while */
    
return (retVal_u8);
}


/*============================================================================
function name    :  DEH_ESiF_LookForPosEM ()
input parameter  :  Event_u16 : event or indication for free position
                    *position_uT : address for the found result;
output parameter :   
globals          :   
return value     :  Returncode for sucessfull search or not.
description      :  This function looks for the received event in the event 
                    memory. If the Event_u16 == DEH_EM_FREE_POS than is this 
                    function looking for a free position.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
11.11.03    GIGE    built base version 

============================================================================*/
static tDEH_Return DEH_ESiF_LookForPosEM(uint16_t Event_u16, tDEH_EventMemory *position_uT)
{
  tDEH_EventMemory  i         = (tDEH_EventMemory) 0 ;
  tDEH_Return        retVal_u8 = (tDEH_Return) DEH_E_NO_DATA;

    /* init to a not valid event memory position */
  *position_uT = DEH_EM_NO_VALID_POS;

  while ((i < DEH_MAX_NUMBER_EV_MEM) && (retVal_u8 == DEH_E_NO_DATA))
  {
      /* if the position in the array equal the wanted Event_T */
    if (DEH_EventMemBuffer[i] == Event_u16)
    {
        /* set flag to abort the while loop */
      retVal_u8 = (tDEH_Return) DEH_OK;

        /* store the first free position in the variable */
      *position_uT = i;

    }

      /* increment the internal counter */
    i++;

  } /* end while */
  
  if (retVal_u8 == DEH_E_NO_DATA)
  {
    DEH_stFullEvMem_mb = (_Bool)TRUE;
  }
  else
  {
    DEH_stFullEvMem_mb = (_Bool)FALSE;
  }
  return(retVal_u8);
}


/*============================================================================
function name    :   DEH_ESiF_LookForPosToOvEM
input parameter  :   *Position      : pointer to the found position
                     EventNumber_u16 : event number for read freeze frames
output parameter :   
globals          :   
return value     :   
description      :   Function to lock for a event memory entry to overwrite.

------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
14.11.03    GIGE    First implementation
============================================================================*/
static tDEH_Return DEH_ESiF_LookForPosToOvEM(  uint16_t EventNumber_u16, tDEH_EventMemory *Position_uT )
{
  tDEH_Return         retVal_u8          = DEH_E_NO_DATA;
  tDEH_EventMemory    i;
  uint16_t            Event_u16;
  uint8_t             smallestPrio_u8    = (uint8_t) 0x00;
  uint8_t             tmp_prio_u8; 
  uint8_t             Buffer_u8[ EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC + EMB_BL_SIZE_LUC ];
  tDEH_EventMemory    PasEvPos_uT        =  DEH_EM_NO_VALID_POS;
  tDEH_EventMemory    ActEvPos_uT        =  DEH_EM_NO_VALID_POS;
  uint16_t            ActSmallestDif_u16 = (uint16_t) 0xFFFF;
  uint16_t            PasSmallestDif_u16 = (uint16_t) 0xFFFF;
  uint16_t            DifLucUnlCnt_u16;
  uint8_t             tmp_UnlearnCnt_u8;

    /* first search: look for smallest stored Priority         */
    /* --------------------------------------------------------*/

    /* T0_1_1 .. START */


  for (i =((tDEH_EventMemory) 0) ; i < ((tDEH_EventMemory) DEH_MAX_NUMBER_EV_MEM) ; i++)
  {
      /* read the stored event number */
    Event_u16 = DEH_EventMemBuffer[i];

    if (Event_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS)
    {
        /* read the connected priority from the dataset */
      tmp_prio_u8 =(uint8_t) DEH_GET_EVENT_PRIORITY(Event_u16);

        /* ------------------------------------------------------- */
        /* warning::: prio 0 is highest prio and prio 15 is lowest */
        /* ------------------------------------------------------- */
      if (tmp_prio_u8 > smallestPrio_u8)
      {
          /* store the lowest priority (highest value )*/
        smallestPrio_u8 = tmp_prio_u8;

      } /* end prio   */
    } /* end if ( Event_uT < DEH_MAX_NUMBER_OF_EVENTPATHS  )*/

      /* set the abort conditions */
    else
    {
        /* set error message for event memory searching */
       DEH_EventMemoryErrors_mu8 |= DEH_EM_MSK_NO_PLAUS_EVNUM;
    }
  } /* end for        */
    /* T0_1_1 .. END */

      /* set message for system info */

  DEH_EM_smallestPrio_mu8 = smallestPrio_u8;

    /* T1_1_0 ... START */
  if ( smallestPrio_u8 < DEH_GET_EVENT_PRIORITY(EventNumber_u16))
  {
    *Position_uT = DEH_EM_NO_VALID_POS;
  } /* T1_1_0 ... END */
  
      /* T1_2_2 ... START */
  else
  {
    DEH_stPassiveEvents_mb = (_Bool)FALSE;

    for (i =((tDEH_EventMemory) 0) ; i < ((tDEH_EventMemory) DEH_MAX_NUMBER_EV_MEM) ; i++)
    {
        /* read the stored event number */
      Event_u16 = DEH_EventMemBuffer[i];
      
      if ( Event_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS  )
      {
          /* read the connected priority from the dataset */
        tmp_prio_u8 = (uint8_t) DEH_GET_EVENT_PRIORITY(Event_u16);

          /* test if the actual prio equal the smallest found prio*/
        if (smallestPrio_u8 == tmp_prio_u8)
        {
            /* read the actual event state for this event from event memory*/
          retVal_u8 = APL_DEH_EEPReadEventData( (uint16_t) (EMB_BLOCK_SIZE*i) ,
                                                (uint16_t) (EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC + EMB_BL_SIZE_LUC),
                                                         &Buffer_u8[0]); 
            /* test result and set failure flag */
          if (retVal_u8 != DEH_OK)
          {
               /* set failure flag for EEP Read problems */
            DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_READ; 
          }

            /* get only the Event Memory Storage State in statusOfDTC */
          Buffer_u8[EMB_POS_STOFDTC] &= DEH_EM_ACTIVE_STORED;

            /* read the global Unlearn Counter */
          tmp_UnlearnCnt_u8 = APL_DEH_ReadUnlearnCounter();

            /* built the difference stored LUC and global unlearn counter */
          if (Buffer_u8[EMB_POS_LUC] > tmp_UnlearnCnt_u8)
          {
            DifLucUnlCnt_u16 =(uint16_t) (Buffer_u8[EMB_POS_LUC] - tmp_UnlearnCnt_u8);
          }
          else
          {
              /* calculate the difference with overflow */
            DifLucUnlCnt_u16 =(uint16_t) (Buffer_u8[EMB_POS_LUC] + DEH_LUC_OVERFLOW - tmp_UnlearnCnt_u8);
          }

          if (Buffer_u8[EMB_POS_STOFDTC] == DEH_EM_ACTIVE_STORED )
          {
            if (DifLucUnlCnt_u16 < ActSmallestDif_u16)
            {
                /* store the found smaller difference */
              ActSmallestDif_u16 = DifLucUnlCnt_u16;

                /* store the connected event position */
              ActEvPos_uT = i;
            }
          } /* end if ACTIVE STORED */
          else if (Buffer_u8[EMB_POS_STOFDTC] == DEH_EM_PASSIVE_STORED )
          {
            if (DifLucUnlCnt_u16 < PasSmallestDif_u16)
            {
                /* store the found smaller difference */
              PasSmallestDif_u16 = DifLucUnlCnt_u16;

                /* store the connected event position */
              PasEvPos_uT =i;

                /* set global flags for indication PASSIVE events in event mem.*/
              DEH_stPassiveEvents_mb = (_Bool) TRUE;

            }
          }/* end else if PASSIVE STORED */
        } /* end if (smallestPrio_u8 == tmp_prio_u8) */
      }  /*  if ( Event_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS  ) */
    }   /* end for (i = 0 ; i < DEH_MAX_NUMBER_EV_MEM; i++) */
  }   /* end else */
    /* ----------------*/
    /* T1_2_2 ... END */
    /* ----------------*/

    /* T2_1_0 ... START */
  if ( PasEvPos_uT != DEH_EM_NO_VALID_POS)
  {
      /* write back the found position */
    *Position_uT = PasEvPos_uT;

      /* set the correct return value */
    retVal_u8 = DEH_OK;
  }
   /* T2_1_0 ... END */

   /* T2_2_0 ... START */
  else if ( ( ActEvPos_uT != DEH_EM_NO_VALID_POS ) && 
            ( smallestPrio_u8 > DEH_GET_EVENT_PRIORITY(EventNumber_u16) ) )
  {
      /* write back the found position */
    *Position_uT = ActEvPos_uT;

      /* set the correct return value */
    retVal_u8 = DEH_OK;
    
  } 
    /* T2_3_0 ... START */
  else
  {
      /* write back a no valid position */
    *Position_uT = DEH_EM_NO_VALID_POS;

      /* set the correct return value for no successfull search*/
    retVal_u8 = DEH_E_NO_DATA;
  } /* T2_3_0 ... END */

  return (retVal_u8);
}


/*============================================================================
function name    :  DEH_ESiF_TstAndClearEM ()
input parameter  :  EventNumber_uT   : actual EventNumber_uT
                    actUnlaernCnt_u8 : actual global unlearn counter 
output parameter :   
globals          :   
return value     :   
description      :  Function to read the stored Unlearn Counter and test 
                    if the actual global Unlearn Counter equal the stored value.
                    If this true then should be clear the entry and set the .
                    EventNumber_uT to DEH_EM_FREE_POS (0xFF).
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
12.11.03    GIGE    built base version 

============================================================================*/
static tDEH_Return DEH_ESiF_TstLUCandClearEM(tDEH_EventMemory EvMemoryPos_uT)
{
  tDEH_Return      retVal_u8 = DEH_OK;
  uint8_t          Buffer_u8[EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC + EMB_BL_SIZE_LUC];
  uint8_t          tmp_GUC_u8;


  if (EvMemoryPos_uT <DEH_MAX_NUMBER_EV_MEM )
  {
      /* read EventNumber_uT, Status Of DTC and LUC from event Memory */
    retVal_u8 = APL_DEH_EEPReadEventData(
                    (uint16_t) (EMB_BLOCK_SIZE * EvMemoryPos_uT),
                    (uint16_t) (EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC + EMB_BL_SIZE_LUC),
                             &Buffer_u8[0]       );

      /* read the global unlearn counter */
    tmp_GUC_u8 = APL_DEH_ReadUnlearnCounter();

      /* test if the stored limit equal to the actual global unlearn counter */
      /* and the global unlearn counter is not equal to 0xFF                 */
    if (   (retVal_u8 == DEH_OK) 
        && (Buffer_u8[EMB_POS_LUC] == tmp_GUC_u8) 
        && (tmp_GUC_u8 != DEH_NO_VALID_UNLEARN_COUNTER))
    {

        /* set the internal Buffer value to EM FREE POS       */
        /* the lowbyte first then the highbyte of EventNumber */
      Buffer_u8[EMB_POS_EV_NUM_LOWBYTE]   = (uint8_t) DEH_EM_FREE_POS;
      Buffer_u8[EMB_POS_EV_NUM_HIGHBYTE]  = (uint8_t)(DEH_EM_FREE_POS >> 8);

        /* write the buffer back to external event memory */
      retVal_u8 = APL_DEH_EEPWriteEventData( 
                                 (uint16_t) (EMB_BLOCK_SIZE * EvMemoryPos_uT) ,
                                 (uint16_t)  EMB_BL_SIZE_EV_NUM,
                                            &Buffer_u8[0]                     );

        /* test the result and set failure flag */
      if (retVal_u8 != DEH_OK)
      {
         /* set failure flag in global error message */
       DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_WRITE;
      }
      else 
      {
          /* set the correct return value that the entry is cleared */
        retVal_u8 = DEH_EM_CLR_EV;

          /* update the internal event memory buffer to EM FREE POS */
        DEH_EventMemBuffer[EvMemoryPos_uT] = DEH_EM_FREE_POS;

      } /* end if (retVal_u8 == DEH_OK) */

    } /* end if (Buffer[EMB_POS_LUC] == actUnlaernCnt_u8)*/
  } /* if ((retVal_u8 == DEH_OK) && (PosEM_uT <DEH_MAX_NUMBER_EV_MEM )) */
return (retVal_u8);
} /* end function DEH_ESiF_TstLUCandClearEM() */


/*============================================================================
function name    :  DEH_ESiF_UpdateEM ()
input parameter  :  
                   -  EventNumber_u16  : actual Eventnumber
                   - NewEventState_u8 : new event state for the event
                   -  EvMemoryPos_uT  : position in the event memory
output parameter :   
globals          :   
return value     :   
description      :  Function to update the event memery with new parameter for
                    the event like changes PASSIVE -> ACTIVE and actual 
                    unlearn counter und StatusOfDTC byte
                    If no freeze frame stored then will be all output data
                    set to 0xFF.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
11.11.03    GIGE    built base version 

============================================================================*/
static tDEH_Return DEH_ESiF_UpdateEM(uint16_t         EventNumber_u16,
                                     uint8_t          NewEventState_u8, 
                                     tDEH_EventMemory EvMemoryPos_uT)
{
  tDEH_Return      retVal_u8         = DEH_OK;
  uint8_t          blocksize_u8      = (uint8_t) 0; 
  uint16_t         tmp_LUC_u16       = (uint16_t) 0;
  uint8_t          Buffer_u8[EMB_BLOCK_UPDATE_EM];
  uint8_t          tmp_UnlearnCnt_u8 = (uint8_t) 0;

  if (EvMemoryPos_uT <DEH_MAX_NUMBER_EV_MEM )
  {
      /* calculate the Blocksize for Update Event Memory */
    blocksize_u8 = (uint8_t) EMB_BLOCK_UPDATE_EM;

      /* read Status Of DTC , LUC and OCC from event Memory */
    retVal_u8 = APL_DEH_EEPReadEventData(
                         (uint16_t) (EMB_BLOCK_SIZE * EvMemoryPos_uT),
                         (uint16_t)  blocksize_u8,
                                  &Buffer_u8[0]             );
      /* test result and set failure flag */
    if (retVal_u8 != DEH_OK)
    {
         /* set failure flag for EEP Read problems */
      DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_READ; 
    }
     
      /* on position Null is Status Of DTC stored */
    Buffer_u8[EMB_POS_STOFDTC] = 
                     (uint8_t) (DEH_SUiF_StatusOfDTC(EventNumber_u16,WRITE_EM) );

      /* read the current unlearn counter from the system */
    tmp_UnlearnCnt_u8 = APL_DEH_ReadUnlearnCounter();

    if (NewEventState_u8 == DEH_EVENT_PASSIVE)
    {
         /* reset the warning lamp bit  */
       Buffer_u8[EMB_POS_STOFDTC] &= DEH_STOFDTC_MSK_LAMP_RESET;

        /* algorithm to calculate the new Limit to clear an stored event     */
        /* the unlearn counter is normally 8 bit and the last valid unlearn  */
        /* counter is 254 !!!                                                */
        /* For a simpler calculation the tmp value is 16 bit.                */
        /* First step is summary of the actual unlearn counter with the      */
        /* unlearn offset  (40decimal).                                      */
        /* Second step is check if the result greater as the last valid      */
        /* value (254 decimal). If the value greater than minus the constant */
        /* DEH_LUC_OVERFLOW (255 decimal) and the result is the new Limit    */
        /* unlearn counter with overflow consideration.                      */
      
      if (tmp_UnlearnCnt_u8 != DEH_NO_VALID_UNLEARN_COUNTER)
      {
        tmp_LUC_u16 =(uint16_t) (tmp_UnlearnCnt_u8 + DEH_UNLEARN_OFFSET);
          /* check for a valid Limit */
        if (tmp_LUC_u16 > DEH_LAST_VALID_LUC)
        {
            /* caculate the new value if a  overflow occurs */
          tmp_LUC_u16 -= DEH_LUC_OVERFLOW;
        }
      }
      else
      {
        tmp_LUC_u16 =(uint16_t) DEH_NO_VALID_UNLEARN_COUNTER;
      }
    }
    else if (NewEventState_u8 == DEH_EVENT_ACTIVE)
    {
        /* analyze the new event state for the warning lamp state */
        /* and the connected lamp parameter                        */
      if ( DEH_EVENT_LAMP_PAR(EventNumber_u16))
      {
           /* set the warning lamp bit */
         Buffer_u8[EMB_POS_STOFDTC] |= DEH_STOFDTC_MSK_LAMP;
      }  

        /* the new Limit to Unlearn is the actual global Unlearn counter */
      tmp_LUC_u16 =(uint16_t) tmp_UnlearnCnt_u8;

        /* Update the OCC = OCC + 1 and test for overflow*/
      if (Buffer_u8[EMB_POS_OCC] < 254)
      {
        Buffer_u8[EMB_POS_OCC] += (uint8_t) 1;
      }
    }

      /* store the new LUC into the buffer */
    Buffer_u8[EMB_POS_LUC] = (uint8_t) tmp_LUC_u16;

      /* write back the new calculated values */
    retVal_u8 = APL_DEH_EEPWriteEventData(
                                   (uint16_t) (EMB_BLOCK_SIZE * EvMemoryPos_uT),
                                   (uint16_t)  blocksize_u8,
                                             &Buffer_u8[0]                    );

      /* test the result and set failure flag */
    if (retVal_u8 != DEH_OK)
    {
       /* set failure flag in global error message */
     DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_WRITE;
    }

  } /*end if free position  found */
  
  return(retVal_u8);
}

/*============================================================================
function name    :   DEH_ESiF_WarnLampAndSrvBitUpd(void)
input parameter  :  
output parameter :   
globals          :   
return value     :   _Bool: Service Bit for "Kundendienst"
description      :   Function is responsible for two information:
                      I) - one or more error are in the error memory 
                            => Service Bit is TRUE (return value)
                      II) current state for the Waerning Lamp
                          TRUE : if one or more ACTIVE error in the error memory 
                                 AND this ACTIVE error request the Warning Lamp
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
26.01.07    GIGE      built base version 
============================================================================*/
static _Bool DEH_ESiF_WarnLampAndSrvBitUpd(void)
{
  _Bool             ret_val_b =(_Bool) FALSE; 
  tDEH_EventMemory  k; 
  uint16_t          EvNumber_u16;
  uint8_t           tmp_u8 = (uint8_t)0;
    /* look into the error memory for a error entry with warning lamp */
  for  (k =((tDEH_EventMemory) 0); k < DEH_MAX_NUMBER_EV_MEM; k++) 
  {
      /* read the stored event number */
    EvNumber_u16 = DEH_EventMemBuffer[k];

      /* test for a valid event memory entry */
    if (EvNumber_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS)
    {

        /* CAN event bit is set only for events with smaller priority than 9 */
      if ((DEH_GET_EVENT_PRIORITY(EvNumber_u16) < DEH_DEVELOP_EVENT_PRIO))
      {
           /* set the CAN_EVENT_BIT */
        ret_val_b = (_Bool) TRUE;
      }
        /* 1) test if EVENT defect AND */ 
        /* 2) Prio is smaller than 9   */
      if (    (DEH_IS_EVENT_DEFECT(EvNumber_u16))
           && (DEH_GET_EVENT_PRIORITY(EvNumber_u16) < DEH_DEVELOP_EVENT_PRIO))
      {
          /* lock for a connected warning lamp */
        tmp_u8 =(uint8_t) ( tmp_u8 | DEH_EVENT_LAMP_PAR(EvNumber_u16));
      }
    }  /* end  if ( (EvNum_uT != DEH_EM_FREE_POS) &&  .. */
  }  /* end for */

    /* set the global message to the calculated result */
  DEH_stWarnLamp_mu8 = tmp_u8;
  return (ret_val_b);
}
/*============================================================================
function name    :   DEH_ESiF_WriteEM(void))
input parameter  :  
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read all event memory entries and update the 
                     event state in the DEH_EventBuffer[x] array.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE      built base version 

============================================================================*/
static tDEH_Return DEH_ESiF_WriteEM(uint16_t EventNumber_u16, uint8_t eventstate_u8)
{
  tDEH_EventMemory      i;
  tDEH_EventMemory      FreePosEM_uT  = DEH_EM_NO_VALID_POS;
  tDEH_Return           retVal_u8     = DEH_OK;
  uint8_t               blocksize_u8;
  uint8_t               Buffer_u8[EMB_BLOCK_SIZE];

#ifdef DEH_READ_ADD_FF
#ifdef DEH_SUP_KWP2000
  tDEH_AddFreezeFrame   AddFF[DEH_MAX_NUMBER_ADD_FF];  /* tmp. Variable to get the Add. FF */
  uint8_t               index_u8;
#endif

#endif

#ifdef DEH_READ_STD_FF
  tDEH_StdFreezeFrame   StdFFvar_T;  /* tmp. Variable to get the Std. FF */
#endif
  
  uint16_t              tmp_UC_u16;

    /* Init the byte buffer for all event memory datas with 0xFF */
  for (i= ((tDEH_EventMemory) 0 ); i < ((tDEH_EventMemory) EMB_BLOCK_SIZE); i++)
  {
    Buffer_u8[i] = (uint8_t) 0xFF;
  }

    /* test if the event NOT locked (Bit 3 = 0 ) */
  if (! (DEH_EVENT_BUFFER_STATE(EventNumber_u16) & DEH_EB_MSK_LOCK_DTC) )
  {
      /* look for a free position in the event memory */
    retVal_u8 = DEH_ESiF_LookForPosEM( DEH_EM_FREE_POS, &FreePosEM_uT);

      /* found no free positions */
    if(retVal_u8 != DEH_OK)
    {
      /* Call the function to find an entry to overwrite */
      retVal_u8 = DEH_ESiF_LookForPosToOvEM( EventNumber_u16, &FreePosEM_uT );

      if ((retVal_u8 == DEH_OK) && (FreePosEM_uT < DEH_MAX_NUMBER_EV_MEM))
      {
          /* read the event number and update there state */
        tmp_UC_u16 = DEH_EventMemBuffer[FreePosEM_uT];

          /* set storage state to NOT STORED */
        DEH_EVENT_BUFFER_STATE(tmp_UC_u16) &= DEH_EB_MSK_EV_STO_RST;
      }  
    }

    if (retVal_u8 == DEH_OK)
    {
      
        /* test for valid position */
      if (FreePosEM_uT < DEH_MAX_NUMBER_EV_MEM)
      {

          /* store the Event Number in the Buffer */
        Buffer_u8[EMB_POS_EV_NUM_LOWBYTE]   = (uint8_t) EventNumber_u16;
        Buffer_u8[EMB_POS_EV_NUM_HIGHBYTE]  = (uint8_t)(EventNumber_u16 >> 8);

          /* increase the temp. var for the size vaiable */
        blocksize_u8 = (uint8_t) EMB_BL_SIZE_EV_NUM;

          /* get the information StatusOfDTC ADD the new state and store in Buffer */
        Buffer_u8[EMB_POS_STOFDTC] = (uint8_t) 
                                   DEH_SUiF_StatusOfDTC(EventNumber_u16, WRITE_EM);

          /* analyze the actual event state and set the storage state */
        if (eventstate_u8 == DEH_EVENT_ACTIVE)
        {
          Buffer_u8[EMB_POS_STOFDTC] |= (uint8_t)(DEH_STOFDTC_MSK_ACTIVE | DEH_STOFDTC_MSK_STORED);
        }
        else if (eventstate_u8 == DEH_EVENT_PASSIVE)
        {
          Buffer_u8[EMB_POS_STOFDTC] |= DEH_STOFDTC_MSK_STORED;
        }

          /* analyze the new event state for the warning lamp state */
          /* and the connected lamp parameter                        */
        if (     ( eventstate_u8 == DEH_EVENT_ACTIVE ) 
             &&  ( DEH_EVENT_LAMP_PAR(EventNumber_u16 ) ) )
        {
            /* set the new warning lamp state */
          Buffer_u8[EMB_POS_STOFDTC] |=  DEH_STOFDTC_MSK_LAMP;
        }

          /* increase the temp. var for the size vaiable with DTC val. */
        blocksize_u8  +=(uint8_t) EMB_BL_SIZE_STOFDTC;

          /* if the event ERASABLE then read the current unlearn counter */
          /* from the application                                        */
        if(!( DEH_EVENT_PARAMETER(EventNumber_u16) & DEH_EP_MSK_EV_NO_CLEAR))
        {   /* read the actual unlearn counter */
          tmp_UC_u16 =(uint16_t) APL_DEH_ReadUnlearnCounter();
        }
          /* event ist NOT ERASABLE => set unlearn counter to 0xFF */
        else
        {
          tmp_UC_u16 =(uint16_t) DEH_NO_VALID_UNLEARN_COUNTER;
        }

          /* calculate the new unlearn counter limit if: */
          /* i)  eventstate is PASSIVE                   */
          /* ii) event is erasable                       */
        if (   (eventstate_u8 == DEH_EVENT_PASSIVE)
             &&(!( DEH_EVENT_PARAMETER(EventNumber_u16) & DEH_EP_MSK_EV_NO_CLEAR)))
        {
            /* algorithm to calculate the new Limit to clear an stored event  */
            /* unlearn counter is normally 8 bit and the last valid unlearn   */
            /* counter is 254 !!!                                             */
            /* For a simpler calculation the tmp value is 16 bit.             */
            /* First step is summary of the actual unlearn counter with the   */
            /* unlearn offset  (40decimal).                                   */
            /* Second step is check if the result greater as the last valid   */
            /* value (254 decimal). If the value greater than minus the const.*/
            /* DEH_LUC_OVERFLOW (255 decimal) and the result is the new Limit */
            /* unlearn counter with overflow consideration.                   */
          tmp_UC_u16 += DEH_UNLEARN_OFFSET;

            /* check for a valid Limit */
          if (tmp_UC_u16 > DEH_LAST_VALID_LUC)
          {
              /* caculate the new value if a  overflow occurs */
            tmp_UC_u16 -= DEH_LUC_OVERFLOW;
          }
        }

          /* store in Buffer the actual Unlearn Counter */
        Buffer_u8[EMB_POS_LUC] =(uint8_t) tmp_UC_u16;

          /* increase the temp. var for the size vaiable with LUC val. */
        blocksize_u8 +=(uint8_t) EMB_BL_SIZE_LUC;

          /* store in Buffer OCC = 1 */
        Buffer_u8[EMB_POS_OCC] = (uint8_t) 1;

          /* increase the temp. var for the size vaiable with OCC val. */
        blocksize_u8 +=(uint8_t) EMB_BL_SIZE_OCC;

#ifdef DEH_MAX_NUMBER_FF
        {
          /* read freeze frames and write to external buffer */
#ifdef DEH_READ_STD_FF
          {
              /* read standard freeze frame from int. Freeze Frame controller */
            DEH_FFiF_GetStdFF(EventNumber_u16, &StdFFvar_T);

              /* store the kilometer values in the internal buffer */
            for (i= ((tDEH_EventMemory) 0) ; i < ((tDEH_EventMemory) EMB_BL_SIZE_STDFF); i++)
            {
              Buffer_u8[EMB_POS_STDFF+i] = StdFFvar_T.StdFF[i];

            }

              /* increase the temp. var for the blocksize with StdFF val. */
            blocksize_u8 +=(uint8_t) EMB_BL_SIZE_STDFF;

          } 
#endif /* end if (DEH_READ_STD_FF) */
        
#ifdef DEH_READ_ADD_FF
          {
              /* read the stored additional freeze frames from FF-Controller */
#ifdef DEH_SUP_KWP2000
            DEH_FFiF_GetAddFF(EventNumber_u16, AddFF);

              /* calculate the buffer position */


            for (i= (( tDEH_EventMemory) 0); i < ((tDEH_EventMemory) DEH_MAX_NUMBER_ADD_FF); i++)
            {
  
                /* caculate the array index */
              index_u8 = (uint8_t) ( ( (uint8_t) EMB_POS_ADDFF) + 
                                      (i* ( (uint8_t) DEH_SIZEOF_ADDFF) ) );


                /* store the read information into the internal buffer */
              Buffer_u8[index_u8] = AddFF[i].formula;
              Buffer_u8[index_u8+1] = AddFF[i].normValue;
              Buffer_u8[index_u8+2] = AddFF[i].measurement;

            }

              /* increase the temp. var for the blocksize with AddFF val. */
            blocksize_u8 +=(uint8_t) (DEH_SIZEOF_ADDFF*DEH_MAX_NUMBER_ADD_FF);
#endif  /* KWP2000 */
#ifdef DEH_SUP_UDS
            DEH_FFiF_GetExtFF(EventNumber_u16, &Buffer_u8[EMB_POS_ADDFF]);
              /* increase the temp. var for the blocksize with ExtRecData val. */
            blocksize_u8 +=(uint8_t) (DEH_MAX_EXT_REC_BUFFER_DATA*DEH_MAX_NUMBER_ADD_FF);
#endif /* DEH_SUP_UDS */
          }
#endif  /* end if DEH_READ_ADD_FF */
        } 
#endif /* end if (DEH_MAX_NUMBER_FF>0) */

        retVal_u8 = APL_DEH_EEPWriteEventData( (uint16_t) (EMB_BLOCK_SIZE * FreePosEM_uT),
                                               (uint16_t)  blocksize_u8,
                                                          &Buffer_u8[0]         );
          /* test the result and set failure flag */
        if (retVal_u8 == DEH_OK)
        {
            /* update the Event Memory Buffer with the connected event number */
          DEH_EventMemBuffer[FreePosEM_uT] = EventNumber_u16;
        }
        else
        {
            /* set failure flag in global error message */
          DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_WRITE;
        }

      } /* end if test for valid POS */
    } /* end if (FoundFreePos_b) */
    else
    {
        /* nor free position in event memory neither an event to overwrite */
      retVal_u8 = DEH_E_FULL;
    }  /* end else (FoundFreePos_b) */

  } /* end if (!EventLock)*/
  else
  {
      /* return value EVENT LOCKED */
    retVal_u8 = DEH_E_EV_LOCKED;
  } /* end else (!EventLock)*/
  
  return(retVal_u8);
} /* end function DEH_ESiF_WriteEM */


/*============================================================================
function name    :   DEH_FFiF_InitFF()
input parameter  :   
output parameter :   
globals          :   
return value     :   
description      :   Function to INIT freeze frames buffer
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
27.11.06    GIGE    First implementation
============================================================================*/
static tDEH_Return DEH_FFiF_InitFF(void)
{
  tDEH_Return retVal_u8 = DEH_OK;
  uint8_t                 i;
  uint8_t                 k;
#if (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_UDS))
  uint8_t                 m;  
#endif

    /* message for free Freeze Frame buffers */
  DEH_numOfFreeFF_mu8 = (uint8_t) DEH_FF_BUFFER_SIZE;

      /* counter for free Freeze Frame Buffer position */
  for (i =((uint8_t) 0); i < ((uint8_t) DEH_FF_BUFFER_SIZE) ; i++)
  {
      /* store the in the found  position the value FREE POS*/
    DEH_FF_Buffer[i].EventNumber = DEH_FF_FREE_POS;

    for (k =((uint8_t) 0); k < ((uint8_t) DEH_SIZEOF_STDFF) ; k++)
    {
      DEH_FF_Buffer[i].StdFFrames.StdFF[k] = (uint8_t) DEH_STO_NO_FF;
    }

#if (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_KWP2000))
    for (k =((uint8_t) 0); k < ((uint8_t) DEH_MAX_NUMBER_ADD_FF) ; k++)
    {
      DEH_FF_Buffer[i].AddFFrame[k].formula     = DEH_FFBUF_INIT_FORM;
      DEH_FF_Buffer[i].AddFFrame[k].normValue   = DEH_FFBUF_INIT_NORMV;
      DEH_FF_Buffer[i].AddFFrame[k].measurement = DEH_FFBUF_INIT_MEAS;
    }
#endif

#if (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_UDS))
    for (k =((uint8_t) 0); k < ((uint8_t) DEH_MAX_NUMBER_ADD_FF) ; k++)
    {
      for (m =((uint8_t) 0); m < ((uint8_t) (DEH_MAX_EXT_REC_BUFFER_DATA)) ; m++)
      {    
          /* clear all information in the buffer */
        DEH_FF_Buffer[i].ExtDataRec_u8[k][m]= DEH_NO_EXT_REC_DATA_AVAILABLE;
          /* increase the buffer counter for next element */
      }
    }

#endif /* (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_UDE)) */
  }
  return (retVal_u8);
}

/*============================================================================
function name    :   DEH_FFiF_ClearFF()
input parameter  :   EventNumber_u16 : event number for read freeze frames
output parameter :   
globals          :   
return value     :   
description      :   Function to clear stored freeze frames for the received 
                     event number.
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
12.11.03    GIGE    First implementation
============================================================================*/
static tDEH_Return DEH_FFiF_ClearFF(uint16_t EventNumber_u16)
{
  tDEH_Return retVal_u8 = DEH_E_NO_DATA;
  uint8_t                 i;
  uint8_t                 k;
#if (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_UDS))
  uint8_t                 m;  
#endif
      /* look for free Freeze Frame Buffer position */
  for (i =((uint8_t) 0); i < ((uint8_t) DEH_FF_BUFFER_SIZE) ; i++)
  {
    if (DEH_FF_Buffer[i].EventNumber == EventNumber_u16)
    { 
        /* store the in the found  position the value FREE POS*/
      DEH_FF_Buffer[i].EventNumber = DEH_FF_FREE_POS;

         /* increment the freeze frame counter */
      DEH_numOfFreeFF_mu8 += (uint8_t) 1;

      for (k =((uint8_t) 0); k < ((uint8_t) DEH_SIZEOF_STDFF) ; k++)
      {
        DEH_FF_Buffer[i].StdFFrames.StdFF[k] = (uint8_t) DEH_STO_NO_FF;
      }

#if (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_KWP2000))
      for (k =((uint8_t) 0); k < ((uint8_t) DEH_MAX_NUMBER_ADD_FF) ; k++)
      {
        DEH_FF_Buffer[i].AddFFrame[k].formula     = DEH_FFBUF_INIT_FORM;
        DEH_FF_Buffer[i].AddFFrame[k].normValue   = DEH_FFBUF_INIT_NORMV;
        DEH_FF_Buffer[i].AddFFrame[k].measurement = DEH_FFBUF_INIT_MEAS;
      }
#endif

#if (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_UDS))
      for (k =((uint8_t) 0); k < ((uint8_t) DEH_MAX_NUMBER_ADD_FF) ; k++)
      {
#if defined(DEH_READ_ADD_FF)
        for (m =((uint8_t) 0); m < ((uint8_t) (DEH_MAX_EXT_REC_BUFFER_DATA)) ; m++)
        {    
            /* clear all information in the buffer */
          DEH_FF_Buffer[i].ExtDataRec_u8[k][m]= DEH_NO_EXT_REC_DATA_AVAILABLE;
            /* increase the buffer counter for next element */
        }
#endif
      }


#endif /* (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_UDE)) */

        /* set the correct return value */
      retVal_u8 = DEH_OK;
    }
  }

  return (retVal_u8);
}


/*============================================================================
function name    :  DEH_FFiF_GetAddFF()
input parameter  :  uint16_t EventNumber_u16
output parameter :   
globals          :   
return value     :   tDEH_AddFreezeFrame
description      :   Function to read the stored additional freeze frame from 
                     the Freeze Frame Controller for a special event number.
                     If no freeze frame stored then will be all output data
                     set to 0xFF.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
09.11.03    GIGE    built base version 

============================================================================*/
#if  (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_KWP2000))
static void DEH_FFiF_GetAddFF(uint16_t EventNumber_u16, tDEH_AddFreezeFrame* AddFF)
{
  _Bool     found_b = (_Bool)    FALSE;
  uint8_t   i       = (uint8_t) 0;

  uint8_t   k;
  
  while ((i < DEH_FF_BUFFER_SIZE) && (!found_b))
  {
      /* test if wanted event number equal the stored event number */
    if (DEH_FF_Buffer[i].EventNumber  == EventNumber_u16)
    {
      for (k =((uint8_t) 0); k < ((uint8_t) DEH_MAX_NUMBER_ADD_FF) ; k++)
      {
#if (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_KWP2000))
          /* copy the stored information to the received address */
        AddFF[k] = DEH_FF_Buffer[i].AddFFrame[k];
#endif
      }

        /* set flag for sucessfull search */
      found_b = (_Bool)TRUE;
    }

      /* increase the internal counter */
    i++;
  }
 

    /* no stored standard freeze frames => get back 0xFF */
  if (!found_b)
  {
    for (i = ((uint8_t) 0) ; i < ((uint8_t) DEH_MAX_NUMBER_ADD_FF) ; i++)
    {
      AddFF[i].formula     =  DEH_FFBUF_INIT_FORM;
      AddFF[i].normValue   =  DEH_FFBUF_INIT_NORMV;
      AddFF[i].measurement =  DEH_FFBUF_INIT_MEAS; 
      
    }
  }  

  return;
}
#endif

#if  (defined(DEH_READ_ADD_FF) && defined(DEH_SUP_UDS))
/*============================================================================
function name    :  DEH_FFiF_GetExtFF()
input parameter  :  uint16_t EventNumber_u16
output parameter :   
globals          :   
return value     :   ExtDataRecord_T
description      :   Function to read the stored external freeze frame from 
                     the Freeze Frame Controller for a special event number.
                     If no freeze frame stored then will be all output data
                     set to 0xFF.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
27.11.03    GIGE    built base version 

============================================================================*/
static void DEH_FFiF_GetExtFF(uint16_t EventNumber_u16, uint8_t *ExtDataRecord_u8)
{
  _Bool     found_b = (_Bool)    FALSE;
  uint8_t   i       = (uint8_t) 0;
  uint8_t   k;
  uint8_t   m;
  uint16_t  buffer_u16 = (uint16_t)0;

  while ((i < DEH_FF_BUFFER_SIZE) && (!found_b))
  {
      /* test if wanted event number equal the stored event number */
    if (DEH_FF_Buffer[i].EventNumber  == EventNumber_u16)
    {
      for (k =((uint8_t) 0); k < ((uint8_t) DEH_MAX_NUMBER_ADD_FF) ; k++)
      {
#if defined(DEH_READ_ADD_FF)
        for (m =((uint8_t) 0); m < ((uint8_t) (DEH_MAX_EXT_REC_BUFFER_DATA)) ; m++)
        {    
            /* copy the stored information to the received address */
          ExtDataRecord_u8[buffer_u16] = DEH_FF_Buffer[i].ExtDataRec_u8[k][m];
            /* increase the buffer counter for next element */
          buffer_u16++;
        }
#endif
      }

        /* set flag for sucessfull search */
      found_b = (_Bool)TRUE;
    }

      /* increase the internal counter */
    i++;
  } /* end while */
 
  if(!(found_b) )
  {
    for (m =((uint8_t) 0); m < ((uint8_t) (DEH_MAX_EXT_REC_BUFFER_DATA)) ; m++)
    {    
        /* set the FSP to defined values (0xFF) */
      ExtDataRecord_u8[m] = DEH_NO_EXT_REC_DATA_AVAILABLE;
    }
  }
  return;
}
#endif

#ifdef DEH_READ_STD_FF
/*============================================================================
function name    :  DEH_FFiF_GetStdFF ()
input parameter  :  uint16_t EventNumber_u16
                 :  tDEH_StdFreezeFrame *StdFFvar_T
output parameter :   
globals          :   
return value     :   
description      :   Function to read the stored standard freeze frame from 
                     Freeze Frame Controller for a special event number.
                     If no freeze frame stored then will be all output data
                     set to 0xFF.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
09.11.03    GIGE    built base version 

============================================================================*/
static void DEH_FFiF_GetStdFF(uint16_t EventNumber_u16, tDEH_StdFreezeFrame *StdFFvar_T )
{
  _Bool     found_b = (_Bool)   FALSE;
  uint8_t   i       = (uint8_t) 0;
  uint8_t   k;

#ifdef DEH_FF_BUFFER_SIZE
  while ((i < DEH_FF_BUFFER_SIZE) && (!found_b))
  {
      /* test if wanted event number equal the stored event number */
    if (DEH_FF_Buffer[i].EventNumber  == EventNumber_u16)
    {
        /* copy the stored information to the received address */
      for (k =((uint8_t) 0); k < ((uint8_t) DEH_SIZEOF_STDFF ) ; k++)
      {
        StdFFvar_T->StdFF[k] = DEH_FF_Buffer[i].StdFFrames.StdFF[k];
      }

        /* set flag for sucessfull search */
      found_b = (_Bool)TRUE;
    }
      /* increase the internal counter */
    i++;
  }
#endif

    /* no stored standard freeze frames => get back NOT AVAILABLE constants */
  if (!found_b)
  {
    for (i =((uint8_t) 0); i < ((uint8_t) DEH_SIZEOF_STDFF_KM) ; i++)
    {
      StdFFvar_T->StdFF[i] = (uint8_t) DEH_STDFF_KM_NOT_AVAILABLE;
    }

    for (i =((uint8_t) DEH_SIZEOF_STDFF_KM); i < ((uint8_t) DEH_SIZEOF_STDFF) ; i++)
    {
      StdFFvar_T->StdFF[i] = (uint8_t) DEH_STDFF_TIME_NOT_AVAILABLE;
    }
  }

  return;
}
#endif

/*============================================================================
function name    :   DEH_FFiF_ReadFF()
input parameter  :   EventNumber_u16 : event number for read freeze frames
output parameter :   
globals          :   
return value     :   
description      :   Function to read and store freeze frames for the received 
                     event number.
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
12.11.03    GIGE    First implementation
============================================================================*/
static tDEH_Return DEH_FFiF_ReadFF(uint16_t EventNumber_u16)
{

#ifdef DEH_FF_BUFFER_SIZE 
  tDEH_Return retVal_u8 = DEH_OK;
  uint8_t i             = (uint8_t) 0 ;
  uint8_t indexFF_u8;
  uint8_t FreePos_u8    = DEH_FF_NO_VALID_POS;

#if (defined(DEH_READ_STD_FF ) || defined(DEH_READ_ADD_FF))
  uint8_t actFF_u8;
#endif

#ifdef DEH_READ_ADD_FF
  uint8_t AddFfCnt_u8   = (uint8_t) 0;
  #ifdef DEH_SUP_UDS
    uint8_t numOfBytes_u8 = (uint8_t) 0;
  #endif
#endif
  
  if (DEH_numOfFreeFF_mu8)
  {
      /* read the index to the freeze frame table */
    indexFF_u8= (uint8_t) DEH_EVENT_IDX_FF(EventNumber_u16);

      /* test if freeze frames should be stored */
    if (indexFF_u8 != DEH_STO_NO_FF)
    {
        /* look for free Freeze Frame Buffer position */
      while ( ( i < DEH_FF_BUFFER_SIZE ) && (FreePos_u8 == DEH_FF_NO_VALID_POS ) )
      {
        if (DEH_FF_Buffer[i].EventNumber == DEH_FF_FREE_POS)
        { 
            /* store the found free position */
          FreePos_u8 = i;
        }
          /* increase the internal counter */
        i++;
      }

      if (FreePos_u8 != DEH_FF_NO_VALID_POS)
      {
          /* store the received event number into the FF buffer */
        DEH_FF_Buffer[FreePos_u8].EventNumber = EventNumber_u16;

          /* decrement the freeze frame counter */
        DEH_numOfFreeFF_mu8 -= (uint8_t) 1;

          /* compile the next function block only if you have defined freeze frames*/
#if (defined(DEH_READ_STD_FF ) || defined(DEH_READ_ADD_FF))
          /* loop to read out all values from freeze frame table */
        for (i =((uint8_t) 0) ; i < ((uint8_t) DEH_MAX_NUMBER_FF); i++)
        {
            /* get the stored freeze frame number */
         actFF_u8 = DEH_FreezeFrameTable[indexFF_u8][i];

            /* test if the entry a valid freeze frame number */
          if (actFF_u8 != DEH_STO_NO_FF)
          {

              /* test if the read freeze frame is a Standard Freeze Frame */
            if (actFF_u8 == STD_FF_NUMBER )
            {
#ifdef DEH_READ_STD_FF
                /* call the function to read a standard freeze frame */
              retVal_u8 = APL_DEH_ReadStdFF(&DEH_FF_Buffer[FreePos_u8].StdFFrames);
#endif                                                              
            } /* end if read out the standard freeze frame */
            else
            {
#ifdef DEH_READ_ADD_FF
              if (AddFfCnt_u8 < DEH_MAX_NUMBER_ADD_FF)
              {
                  /* call the function to read a additional freeze frame */
#if defined(DEH_SUP_KWP2000)
                retVal_u8 = APL_DEH_ReadAddFF(actFF_u8,&DEH_FF_Buffer[FreePos_u8].AddFFrame[AddFfCnt_u8]);
#elif defined(DEH_SUP_UDS)

                retVal_u8 = APL_DEH_ReadAddFF(actFF_u8,
                       DEH_MAX_LEN_ADD_FF,
                      &numOfBytes_u8,
                      &DEH_FF_Buffer[FreePos_u8].ExtDataRec_u8[AddFfCnt_u8][2]);

                  /* store to first pos the Freeze Frame Number */
                  /* store to second pos num. of bytes return value */
                if (retVal_u8 == DEH_OK)
                {
                  DEH_FF_Buffer[FreePos_u8].ExtDataRec_u8[AddFfCnt_u8][0]= actFF_u8;
                  DEH_FF_Buffer[FreePos_u8].ExtDataRec_u8[AddFfCnt_u8][1]= numOfBytes_u8;
                }
#endif
                  /* increase the internal additional freeze frame pos counter */
                if (retVal_u8 == DEH_OK)
                {
                  AddFfCnt_u8 += (uint8_t) 1;
                }
                else
                {
                    /* release the  buffer if return code NOT OK */
                  DEH_FF_Buffer[FreePos_u8].EventNumber = DEH_FF_FREE_POS;                
                }
              }                         
              else
              {
                /* -------------------------------------- */
                /* define fault code for indication       */
                /* -------------------------------------- */

              }
#endif
            } /* end else read additinal freeze frame */
          }
        }
#endif /* only compiled if freeze frames defined */
      
      } /* free position found */

    else
    {
      retVal_u8 = DEH_E_FULL;
    }
  } /* end   if (!DEH_numOfFreeFF_mu8 ) */

  else
  {
    retVal_u8 = DEH_E_FULL;
  }
}
#endif 


#ifndef DEH_FF_BUFFER_SIZE
  tDEH_Return retVal_u8 = DEH_E_NO_BUFFER;
#endif   
  return (retVal_u8);
}

/*============================================================================
function name    :  DEH_GetNumberOfIdentEvents ()
input parameter  :  tDEH_DTCGroup DTCGroup - special DTC-group 
                    (actual only 0xFF00 - All groups supported)
                    tDEH_EventNumber* EventNum_uT - pointer to a buffer 
                    
output parameter :  
globals          :  
return value     :  tDEH_Return
description      :  Function to read the number of identified events. 
                    The function copies the information into the buffer.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
28.10.03    GIGE    built base version 
13.11.03    GIGE    first solution 

============================================================================*/
#ifdef DEH_SUP_KWP2000
tDEH_Return DEH_GetNumberOfIdentEvents (tDEH_DTCGroup DTCGroup, tDEH_EventNumber* EventNum_uT)
{
  tDEH_EventNumber       EventCnt_uT = (tDEH_EventNumber)0 ;
  tDEH_Return            retVal_u8    = DEH_OK ;
  tDEH_EventMemory       i ;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
      /* test the received DTC-group */
    if (DTCGroup == DEH_DTC_ALL_GROUPS)
    {
        /* counter for all event memory entries */
      for( i=((tDEH_EventMemory) 0) ; i < ((tDEH_EventMemory) DEH_MAX_NUMBER_EV_MEM) ; i++)
      {

          /* test if the actual pos not equal FREE POS */
        if (DEH_EventMemBuffer[i] != DEH_EM_FREE_POS)
        {
            /* test the current diagnose session                             */
            /* count the event in event memory if :                          */
            /* 1) - current diag. session is NOT equal to Developper session */
            /*    AND - prio is smaller than 9                               */
            /* OR                                                            */
            /* 2) diagnose session IS EQUAL TO Developper session            */

          if(   (   (DEH_actDiaSes_muT != DEH_ENTW_DIAG_SESSION) 
                 && (DEH_GET_EVENT_PRIORITY(DEH_EventMemBuffer[i])
                                             < DEH_DEVELOP_EVENT_PRIO))
              || (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION))
          {
              /* increase the internal counter */
            EventCnt_uT += (tDEH_EventNumber) 1;
          }
        }
      }

        /* write back the result */
      *EventNum_uT = EventCnt_uT;

    }
    else
    {
        /* set the return code for unknown DTC-group */
      retVal_u8 = DEH_E_UNKNOWN_DTCGROUP;
    }
  }
  else /* no correct init */
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }    

  return(retVal_u8); 
}  /* end of DEH_GetNumberOfIdentEvents() */
#endif  /* DEH_SUP_KWP2000 */

/*============================================================================
function name    :  DEH_GetNumberOfSupEvents ()
input parameter  :  tDEH_DTCGroup DTCGroup - special DTC-group 
                    (actual only 0xFF00 - All groups supported)
                    tDEH_EventNumber* EventNum_uT - pointer to a buffer 
                    
output parameter :   
globals          :   
return value     :  tDEH_Return
description      :  Function to read the number of supported events. 
                    The function copies the information into the buffer.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
28.10.03    GIGE    built base version 

============================================================================*/
#ifdef DEH_SUP_KWP2000
tDEH_Return DEH_GetNumberOfSupEvents (tDEH_DTCGroup DTCGroup, tDEH_EventNumber* EventNum_uT)
{

  tDEH_Return retVal_u8 = DEH_OK;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
      /* test the received DTC-group */
    if (DTCGroup == DEH_DTC_ALL_GROUPS)
    {
        /* write back the SYSTEM CONSTANT */
      *EventNum_uT = (tDEH_EventNumber) DEH_MAX_NUMBER_OF_EVENTPATHS;
    }
    else
    {
        /* set the return code for unknown DTC-group */
      retVal_u8 = DEH_E_UNKNOWN_DTCGROUP;
    }
  }
  else /* no correct init */
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }
      
  return (retVal_u8);
}  /* end of DEH_GetNumberOfSupEvents() */
#endif  /* DEH_SUP_KWP2000 */

/*============================================================================
function name    :   void DEH_Init(void)
input parameter  :   no
output parameter :   tDEH_Return - function return value  
globals          :   
return value     :   
description      :   
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
01.11.03    GKSC    built base version 

============================================================================*/
tDEH_Return DEH_Init(void)
{

  tDEH_Return      retVal_u8 = DEH_OK;
  tDEH_EventNumber i;

    /* Init for the event counter to read supported, identified or by status events */
  DEH_cntEvent_mu16 = ((uint16_t) 0);

    /* set the last received diagnostic Function to no valid value */
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_RSE_RESET;

      /* reset all error flags */
  DEH_EventMemoryErrors_mu8 =(uint8_t) 0;

    /* reset counter for read freeze frames */
  DEH_cntReadFF_mu8 =(uint8_t) 0;

    /* reset the global message for warning lamp state */
  DEH_stWarnLamp_mu8 = (uint8_t) 0;

    /* reset the message for the CAN-Event bit */
  DEH_stCANEventBit_mb = (_Bool) FALSE;

    /* reset counter for update event memory */
  DEH_cntEvUpd_mu8 =(uint8_t) 0;

    /* reset counter for check warning lamp status */
  DEH_cntWLP_mu8 =(uint8_t) 0;

    /* reset flag for update event memory */
  DEH_stUpdateEM_b = (_Bool) FALSE;

    /* reset flag read freeze frame necessary */
  DEH_stReadFF_b = (_Bool) FALSE;

    /* set the unlearn state machine to state INIT */
  DEH_StM_UnAlg_mu8 = (uint8_t) DEH_UC_INIT;
    /* set the old unlearn counter to an undefined value */
  DEH_oldUnlearnCnt_mu8 = (uint8_t) DEH_NO_VALID_UNLEARN_COUNTER;

    /* set flag for event memory full */
  DEH_stFullEvMem_mb = (_Bool) FALSE;

    /* set flag for event PASSIVE in event memory */
  DEH_stPassiveEvents_mb = (_Bool)FALSE;
    /* reset all event information in the Buffer-Array*/
  for (i=((tDEH_EventNumber) 0); i < DEH_MAX_NUMBER_OF_EVENTPATHS; i++)
  {
      /* set the actual event state to PASSIVE and DTC NOT TESTED */
    DEH_EVENT_BUFFER_STATE(i) = (uint8_t) ( DEH_EVENT_PASSIVE | DEH_EB_MSK_DTC_RB);

      /* set the actual debounce value to stop value*/
    DEH_EVENT_DEBOUNCE(i) = DEH_DEB_STOP;
  }

    /* init the event memory from stored EEP Event Memory */
  retVal_u8 = DEH_ESiF_InitEM();

    /* test the result of the init function               */
    /* and set the indication flag depended of the result */
  if (retVal_u8 == DEH_E_INIT_MEMORY)
  {
      /* no correct event memory init */
    DEH_stInitEM_b =(_Bool) FALSE;
  }
  else
  {
      /* correct event memory init */
    DEH_stInitEM_b =(_Bool) TRUE;
  }

    /* call fcuntion to INIT the freeze frame buffer */
  DEH_FFiF_InitFF();
  

    /* call init function for all StatusOfDTC information */
  DEH_SUiF_InitStatusOfDTC();

    /* inform the application about DEH INIT if */
    /* SNAP SHOT SUPPORT is defined             */
#if ((defined(DEH_SUP_UDS)) && (defined (DEH_SUP_SNAP_SHOT_DATA)))
    APL_DEH_SnapShotDatenInit(DEH_INIT);
#endif

  return(retVal_u8);
} /* end void DEH_Init(void) */


/*============================================================================
function name    :  DEH_ReadAddFreezeFrameData()
input parameter  :  tDEH_DTC DTC - Diagnostic Trouble Code
                    tDEH_FreezeFrameNumber FF_Number - Number Freeze Frame
                    uint8_t *pNumOfBytes - Number of copied bytes
                    tDEH_AddFreezeFrame *pAddFFrames - buffer for the copied bytes
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read stored additional freeze frames for a DTC
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
31.10.03    GIGE    built base version 

============================================================================*/
#ifdef DEH_SUP_KWP2000
tDEH_Return DEH_ReadAddFreezeFrameData(tDEH_DTC Dtc, tDEH_FreezeFrameNumber FF_Number,uint8_t *pNumOfBytes, tDEH_AddFreezeFrame *pAddFFrames)
{
  tDEH_Return       retVal_u8 = DEH_E_UNKNOWN_DTC;

  /* ------------------------------------------------------------------------ */
  /* if addtitional freeze frames are stored in the event memory              */
  /* ------------------------------------------------------------------------ */
 #ifdef DEH_READ_ADD_FF
  uint16_t          EvNum_u16;
  tDEH_DTC          DTC_uT;
  uint8_t           Buffer_u8[EMB_BL_SIZE_ADDFF];
  tDEH_EventNumber  k;
  _Bool             foundNext_b = (_Bool) FALSE;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {

    if (DEH_SRV_lastRecFct_mu8 != DEH_SRV_KWP_READ_ADD_FF)
    {
        /* reset the internal event position counter */
      DEH_cntEvent_mu16 = (uint16_t) 0;
    }

      /* test for FF_Number*/
    if (FF_Number == DEH_KWP_READ_ALL_FF)
    {
        /* look in the event parameters for the DTC */
      for (k = (tDEH_EventNumber)0; 
           k < (tDEH_EventNumber) DEH_MAX_NUMBER_OF_EVENTPATHS; k++)
      {
        if ( ( (tDEH_DTC) DEH_EVENT_DTC(k) )== Dtc)
        {
             /* set flag for a known DTC                    */
             /* and the actual return code NO freeeze frame */
           retVal_u8 = DEH_E_UNKNOWN_FF;
        }
      }     

        /* loop for search in the event memory */
        /* internal counter is not greater tham the event memory  AND */
        /* found flag is FALSE                                    AND */
        /* a valid DTC is received                                    */
      while (    (DEH_cntEvent_mu16 < DEH_MAX_NUMBER_EV_MEM) 
              && (!foundNext_b) 
              && (retVal_u8 != DEH_E_UNKNOWN_DTC) ) 
      {
          /* read the stored event number */
        EvNum_u16 = DEH_EventMemBuffer[DEH_cntEvent_mu16];

          /* if in the actual eventnumber a correct event ( NOT FREE) and */
          /* valid value                                                  */
        if ( (EvNum_u16 != DEH_EM_FREE_POS) && 
             (EvNum_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS))
        {
              /* read the connected DTC */
            DTC_uT = (tDEH_DTC)DEH_EVENT_DTC(EvNum_u16);

            /* test if the received DTC equal to the read DTC */
          if (DTC_uT == Dtc)
          {
              /* set flag for sucessfull found DTC */
            foundNext_b = (_Bool) TRUE;

              /* read out the whole stored event memory */
            retVal_u8 = APL_DEH_EEPReadEventData(
                                         (uint16_t) ((EMB_BLOCK_SIZE*DEH_cntEvent_mu16)+ EMB_POS_ADDFF),
                                         (uint16_t)   EMB_BL_SIZE_ADDFF,
                                                   &Buffer_u8[0]               ); 
              /* test result and set failure flag */
            if (retVal_u8 != DEH_OK)
            {
                 /* set failure flag for EEP Read problems */
              DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_READ; 
            }

              /* copy all stored in formation in the received struct buffer  */
              /* and read the not stored information from the event dataset  */

              /* block to copy the stored additional freeze frames to the buffer */
              /* --------------------------------------------------------------- */

              /* reset the number of bytes buffer */
            *pNumOfBytes =  (uint8_t) 0; 

            for (k =((tDEH_EventNumber) 0) ;
                 k < ((tDEH_EventNumber) DEH_MAX_NUMBER_ADD_FF); k++)
            {
              pAddFFrames[k].formula     =   
                             Buffer_u8[k*DEH_SIZEOF_ADDFF];

              pAddFFrames[k].normValue   = 
                             Buffer_u8[k*DEH_SIZEOF_ADDFF+1];

              pAddFFrames[k].measurement = 
                             Buffer_u8[k*DEH_SIZEOF_ADDFF+2];

                /* increase the byte counter of copied freeze frames */
              *pNumOfBytes +=(uint8_t) DEH_SIZEOF_ADDFF;

            } /* end for k */
          }  /* end if (DTC_uT == Dtc) */
        } /* end  if ( (EvNum_uT != DEH_EM_FREE_POS) &&  .. */

          /* increase the event counter */
        DEH_cntEvent_mu16 += (uint16_t) 1 ;

      }  /* end while */
    } /* end mode FF_number == ALL FREEZE FRAMES */
    else
    { 
      retVal_u8 = DEH_E_UNKNOWN_FF;
    }
  }
  else /* no correct init */
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }  
    /* store the last received service function */
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_KWP_READ_ADD_FF;

  /* ----------------------------------------------------------------------- */
  /* generate only a failure code if there are no add freeze frames stored   */
  /* ----------------------------------------------------------------------- */
#else
  retVal_u8 = DEH_E_NO_DATA;
#endif

  return(retVal_u8);
} /* end of DEH_ReadAddFreezeFrameData() */
#endif  /* DEH_SUP_KWP2000 */

/*============================================================================
function name    :  DEH_ReadStdFreezeFrameData()
input parameter  :  tDEH_DTC DTC - Diagnostic Trouble Code
                    tDEH_StdFreezeFrameData *pStdFFrames - buffer for the copied bytes
output parameter :   
globals          :   
return value     :  tDEH_Return
description      :  Function to read the stored standard freeze frame for a DTC
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
31.10.03    GIGE    built base version 

============================================================================*/
#ifdef DEH_SUP_KWP2000
tDEH_Return DEH_ReadStdFreezeFrameData(tDEH_DTC Dtc, tDEH_StdFreezeFrameData *pStdFFrames)
{

    /* tmp. Variable for Return value -> init with DEH_OK */
  tDEH_Return       retVal_u8 = DEH_E_UNKNOWN_DTC;
  uint16_t          EvNum_u16;
  tDEH_DTC          DTC_uT;
  uint8_t           Buffer_u8[EMB_BLOCK_SIZE - EMB_BL_SIZE_ADDFF];
  tDEH_EventNumber  i;
  uint8_t           k;
  _Bool             foundNext_b = (_Bool) FALSE;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
    if (DEH_SRV_lastRecFct_mu8 != DEH_SRV_KWP_READ_STD_FF)
    {
        /* reset the internal event position counter */
      DEH_cntEvent_mu16 = (uint16_t) 0;
    }

     /* look in the event parameters for the DTC and set correct return code */
    for (i = (tDEH_EventNumber)0; 
         i < (tDEH_EventNumber) DEH_MAX_NUMBER_OF_EVENTPATHS; 
       i++)
    {
      if ( ( (tDEH_DTC) DEH_EVENT_DTC(i) )== Dtc)
      {
           /* set flag for a known DTC                    */
           /* and the actual return code NO freeeze frame */
         retVal_u8 = DEH_E_UNKNOWN_FF;
      }
    }     

        /* loop for search in the event memory */
        /* internal counter is not greater tham the event memory  AND */
        /* found flag is FALSE                                    AND */
        /* a valid DTC is received                                    */
        /* loop for search in the event memory */
    while (    (DEH_cntEvent_mu16 < DEH_MAX_NUMBER_EV_MEM) 
            && (!foundNext_b) 
            && (retVal_u8 != DEH_E_UNKNOWN_DTC)           )
    {
        /* read the stored event number */
      EvNum_u16 = DEH_EventMemBuffer[DEH_cntEvent_mu16];

        /* if in the actual eventnumber a correct event ( NOT FREE) and */
        /* valid value                                                  */
      if ( (EvNum_u16 != DEH_EM_FREE_POS) && 
           (EvNum_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS))
      {
          /* read the connected DTC */
        DTC_uT = (tDEH_DTC) DEH_EVENT_DTC(EvNum_u16);

          /* test if the received DTC equal to the read DTC */
        if (DTC_uT == Dtc)
        {
            /* set flag for sucessfull found DTC */
          foundNext_b = (_Bool) TRUE;

            /* read out the whole stored event memory */
          retVal_u8 = APL_DEH_EEPReadEventData
                                ( (uint16_t) (EMB_BLOCK_SIZE*DEH_cntEvent_mu16) ,
                                  (uint16_t) (EMB_BLOCK_SIZE - EMB_BL_SIZE_ADDFF),
                                            &Buffer_u8[0]                       ); 
            /* test result and set failure flag */
          if (retVal_u8 != DEH_OK)
          {
               /* set failure flag for EEP Read problems */
            DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_READ; 
          }
            /* get all relevant information from the read information */
          else
          {
              /* copy all stored information in the received struct buffer   */
              /* and read the not stored information from the event dataset  */

            pStdFFrames->StatusOfDTC = DEH_SUiF_StatusOfDTC(EvNum_u16, SERVICE_REQ);

              /* get the priority  and fault symptom and shift to right */
            pStdFFrames->Priority = (uint8_t) (DEH_GET_EVENT_PRIORITY(EvNum_u16));

              /* occurance counter */
            pStdFFrames->OCC = Buffer_u8[EMB_POS_OCC];

              /* limt to unlearn or clear the stored event */
            pStdFFrames->UnlearnCounter = Buffer_u8[EMB_POS_LUC];

              /* block to copy the stored standard freeze frames to the buffer */
              /* ------------------------------------------------------------- */

              /* if standard freeze frame are stored in the event memory buffer */
              /* -------------------------------------------------------------- */
  #ifdef DEH_READ_STD_FF

            for (k =((uint8_t) 0) ; k < DEH_SIZEOF_STDFF_KM; k++)
            {
              pStdFFrames->Km[k] = Buffer_u8[EMB_POS_STDFF+k];

            } /* end for (k = ((uint8_t) 0) ; k < DEH_SIZEOF_STDFF_KM; k++) */

            for (k = ((uint8_t) 0) ; k < DEH_SIZEOF_STDFF_TIME; k++)
            {
              pStdFFrames->TimeStamp[k] = Buffer_u8[  EMB_POS_STDFF
                                                      + DEH_SIZEOF_STDFF_KM
                                                      + k];
            } /* end for (k = 0 ; k < DEH_SIZEOF_STDFF_TIME; k++) */
  #endif 

              /* -------------------------------------------------------------- */
              /* if no standard freeze frame stored in the event memory buffer  */
              /* -------------------------------------------------------------- */

  #ifndef DEH_READ_STD_FF
              for (k =((uint8_t) 0); k < DEH_SIZEOF_STDFF_KM; k++)
              {
                 pStdFFrames->Km[k] = (uint8_t) DEH_STDFF_KM_NOT_AVAILABLE;
              } /* end for */

              for (k =((uint8_t) 0); k < DEH_SIZEOF_STDFF_TIME; k++)
              {
                pStdFFrames->TimeStamp[k] = (uint8_t) DEH_STDFF_TIME_NOT_AVAILABLE;
              } /* end for */

  #endif
                /* set the TIME INDICATION Bit to a fix value */
              pStdFFrames->TimeIndicationBit = DEH_TIME_IND_BIT_VAL;
           }

          } /* end if (DTC_uT == Dtc)*/
        }  /* end  if ( (EvNum_uT != DEH_EM_FREE_POS) &&  .. */

         /* increase the event counter */
      DEH_cntEvent_mu16 += (uint16_t) 1 ;

    } /* end while */
      /* store the last received service function */
  }
  else  /* no correct init*/
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }
  
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_KWP_READ_STD_FF;

  return(retVal_u8);
} /* end of DEH_ReadStdFreezeFrameData() */
#endif  /* DEH_SUP_KWP2000 */

/*============================================================================
function name    :  DEH_ReadIdentifiedEvents ()
input parameter  :  tDEH_DTCGroup DTCGroup - Group of wanted DTCs
                    tDEH_DTCAndStatus* pDestination - pointer to a buffer 
                    uint8_t Eventmode_u8 - special mode_u8 for filtering information
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read the identified events and there status.
                    With the paramter Eventmode_u8 it is planed to filter the
                    information (active or passive events etc.).
                    This feature has to be defined later.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
12.10.03    GIGE    built base version 
13.11.03    GIGE    first implementation

============================================================================*/
#ifdef DEH_SUP_KWP2000
tDEH_Return DEH_ReadIdentifiedEvents (tDEH_DTCGroup DTCGroup, tDEH_DTCAndStatus* pDestination, uint8_t Eventmode_u8)
{
    /* counter for service DEH_ReadIdentified events */
  static tDEH_EventNumber DEH_numOfReadEvents_muT;

    /* message for detected and stored events in event memory */
  static tDEH_EventNumber DEH_numOfIdentEvents_muT;

  tDEH_Return       retVal_u8 = DEH_E_DATA_STILL_AVAILABLE;
  _Bool             foundNext_b   = FALSE;
  uint16_t          tmp_event_u16;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
    if ((DTCGroup == DEH_DTC_ALL_GROUPS) && (Eventmode_u8 == DEH_READ_ALL_EVENTS)) 
    {
      if (DEH_SRV_lastRecFct_mu8 != DEH_SRV_KWP_READ_IDT_EVENTS)
      {
          /* read the actual number of ident events in the event memory buffer */
        DEH_GetNumberOfIdentEvents (DEH_DTC_ALL_GROUPS, &DEH_numOfIdentEvents_muT);

          /* set the actual read out counter to NULL */
        DEH_numOfReadEvents_muT = (tDEH_EventNumber) 0;

          /* reset the internal event position counter */
        DEH_cntEvent_mu16 = (uint16_t) 0;
      }

      while (    (DEH_numOfReadEvents_muT < DEH_numOfIdentEvents_muT) 
              && (DEH_cntEvent_mu16 < DEH_MAX_NUMBER_EV_MEM)
              && (!foundNext_b) ) 
      {
          /* read the content of event buffer */
        tmp_event_u16 = DEH_EventMemBuffer[DEH_cntEvent_mu16];

          /* test for a valid event memory entry */
        if (    ( tmp_event_u16 != DEH_EM_FREE_POS ) 
             && ( tmp_event_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS) )
        {

            /* test the current diagnose session                             */
            /* read out the event in event memory if :                       */
            /* 1) - current diag. session is NOT equal to Developper session */
            /*    AND - prio is smaller than 9                               */
            /* OR                                                            */
            /* 2) diagnose session IS EQUAL TO Developper session            */
          if(   (   (DEH_actDiaSes_muT != DEH_ENTW_DIAG_SESSION) 
                 && (DEH_GET_EVENT_PRIORITY(tmp_event_u16)
                                             < DEH_DEVELOP_EVENT_PRIO))
            || (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION))
          {

              /* store the StOfDTC to the given address */
            pDestination->StatusOfDTC = DEH_SUiF_StatusOfDTC(tmp_event_u16, SERVICE_REQ);

              /* write the DTC to the position */
            pDestination->DTC = (tDEH_DTC) DEH_EVENT_DTC(tmp_event_u16);

              /* increase the counter for successfull read */
            DEH_numOfReadEvents_muT +=(tDEH_EventNumber) 1;
                         
              /* set flag for abort the searching after one sucessfull result*/
            foundNext_b =(_Bool) TRUE;
          }
        }  /* end  if ( DEH_EventMemBuffer[DEH_cntEvent_mu16] != DEH_EM_FREE_POS) */

          /* increase the event memory position counter */
        DEH_cntEvent_mu16 += (uint16_t) 1;

      } /* end while */

        /* store the last received service function */
      DEH_SRV_lastRecFct_mu8 = DEH_SRV_KWP_READ_IDT_EVENTS;

        /* analyze the result for this function */
        /* all events are read out OR           */
        /* the internal counter for event memory is greater as the event memory  */
        /* OR NO identified events                                               */
        /* OR NO further event are found in the event memory                     */
      if ( ( DEH_numOfReadEvents_muT == DEH_numOfIdentEvents_muT) || 
             (DEH_cntEvent_mu16 >= DEH_MAX_NUMBER_EV_MEM )        ||
             (!DEH_numOfIdentEvents_muT )                         || 
             (!foundNext_b)                                         )
      {
           /* test if events are stored in event memory */
        if (DEH_numOfIdentEvents_muT )
        {
            /* all events are read out */
          retVal_u8 = DEH_OK;

        }
        else
        {
            /* no data available */
          retVal_u8 = DEH_E_NO_DATA;
        }
          /* reset the last diagnose command to init this function */
        DEH_SRV_lastRecFct_mu8 = DEH_SRV_RSE_RESET;
      } /* end if (4 conditions ) */
    } /*  end  if (DTCGroup == DEH_DTC_ALL_GROUPS) */
    else
    {
        /* reset the last diagnose command to init this function */
      DEH_SRV_lastRecFct_mu8 = DEH_SRV_RSE_RESET;

      if (DTCGroup != DEH_DTC_ALL_GROUPS)
      {
          /* set the return code for unknown DTC-group */
        retVal_u8 = DEH_E_UNKNOWN_DTCGROUP;
      }
      else
      {
          /* set the return code for wrong mode */
        retVal_u8 = DEH_E_MODE;
      }
    }
  }
  else  /* no correct init*/
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }
  return(retVal_u8);
} /* end of DEH_ReadIdentifiedEvents() */
#endif  /* DEH_SUP_KWP2000 */

/*============================================================================
function name    :  DEH_ReadSupportedEvent ()
input parameter  :  tDEH_DTCGroup DTCGroup
                    tDEH_DTCAndStatus* pDestination
output parameter :   
globals          :   
return value     :  tDEH_Return
description      :  Function to read all supported events and there status.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
12.10.03    GIGE    built base version 

============================================================================*/
#ifdef DEH_SUP_KWP2000
tDEH_Return DEH_ReadSupportedEvent (tDEH_DTCGroup DTCGroup, tDEH_DTCAndStatus* pDestination)
{
    /* tmp. Variables */
  uint8_t tmp_val1_u8 ;

    /* tmp. Variable for Return value -> init with DEH_OK */
  tDEH_Return retVal_u8;
  retVal_u8 = DEH_OK;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
    if (DTCGroup == DEH_DTC_ALL_GROUPS)
    {
      if (DEH_SRV_lastRecFct_mu8 != DEH_SRV_KWP_READ_SUP_EVENTS)
      {
          /* reset the internal counter to read the event data */
        DEH_cntEvent_mu16 = (uint16_t) 0;
      }

      if (DEH_cntEvent_mu16 < DEH_MAX_NUMBER_OF_EVENTPATHS)
      {
          /* Block to read the supported Events and there datas */
        tmp_val1_u8 = DEH_SUiF_StatusOfDTC(DEH_cntEvent_mu16, SERVICE_REQ);
      
          /* write the result StatusOfDTC to the position */
        pDestination->StatusOfDTC = tmp_val1_u8;

          /* write the DTC to the position */
        pDestination->DTC = (tDEH_DTC) DEH_EVENT_DTC(DEH_cntEvent_mu16);

      } 
      /* end if (DEH_cntEvent_mu16 < DEH_MAX_NUMBER_OF_EVENTPATHS)*/

        /* increase the internal counter */
      DEH_cntEvent_mu16 += (uint16_t) 1;

      if (DEH_cntEvent_mu16 >= DEH_MAX_NUMBER_OF_EVENTPATHS)
      {
          /* reset the internal vounter to read the event data */
        DEH_cntEvent_mu16 = (uint16_t) 0;

      } /* if (DEH_cntEvent_mu16 >= DEH_MAX_NUMBER_OF_EVENTPATHS) */
      else
      {
          /* further datas are available */
        retVal_u8 = DEH_E_DATA_STILL_AVAILABLE;

      }/* end else (DEH_cntEvent_mu16 >= DEH_MAX_NUMBERS_OF_EVENTPATHS) */

    } /* end if start Read Supported Events */
    else                                                                                                                               
    {
        /* return value is unknown DTC-Group */
      retVal_u8 = DEH_E_UNKNOWN_DTCGROUP;

    } /* end else wrong DTC Group for Read Supported Events */
  }
  else  /* no correct init*/
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }

    /* store the last received service function */
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_KWP_READ_SUP_EVENTS;

  return(retVal_u8);
} /* end of function() */
#endif  /* DEH_SUP_KWP2000 */

/*============================================================================
function name    :  DEH_SetDiagnosticSession()
input parameter  :  tDEH_DiagnosticType DiagnosticType
output parameter :  tDEH_Return
globals          :   
return value     :   
description      :  Function to inform the DEH about the actual diagnostic
                    session. This information is important because some
                    function and features from DEH are connected to a special 
                    diagnostic session
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
12.10.03    GIGE    built base version 

============================================================================*/
tDEH_Return DEH_SetDiagnosticSession (tDEH_DiagnosticType DiagnosticType)
{
  tDEH_Return retVal_u8; 

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {

    switch (DiagnosticType)
    {
      case DEH_EXT_DIAG_SESSION:

      case DEH_VW_EOL_DIAG_SESSION:
  
    #ifdef DEH_SUP_KWP2000
      case DEH_EOBD_DIAG_SESSION:
    #endif

      case DEH_ECU_PRG_DIAG_SESSION:
  
      case DEH_ENTW_DIAG_SESSION:

      case DEH_VW_DEFAULT_DIAG_SESSION:

          /* set function return code */
        retVal_u8 = DEH_OK;
    
        if(DEH_actDiaSes_muT != DiagnosticType)
        {
            /* deactivate all blocked DTCs */
          DEH_ControlDTCSetting (DEH_SET_DTC_ON , DEH_DTC_ALL);
        }
          /* set the system message to the received Diagnostic Type */
        DEH_actDiaSes_muT = DiagnosticType;

      break; /* end if test diagnostic session */

      default: 
  
          /* set function return code */
        retVal_u8 = DEH_E_UNKNOWN_SESSION_TYPE;
  
          /* set the actual diagnose mode_u8 to NO DIAG SESSION */
        DEH_actDiaSes_muT = DEH_NO_DIAG_SESSION;
  
        /* end else test diagnostic session */
      break;
    } /*end switch case */
  }
  else  /* no correct init*/
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }
    /* store the last received service function */
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_SET_DIAG_SES;
  
  return (retVal_u8);
} /* end of funtion DEH_SetDiagnosticSession() */

/*============================================================================
function name    :  DEH_StM_Event()
input parameter  :  - 
output parameter :  tDEH_Return
globals          :  
return value     :  
description      :  function to init the  event-statemachine and
                    to test for valid parameters and messages
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
11.10.03    GIGE    built base version 

============================================================================*/
tDEH_Return DEH_StM_Event(tDEH_EventNumber DEH_EventNumber_muT, _Bool DEH_StEV_stRepEvent_mu8, uint8_t DEH_StEV_ExtEventFkt_mu8)
{ 
  uint8_t             tmp_val1_u8  = (uint8_t) 0;
  tDEH_Return         retVal_u8    =  DEH_OK;
  
    /* actual event buffer state */
  uint8_t DEH_StEV_EvBuf_State_mu8;

    /* actual event state */
  uint8_t DEH_StEV_state_mu8; 

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
    /* message for a event state change indication */
  static uint8_t DEH_StEV_chgState_mu8;
#endif
    /* test for a valid Event Number */
  if(DEH_EventNumber_muT >= DEH_MAX_NUMBER_OF_EVENTPATHS)
    {
      retVal_u8 = (tDEH_Return) DEH_E_UNKNOWN_EVENTPATH;
    }
  else /* Start parameter analyze and call   Event-Statemachine */
  {

      /* get the actual state of event */
    DEH_StEV_EvBuf_State_mu8 = DEH_EVENT_BUFFER_STATE(DEH_EventNumber_muT);
                                                                 
                               
      /* get the Event state (first 3 Bit in Event Buffer State (DEH_StEV_EvBuf_State_mu8)*/
    DEH_StEV_state_mu8 =(uint8_t) (DEH_StEV_EvBuf_State_mu8 & ((uint8_t) DEH_EB_MSK_EV_ST));

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
    /* set the event state change flag to FALSE*/
  DEH_StEV_chgState_mu8 =(uint8_t) FALSE; 
#endif

  switch (DEH_StEV_state_mu8)
  { 
    case DEH_EVENT_PASSIVE :
    {
         /* test for condition to change the state ( T0_1_2 )*/
         /* first condition : external function call DEH_ReportEvent() AND */
         /* second condition: the event status  == TRUE                   */
       if ((DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
           (DEH_StEV_stRepEvent_mu8) )
       {
           /* change the event state to DEBOUNCE PASSIVE */
         DEH_StEV_state_mu8 = DEH_EVENT_DEBOUNCE_PASSIVE;

#ifdef DEH_SUP_UDS
  #ifdef DEH_SUP_UDS_OBD_STATUS 
           /* set the pending flag for this event if DTC NOT LOCKED*/
        if (! (DEH_EVENT_BUFFER_STATE(DEH_EventNumber_muT) & DEH_EB_MSK_LOCK_DTC) )
        {
          DEH_SET_PENDING_DTC(DEH_EventNumber_muT);
        }
  #endif
           /* set test failed last state to ONE */
         DEH_SET_TEST_LAST_STATE(DEH_EventNumber_muT);
#endif
           /* start the timer or counter to debounce the event */         
         DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) = (tDEH_QualET) 1;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
           /* flag to indicate a event state change */
         DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif

            /* read freeze frames if the event not stored (first detetction) */
            /* AND DTC is not locked                                         */
         if (   (!(DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_EV_STORED) ) 
             && (! (DEH_EVENT_BUFFER_STATE(DEH_EventNumber_muT) & DEH_EB_MSK_LOCK_DTC)))
         {
            /* set the flag to read freeze frames for this event */
           DEH_StEV_EvBuf_State_mu8 |= DEH_EB_MSK_READ_FF;

            /* set the GLOBAL flag for Read Freeze Frames necesarry */
           DEH_stReadFF_b = (_Bool) TRUE;
         }
       }
#ifdef DEH_SUP_UDS
       else
       {
           /* clear test failed last state */
         DEH_CLR_TEST_LAST_STATE(DEH_EventNumber_muT);
       }
#endif
       break;
    }
    /* end DEH_EVENT_PASSIVE state */
    
      /* special init state after initialisation of the system if a fault was */
      /* ACTIVE STORED                                                        */
    case DEH_EVENT_DEB_ACT_STO:

        tmp_val1_u8  =(uint8_t) (DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_DTC_RB);
        /* T1_0_1 */
        /* Update Debounce Value if one of  two Conditions ist TRUE */
        /*-----------------------*/
        /* I.  - Testes Bit is Set to ZERO AND                      */
        /*     - DEH_ReportEvent(state == FALSE) AND                */
        /*     - cyclic Debounce Event                              */
        /* II. - Testes Bit is Set  AND                             */
        /*     - cyclic Task is ACTIVE (DEH_StEV_zykTst_u8 == TRUE )*/
        /*     - Event is time debounced                            */
        if (      (!tmp_val1_u8)
               && (  (DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
                     (!DEH_StEV_stRepEvent_mu8) &&
                     ( DEH_EVENT_DEBTYPE(DEH_EventNumber_muT) == 
                                                           (uint8_t) EVENT_DEB)
                  )
           )
        {
            /* test if no overflow */
          if (DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) < DEH_DEB_OVERFLOW)
          {
              /* inkrement the content of debounce buffer */
            DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) +=(tDEH_QualET) 1;
          }
        } /* end T1_0_1 */

          /* T1_1_4 */
          /* test if an Event Report with state TRUE */
        else if ( (DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
                  (DEH_StEV_stRepEvent_mu8) )
        {
#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
            /* set the stae change indication */
          DEH_StEV_chgState_mu8 = (_Bool) TRUE;  
#endif
            /* change the event state to ACTIVE */
          DEH_StEV_state_mu8 = DEH_EVENT_ACTIVE;
#ifdef DEH_SUP_UDS  
  #ifdef DEH_SUP_UDS_OBD_STATUS
          DEH_SET_TEST_FAILED_TOC(DEH_EventNumber_muT);
             /* set the pending flag for this event if DTC NOT LOCKED*/
          if (! (DEH_EVENT_BUFFER_STATE(DEH_EventNumber_muT) & DEH_EB_MSK_LOCK_DTC) )
          {
            DEH_SET_PENDING_DTC(DEH_EventNumber_muT);
          }
  #endif
            /* set Test Failed Last State to ONE*/
          DEH_SET_TEST_LAST_STATE(DEH_EventNumber_muT);
#endif
        } /* end trasition T1_1_4 */
    
          /* T0_3_0 */
          /* test if this the first Event report (Tested Bit NOT ZERO) */
          /* with state FALSE                                          */
          /* if this TRUE than start the debouncing                    */
        else if ( 
                   (DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_DTC_RB) &&
                   (DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
                   (!DEH_StEV_stRepEvent_mu8)
                )
        {
#ifdef DEH_SUP_UDS
             /* clear Test Failed Last State */
           DEH_CLR_TEST_LAST_STATE(DEH_EventNumber_muT);
#endif
             /* start the timer or counter to debounce the event */         
           DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) = (tDEH_QualET) 1;
        
        }  /* end T0_3_0 */

      break;

    case DEH_EVENT_DEBOUNCE_PASSIVE :
    {
         /* T2_1_0 and T2_2_0                                              */
         /* test for condition to change the state                         */
         /* first condition : external function call DEH_ReportEvent() AND */
         /* second condition: the event status  ==(uint8_t) FALSE                   */
         /* OR              : external function call DEH_ResetEvent()      */
      if ( ( (DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
             (!DEH_StEV_stRepEvent_mu8)                        ) ||
           (DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)           )
      {
          /* stop the timer or counter to debounce the event */
        DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) = DEH_DEB_STOP;

          /* change the event state to PASSIVE */
        DEH_StEV_state_mu8 = DEH_EVENT_PASSIVE;

#ifdef DEH_SUP_UDS
          /* clear Test Failed Last State */
        DEH_CLR_TEST_LAST_STATE(DEH_EventNumber_muT);
#endif

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
          /* flag to indicate a event state change */
        DEH_StEV_chgState_mu8 =(uint8_t) TRUE;
#endif
        if (DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)
        {
            /* reset the DTC Readiness Bit (set to not tested) */
          DEH_StEV_EvBuf_State_mu8 |= (uint8_t) DEH_EB_MSK_DTC_RB;
        }

          /* clear stored freeze frames */
        if ( !(DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_EV_STORED) ) 
        {
             retVal_u8 = DEH_FFiF_ClearFF(DEH_EventNumber_muT);
        }

      } /* end if T2_1_0 and T2_2_0  */

          /* T2_3_3                                      */
          /* if external Function Call DEH_SuspendEvent()*/
      else if (DEH_StEV_ExtEventFkt_mu8 == DEH_SUSPEND_EVENT)
      {
          /* change the event state to PASSIVE DEBOUNCE SUSPEND*/
        DEH_StEV_state_mu8 =  DEH_EVENT_DEB_PAS_SUSPEND;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
          /* flag to indicate a event state change */
        DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif     
      } /* end T2_3_3 (Suspend Receive)*/

         /* T2_5_2                                                     */
         /* if a  new event report for a cycle debounce event OR       */
         /* if the cyclic Task activ and a time debounce event have to */
         /* update the Debounce value                                  */
      else if (   (DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
                   DEH_StEV_stRepEvent_mu8                       &&
                  (DEH_EVENT_DEBTYPE(DEH_EventNumber_muT) 
                                                      == (uint8_t) EVENT_DEB ) )
      {
          /* test if no overflow */
        if (DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) < DEH_DEB_OVERFLOW)
        {
            /* increment the content of debounce buffer */
          DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) +=(tDEH_QualET) 1;
        }
    
      } /* end T2_5_2 */
      
      break;
    }
      /* end DEH_EVENT_DEBOUNCE_PASSIVE state */    

    case DEH_EVENT_DEB_PAS_SUSPEND :
    {
          /* T3_1_0 and T3_2_0 */
        if ((DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)  ||
            (DEH_StEV_ExtEventFkt_mu8 == DEH_RESTART_EVENT)  )
        {
            /* stop the timer or counter to debounce the event */
          DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) = DEH_DEB_STOP;

            /* change the event state to PASSIVE */
          DEH_StEV_state_mu8 = DEH_EVENT_PASSIVE;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
            /* flag to indicate a event state change */
          DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif
          if (DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)
          {
              /* reset the DTC Readiness Bit (set to not tested) */

            DEH_StEV_EvBuf_State_mu8 |= (uint8_t) DEH_EB_MSK_DTC_RB;
          }
  
          if ( !(DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_EV_STORED) ) 
          {
               /* call the function to clear the stored Freeze Frames */
            retVal_u8 = DEH_FFiF_ClearFF(DEH_EventNumber_muT);

          }
        } /* end if T3_1_0 and T3_2_0 */

        /* T3_3_2                                 */
        /* test for a received  DEH_ResumeEvent() */
      else if (DEH_StEV_ExtEventFkt_mu8 == DEH_RESUME_EVENT)
      {
          /* change the event state to DEBOUNCE_PASSIVE */
        DEH_StEV_state_mu8 = DEH_EVENT_DEBOUNCE_PASSIVE;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
          /* flag to indicate an event state change */
        DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif
      } /* end T3_3_2 */

      break;
    }
      /* end DEH_EVENT_IS_DEB_PAS_SUSPEND state */    

    case DEH_EVENT_ACTIVE :
    {
         /* test for condition to change the state (T4_1_6)                */
         /* first condition : external function call DEH_ReportEvent() AND */
         /* second condition: the event status  ==(uint8_t) FALSE          */
       if ((DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
           (!DEH_StEV_stRepEvent_mu8) &&
           (!(DEH_EVENT_PARAMETER( DEH_EventNumber_muT) & DEH_EP_MSK_EV_NO_CLEAR)))
       {
           /* change the event state to DEBOUNCE ACTIVE */
         DEH_StEV_state_mu8 = DEH_EVENT_DEBOUNCE_ACTIVE;
#ifdef DEH_SUP_UDS
           /* clear Test Failed Last State */
         DEH_CLR_TEST_LAST_STATE(DEH_EventNumber_muT);
#endif
           /* start the timer or counter to debounce the event */
         DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) = (tDEH_QualET) 1;
       
#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
           /* flag to indicate an event state change */
         DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif
       }
#ifdef DEH_SUP_UDS
       else
       {
           /* set test failed last state */
         DEH_SET_TEST_LAST_STATE(DEH_EventNumber_muT);
       }
#endif      
      break;
    }
      /* end DEH_EVENT_ACTIVE state */    
          
    case DEH_EVENT_DEBOUNCE_ACTIVE :
    {
        /* T6_1_4 and T6_2_4 */
        /* test for condition to change the state */
        /* first condition : external function call DEH_ReportEvent() AND */
        /* second condition: the event status  == TRUE                   */
      if ( ((DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
            (DEH_StEV_stRepEvent_mu8)                         )||
           (DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)          )
      {
           /* T6_1_4 */
           /* Reset Event */
        if (DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)
        {
            /* reset the DTC Readiness Bit (set to not tested) */
          DEH_StEV_EvBuf_State_mu8 |=(uint8_t)DEH_EB_MSK_DTC_RB;
        } 
          /* T6_1_4 */
  
            /* change the event state to ACTIVE */
        DEH_StEV_state_mu8 = DEH_EVENT_ACTIVE;
#ifdef DEH_SUP_UDS
          /* set Test Failed Last State to ONE*/
        DEH_SET_TEST_LAST_STATE(DEH_EventNumber_muT);
#endif
          /* stop the timer or counter to debounce the event */
        DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) = DEH_DEB_STOP;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
          /* flag to indicate an event state change */
        DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif
      } /* end T6_1_4 and T6_2_4 */

        /* T6_3_7 */
        /* Test for a DEH_SuspendEvent() */
      else if (DEH_StEV_ExtEventFkt_mu8 == DEH_SUSPEND_EVENT)
      {
          /* change the event state to DEH_EVENT_DEB_ACT_SUSPEND */
        DEH_StEV_state_mu8 = DEH_EVENT_DEB_ACT_SUSPEND;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
         /* flag to indicate an event state change */
        DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif
      }
        /* end if T6_3_7 */

        /* T6_5_6 */
        /* if a  new event report for a cycle debounce event OR       */
        /* if the cyclic Task activ and a time debounce event have to */
        /* update the Debounce value                                  */

        /* condition for state update debounce value                        */
        /* I. DEH_EventReport() for cycle deb. event AND with state = FALSE */
        /* cycle debounce => tmp_val1_u16 = 1 (!=0)                         */
        /* -----------------------------------------------------------------*/
        /* II.                                                              */
        /* cyclic Task is activ and event is time debounce                  */
        /* if I. OR ii. TRUE then update Debounce value                     */
          
      else if ( (DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT) &&
                (!DEH_StEV_stRepEvent_mu8)                     &&
                (DEH_EVENT_DEBTYPE(DEH_EventNumber_muT) == 
                                                       ((uint8_t) EVENT_DEB) ) )
      {
          /* test if no overflow occurs */
        if (DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) < DEH_DEB_OVERFLOW)
        {
            /* update the internal debounce buffer */
          DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT)+= (tDEH_QualET) 1;
        }

      } /* end T6_5_6 */

      break;
    }
      /*---------------------------------------------------------------------*/
      /* end DEH_EVENT_DEBOUNCE_ACTIVE state = 0x06 */    
      /*---------------------------------------------------------------------*/          
          
    case DEH_EVENT_DEB_ACT_SUSPEND :
    {
        /* T7_1_4 and T7_2_4 */
      if ((DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)  ||
          (DEH_StEV_ExtEventFkt_mu8 == DEH_RESTART_EVENT)  )
      {
          /* stop the timer or counter to debounce the event */
        DEH_EVENT_DEBOUNCE(DEH_EventNumber_muT) = DEH_DEB_STOP;

          /* change the event state to ACTIVE */
        DEH_StEV_state_mu8 = DEH_EVENT_ACTIVE;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
          /* flag to indicate a event state change */
        DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif
        if (DEH_StEV_ExtEventFkt_mu8 == DEH_RESET_EVENT)
        {
            /* reset the DTC Readiness Bit (set to not tested) */
          DEH_StEV_EvBuf_State_mu8 |= (uint8_t) DEH_EB_MSK_DTC_RB;

        }  /* end if RESET -> Reset DTC-TestComplete Bit */
      }  /* end T7_1_4 and T7_2_4 */
   
      /* T7_3_6                                 */
      /* test for a received  DEH_ResumeEvent() */
      else if (DEH_StEV_ExtEventFkt_mu8 == DEH_RESUME_EVENT)
      {
          /* change the event state to DEBOUNCE_ACTIVE */
        DEH_StEV_state_mu8 = DEH_EVENT_DEBOUNCE_ACTIVE;

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
          /* flag to indicate an event state change */
        DEH_StEV_chgState_mu8 =(uint8_t) TRUE; 
#endif
      } /* end T7_3_6 */

     break;
    }

    default:       
    { 
      
      break;
    }
      /* end DEH_EVENT_IS_DEB_ACT_SUSPEND state */    

  } /* end for switch - case */

    /* -----------------------------------------------------------------------*/
    /* MODUL : DEH_StM_Event ()  ... END ...                                  */
    /* -----------------------------------------------------------------------*/
  

    /* Update the new Event-State in the Event Buffer */
    /* get all information without the old event state */
  DEH_StEV_EvBuf_State_mu8 &= (uint8_t) DEH_EB_MSK_WO_ST_MSK;

    /* set the event state bits  to the new event state */
  DEH_StEV_EvBuf_State_mu8 |=(uint8_t) DEH_StEV_state_mu8;
    
    /* set the DTC TestComplete  bit to TESTED */
  if (DEH_StEV_ExtEventFkt_mu8 == DEH_REPORT_EVENT)
  {
      /* set the tested flag if received a DEH_ReportEvent()*/
    DEH_StEV_EvBuf_State_mu8 =(uint8_t)(DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_DTC_RB_SET);
      /* set Readiness Bit to Tested (NULL) */
    DEH_SET_TEST_COMPLETE(DEH_EventNumber_muT);

  }

        /* store the result in the Event-Buffer */
  DEH_EVENT_BUFFER_STATE(DEH_EventNumber_muT) = DEH_StEV_EvBuf_State_mu8;

/*----------------------------------------------------------------------------*/
/*      Sub Modul DEH_StM_SubM_TstChgSt for Detection Event state change      */
/*----------------------------------------------------------------------------*/
/* START compiler switch if option not activated then no compiling */
#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE

      if(DEH_StEV_chgState_mu8)            
      {
          /* get the EventParameter from Dataset */
        tmp_val1_u8 = DEH_EVENT_PARAMETER(DEH_EventNumber_muT);

          /* get the Flag for change state detection */
        tmp_val1_u8 =(uint8_t) (tmp_val1_u8 & DEH_EP_MSK_EV_REP_CHG_ST);

          /* test if flag active and call the ext. function */
        if (tmp_val1_u8) 
        {
          APL_DEH_ReportChgEventState(DEH_EventNumber_muT,
                                                      DEH_StEV_state_mu8);
        }
   /* end if (tmp_val1_u8) */

      } 
  /* END compiler switch if option not activated then no compiling */
#endif
    
   } /* end first else */ 

return (retVal_u8);
}

/*============================================================================
function name    :  DEH_StEV_Event()
input parameter  :  - 
output parameter :  tDEH_Return
globals          :  
return value     :  
description      :  function to init the  event-statemachine and
                    to test for valid parameters and messages
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
11.10.03    GIGE    built base version 

============================================================================*/
static tDEH_Return DEH_StM_Event_cyclic(void)
{ 
  uint8_t             tmp_val1_u8  = (uint8_t) 0;
  tDEH_Return         retVal_u8    =  DEH_OK;
  tDEH_EventNumber    EvNum_uT;

    /* actual event buffer state */
  uint8_t DEH_StEV_EvBuf_State_mu8;

    /* actual event state */
  uint8_t DEH_StEV_state_mu8; 

    /* flag for event memory state machine */
  _Bool DEH_StEV_chgSt_updEM_b;

    /* test for a valid Event Number */
  for ( EvNum_uT = (tDEH_EventNumber) 0;
        EvNum_uT < ((tDEH_EventNumber)DEH_MAX_NUMBER_OF_EVENTPATHS);
        EvNum_uT++)
  {

      /* get the actual state of event */
    DEH_StEV_EvBuf_State_mu8 = DEH_EVENT_BUFFER_STATE(EvNum_uT);
                                                                  
                               
      /* get the Event state (first 3 Bit in Event Buffer State (DEH_StEV_EvBuf_State_mu8)*/
    DEH_StEV_state_mu8 =(uint8_t) (DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_EV_ST);

      /* set the flag for update event memory and change state detection */
      /* to FALSE                                                        */
    DEH_StEV_chgSt_updEM_b = (_Bool) FALSE;

    switch (DEH_StEV_state_mu8)
    { 

        /* special init state after initialisation of the system if a fault was */
        /* ACTIVE STORED                                                        */
      case DEH_EVENT_DEB_ACT_STO:

      {      /* get the test complete bit */
          tmp_val1_u8  =(uint8_t) (DEH_StEV_EvBuf_State_mu8 & DEH_EB_MSK_DTC_RB);

            /* start cyclic analysis only if tested bit is set */
          if (!tmp_val1_u8 )
          {
          
              /* T1_0_1 */
              /*-----------------------*/
              /* conditions:                                              */
              /*     - Testes Bit is Set  AND                             */
              /*     - Event is time debounced                            */
            if( DEH_EVENT_DEBTYPE(EvNum_uT) == (uint8_t) TIME_DEB)
            {
                /* test if no overflow */
              if (DEH_EVENT_DEBOUNCE(EvNum_uT) < DEH_DEB_OVERFLOW)
              {
                  /* inkrement the content of debounce buffer */
                DEH_EVENT_DEBOUNCE(EvNum_uT) +=(tDEH_QualET) 1;
              }
            } /* end T1_0_1 */

              /* T1_2_0                                                       */
              /* Testes Bit is Set AND the debounce end value is reached than */
              /* change the state to PASSIVE                                  */

            if(  DEH_EVENT_DEBOUNCE(EvNum_uT) >= DEH_EVENT_DEQUAL(EvNum_uT) )
            {
                /* stop the timer or counter to debounce the event */
              DEH_EVENT_DEBOUNCE(EvNum_uT) = DEH_DEB_STOP;

                /* change the event state to PASSIVE */
              DEH_StEV_state_mu8 = DEH_EVENT_PASSIVE;

                /* set the flag for update event memory and change state */
                /* detection to TRUE                                     */
              DEH_StEV_chgSt_updEM_b = (_Bool) TRUE;

            } /* end T1_2_0 */
          } /* end test if tested bit is set */
        break;
      }

      case DEH_EVENT_DEBOUNCE_PASSIVE :
      {
          /* T2_4_4*/
          /* the actual debounce value is equal Or greater as the    */
          /* Parameter QualEvent in the Event Dataset                */
        if (  DEH_EVENT_DEBOUNCE(EvNum_uT) >= DEH_EVENT_QUAL(EvNum_uT) )
        {
            /* stop the timer or counter to debounce the event */
          DEH_EVENT_DEBOUNCE(EvNum_uT) = DEH_DEB_STOP;
            /* if freeze frame read bit is set      */
            /* look for bit 7 in event buffer state */
          if (DEH_EVENT_BUFFER_STATE(EvNum_uT) & DEH_EB_MSK_READ_FF)
          {
              /* call the function to read the Freeze Frames  */
            DEH_FFiF_ReadFF(EvNum_uT);
              /* reset the freeze frame flag */
            DEH_StEV_EvBuf_State_mu8 &= DEH_EB_MSK_RST_READ_FF;
          }
            /* change the event state to ACTIVE */
          DEH_StEV_state_mu8 = DEH_EVENT_ACTIVE;
#ifdef DEH_SUP_UDS_OBD_STATUS
          DEH_SET_TEST_FAILED_TOC(EvNum_uT);
          DEH_SET_TEST_FAILED_SLC(EvNum_uT);
#endif

            /* set the flag for update event memory and change state */
            /* detection to TRUE                                     */
          DEH_StEV_chgSt_updEM_b = (_Bool) TRUE;


        } /* end if T2_4_4 */

           /* T2_5_2                                                     */
           /*  time debounce event have to update the Debounce value     */
        else if ( (DEH_EVENT_DEBTYPE(EvNum_uT) == (uint8_t)TIME_DEB)) 
        {
            /* test if no overflow */
          if (DEH_EVENT_DEBOUNCE(EvNum_uT) < DEH_DEB_OVERFLOW)
          {
              /* increment the content of debounce buffer */
            DEH_EVENT_DEBOUNCE(EvNum_uT) +=(tDEH_QualET) 1;
          }
    
        } /* end T2_5_2 */
      
        break;
      }
        /* end DEH_EVENT_DEBOUNCE_PASSIVE state */    

      case DEH_EVENT_DEBOUNCE_ACTIVE :
      {

          /* start test T6_4_0 */
          /* debouncing ist sucessfull  */
        if (  DEH_EVENT_DEBOUNCE(EvNum_uT) >=  DEH_EVENT_DEQUAL(EvNum_uT) )  
        {
            /* stop the timer or counter to debounce the event */
          DEH_EVENT_DEBOUNCE(EvNum_uT) = DEH_DEB_STOP;

            /* change the event state to PASSIVE */
          DEH_StEV_state_mu8 = DEH_EVENT_PASSIVE;

            /* set the flag for update event memory and change state */
            /* detection to TRUE                                     */
          DEH_StEV_chgSt_updEM_b = (_Bool) TRUE;

        } 
          /* end if T6_4_0 */

          /* T6_5_6 */
          /* if a  new event report for a cycle debounce event OR       */
          /* if the cyclic Task activ and a time debounce event have to */
          /* update the Debounce value                                  */

          /* condition for state update debounce value                        */
          /* an event is time debounce                                        */
          
        else if (  DEH_EVENT_DEBTYPE(EvNum_uT) == ((uint8_t) TIME_DEB) )   
        {
            /* test if no overflow occurs */
          if (DEH_EVENT_DEBOUNCE(EvNum_uT) < DEH_DEB_OVERFLOW)
          {
              /* update the internal debounce buffer */
            DEH_EVENT_DEBOUNCE(EvNum_uT)+= (tDEH_QualET) 1;
          }

        } /* end T6_5_6 */

        break;
      }
        /*---------------------------------------------------------------------*/
        /* end DEH_EVENT_DEBOUNCE_ACTIVE state = 0x06 */    
        /*---------------------------------------------------------------------*/          
          
      default:       

             break;
        /* end DEH_EVENT_IS_DEB_ACT_SUSPEND state */    

    } /* end for switch - case */

    /* -----------------------------------------------------------------------*/
    /* MODUL : DEH_StM_Event ()  ... END ...                                  */
    /* -----------------------------------------------------------------------*/
  

    /* Update the new Event-State in the Event Buffer */
    /* get all information without the old event state */
  DEH_StEV_EvBuf_State_mu8 &= (uint8_t) DEH_EB_MSK_WO_ST_MSK;

    /* set the event state bits  to the new event state */
  DEH_StEV_EvBuf_State_mu8 |=(uint8_t) DEH_StEV_state_mu8;

  if(DEH_StEV_chgSt_updEM_b)            
  {
         /* store the result in the Event-Buffer */
    DEH_EVENT_BUFFER_STATE(EvNum_uT) = DEH_StEV_EvBuf_State_mu8;

    /* if an event state change AND the new state is PASSIVE OR ACTIVE  */
    /* then call the event memory statemachine and update the connected */
    /* informationen                                                    */

  if (! (DEH_EVENT_BUFFER_STATE(EvNum_uT) & DEH_EB_MSK_LOCK_DTC) )
  {
      /* set the flag for event memory stored indication */
    DEH_EVENT_BUFFER_STATE(EvNum_uT) |= DEH_EB_MSK_EV_STO_UPD;
  }

      /* set the flag for event memory update */
    DEH_stUpdateEM_b = (_Bool) TRUE;
  }

/*----------------------------------------------------------------------------*/
/*      Sub Modul DEH_StM_SubM_TstChgSt for Detection Event state change      */
/*----------------------------------------------------------------------------*/
/* START compiler switch if option not activated then no compiling */
#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE

      if(DEH_StEV_chgSt_updEM_b)            
      {
          /* get the EventParameter from Dataset */
        tmp_val1_u8 = DEH_EVENT_PARAMETER(EvNum_uT);

          /* get the Flag for change state detection */
        tmp_val1_u8 =(uint8_t) (tmp_val1_u8 & DEH_EP_MSK_EV_REP_CHG_ST);

          /* test if flag active and call the ext. function */
        if (tmp_val1_u8) 
        {
          APL_DEH_ReportChgEventState(EvNum_uT, DEH_StEV_state_mu8);
        }
  /* end if (tmp_val1_u8) */

      }
  /* end if (detection state change) */
  /* END compiler switch if option not activated then no compiling */
#endif
    
        /* end for start of the Event Statemachine */
  } /* end first else */ 

return (retVal_u8);
}

/*============================================================================
function name    :   DEH_StM_Memory()
input parameter  :   
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function for Event Memory statemachine. This function will be
                     called if a new valid event state detect and test if some
                     further action to store/update or clear an event are 
                     necessary.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
08.11.03    GIGE    built base version 

============================================================================*/

static tDEH_Return DEH_StM_Memory(uint8_t mode_u8, uint16_t EventNumber_u16)
{
  uint8_t          EM_State_u8 = DEH_EM_NO_ENTRY;
  uint8_t          newEventState_u8;
  tDEH_Return      retVal_u8   = DEH_OK;
  tDEH_EventMemory EvMemPos_uT = DEH_EM_NO_VALID_POS;
  tDEH_EventMemory i           = (tDEH_EventMemory) 0;
  uint8_t          Buffer_u8[EMB_BL_SIZE_STOFDTC];

    /* get the information if event stored flag is set or not */
    /* event is stored -> look for position in memory and          */
    /* read the event memory storage state  Bit 5,6 in StatusOfDTC */

  if ((DEH_EVENT_BUFFER_STATE(EventNumber_u16)& DEH_EB_MSK_EV_STORED))
  {
      /* look for the position in the event memory */
    while  ((i < ((tDEH_EventMemory) DEH_MAX_NUMBER_EV_MEM)) 
             && ((EvMemPos_uT == DEH_EM_NO_VALID_POS)) )
    {
        /* read the stored event and check for wanted event */
      if (DEH_EventMemBuffer[i] == EventNumber_u16)
      {
          /* store the found position in the variable */
        EvMemPos_uT = i;

          /* read the status Of DTC value */
        retVal_u8 = APL_DEH_EEPReadEventData( 
                          (uint16_t) ( (EMB_BLOCK_SIZE*i) + EMB_POS_STOFDTC),
                          (uint16_t) (EMB_BL_SIZE_STOFDTC),
                                    &Buffer_u8[0]);
        if (retVal_u8 == DEH_OK)
        {
            /* get the actual event memory state */
          EM_State_u8 = (uint8_t) (Buffer_u8[0] & DEH_EM_ACTIVE_STORED);
        }
        else
        {
            /* set the state to an invalid value */
          EM_State_u8 = (uint8_t) DEH_EM_NO_VALID_STATE;
        }
      }
        /* incrememt the event memory position counter */
      i++;

    } /* end while loop */
  } /* end if STORED */

    /* get the actual event state from buffer */
  newEventState_u8 = (uint8_t) DEH_GET_EVENT_STATE(EventNumber_u16);

    /* statemachine for event memory */
  switch (EM_State_u8)
  {
    case DEH_EM_NO_ENTRY:
    {
        /* T0_1_96  and T0_2_32 */
      if(     (   (newEventState_u8 == DEH_EVENT_ACTIVE)
               || (newEventState_u8 == DEH_EVENT_PASSIVE) )
           && (mode_u8 == DEH_EM_UPDATE))
      {

          /* write the first event memory entry in the external buffer */
        retVal_u8 = DEH_ESiF_WriteEM(EventNumber_u16,newEventState_u8);
    
          /* change the update event memory flag if event successfull stored */
        if (retVal_u8 == DEH_OK)
        {
            /* reset the flag for event memory update indication */
          DEH_EVENT_BUFFER_STATE(EventNumber_u16) &= DEH_EB_MSK_EV_STO_UPD_RST;

            /* and set the event memory stored flag              */
          DEH_EVENT_BUFFER_STATE(EventNumber_u16) |= DEH_EB_MSK_EV_STORED;

            /* clear the stored freeze frames */
          retVal_u8 = DEH_FFiF_ClearFF(EventNumber_u16);
        }
      } /* end  T0_1_96  and T0_2_32 */
    
      break;
    } /* end case DEH_EM_NO_ENTRY */

    case DEH_EM_ACTIVE_STORED:
    {
        /* T96_1_32 */
      if(    (newEventState_u8 == DEH_EVENT_PASSIVE)
          && (mode_u8 == DEH_EM_UPDATE) )
      {

          /* update the old active entry with new paramters ((passive etc.)*/
        retVal_u8 =  DEH_ESiF_UpdateEM( EventNumber_u16, newEventState_u8, EvMemPos_uT);

          /* change the update event memory flag if event successfull stored */
        if (retVal_u8 == DEH_OK)
        {
            /* reset the flag for event memory stored indication */
          DEH_EVENT_BUFFER_STATE(EventNumber_u16) &= DEH_EB_MSK_EV_STO_UPD_RST;

            /* set flag for new PASSIVE event */
          DEH_stPassiveEvents_mb = (_Bool)TRUE;
            /* set correct return value */
          retVal_u8 = DEH_OK;
        }

      } /* end if */

      break;
    } /* end case DEH_EM_ACTIVE_STORED */

    case DEH_EM_PASSIVE_STORED:
    {
          /* T32_2_0 */
      if (mode_u8 == DEH_EM_CHECK_LUC)
      {

          /* test the stored LUC again the global unlearn counter */
        retVal_u8 = DEH_ESiF_TstLUCandClearEM(EvMemPos_uT);
  
          /* if the event is cleared from event memory change the memory flag */
        if (retVal_u8 == DEH_EM_CLR_EV)
        {
            /* reset the flag for event memory stored indication */
            /* and event ist stored indication                   */
          DEH_EVENT_BUFFER_STATE(EventNumber_u16) &= DEH_EB_MSK_EV_STO_RST;
            /* after clear is the event memory NOT full */
          DEH_stFullEvMem_mb = (_Bool) FALSE;
            /* set correct return value */
          retVal_u8 = DEH_OK;
        }
      } /* end if */

        /* T32_1_96 */
      else if(    (newEventState_u8 == DEH_EVENT_ACTIVE)
               && (mode_u8 == DEH_EM_UPDATE) )
      {

          /* Update Event Memory */
        retVal_u8 = DEH_ESiF_UpdateEM(EventNumber_u16,newEventState_u8, EvMemPos_uT);

          /* change the update event memory flag if event successfull stored */
        if (retVal_u8 == DEH_OK)
        {
            /* reset the flag for event memory stored indication */
            /* and event ist stored indication                   */
          DEH_EVENT_BUFFER_STATE(EventNumber_u16) &= DEH_EB_MSK_EV_STO_UPD_RST;
        }
      }

      break;
    } /* end case DEH_EM_PASSIVE_STORED */

    default:
    {
/* --------------------------------------------------------------------------*/
/* Missing Fault definition for illegal state                                */
/* --------------------------------------------------------------------------*/
      break;
    }
  } /* end switch .. case */
  
  return(retVal_u8);
} /* end of function  DEH_StM_Memory() */


/*============================================================================
function name    :  DEH_SUiM_StatusOfDTC ()
input parameter  :  uint16_t      EventNumber_u16
                    uint8_t      mode_u8
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to built the variable StatusOfDTC.
                     This function works in two different modes.
------------------------------------------------------------------------------
date        short     change description
------------------------------------------------------------------------------
09.11.03    GIGE    built base version 

============================================================================*/
#if (defined(DEH_TEST))
uint8_t DEH_SUiF_StatusOfDTC(uint16_t EventNumber_u16, uint8_t mode_u8)

#elif (!defined(DEH_TEST) )

static uint8_t DEH_SUiF_StatusOfDTC(uint16_t EventNumber_u16, uint8_t mode_u8)
#endif
{


  uint8_t StOfDTC_u8 = (uint8_t) 0;
  uint8_t eventbuffer_u8;
#ifdef DEH_SUP_KWP2000
  uint8_t eventstate_u8;

    /* get the information about the current event state */
  eventstate_u8 = (uint8_t) DEH_GET_EVENT_STATE(EventNumber_u16);
#endif

     /* get the actual event buffer state */
  eventbuffer_u8 =(uint8_t) DEH_EVENT_BUFFER_STATE(EventNumber_u16);
  
    /* ================================================================== */
    /* status bit 7 -> MIL (warning lamp state)                           */
    /* ================================================================== */
    /* test if event is stored in Event Memory */
  if ( eventbuffer_u8 & DEH_EB_MSK_EV_STORED )
  {
      /* set bit for event is STORED */
    StOfDTC_u8 |= DEH_STOFDTC_MSK_STORED;
     
      /* test if this event DEFECT AND warning lamp is configured */
    if(   (DEH_IS_EVENT_DEFECT(EventNumber_u16))
       && (DEH_EVENT_LAMP_PAR(EventNumber_u16)))
    {
        /* set bit for status warning lamp is ACTIVE */
      StOfDTC_u8 |= DEH_STOFDTC_MSK_LAMP; 
    }
  }  

    /* ================================================================== */
    /* status bit 4 -> TestNotCompletedSinceLastClear                     */
    /* ================================================================== */
    /* get DTC Readiness information */
    /* and plausible with storage flag (if stored then test was completed */
  if (DEH_GET_TEST_COMPLETE(EventNumber_u16))
  {
    StOfDTC_u8 |= DEH_STOFDTC_MSK_TESTNC_SLC;    
  }

    /* test if event in a debounce state */
  if((mode_u8 == SERVICE_REQ)|| (mode_u8 == WRITE_EM))
  {
#ifdef DEH_SUP_KWP2000
      /* all ACTIVE states */
    if (    (eventstate_u8 == DEH_EVENT_ACTIVE)
         || (eventstate_u8 == DEH_EVENT_DEBOUNCE_PASSIVE)
         || (eventstate_u8 == DEH_EVENT_DEB_ACT_SUSPEND))           
    {
        /* set the bit 6 to ACTIVE- actual undebounced event state */
      StOfDTC_u8 |= DEH_STOFDTC_MSK_ACTIVE;
    }
#endif
    
#ifdef DEH_SUP_UDS

      /* ================================================================== */
      /* status bit 6 -> TestNotCompletedThisOperation cycle                */
      /* ================================================================== */
      /* get DTC Readiness information */
  #ifdef DEH_SUP_UDS_OBD_STATUS
    if ( eventbuffer_u8 & DEH_EB_MSK_DTC_RB )
    {
      StOfDTC_u8 |= DEH_STOFDTC_MSK_RB_TOC;    
    }

      /* ================================================================== */
      /* status bit 5 -> TestFailedSinceLastClear                           */
      /* ================================================================== */
    if (DEH_GET_TEST_FAILED_SLC(EventNumber_u16))
    {
      StOfDTC_u8 |= DEH_STOFDTC_MSK_FAILED_CLEAR;    
    }
  #endif
      /* ================================================================== */
      /* status bit 3 -> Confirmed DTC since last current OR last op.cycle  */
      /* ================================================================== */
        /* test if event is stored */
    if (eventbuffer_u8 & DEH_EB_MSK_EV_STORED)
    {
      StOfDTC_u8 |= DEH_STOFDTC_MSK_STORED;
    }

      /* ================================================================== */
      /* status bit 2 -> Pending DTC (fault in last OR current cycle )      */
      /*                             ( without debouncing)                  */
      /* ================================================================== */
  #ifdef DEH_SUP_UDS_OBD_STATUS
    if (    (DEH_GET_PENDING_DTC_CUR_DC(EventNumber_u16))
         || (DEH_GET_PENDING_DTC_LAST_DC(EventNumber_u16)))
    {
      StOfDTC_u8 |= DEH_STOFDTC_MSK_PENDING; 
    }

      /* ================================================================== */
      /* status bit 1 -> TestfailedThisOperationCycle                       */
      /* ================================================================== */
    if (DEH_GET_TEST_FAILED_TOC(EventNumber_u16))
    {
      StOfDTC_u8 |= DEH_STOFDTC_MSK_FAILED_CYCLE;    
    }
  #endif

      /* ================================================================== */
      /* status bit 0 -> TestFailed (current state)                         */
      /* ================================================================== */
      /* get information from the connected state array                     */
    if (DEH_GET_TEST_LAST_STATE(EventNumber_u16))
    {
      StOfDTC_u8 |= DEH_STOFDTC_MSK_ACTIVE;
    }
#endif
  }

  
#ifdef DEH_SUP_KWP2000
    /* get the connected fault symptom from event dataset and store it */
  StOfDTC_u8 |= (uint8_t) (DEH_EVENT_SYMPRIO(EventNumber_u16) & ((uint8_t) DEH_ESP_MSK_FAULT_SYM));
#endif

#ifdef DEH_SUP_UDS
    /* mask not valid status bits */
  StOfDTC_u8 &=  DEH_DTC_STATUS_AVAILABILITY_MASK;
#endif

  return(StOfDTC_u8);
} /* end function DEH_SUiF_StatusOfDTC */

/*============================================================================
function name    :   DEH_ProcessDiagEventHandler()
input parameter  :   
output parameter :   
globals          :   
return value     :   
description      :   cyclic task to test the started debouncing and to initiate 
                     further actions.
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
07.03.03    GKSC    built base version 
13.10.03    GIGE    First implementation
============================================================================*/
void DEH_ProcessDiagEventHandler(void)
{
    /* number of sucessfull finished unlearn cycles */
  static uint8_t DEH_numOfUnlearnCycles_mu8;

    /* counter for WAIT state after unlearn algorithm was BUSSY */
  static uint8_t DEH_cntWaitUC_u8;

  uint16_t            i;
  uint8_t             tmp_u8     = (uint8_t) 0;
  uint8_t             tmp_prio_u8;
  tDEH_Return         retVal_u8;

    /* start the cyclic analysis only after a correct Event Memory Init */
  if (DEH_stInitEM_b)
  {

      /* ------------------------------------------ */
      /* Freeze Frame read algorithm:               */
      /* test if the Read Freeze Frame Flag is set  */
      /* for all defined eventnumber                */
      /* if the global flag DEH_stReadFF_b is set   */
    if ((DEH_cntReadFF_mu8 >= DEH_READ_FF_PERIOD) && (DEH_stReadFF_b))
    {
        /* start analyze if read freeze frames necessary */  
      for (i= ((uint16_t) 0) ; i < ((uint16_t) DEH_MAX_NUMBER_OF_EVENTPATHS);i++)
      {
          /* look for bit 7 in event buffer state */
        if (DEH_EVENT_BUFFER_STATE(i) & DEH_EB_MSK_READ_FF)
        {
            /* call the function for the event memory update */
          tmp_u8 = DEH_FFiF_ReadFF(i);
      
            /* reset the freeze frame flag */
          DEH_EVENT_BUFFER_STATE(i) &= DEH_EB_MSK_RST_READ_FF;
        }
      } /* end for */
      
        /* reset the status flag */
      DEH_stReadFF_b = (_Bool) FALSE;

        /* reset the counter */
      DEH_cntReadFF_mu8 = (uint8_t) 0;
    } /* debug ende */

      /* ==================================== */
      /* start analyze all events             */
      /* call the cyclic Statemachine         */
      /* debouncing all time debounced events */
      /* ==================================== */

    DEH_StM_Event_cyclic();
      /* ----------------------------------------------------------------- */
      /* block to analyze the event memory and set or reset the connected  */
      /* state variables.                                                  */
      /* UPDATE EV-MEM     : Analyse the update flag for the event memory  */
      /*                     and start the update procedure                */
      /* CAN-BIT : if an event in the event memory (don't matter active or */
      /*           passive )                                               */
      /*                                                                   */
      /* WARNING LAMP BITS : check all events in event memory and set the  */
      /*                     connected warning lamp bits                   */
      /*                                                                   */
      /* UNLEARN Algorithm:  check the conditions to unlearn stored        */
      /*                     PASSIVE EVENTS                                */
      /*------------------------------------------------------------------ */

    if ((DEH_cntEvUpd_mu8 >= DEH_EVENT_MEM_UPD_PERIOD) && (DEH_stUpdateEM_b))
    {

         /* reset the flag for event memory update */
      DEH_stUpdateEM_b = (_Bool) FALSE;

        /* start analyze all events and update the event Memory */  
      for (i= ((uint16_t) 0); i < ((uint16_t)DEH_MAX_NUMBER_OF_EVENTPATHS); i++)
      {
        if (DEH_EVENT_BUFFER_STATE(i) & DEH_EB_MSK_EV_STO_UPD)
        {
            
            tmp_prio_u8 =(uint8_t) DEH_GET_EVENT_PRIORITY(i);


              /* start the Update procedure only with  following conditions:  */
              /* -------------------------------------------------------------*/
              /* I) event memory is NOT full                                  */
              /* OR                                                           */
              /* II) event memory is stored                                   */
              /* OR                                                           */
              /* III) event memory full AND event with smaller prio. is avail.*/
              /* OR                                                           */
              /* IIII)    event memory is full                               */
              /*      AND with equal prio                                     */
              /*      AND PASSIVE event is available                          */

           if (   (!(DEH_stFullEvMem_mb))
                ||(DEH_EVENT_BUFFER_STATE(i) & DEH_EB_MSK_EV_STORED)
                ||(     (DEH_stFullEvMem_mb) 
                     && (tmp_prio_u8 < DEH_EM_smallestPrio_mu8) ) 
                ||(     (DEH_stFullEvMem_mb) 
                     && (tmp_prio_u8 == DEH_EM_smallestPrio_mu8) 
                     && (DEH_stPassiveEvents_mb)  )   )
           {
                /* call the function for the event memory update */
              DEH_StM_Memory(DEH_EM_UPDATE,(i));

                /* test if update was sucessfull OR not */
              if (DEH_EVENT_BUFFER_STATE(i) & DEH_EB_MSK_EV_STO_UPD)
              {
                     /* reset the flag for event memory update */
                  DEH_stUpdateEM_b = (_Bool) TRUE;
              }
           }
           else
           {
                 /* reset the flag for event memory update */
              DEH_stUpdateEM_b = (_Bool) TRUE;
           }
        }
      } /* end for */


        /* reset the counter */
      DEH_cntEvUpd_mu8 = (uint8_t)0;
    }
        /* -------------------------------------- */
        /* temp variable stCAN_b is default FALSE */
        /* -------------------------------------- */

    if (DEH_cntWLP_mu8 >= DEH_CANBIT_WARNLAMP_PERIOD)
    {
        /* call function to set the CAN-Event bit state */
      DEH_stCANEventBit_mb = DEH_ESiF_WarnLampAndSrvBitUpd();
        /* reset the counter */
      DEH_cntWLP_mu8 = (uint8_t)0;
    } /* end if (DEH_cntWLP_mu8 > = DEH_CANBIT_WARNLAMP_PERIOD) */

      /* increase the period counter for Warning Lamp*/
    DEH_cntWLP_mu8 +=(uint8_t) 1;

      /* increase the period counter for Event Memory Update*/
    DEH_cntEvUpd_mu8 +=(uint8_t) 1;

      /* increase the period counter for Read Freez Frames*/
    DEH_cntReadFF_mu8 +=(uint8_t) 1;

      /* ==================================================================== */
      /*      S T A T E M A C H I N E -> U N L E A R N  A L G O R I T H M     */
      /*      -----------------------------------------------------------     */
      /* statemachine to unlearn old Passive Events in the EEPROM             */
      /* the number of unlearn cycles is limited to DEH_MAX_NUM_OF_UC_CYCLES  */
      /* (default is 2)                                                       */
      /* ==================================================================== */

    switch (DEH_StM_UnAlg_mu8)
    {
        /* ================================================================== */
        /*      INIT state for unlearn algorithm => reset unlearn counter     */
        /* ================================================================== */
      case DEH_UC_INIT:
      {
          DEH_numOfUnlearnCycles_mu8 =(uint8_t) 0;
            /* switch to new state */
          DEH_StM_UnAlg_mu8 = DEH_UC_READ;
        break;
      }

        /* ================================================================== */
        /* start state for unlearn algorithnm                                 */
        /*   --> wait for a new valid unlearn counter                         */
        /* ================================================================== */
      case DEH_UC_READ:
      {
          /* T0_1_0                          */
          /* read the actual unlearn counter */
        tmp_u8 = APL_DEH_ReadUnlearnCounter(); 
        
          /* T0_2_1 */
          /* if an current unlearn counter not equal to old unlearn counter */
          /* AND current Unlearn Counter NOT EQUAL to 0xFF                  */
        if (     (tmp_u8 != DEH_oldUnlearnCnt_mu8)
              && (tmp_u8 != DEH_NO_VALID_UNLEARN_COUNTER))
        {
            /* switch to new state */
          DEH_StM_UnAlg_mu8 = DEH_UC_UNLEARN;
        }
          /* T0_3_7 */
          /* test if the limit for the unlearn algorithm reached */
        else if (DEH_numOfUnlearnCycles_mu8 >= DEH_MAX_NUM_OF_UC_CYCLES)
        {
            /* switch to new state */
          DEH_StM_UnAlg_mu8 = DEH_UC_STOP;
        }

            /* store the last valid unlearn counter */
          DEH_oldUnlearnCnt_mu8 = tmp_u8;
        break;
      }
        /* ================================================================== */
        /* UNLEARN  state                                                     */
        /*   --> call function DEH_ESiF_UnlearnAlg() to check all passive     */
        /*       stored events if the unlearn limit reached and the event     */
        /*       can be cleared in the non volatile RAM (typically EEPROM)    */
        /* ================================================================== */
      case DEH_UC_UNLEARN:
      {
          /* T1_1_1 */
          /* start the unlearn algorithm */
        retVal_u8 = DEH_ESiF_UnlearnAlg();
          /* call function to set the CAN-Event bit state */
        DEH_stCANEventBit_mb = DEH_ESiF_WarnLampAndSrvBitUpd();
          /* T1_2_3 */
          /* test if the Unlearn Algorithm BUSSY(NOT DEH_OK) or NOT  */
        if (retVal_u8 != DEH_OK)
        {
            /* reset the wait counter */
          DEH_cntWaitUC_u8 = (uint8_t) 0;

            /* switch to new state */
          DEH_StM_UnAlg_mu8 = DEH_UC_WAIT;
        }
          /* T1_3_0 */
          /* unlearn algorithm was sucessfull -> go back to state DEH_UC_READ */
        else 
        {
            /* switch to new state */
          DEH_StM_UnAlg_mu8 = DEH_UC_READ;

            /* increment the number of unlearn cycles */
          DEH_numOfUnlearnCycles_mu8++;
        }
        break;
      }

        /* ================================================================== */
        /* WAIT  state                                                        */
        /*   --> if the nonvolatile RAM (typicaly EEPROM) BUSSY than wait     */
        /*       a defined time before the next try to start the unlearn      */
        /*       algorithm                                                    */ 
        /* ================================================================== */
      case DEH_UC_WAIT:
      {
          /* increment the wait counter for Unlearn Algorithm */
        DEH_cntWaitUC_u8++;

          /* T3_1_1 */
        if (DEH_cntWaitUC_u8 >= DEH_WAIT_UNLEARN_PERIOD)
        {
            /* switch to new state */
          DEH_StM_UnAlg_mu8 = DEH_UC_UNLEARN;
        }
        break;
      }

      case DEH_UC_STOP:
      {
          /* block unlearn algorithm for the whole klemme 15 cycle         */
          /* the unlearn algorithm is ready and can be started only in the */
          /* next power ON cycle                                           */
        DEH_StM_UnAlg_mu8 = DEH_UC_STOP;

        break;
      }

      default:
      {
          /* reset statevariable to start value */
        DEH_StM_UnAlg_mu8 = DEH_UC_READ;
        break;
      }
    } /* end switch ... case */
  }
} /*  end  */

/*============================================================================
function name    :   DEH_ESiF_UnlearnAlg()
input parameter  :   
output parameter :   
globals          :   
return value     :   
description      :   function to read a stored event and check if the unlearn 
                     algorithm valid for this event (state == PASSIVE and
                     40 cycle long was the state PASSIVE 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
07.03.03    GKSC    built base version 
13.10.03    GIGE    First implementation
============================================================================*/
static tDEH_Return DEH_ESiF_UnlearnAlg(void)
{
  tDEH_EventMemory    k         = (uint8_t) 0;
  tDEH_Return         retVal_u8 =  DEH_OK;
  uint16_t            EvNumber_u16;

    /* anaylse all event memory entries (DEH_MAX_NUMBER_EV_MEM) */
    /* and exit the function if the return value of             */
    /* DEH_StM_Memory() function not equal DEH_OK (=>BUSSY)     */ 

  while (     (k < (tDEH_EventMemory) DEH_MAX_NUMBER_EV_MEM) 
           && (retVal_u8 == DEH_OK)
        )
  {
      /* read the stored event number from internal buffer */
    EvNumber_u16 = DEH_EventMemBuffer[k];

      /* test for valid entry EventNumber smaller than the max. Event Number */

    if(EvNumber_u16 < DEH_MAX_NUMBER_OF_EVENTPATHS) 
    {
        /* check the event memory for events to unlearn an event */
      retVal_u8 = DEH_StM_Memory(DEH_EM_CHECK_LUC, EvNumber_u16 );
    }
      /* increment the counter for next element */
    k++;
  } /* end for */
  return (retVal_u8);
}

/*============================================================================
function name    :   DEH_GetVersionInfo()
input parameter  :   tDEH_StdVersionInfo *pVersionInfo - pointer to destination buffer 
output parameter :   
globals          :   
return value     :   
description      :   Function to read a standard version information of
                     Diagnostic Event Handler. 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
18.10.06    GAFR    built base version 
============================================================================*/
void DEH_GetVersionInfo(tDEH_StdVersionInfo *pVersionInfo)
{
    /* copy DEH standard version information into received struct buffer */
  pVersionInfo->vendorID          = DEH_VENDOR_ID;
  pVersionInfo->moduleID          = DEH_MODULE_ID;
  pVersionInfo->sw_major_version  = DEH_SW_MAJOR_VERSION;
  pVersionInfo->sw_minor_version  = DEH_SW_MINOR_VERSION;
  pVersionInfo->sw_patch_version  = DEH_SW_PATCH_VERSION;  
}

#ifdef DEH_SUP_UDS
/*============================================================================
function name    :   DEH_ReadDTCByStatus()
input parameter  :   tDEH_DTCInfoType DTCInfoType - every DTC or only OBD DTC
                     tDEH_DTCStatusMask DTCStatusMask - mask for status of DTC
                     tDEH_DTCAndStatus *pDTCAndStatus - pointer to destination buffer 
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read all supported DTC with specific status. 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
18.10.06    GAFR    built base version 
============================================================================*/
tDEH_Return DEH_ReadDTCByStatus(tDEH_DTCInfoType DTCInfoType, tDEH_DTCStatusMask DTCStatusMask, tDEH_DTCAndStatus *pDTCAndStatus)
{
    /* variable to store the last status mask for the service ReadDTCbyStatusMask */
  static uint8_t DEH_oldDTCStatusMask_u8;

  tDEH_Return   retVal_uT     = DEH_E_NO_DATA;
  uint8_t       tmp_val_u8;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
    if (    (DTCInfoType == DEH_READ_ALL_DTC) 
         || (DTCInfoType == DEH_READ_EMISSION_REL_DTC))
    {
      if(    (DEH_SRV_lastRecFct_mu8 != DEH_SRV_UDS_READ_DTC_BY_STATUS)
          || (DEH_oldDTCStatusMask_u8 != DTCStatusMask)
          || (DEH_cntEvent_mu16 >= DEH_MAX_NUMBER_OF_EVENTPATHS))
      {
          /* reset the internal counter to read the event data */
        DEH_cntEvent_mu16 = (uint16_t) 0;
          /* store the last valid status mask */
        DEH_oldDTCStatusMask_u8 = DTCStatusMask;
      }

        /* search as long as ..............   */
        /* I) not all events are analysed AND */
        /* II) NO DATAS are found             */
      while (    (DEH_cntEvent_mu16 < DEH_MAX_NUMBER_OF_EVENTPATHS)
              && (retVal_uT == DEH_E_NO_DATA)) 
      {

          /* check if ................. */
          /* I) all DTC are relevant OR */
          /* II) emission related DTCs  */
        
        if (   (DTCInfoType == DEH_READ_ALL_DTC)
             ||(DEH_EVENT_PARAMETER(DEH_cntEvent_mu16) & DEH_EP_MSK_EV_IS_EMISSION_REL))
        {
            /* read the actual status of the event */
          tmp_val_u8 = DEH_SUiF_StatusOfDTC(DEH_cntEvent_mu16, SERVICE_REQ);

            /* check the status whith the given status mask                  */
            /* logical bit operation NOT logical AND                         */
            /* AND                                                           */
            /* test the current diagnose session                             */
            /* 1) - current diag. session is NOT equal to Developper session */
            /*    AND - prio is smaller than 9                               */
            /* OR                                                            */
            /* 2) diagnose session IS EQUAL TO Developper session            */
            
          if(    ( tmp_val_u8 & DTCStatusMask )
              && (   (    (DEH_actDiaSes_muT != DEH_ENTW_DIAG_SESSION) 
                       && (DEH_GET_EVENT_PRIORITY(DEH_cntEvent_mu16) < DEH_DEVELOP_EVENT_PRIO))
                    || (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION))
             )
          {
              /* store the StOfDTC to the given address */
            pDTCAndStatus->StatusOfDTC = tmp_val_u8;
              /* write the DTC to the position */
            pDTCAndStatus->DTC = (tDEH_DTC) DEH_EVENT_DTC(DEH_cntEvent_mu16);
          
              /* test if the current counter equal to the last valid event */
              /* if YES then all datas are sucessfull read out             */
            if (DEH_cntEvent_mu16 == (DEH_MAX_NUMBER_OF_EVENTPATHS-1))
            {
              retVal_uT = DEH_OK;
            }
              /* if NOT then maybe exist further events with DTC statues mask */
            else
            {
              retVal_uT = DEH_E_DATA_STILL_AVAILABLE;
            }
          } /* end if ( tmp_val_u8 & DTCStatusMask )*/
        } /* end if DTC ALL OR emision rel.*/
          /* increase internal counter */
        DEH_cntEvent_mu16++;
      } /* end while */
    } /* end if DTC Info type test */ 
    else                                                                                                                               
    {
        /* return value is unknown DTCInfoType */
      retVal_uT = DEH_E_MODE;
    } /* end else wrong DTCInfoType */
  }
  else  /* no correct init*/
  {
    retVal_uT =(tDEH_Return) DEH_E_INIT;  
  }
    /* store the last received service function */
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_UDS_READ_DTC_BY_STATUS;

  return(retVal_uT);
}
#endif
/*============================================================================
function name    :   DEH_ReadDTCSnapshotRecord()
input parameter  :   tDEH_DTC DTC - DTC to read the snapshot records
                     uint8_t *pStatusOfDTC - pointer to a value for a status
                     uint16_t BuffSize - size of destination buffer
                     uint16_t *pNumOfBytes - pointer to a value for a number of copied bytes
                     uint8_t *pDestBuffer - pointer to destination buffer
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read stored snapshot records for a DTC. 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
18.10.06    GAFR    built base version 
============================================================================*/
#ifdef DEH_SUP_SNAP_SHOT_DATA
tDEH_Return DEH_ReadDTCSnapshotRecord(tDEH_DTC DTC, uint8_t *pStatusOfDTC, uint16_t BuffSize, uint16_t *pNumOfBytes, uint8_t *pDestBuffer)
{
  tDEH_Return       retVal_uT   = DEH_E_UNKNOWN_DTC;
  tDEH_EventNumber  EvNum_u16   = (uint16_t) 0;
  tDEH_EventNumber  i;
  uint16_t          tmpNumOfBytes_u16;
  uint8_t           Buffer_u8[DEH_MAX_SNAPSHOT_DATA_LEN];

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
      /* check the curent diagnostic session */
    if ( DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION )
    {
        /* look in the event parameters for the DTC */
      for ( i = (tDEH_EventNumber) 0;
            i < (tDEH_EventNumber) DEH_MAX_NUMBER_OF_EVENTPATHS; 
            i++ )
      {
        if ( ( (tDEH_DTC) DEH_EVENT_DTC(i) ) == DTC )
        {
             /* set flag for a known DTC              */
             /* and the curent return code to NO DATA */
           retVal_uT = DEH_E_NO_DATA;
             /* store the connected Event Number */
           EvNum_u16 = i;
            /* store the StOfDTC to the given address */
          *pStatusOfDTC = DEH_SUiF_StatusOfDTC(EvNum_u16, SERVICE_REQ);
        }
      }     

        /* if DTC supports snapshot records, read them from system */          
        /* if DTC is available                                     */
      if(   ( DEH_EVENT_PARAMETER(EvNum_u16) & DEH_EP_MSK_EV_SUP_SNAPSHOTS )
         && (retVal_uT == DEH_E_NO_DATA))
      {
        retVal_uT = APL_DEH_ReadDTCSnapshotRecord(DTC, BuffSize, &tmpNumOfBytes_u16, &Buffer_u8[0]);
          /* copy the received values to function caller */
        *pNumOfBytes= tmpNumOfBytes_u16;

          /* copy data if return code OK */
        if(retVal_uT == DEH_OK)
        {
          for(i=0;i<tmpNumOfBytes_u16;i++)
          {
              /* ckeck the number of received bytes */
            if (i<BuffSize)
            {
              pDestBuffer[i] = Buffer_u8[i];
            }
          }
        }
      }
    } /* diagsession == 0x4F */
    else
    {
      retVal_uT = DEH_E_WRONG_DIAG_SESSION;
    }
  }
  else  /* no correct init*/
  {
    retVal_uT =(tDEH_Return) DEH_E_INIT;  
  }
    /* store the last received service function */
  DEH_SRV_lastRecFct_mu8 = DEH_SRV_UDS_READ_SNAPSHOT_REC;
  
  return (retVal_uT);
}
#endif
/*============================================================================
function name    :   DEH_ReadDTCExtDataRecord()
input parameter  :   tDEH_DTC DTC - DTC to read the extended data record
                     uint8_t *pStatusOfDTC - pointer to a value for a status
                     tDEH_RecordNumber RecordNumber - number of the extended data record
                     uint16_t BuffSize - size of destination buffer
                     uint16_t *pNumOfBytes - pointer to a value for a number of copied bytes
                     uint8_t *pDestBuffer - pointer to destination buffer
output parameter :   
globals          :   
return value     :   tDEH_Return
description      :   Function to read all supported DTC with specific status. 
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
19.10.06    GAFR    built base version 
============================================================================*/
#if defined(DEH_SUP_UDS)
tDEH_Return DEH_ReadDTCExtDataRecord(tDEH_DTC           DTC,
                                     uint8_t            *pStatusOfDTC, 
                                     tDEH_RecordNumber  RecordNumber, 
                                     uint16_t           BuffSize, 
                                     uint16_t           *pNumOfBytes, 
                                     uint8_t            *pDestBuffer)
{

    /* tmp. Variable for Return value -> init with DEH_OK */
  tDEH_Return       retVal_u8 = DEH_E_UNKNOWN_DTC;
  uint16_t          EvNum_u16 = (uint16_t) 0;
  tDEH_DTC          DTC_uT;
  uint8_t           Buffer_u8[EMB_BLOCK_SIZE];
  tDEH_EventNumber  i;
  uint8_t           k;
  uint8_t           EventMemoryPos_u8 = DEH_EM_NO_VALID_POS;
  uint16_t          min_buf_size_u16  = (uint16_t) 0;
  uint16_t          buf_pos_u16;
  uint8_t           posExtRecData_u8;
  uint8_t           NumberOfExtRecordData_u8;
  uint8_t           posExtRecDataAnswer_u8;
  _Bool             found_b =(_Bool) FALSE;
  uint16_t          tmpNumOfBytes_u16 = (uint16_t)0;
  *pStatusOfDTC = (uint8_t)  0;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
      /* check if the received DTC valid */

    /* DEH_STD_DTC_INFO                   0x01 */
    /* DEH_STD_FREEZE_FRAME_INFO          0x02 */
    /* DEH_ALL_STDFF_AND_DTC_INFO         0x8F */
    /* DEH_ALL_ADD_STDFF_AND_ADDFF        0xFF */

      /* test the service and set the connected buffersize */
      /* set the transmitted bytes                         */    

    switch (RecordNumber)
    {
      case DEH_STD_DTC_INFO_01:
        min_buf_size_u16  = DEH_STD_DTC_INFO_MIN_BUFFER;
        *pNumOfBytes = DEH_STD_DTC_INFO_MIN_BUFFER;
        break;  
      
      case DEH_STD_FREEZE_FRAME_INFO_02:
        min_buf_size_u16  = DEH_STD_FF_INFO_MIN_BUFFER;
        *pNumOfBytes = DEH_STD_FF_INFO_MIN_BUFFER;
        break;  

      case DEH_ALL_STDFF_AND_DTC_INFO_8F:
        min_buf_size_u16  = DEH_ALL_STDFF_DTC_INF_MIN_BUFFER;
        *pNumOfBytes  = DEH_ALL_STDFF_DTC_INF_MIN_BUFFER;
        break;  

      case DEH_ALL_ADD_STDFF_AND_ADDFF_FF:
        min_buf_size_u16  = DEH_ALL_STDFF_DTC_INF_MIN_BUFFER;
        break;  

      default: 
        retVal_u8 = DEH_E_PARAMETER;
        *pNumOfBytes =(uint16_t) 0;
      break;
    }
  
      /* test if the received buffer big enough  */
      /* if the Ext Data Record NOT 0x70-0x7F    */
    if((    (RecordNumber <= DEH_EXT_REC_NUM_MIN_6F)
          ||(RecordNumber>=DEH_EXT_REC_NUM_MAX_80))
       && (BuffSize < min_buf_size_u16))
    {
       retVal_u8 = DEH_E_NO_BUFFER;
    }
       
      /* start the service if :           */
      /* 1) valid DTC received            */
      /* 2) received buffer is big enough */
    if (  (DTC <= DEH_LAST_VALID_DTC )
        &&(retVal_u8 != DEH_E_NO_BUFFER))
    {
        /* look in the event parameters for the DTC and set correct return code */
      for (i = (tDEH_EventNumber) 0; 
           i < (tDEH_EventNumber) DEH_MAX_NUMBER_OF_EVENTPATHS; 
           i++)
      {
          /* test if the received DTC equal to one internal event DTC */
        if ( ( (tDEH_DTC) DEH_EVENT_DTC(i) )== DTC)
        {
            /* set flag for a known DTC                    */
          retVal_u8 = DEH_OK;
            /* get the Status of DTC information */
          *pStatusOfDTC = DEH_SUiF_StatusOfDTC(i, SERVICE_REQ);;        
            /* set the number of copied bytes to 0 */
          tmpNumOfBytes_u16 = (uint16_t) 0;

            /* test the current diagnose session                             */
            /* Read the Extended Freeze Frame Data if                        */
            /* 1) - current diag. session is NOT equal to Developper session */
            /*    AND - prio is smaller than 9                               */
            /* OR                                                            */
            /* 2) diagnose session IS EQUAL TO Developper session            */
          if(   (   (DEH_actDiaSes_muT != DEH_ENTW_DIAG_SESSION) 
                 && (DEH_GET_EVENT_PRIORITY(i)< DEH_DEVELOP_EVENT_PRIO))
            || (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION))
          {
              /* look for stored events and there connected DTCs */
            for( k=0; k < DEH_MAX_NUMBER_EV_MEM; k++)
            {
                /* test if the event memory FREE or one event is stored */
              if(DEH_EventMemBuffer[k] != DEH_EM_FREE_POS)
              {
                  /* read the connected DTC */
                DTC_uT = (tDEH_DTC) DEH_EVENT_DTC(DEH_EventMemBuffer[k]);

                  /* test if the received DTC equal to the read DTC */
                if (DTC_uT == DTC)
                {
                    /* read the stored event number */
                  EvNum_u16 = DEH_EventMemBuffer[k];
                    /* store the position in the variable */
                  EventMemoryPos_u8 = k;
                } /* end if DTC            */
              } /* end if DEH_EM_FREE_POS */
            } /* end for                 */
          } /* end if diag session      */
        } /* end if                    */
      } /* end for                    */

        /* test if a valid event memory entries is found */
      if (EventMemoryPos_u8 != DEH_EM_NO_VALID_POS)
      {
          /* read out the whole stored event memory */
        retVal_u8 = APL_DEH_EEPReadEventData
                              ( (uint16_t) (EMB_BLOCK_SIZE * EventMemoryPos_u8) ,
                                (uint16_t) (EMB_BLOCK_SIZE),
                                          &Buffer_u8[0]                       ); 
          /* test result and start data read */
        if (retVal_u8 == DEH_OK)
        {
            /* ============================================================= */
            /*    StandardDTCInformation (0x01)                              */
            /*    included if service :                                      */
            /*    - DEH_ALL_STDFF_AND_DTC_INFO_8F  (0x8F)                       */
            /*    - DEH_ALL_ADD_STDFF_AND_ADDFF_FF (0xFF)                       */
            /* ============================================================= */
          if (   (RecordNumber == DEH_STD_DTC_INFO_01)
              || (RecordNumber == DEH_ALL_STDFF_AND_DTC_INFO_8F)
              || (RecordNumber == DEH_ALL_ADD_STDFF_AND_ADDFF_FF))
          {
                /* service information */
              pDestBuffer[0] = (uint8_t)DEH_STD_DTC_INFO_01;
                /* priority 0x */
              pDestBuffer[1] = (uint8_t) DEH_GET_EVENT_PRIORITY(EvNum_u16);
                /* occcurance counter */
              pDestBuffer[2] = Buffer_u8[EMB_POS_OCC];
                /* set the number of copied bytes to 3 */
              tmpNumOfBytes_u16 = (uint16_t) 3;
          }

            /* ============================================================= */
            /*    StandardFreezeFrame (0x02)                                 */
            /*    included if service :                                      */
            /*    - DEH_ALL_STDFF_AND_DTC_INFO_8F  (0x8F)                       */
            /*    - DEH_ALL_ADD_STDFF_AND_ADDFF_FF (0xFF)                       */
            /*    => only the return buffer position will be changed         */
            /* ============================================================= */
          if (    (RecordNumber == DEH_STD_FREEZE_FRAME_INFO_02)
               || (RecordNumber == DEH_ALL_STDFF_AND_DTC_INFO_8F)
               || (RecordNumber == DEH_ALL_ADD_STDFF_AND_ADDFF_FF))
          {
              /* set the buffer position for the datas */
            if (RecordNumber == DEH_STD_FREEZE_FRAME_INFO_02)
            {
              buf_pos_u16 = (uint16_t)0; 
            }
            else
            {
              buf_pos_u16 = (uint16_t)3;               
            }
          
              /* service information */
            pDestBuffer[buf_pos_u16] = (uint8_t)DEH_STD_FREEZE_FRAME_INFO_02;
              /* aging counter (or limit unlearn counter */
            pDestBuffer[buf_pos_u16+1] = Buffer_u8[EMB_POS_LUC];
              /* time indication bit */
            pDestBuffer[buf_pos_u16 + 5] = (uint8_t) 0x00;
              /* increase the number of copied bytes with 3 + 8 (StdFF)*/
            tmpNumOfBytes_u16 =(uint16_t) (tmpNumOfBytes_u16 +  11);

            
  #ifdef DEH_READ_STD_FF

            for (k =((uint8_t) 0) ; k < DEH_SIZEOF_STDFF_KM; k++)
            {
              pDestBuffer[buf_pos_u16 + 2 + k] = Buffer_u8[EMB_POS_STDFF+k];

            } /* end for (k = ((uint8_t) 0) ; k < DEH_SIZEOF_STDFF_KM; k++) */

            for (k = ((uint8_t) 0) ; k < DEH_SIZEOF_STDFF_TIME; k++)
            {
              pDestBuffer[buf_pos_u16 + 6 + k] = Buffer_u8[  EMB_POS_STDFF
                                                      + DEH_SIZEOF_STDFF_KM
                                                      + k];
            } /* end for (k = 0 ; k < DEH_SIZEOF_STDFF_TIME; k++) */
  #endif 

            /* -------------------------------------------------------------- */
            /* if no standard freeze frame stored in the event memory buffer  */
            /* -------------------------------------------------------------- */

  #ifndef DEH_READ_STD_FF
            for (k =((uint8_t) 0); k < DEH_SIZEOF_STDFF_KM; k++)
            {
              pDestBuffer[buf_pos_u16 + 1 + k] = (uint8_t) DEH_STDFF_KM_NOT_AVAILABLE;
            } /* end for */

            for (k =((uint8_t) 0); k < DEH_SIZEOF_STDFF_TIME; k++)
            {
              pDestBuffer[buf_pos_u16 + 6 + k] = (uint8_t) DEH_STDFF_TIME_NOT_AVAILABLE;
            } /* end for */
  #endif
          } /* end if service 0x02, 0x8F , 0xFF */

            /* ====================================== */
            /* Extended Record Datas ... 0x70-0x7F ...*/
            /* ====================================== */          
            /* test the correct range of record number */
          if (    (RecordNumber > DEH_EXT_REC_NUM_MIN_6F) 
               && (RecordNumber < DEH_EXT_REC_NUM_MAX_80))
          {
              /* set the index to the ADDITIONAL FF position */
            posExtRecData_u8 = EMB_POS_ADDFF;
            /* lock for the correct record number in the Event Memory */
            while((posExtRecData_u8<EMB_BLOCK_SIZE) && (!found_b))
            {
              if(Buffer_u8[posExtRecData_u8] == RecordNumber)
              {
                  /* set flag for successfull search */
                found_b =(_Bool) TRUE;
                  /* read out number of extended record datas*/
                NumberOfExtRecordData_u8 = Buffer_u8[posExtRecData_u8+1];
                  /* increase the number of copied bytes with 1 */
                tmpNumOfBytes_u16 =(uint16_t) (tmpNumOfBytes_u16 + 1);
                  /* test if the numberf of copied Extended Rec Datas smaller */
                  /* than the received buffer                                 */
                if (NumberOfExtRecordData_u8 < BuffSize)
                {
                    /* [0] = Record Numvber 0x70-0x7F */
                  pDestBuffer[0] = RecordNumber;
                    /* read out extended record data */
                  for (k = 2;k <(NumberOfExtRecordData_u8+2); k++ )
                  {
                      /* copy the Extended Record data from Event memory to */
                      /* destination buffer                                  */
                    pDestBuffer[k-1] = Buffer_u8[posExtRecData_u8+k];
                      /* increase the number of copied bytes with 1 */
                    tmpNumOfBytes_u16 =(uint16_t) (tmpNumOfBytes_u16 + 1);
                  }
                }
                else
                {
                    /* return code -> buffer to small */
                  retVal_u8 = DEH_E_NO_BUFFER;
                }
              } /* if(Buffer_u8[posExtRecData_u8] == RecordNumber) */
                /* increase the index in the event memory to next Ext.Rec Data */
              posExtRecData_u8 =(uint8_t) (posExtRecData_u8 + DEH_MAX_EXT_REC_BUFFER_DATA);
            } /* end while */
          
            if (!found_b)
            {
                /* return code -> Extended Record Number is NOT in Event Memory */
              retVal_u8 = DEH_E_NO_DATA;
            }
          } /* end if 0x70 - 0x7F */

            /* ====================================== */
            /* Extended Record Datas ... 0xFF ....    */
            /* Read Extended Record Data if available */
            /* ====================================== */          
            /* test the correct range of record number */
          if (RecordNumber == DEH_ALL_ADD_STDFF_AND_ADDFF_FF) 
          {
              /* set the index to the ADDITIONAL FF position in event memory */
            posExtRecData_u8 = EMB_POS_ADDFF;
  
              /*set index for answer data in service buffer */
            posExtRecDataAnswer_u8 = DEH_FIRST_POS_EXT_REC_DATA_0xFF;
            
              /* lock for the correct record number in the Event Memory */
            while(posExtRecData_u8<EMB_BLOCK_SIZE)
            {
                /* test if a valid record number is available */
              if (    (Buffer_u8[posExtRecData_u8] > DEH_EXT_REC_NUM_MIN_6F) 
                   && (Buffer_u8[posExtRecData_u8] < DEH_EXT_REC_NUM_MAX_80))

              {
                  /* read out number of extended record datas*/
                NumberOfExtRecordData_u8 = Buffer_u8[posExtRecData_u8+1];
                  /* increase the needed buffer wuth the number of stored datas */
                  /* extended Rec Number + number of datas                      */
                min_buf_size_u16 =(uint16_t) (min_buf_size_u16 +  NumberOfExtRecordData_u8+1);

                  /* test if the number of copied Extended Rec Datas smaller */
                  /* than the received buffer                                */
                if (min_buf_size_u16 <= BuffSize)
                {
                    /* [0] = Record Numvber 0x70-0x7F */
                  pDestBuffer[posExtRecDataAnswer_u8] = Buffer_u8[posExtRecData_u8];
                    /* increase the number of copied bytes with 1 */
                  tmpNumOfBytes_u16 = (uint16_t) (tmpNumOfBytes_u16 + 1);
                    /* read out extended record data */
                  for (k = 2;k <(NumberOfExtRecordData_u8+2); k++ )
                  {
                      /* copy the Extended Record data from Event memory to */
                      /* destination buffer                                  */
                    pDestBuffer[posExtRecDataAnswer_u8+k-1] = Buffer_u8[posExtRecData_u8+k];
                      /* increase the number of copied bytes with 1 */
                    tmpNumOfBytes_u16 =(uint16_t) (tmpNumOfBytes_u16 + 1);
                  }
                    /* calculate next position for answer */
                  posExtRecDataAnswer_u8 =(uint8_t)(posExtRecDataAnswer_u8+NumberOfExtRecordData_u8+1);
                }
                else
                {
                    /* return code -> buffer to small */
                  retVal_u8 = DEH_E_NO_BUFFER;
                }
              } /* if(Buffer_u8[posExtRecData_u8] == RecordNumber) */
                /* increase the index in the event memory to next Ext.Rec Data */
              posExtRecData_u8 =(uint8_t) (posExtRecData_u8 + DEH_MAX_EXT_REC_BUFFER_DATA);
            } /* end while */
          }
        } /* endif DEH read EEPROM OK */
        else
        {
             /* set failure flag for EEP Read problems */
          DEH_EventMemoryErrors_mu8 |= DEH_ERR_MSK_EEP_READ; 
        }
      } /* valid EventMemoryPos_u8 found in Event Momory */
    } /* end if DTC check */
    else
    {
      if (DTC > DEH_LAST_VALID_DTC )
      {
          /* wrong DTC received */
        retVal_u8 = DEH_E_PARAMETER;
      }
    }

      /* copy the number of copied bytes back to called function */
    *pNumOfBytes  = tmpNumOfBytes_u16;
  }
  else  /* no correct init*/
  {
    retVal_u8 =(tDEH_Return) DEH_E_INIT;  
  }
  return (retVal_u8);
}
#endif


/*============================================================================
function name    :   DEH_SUiF_InitStatusOfDTC()
reference        :   
input parameter  :   no input parameter
output parameter :   
globals          :   
return value     :   tDEH_Return
                      - DEH_FIRST_INIT
                      - DEH_INIT
                      - DEH_E_NO_DATA
description      :   Function to init the DEH with stored StatusOfDTC 
                     information
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
14.11.06    GIGE    built base version 
============================================================================*/
static tDEH_Return DEH_SUiF_InitStatusOfDTC(void)
{
  uint8_t Buffer_u8[STOFDTC_STORAGE_BLOCK_SIZE];
  uint16_t index_u16;
  tDEH_Return ret_val;

    /* call function to read the stored StatusOfDtC information */
  ret_val = APL_DEH_ReadStatusOfDTC(STOFDTC_STORAGE_BLOCK_SIZE, &Buffer_u8[0]);

    /* start the init procedure only if read out date was successfull */
  if (ret_val == DEH_OK)
  {
      /* load the Status Byte for the INIT procedure */
    DEH_IntitStatusOfDTC_u8 = Buffer_u8[STOFDTC_POS_STATUS_BYTE];

      /* test if the status of DTC storage block initialized */
    if(DEH_IntitStatusOfDTC_u8 == DEH_STOFDTC_INIT_MEM_BLOCK)
    {    
        /* init the datas for Test Completed Since Last Clear */
      for(index_u16=0; index_u16 < STOFDTC_ONE_BLOCK_SIZE; index_u16++)
      {
          /* load the datas from the buffer */
        TestNC[index_u16] = Buffer_u8[index_u16+STOFDTC_POS_TEST_NC];
#ifdef DEH_SUP_UDS
          /* load the Test Failed Last State from the buffer */
        TestFLS[index_u16] = Buffer_u8[index_u16+STOFDTC_POS_TEST_FLS];
  #ifdef DEH_SUP_UDS_OBD_STATUS
          /* set datas to NO Pending DTCs */
        PendingDTC_curDC[index_u16] = DEH_INIT_STOFDTC_NO_PEND_DTC;
          /* load the datas Pending DTC from the buffer */
        PendingDTC_lastDC[index_u16] = Buffer_u8[index_u16 + STOFDTC_POS_PENDING_DTC];
          /* load the datas Test Failed Since Last Clear from the buffer */
        TestFailedSLC[index_u16] = Buffer_u8[index_u16 + STOFDTC_POS_TEST_FAILED_SLC];
          /* clear test failed this operation cycle status bytes*/
        TestFailedTOC[index_u16]= DEH_INIT_STOFDTC_TEST_FAILED;
  #endif
#endif 
      } /* end for                */
    } /* end if DEH_INIT_STOFDTC */
      /* start the first initialization of all status bytes*/
    else
    {
        /* init the datas for Test Completed Since Last Clear */
      for(index_u16=0; index_u16 < STOFDTC_ONE_BLOCK_SIZE; index_u16++)
      {
          /* set datas to Test Not Completed ("1") */
        TestNC[index_u16] = DEH_INIT_STOFDTC_TEST_NOT_COMPL;
#ifdef DEH_SUP_UDS
          /* set  Test Failed Last State to "0" -> no fault */
        TestFLS[index_u16] = DEH_INIT_STOFDTC_TEST_FAILED;
  #ifdef DEH_SUP_UDS_OBD_STATUS
            /* set datas to NO Pending DTCs */
          PendingDTC_curDC[index_u16] = DEH_INIT_STOFDTC_NO_PEND_DTC;
            /* set datas to NO Pending DTCs */
          PendingDTC_lastDC[index_u16] = DEH_INIT_STOFDTC_NO_PEND_DTC;
            /* set datas to NOT Test Failed Since Last Clear */
          TestFailedSLC[index_u16] = DEH_INIT_STOFDTC_TEST_FAILED;
            /* set datas to NO test failed this operation cycle */
          TestFailedTOC[index_u16]= DEH_INIT_STOFDTC_TEST_FAILED;
  #endif
#endif
          /* set the status to stored */
        DEH_IntitStatusOfDTC_u8 = DEH_STOFDTC_INIT_MEM_BLOCK;
          /* call function to write the INIT values to RAM buffer */
        DEH_StoreStatusOfDTCInfos();
      } /* end for                */
    } /* end else */
  } /* end if DEH_OK            */

  return(ret_val);    
}

/*============================================================================
function name    :   DEH_Store_StatusOfDTCInfos()
reference        :   
input parameter  :   no input parameter
output parameter :   
globals          :   
return value     :   tDEH_Return
                      - DEH_FIRST_INIT
                      - DEH_INIT
                      - DEH_E_NO_DATA
description      :   Function to init the DEH with stored StatusOfDTC 
                     information
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
15.11.06    GIGE    built base version 
============================================================================*/
tDEH_Return DEH_StoreStatusOfDTCInfos(void)
{
  uint8_t     Buffer_u8[STOFDTC_STORAGE_BLOCK_SIZE];
  uint16_t     index_u16;
  tDEH_Return ret_val;

    /* check if DEH correct initialized ??? */
  if(DEH_stInitEM_b)
  {
      /* init the datas for Test Completed Since Last Clear */
    for(index_u16=0; index_u16 < STOFDTC_ONE_BLOCK_SIZE; index_u16++)
    {
        /* load the Test NOT Completed to the buffer */
      Buffer_u8[index_u16+STOFDTC_POS_TEST_NC] = TestNC[index_u16];
#ifdef DEH_SUP_UDS
        /* load the Test Failed Last State to the buffer */
      Buffer_u8[index_u16+STOFDTC_POS_TEST_FLS] = TestFLS[index_u16];
  #ifdef DEH_SUP_UDS_OBD_STATUS
        /* load the datas Pending DTC to the buffer */
      Buffer_u8[index_u16 + STOFDTC_POS_PENDING_DTC] = PendingDTC_curDC[index_u16];
        /* load the datas Test Failed Since Last Clear to the buffer */
      Buffer_u8[index_u16 + STOFDTC_POS_TEST_FAILED_SLC] = TestFailedSLC[index_u16];
  #endif
#endif
    } /* end for                */
      /* load the Status Byte for the INIT procedure */
    Buffer_u8[STOFDTC_POS_STATUS_BYTE]=DEH_IntitStatusOfDTC_u8;

      /* call function to read the stored StatusOfDtC information */
    ret_val = APL_DEH_WriteStatusOfDTC(STOFDTC_STORAGE_BLOCK_SIZE, &Buffer_u8[0]);

  }
  else  /* no correct init*/
  {
    ret_val =(tDEH_Return) DEH_E_INIT;  
  }
  return(ret_val);
}

/*============================================================================
function name    :   DEH_SUiF_ClearStatusOfDTCInfos ()
reference        :   
input parameter  :   no input parameter
output parameter :   
globals          :   
return value     :   
description      :   Function to clear the StatusOfDTC information after 
                     function call DEH_ClearDiagnosticInformation()
------------------------------------------------------------------------------
date        short    change description
------------------------------------------------------------------------------
15.11.06    GIGE    built base version 
============================================================================*/
void DEH_SUiF_ClearStatusOfDTCInfos (void)
{
  uint8_t  index_u8;
  uint16_t k;
  uint8_t  priority_u8;
  _Bool    NOT_erasable_b;

    /* fast clear of all status bytes */
  if (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION ) 
  {
    for(index_u8=0; index_u8<STOFDTC_ONE_BLOCK_SIZE ; index_u8++)
    {
        /* SET test NOT complete flag to NOT TESTED "1" */
      TestNC[index_u8]=DEH_INIT_STOFDTC_TEST_NOT_COMPL;

      #ifdef  DEH_SUP_UDS
          /* SET Test Failed Last State = "0" -> NO fault */
        TestFLS[index_u8]=DEH_INIT_STOFDTC_TEST_FAILED;
      
        #ifdef DEH_SUP_UDS_OBD_STATUS

            /* clear test failed since last clear "1" */
          TestFailedSLC[index_u8]=DEH_INIT_STOFDTC_TEST_FAILED;
            /* clear test failed this operation cycle status bytes "1" */
          TestFailedTOC[index_u8]=DEH_INIT_STOFDTC_TEST_FAILED;
            /* clear pending status bits "0" */
          PendingDTC_curDC[index_u8]=DEH_INIT_STOFDTC_NO_PEND_DTC;
          PendingDTC_lastDC[index_u8]=DEH_INIT_STOFDTC_NO_PEND_DTC; 
        #endif
      #endif
    }
  }
    /* slow clear with test of every event to prio 9 */
  else
  {

      /* analyse every event for special cases                                         */
      /* 1) ERASABLE Event AND NOT Prio 9 OR                                           */
      /* 2) NOT Erasable Event AND (diag session is ürogrammer OR deve. diag. session) */
      /* 3) Prio 9  AND  developper diag. session                                      */

    for (k = (uint16_t) 0 ; k < ((uint16_t) DEH_MAX_NUMBER_OF_EVENTPATHS ); k++)

    {

      /* get the parameter priority and erasable */
    priority_u8 = DEH_GET_EVENT_PRIORITY(k);
    NOT_erasable_b = (DEH_EVENT_PARAMETER(k) & DEH_EP_MSK_EV_NO_CLEAR);

     if(   (   (!(NOT_erasable_b))
             &&( priority_u8 != DEH_DEVELOP_EVENT_PRIO))
        || (   (NOT_erasable_b)
            && ( priority_u8 != DEH_DEVELOP_EVENT_PRIO)
            && ((DEH_actDiaSes_muT == DEH_ECU_PRG_DIAG_SESSION)
                ||(DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION)))
        || (   (priority_u8 == DEH_DEVELOP_EVENT_PRIO)
            && (DEH_actDiaSes_muT == DEH_ENTW_DIAG_SESSION))  
       )
     {
        /* SET test NOT complete flag to NOT TESTED "1" */
      DEH_CLR_TEST_COMPLETE(k);
      #ifdef  DEH_SUP_UDS
          /* clear test failed last state status bytes "0" */
        DEH_CLR_TEST_LAST_STATE(k);
        #ifdef DEH_SUP_UDS_OBD_STATUS
            /* clear test failed since last clear "1" */
          DEH_CLR_TEST_FAILED_SLC(k);
            /* clear test failed this operation cycle status bytes "1" */
          DEH_CLR_TEST_FAILED_TOC(k);
            /* clear pending status bits "0" */
          DEH_CLR_PENDING_DTC_CUR_DC(k);
          DEH_CLR_PENDING_DTC_LAST_DC(k);
        #endif
      #endif
     }
    }
  }
}




