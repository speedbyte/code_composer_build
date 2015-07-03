/*==============================================================================
                      Diagnostic Event Handler  (DEH)
                     ---------------------------------
                      Version 1.2.5
                      Author: Ingo Gensert/ IAV GmbH
                      Date:   29.01.2008
                      Modul:  deh.h

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

#ifndef _DEH_H_
  #define  _DEH_H_

#include "deh_cfg.h"

#if defined(__cplusplus)
extern "C"
{
#endif


/*==========================================================================*/
/* Macros                                                                   */
/*==========================================================================*/



  /*  macro definiton */
#define DEH_EVENT_DEBOUNCE(x)       DEH_DebounceBuffer[x]
#define DEH_EVENT_BUFFER_STATE(x)   DEH_EventBuffer[x]
#define DEH_GET_EVENT_STATE(x)     ((tDEH_EventState) (  DEH_EventBuffer[x] & DEH_EB_MSK_EV_ST))
#define DEH_IS_EVENT_DEFECT(x)     ((tDEH_EventState) (  DEH_EventBuffer[x] & DEH_DEFECT_MASK))
#define DEH_GET_LAMP_INFO           DEH_stWarnLamp_mu8
#define DEH_GET_CAN_EVENT_BIT       DEH_stCANEventBit_mb
#define DEH_EVENT_IS_STORED(x)     ((tDEH_EventState) (  DEH_EventBuffer[x] & DEH_EB_MSK_EV_STORED))


  /* access to event parameters */
#define DEH_EVENT_DTC(x)           (DEH_EventPathParameter[x].DTC)
#define DEH_EVENT_PARAMETER(x)     (DEH_EventPathParameter[x].EventParameter)
#define DEH_EVENT_DEBTYPE(x)       (DEH_EventPathParameter[x].EventParameter &DEH_EP_MSK_EV_TYPE)
#define DEH_EVENT_SYMPRIO(x)       (DEH_EventPathParameter[x].EventSymPrio)
#define DEH_EVENT_QUAL(x)          (DEH_EventPathParameter[x].QualEvent)
#define DEH_EVENT_DEQUAL(x)        (DEH_EventPathParameter[x].DeQualEvent)
#define DEH_EVENT_LAMP_PAR(x)      (DEH_EventPathParameter[x].LampParameter)
#define DEH_EVENT_IDX_FF(x)        (DEH_EventPathParameter[x].IndexFFrameTable)
#define DEH_GET_EVENT_PRIORITY(x)  ((DEH_EventPathParameter[x].EventSymPrio) >> DEH_SHIFT_TOGET_PRIO)

/* ========================================================================== */
/* Defines                                                                    */
/* ========================================================================== */

  /* DEH standard version informations */
  /* DEH version 1.1.1                 */
#define DEH_VENDOR_ID         ((uint16_t) 0x000E)
#define DEH_MODULE_ID         ((uint8_t)  0x03)
#define DEH_SW_MAJOR_VERSION  ((uint8_t)  0x01)
#define DEH_SW_MINOR_VERSION  ((uint8_t)  0x02)
#define DEH_SW_PATCH_VERSION  ((uint8_t)  0x05)

  /* status availibility mask for UDS */
#ifdef DEH_SUP_UDS
  #if defined(DEH_SUP_UDS_OBD_STATUS)
    #define DEH_DTC_STATUS_AVAILABILITY_MASK  ((uint8_t) 0xFF)
  #else
    #define DEH_DTC_STATUS_AVAILABILITY_MASK  ((uint8_t) 0x99)
  #endif
#endif

  /* bit mask for defect detection */
#define DEH_DEFECT_MASK              0x04 

  /* debounce types for events */
#define TIME_DEB  0                /* time debounce */
#define EVENT_DEB 1                /* cycle debounce */
#define DEH_DEB_OVERFLOW 0xFFFE    /* constant for debounce buffer overflow */

  /* Event state defines -----------------------------------------------------*/
#define DEH_EVENT_PASSIVE           ((uint8_t) 0x00)
#define DEH_EVENT_DEB_ACT_STO       ((uint8_t) 0x01)
#define DEH_EVENT_DEBOUNCE_PASSIVE  ((uint8_t) 0x02)
#define DEH_EVENT_DEB_PAS_SUSPEND   ((uint8_t) 0x03)
#define DEH_EVENT_ACTIVE            ((uint8_t) 0x04)
#define DEH_EVENT_DEBOUNCE_ACTIVE   ((uint8_t) 0x06)
#define DEH_EVENT_DEB_ACT_SUSPEND   ((uint8_t) 0x07)
  /* end define for Event states */

  /* Defines event states*/
#define DEH_REPORT_EVENT            ((uint8_t) 0x01)
#define DEH_SUSPEND_EVENT           ((uint8_t) 0x02)
#define DEH_RESUME_EVENT            ((uint8_t) 0x03)
#define DEH_RESTART_EVENT           ((uint8_t) 0x04)
#define DEH_RESET_EVENT             ((uint8_t) 0x05)

  /* Define for no valid Report Event State (normal TRUE/FALSE ) */
#define DEH_NO_VALID_REPORT         ((uint8_t) 0xFF)

#define DEH_ReportEvent(EventNum_uT, status_b)  DEH_StM_Event(EventNum_uT, status_b, DEH_REPORT_EVENT )
#define DEH_ResetEvent(EventNum_uT)             DEH_StM_Event(EventNum_uT,(_Bool) DEH_NO_VALID_REPORT, DEH_RESET_EVENT )
#define DEH_RestartEvent(EventNum_uT)           DEH_StM_Event(EventNum_uT,(_Bool) DEH_NO_VALID_REPORT, DEH_RESTART_EVENT )
#define DEH_ResumeEvent(EventNum_uT)            DEH_StM_Event(EventNum_uT,(_Bool) DEH_NO_VALID_REPORT, DEH_RESUME_EVENT )
#define DEH_SuspendEvent(EventNum_uT)           DEH_StM_Event(EventNum_uT,(_Bool) DEH_NO_VALID_REPORT, DEH_SUSPEND_EVENT )

  /* error codes definitions */
#define DEH_OK                      ((tDEH_Return) 0)
#define DEH_E_UNKNOWN_EVENTPATH     ((tDEH_Return) 1)
#define DEH_E_BUSY                  ((tDEH_Return) 3)
#define DEH_E_DATA_STILL_AVAILABLE  ((tDEH_Return) 5)
#define DEH_E_EV_LOCKED             ((tDEH_Return) 6)
#define DEH_E_FULL                  ((tDEH_Return) 7)
#define DEH_E_INIT                  ((tDEH_Return) 8)
#define DEH_E_NO_DATA               ((tDEH_Return) 9)
#define DEH_E_MODE                  ((tDEH_Return) 12)
#define DEH_E_UNKNOWN_DTC           ((tDEH_Return) 13)
#define DEH_E_UNKNOWN_DTCGROUP      ((tDEH_Return) 14)
#define DEH_E_UNKNOWN_FF            ((tDEH_Return) 15)
#define DEH_E_UNKNOWN_SESSION_TYPE  ((tDEH_Return) 17)
#define DEH_E_READ_MEMORY           ((tDEH_Return) 19)
#define DEH_E_INIT_MEMORY           ((tDEH_Return) 20)
#define DEH_E_WRONG_DIAG_SESSION    ((tDEH_Return) 21)
#define DEH_EM_CLR_EV               ((tDEH_Return) 22)
#define DEH_E_NO_BUFFER             ((tDEH_Return) 23)
#define DEH_E_CLEAR                 ((tDEH_Return) 24)
#define DEH_E_PARAMETER             ((tDEH_Return) 25)
  /* defines for the Debounce */
#define DEH_DEB_STOP  ((tDEH_QualET) 0xFFFF) /* stopp debounce value */

  /* Bitmasks */
  /* Masks for the paramter 'EventParameter' */  
#define DEH_EP_MSK_EV_TYPE            ((uint8_t) 0x01)  /* Flag for event type 0-time / 1-cycle deb*/
#define DEH_EP_MSK_EV_NO_UNLEARN      ((uint8_t) 0x02)  /* Flag for block the unlearn algorithm    */
#define DEH_EP_MSK_EV_REP_CHG_ST      ((uint8_t) 0x04)  /* Flag for event state change detetction  */
#define DEH_EP_MSK_EV_NO_CLEAR        ((uint8_t) 0x08)  /* Flag for block clear event              */
#define DEH_EP_MSK_EV_IS_EMISSION_REL ((uint8_t) 0x10)  /* Flag for event type 0-not emision related / 1-emission related*/
#define DEH_EP_MSK_EV_SUP_SNAPSHOTS   ((uint8_t) 0x20)  /* Flag for supporting snapshot records    */


  /* parameter 'EventSymPrio' */
#define DEH_SHIFT_TOGET_PRIO      4     /* shift operation to get the event priority */

  /* Event Buffer */
#define DEH_EB_MSK_EV_ST          ((uint8_t) 0x07)  /* 3 Bit for actual event state           */
#define DEH_EB_MSK_WO_ST_MSK      ((uint8_t) 0xF8)  /* Mask for all Bit without event state   */
#define DEH_EB_MSK_DTC_RB_SET     ((uint8_t) 0xEF)  /* set DTC Readiness bit to tested*/
#define DEH_EB_MSK_DTC_RB         ((uint8_t) 0x10)  /* mask for DTC Testcomplete bit */
#define DEH_EB_MSK_LOCK_DTC       ((uint8_t) 0x08)  /* mask for locked DTC */
#define DEH_EB_MSK_FREE_DTC       ((uint8_t) 0xF7)  /* mask for release a locked DTC */
#define DEH_EB_MSK_EV_STORED      ((uint8_t) 0x40)  /* Bit 6 for indication event is storen in EEP */
#define DEH_EB_MSK_EV_STO_UPD     ((uint8_t) 0x20)  /* Bit 5 for indication event storage state have to be updated  in EEP */
#define DEH_EB_MSK_EV_STO_UPD_RST ((uint8_t) 0xDF)  /* reset Bit 5 for event storage state update indication */
#define DEH_EB_MSK_EV_STO_RST     ((uint8_t) 0x9F)  /* reset Bit 5 and Bit6 for event storage state update indication and event storage ind. */
#define DEH_EB_MSK_READ_FF        ((uint8_t) 0x80)  /* indication for read FF necessary */
#define DEH_EB_MSK_RST_READ_FF    ((uint8_t) 0x7F)  /* reset the indication for read FF */

#define DEH_STOFDTC_INIT_MEM_BLOCK    ((uint8_t) 0xA5)  /* status flag for INIT */
  /* defines for StatusOfDTC */
#if   defined(DEH_SUP_KWP2000)
  #define DEH_STOFDTC_MSK_TESTNC_SLC    ((uint8_t) 0x10)  /* mask for TestNotCompleteSinceLastClear (Readiness#1) bit */
  #define DEH_STOFDTC_MSK_STORED        ((uint8_t) 0x20)  /* mask for Stored bit (32dez) */
  #define DEH_STOFDTC_MSK_ACTIVE        ((uint8_t) 0x40)  /* mask for Active bit(without debounce)*/
#elif defined(DEH_SUP_UDS)
  #define DEH_STOFDTC_MSK_ACTIVE        ((uint8_t) 0x01)  /* mask for TestFailed bit */
  #define DEH_STOFDTC_MSK_FAILED_CYCLE  ((uint8_t) 0x02)  /* mask for TestFailedThisOparationCycle bit */
  #define DEH_STOFDTC_MSK_PENDING       ((uint8_t) 0x04)  /* mask for PendingDTC bit */
  #define DEH_STOFDTC_MSK_STORED        ((uint8_t) 0x08)  /* mask for ConfirmedeDTC bit */
  #define DEH_STOFDTC_MSK_TESTNC_SLC    ((uint8_t) 0x10)  /* mask for TestNotCompleteSinceLastClear (Readiness#1) bit */
  #define DEH_STOFDTC_MSK_FAILED_CLEAR  ((uint8_t) 0x20)  /* mask for TestFailedSinceLastClear bit */  
  #define DEH_STOFDTC_MSK_RB_TOC        ((uint8_t) 0x40)  /* mask for TestNotCompleteThisOperationCycle (Readiness#2) bit */
#endif 
#define   DEH_STOFDTC_MSK_LAMP          ((uint8_t) 0x80)  /* mask for warning lamp bit (on) */
#define   DEH_STOFDTC_MSK_LAMP_RESET    ((uint8_t) 0x7F)  /* mask for warning lamp bit (off) */

  /* defines for the event memory states */
#define DEH_EM_NO_ENTRY           ((uint8_t) 0x00)  /* no entry in the event memory*/
#define DEH_EM_NO_VALID_STATE     ((uint8_t) 0xFF)  /* no valid state */
#define DEH_EM_ACTIVE_STORED      ((uint8_t) (DEH_STOFDTC_MSK_ACTIVE | DEH_STOFDTC_MSK_STORED)) /* active and stored */
#define DEH_EM_PASSIVE_STORED     ((uint8_t) DEH_STOFDTC_MSK_STORED) /* stored but not active */

  /* modes of event memory state machine */
#define DEH_EM_UPDATE             ((uint8_t) 0x01)  /* define to update the event memory */
#define DEH_EM_CHECK_LUC          ((uint8_t) 0x55)  /* define to test the LUC */


  /* -------------------------------------*/
  /* Event Memory Error Flags  Definition */
  /* -------------------------------------*/

  /* wrong event number in event memory (fct.: DEH_ESiF_LookForPosToOvEM())*/
#define DEH_EM_MSK_NO_PLAUS_EVNUM         0x01  

  /* wrong event number in EEP during init phase (fct.: DEH_ESiF_InitEM()) */
#define DEH_ERR_MSK_EVNUM_EM_INIT         0x02  

  /* wrong return code from function DEH_ESiF_ClearEM (fct.: DEH_ESiF_InitEM()) */
#define DEH_ERR_MSK_EM_CLEAR              0x04  

  /* wrong return code from function APL_DEH_EEPReadEventData */
#define DEH_ERR_MSK_EEP_READ              0x08  

  /* wrong return code from function APL_DEH_EEPReadEventData */
#define DEH_ERR_MSK_EEP_WRITE             0x10  

  /* ----------------------*/
  /* Event Data Definition */
  /* ----------------------*/

    /* EventSymPrio Parameter */
#define DEH_ESP_MSK_FAULT_SYM 0x0F  /* mask to get only the fault symptom */

  /* mode Parameter for function DEH_SUiF_StatusOfDTC */
#define  WRITE_EM     ((uint8_t) 0)
#define  SERVICE_REQ  ((uint8_t) 1)

  /* Limt to unlearn a stored passive event */
#define DEH_UNLEARN_OFFSET ((uint8_t)  40 )
#define DEH_LAST_VALID_LUC ((uint8_t) 254 )
#define DEH_LUC_OVERFLOW   ((uint8_t) (DEH_LAST_VALID_LUC+1))

/* ----------- End Data Definition Mask for all parameter --------------------*/

  /* Diagnostic Type Definition */
#define DEH_EXT_DIAG_SESSION          ((uint8_t) 0x03)
#define DEH_VW_EOL_DIAG_SESSION       ((uint8_t) 0x40)
#if defined(DEH_SUP_KWP2000)
  #define DEH_EOBD_DIAG_SESSION       ((uint8_t) 0x81)
  #define DEH_ECU_PRG_DIAG_SESSION    ((uint8_t) 0x85)
  #define DEH_ENTW_DIAG_SESSION       ((uint8_t) 0x86)
  #define DEH_VW_DEFAULT_DIAG_SESSION ((uint8_t) 0x89)
#elif defined(DEH_SUP_UDS)
  #define DEH_VW_DEFAULT_DIAG_SESSION ((uint8_t) 0x01)
  #define DEH_ECU_PRG_DIAG_SESSION    ((uint8_t) 0x02)
  #define DEH_ENTW_DIAG_SESSION       ((uint8_t) 0x4F)
#endif
#define DEH_NO_DIAG_SESSION           ((uint8_t) 0xFF)

  /* DTC Group Definition */
#if defined(DEH_SUP_KWP2000)
  #define DEH_DTC_ALL_GROUPS ((tDEH_DTCGroup) 0xFF00)
#elif defined(DEH_SUP_UDS)
  #define DEH_DTC_ALL_GROUPS ((tDEH_DTCGroup) 0xFFFFFF)
#endif

  /* mode parameter for the function DEH_ReadIdentifiedEvents */
#define DEH_READ_ALL_EVENTS  ((uint8_t) 0xFF)

  /* mode parameter for the function DEH_ReadDTCByStatus */
#define DEH_READ_ALL_DTC                ((uint8_t) 0x02)
#define DEH_READ_EMISSION_REL_DTC       ((uint8_t) 0x13)

  /* Service-Function */
#define DEH_SRV_CLEAR_DIAG_INF          ((uint8_t) 0x01)
#define DEH_SRV_CTRL_DTC_SET            ((uint8_t) 0x02)
#define DEH_SRV_SET_DIAG_SES            ((uint8_t) 0x03)
#define DEH_SRV_KWP_READ_SUP_EVENTS     ((uint8_t) 0x04)
#define DEH_SRV_KWP_READ_IDT_EVENTS     ((uint8_t) 0x05)
#define DEH_SRV_KWP_READ_ADD_FF         ((uint8_t) 0x06)
#define DEH_SRV_KWP_READ_STD_FF         ((uint8_t) 0x07)
#define DEH_SRV_UDS_READ_DTC_BY_STATUS  ((uint8_t) 0x08)
#define DEH_SRV_UDS_READ_SNAPSHOT_REC   ((uint8_t) 0x09)
#define DEH_SRV_UDS_READ_EXT_DATA_REC   ((uint8_t) 0x0A)
#define DEH_SRV_RSE_RESET               ((uint8_t) 0xFF)

  /* macros to get the size of EMB variables*/
#define EMB_BL_SIZE_EV_NUM    ((uint8_t) sizeof(uint16_t))
#define EMB_BL_SIZE_STOFDTC   ((uint8_t) sizeof(uint8_t))
#define EMB_BL_SIZE_LUC       ((uint8_t) sizeof(uint8_t))
#define EMB_BL_SIZE_OCC       ((uint8_t) sizeof(uint8_t))
#define EMB_BL_SIZE_STDFF     ((uint8_t) (DEH_SIZEOF_STDFF))
#ifdef DEH_SUP_KWP2000
  #define EMB_BL_SIZE_ADDFF    ((uint8_t) (DEH_SIZEOF_ADDFF * DEH_MAX_NUMBER_ADD_FF))
#else 
  #define EMB_BL_SIZE_ADDFF    ((uint8_t) (DEH_MAX_NUMBER_ADD_FF* DEH_MAX_EXT_REC_BUFFER_DATA))
#endif
#define EMB_BLOCK_SIZE         ((uint8_t) (EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC + EMB_BL_SIZE_LUC + EMB_BL_SIZE_OCC + EMB_BL_SIZE_STDFF+ EMB_BL_SIZE_ADDFF))

#define EMB_BLOCK_UPDATE_EM    ((uint8_t) (EMB_BL_SIZE_EV_NUM + EMB_BL_SIZE_STOFDTC + EMB_BL_SIZE_LUC + EMB_BL_SIZE_OCC))

#define EMB_BUFFER_SIZE        ((uint16_t) (DEH_MAX_NUMBER_EV_MEM * EMB_BLOCK_SIZE))

   /* positions in the external event buffer array */
#define EMB_POS_EV_NUM_LOWBYTE  ((uint8_t) 0)
#define EMB_POS_EV_NUM_HIGHBYTE ((uint8_t) (EMB_POS_EV_NUM_LOWBYTE  + 1))
#define EMB_POS_STOFDTC         ((uint8_t) (EMB_POS_EV_NUM_LOWBYTE  + EMB_BL_SIZE_EV_NUM))
#define EMB_POS_LUC             ((uint8_t) (EMB_POS_STOFDTC         + EMB_BL_SIZE_STOFDTC))
#define EMB_POS_OCC             ((uint8_t) (EMB_POS_LUC             + EMB_BL_SIZE_LUC))
#define EMB_POS_STDFF           ((uint8_t) (EMB_POS_OCC             + EMB_BL_SIZE_OCC))
#define EMB_POS_ADDFF           ((uint8_t) (EMB_POS_STDFF           + EMB_BL_SIZE_STDFF))

  /* --------------------------- */
  /* Event Memory Buffer Defines */
  /* --------------------------- */
  /* indication for a free event memory entry */

#define DEH_EM_FREE_POS        ((uint16_t) 0xFFFF)
#define DEH_EM_NO_VALID_POS    ((tDEH_EventMemory) 0xFF)

  /* sizeof structure element in standard Freeze Frames */
#define DEH_SIZEOF_STDFF_KM    3  
#define DEH_SIZEOF_STDFF_TIME  5

#define DEH_SIZEOF_STDFF       8

#if defined(DEH_SUP_KWP2000)
  #define DEH_SIZEOF_ADDFF     3
#elif defined(DEH_SUP_UDS)
  #define DEH_SIZEOF_ADDFF     (EMB_BL_SIZE_EV_NUM + sizeof(uint8_t) + DEH_MAX_LEN_ADD_FF)
#endif

  /* --------------------------- */
  /* Freeze Frame Buffer Defines */
  /* --------------------------- */
#define DEH_STO_NO_FF            ((uint8_t) 0xFF)
#define DEH_FF_NO_VALID_POS      ((uint8_t) 0xFF)
#define DEH_FF_FREE_POS          ((uint16_t)0xFFFF)

#if (defined(DEH_SUP_KWP2000))
  #define STD_FF_NUMBER            ((uint8_t) 0x6C)
#elif (defined(DEH_SUP_UDS))
  #define STD_FF_NUMBER            ((uint8_t) 0x02)
#endif

#define DEH_TIME_IND_BIT_VAL     ((uint8_t) 0x00)
#define DEH_KWP_READ_ALL_FF      ((uint8_t) 0x00)

  /* --------------------------- */
  /* CONTROL DTC Defines         */
  /* --------------------------- */
#if defined(DEH_SUP_KWP2000)
  #define DEH_DTC_ALL       ((tDEH_TypeOfDTC) 0xFFFF)
#elif defined(DEH_SUP_UDS)
  #define DEH_DTC_ALL       ((tDEH_TypeOfDTC) 0xFFFFFF)
#endif
#define DEH_DTC_COM         ((tDEH_TypeOfDTC) 0xFFF1)
#define DEH_DTC_NO          ((tDEH_TypeOfDTC) 0x0000)

#define DEH_SET_DTC_ON      ((uint8_t) 0x01)
#define DEH_SET_DTC_OFF     ((uint8_t) 0x02)


#define DEH_NO_VALID_UNLEARN_COUNTER ((uint8_t) 0xFF)

#define DEH_UC_READ    ((uint8_t)  0)
#define DEH_UC_UNLEARN ((uint8_t)  1)
#define DEH_UC_WAIT    ((uint8_t)  3)
#define DEH_UC_STOP    ((uint8_t)  7)
#define DEH_UC_INIT    ((uint8_t) 15)
  /* defines for UDS service DTCExtendedDataRecord (19hex) */
  
#define DEH_STD_DTC_INFO_01                  ((tDEH_RecordNumber) 0x01)
#define DEH_STD_FREEZE_FRAME_INFO_02         ((tDEH_RecordNumber) 0x02)
#define DEH_ALL_STDFF_AND_DTC_INFO_8F        ((tDEH_RecordNumber) 0x8F)
#define DEH_ALL_ADD_STDFF_AND_ADDFF_FF       ((tDEH_RecordNumber) 0xFF)
#define DEH_EXT_REC_NUM_MIN_6F               ((tDEH_RecordNumber) 0x6F)
#define DEH_EXT_REC_NUM_MAX_80               ((tDEH_RecordNumber) 0x80)

#define DEH_NO_EXT_REC_DATA_AVAILABLE     ((tDEH_RecordNumber) 0xFF)

#define DEH_STD_DTC_INFO_MIN_BUFFER       ((uint16_t)  3) 
#define DEH_STD_FF_INFO_MIN_BUFFER        ((uint16_t) 11) 
#define DEH_ALL_STDFF_DTC_INF_MIN_BUFFER  ((uint16_t) 14) 

#define DEH_FIRST_POS_EXT_REC_DATA_0xFF   ((uint8_t)  14) 

#define DEH_LAST_VALID_DTC                ((uint32_t) 0x00FFFFFF)

#define DEH_DEVELOP_EVENT_PRIO                ((uint8_t) 9)

#define DEH_INIT_STOFDTC_TEST_NOT_COMPL  ((uint8_t) 0xFF)

#define DEH_SETBIT(word,pos)     ((word) |=(uint8_t)  (1u<<(pos)))  
#define DEH_CLRBIT(word,pos)     ((word) &=(uint8_t) ~(1u<<(pos)))  
#define DEH_GETBIT(word,pos)     (_Bool)(((word)>>(pos)) & 1u)  
#define DEH_SET_TEST_COMPLETE(num)       DEH_CLRBIT(TestNC[BYTE_POS(num)],BIT_POS(num)) 
#define DEH_GET_TEST_COMPLETE(num)       DEH_GETBIT(TestNC[BYTE_POS(num)],BIT_POS(num)) 
#define DEH_CLR_TEST_COMPLETE(num)       DEH_SETBIT(TestNC[BYTE_POS(num)],BIT_POS(num)) 

  /* ======================================================================== */
  /*  DEFINES for different calls to erase an event                           */
  /* ======================================================================== */
#define DEH_CLEAR_DIAG_INFO 0x01
#define DEH_CLEAR_EVENT     0x02

#ifdef DEH_SUP_UDS
  #define DEH_INIT_STOFDTC_NO_PEND_DTC     ((uint8_t) 0x00) 
  #define DEH_INIT_STOFDTC_TEST_FAILED     ((uint8_t) 0x00) 

  #define DEH_SET_TEST_LAST_STATE(num)     DEH_SETBIT(TestFLS[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_GET_TEST_LAST_STATE(num)     DEH_GETBIT(TestFLS[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_CLR_TEST_LAST_STATE(num)     DEH_CLRBIT(TestFLS[BYTE_POS(num)],BIT_POS(num)) 
  
  #define DEH_SET_TEST_FAILED_TOC(num)     DEH_SETBIT(TestFailedTOC[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_GET_TEST_FAILED_TOC(num)     DEH_GETBIT(TestFailedTOC[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_CLR_TEST_FAILED_TOC(num)     DEH_CLRBIT(TestFailedTOC[BYTE_POS(num)],BIT_POS(num)) 

  #define DEH_SET_PENDING_DTC(num)         DEH_SETBIT(PendingDTC_curDC[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_GET_PENDING_DTC_CUR_DC(num)  DEH_GETBIT(PendingDTC_curDC[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_CLR_PENDING_DTC_CUR_DC(num)  DEH_CLRBIT(PendingDTC_curDC[BYTE_POS(num)],BIT_POS(num)) 

  #define DEH_GET_PENDING_DTC_LAST_DC(num) DEH_GETBIT(PendingDTC_lastDC[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_CLR_PENDING_DTC_LAST_DC(num) DEH_CLRBIT(PendingDTC_lastDC[BYTE_POS(num)],BIT_POS(num)) 
  
  #define DEH_SET_TEST_FAILED_SLC(num)     DEH_SETBIT(TestFailedSLC[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_GET_TEST_FAILED_SLC(num)     DEH_GETBIT(TestFailedSLC[BYTE_POS(num)],BIT_POS(num)) 
  #define DEH_CLR_TEST_FAILED_SLC(num)     DEH_CLRBIT(TestFailedSLC[BYTE_POS(num)],BIT_POS(num)) 
  
/* mode definition for application function APL_DEH_SnapShotDatenInit(mode) */
  #define  DEH_INIT                ((uint8_t) 1)
  #define  DEH_CLEAR_EVENT_MEMORY  ((uint8_t) 3)

#endif  

/* ========================================================================== */
/* Typedefs                                                                   */
/* ========================================================================== */

  /* type for event buffer state */
typedef uint8_t tDEH_EventBufferState;

  /* type for event state */
typedef uint8_t tDEH_EventState;

  /* type definition for event path number */

#if DEH_MAX_NUMBER_OF_EVENTPATHS > 255
  typedef uint16_t tDEH_EventNumber;
#else
  typedef uint8_t tDEH_EventNumber;
#endif

  /* type for Diagnostic Session */
typedef uint8_t tDEH_DiagnosticType; 

  /* Type for enable and disable DTCs */
typedef uint8_t tDEH_DTCSetType; 

  /* type for event memory */
typedef uint8_t tDEH_EventMemory;

  /* number for a Freeze Frame (0xC6 = Std FF) */
typedef uint8_t tDEH_FreezeFrameNumber;

  /* type for warning lamp control */
typedef uint8_t tDEH_LampInfos;
 
  /* Counter or Timer for event qualification or dequalification */
typedef uint16_t tDEH_QualET;  

  /* type for function return value */
typedef uint8_t tDEH_Return;   

#if defined(DEH_SUP_KWP2000)
    /* Diagnostic Trouble Code for KWP2000 */
  typedef uint16_t tDEH_DTC;  

    /* DTC group definition for KWP2000 */
  typedef uint16_t tDEH_DTCGroup;
   
    /* DTC type for KWP2000 */
  typedef uint16_t tDEH_TypeOfDTC;
#elif defined(DEH_SUP_UDS)
    /* Diagnostic Trouble Code for UDS */
  typedef uint32_t tDEH_DTC;  

    /* DTC group definition for UDS */
  typedef uint32_t tDEH_DTCGroup;
   
    /* DTC type for UDS */
  typedef uint32_t tDEH_TypeOfDTC;

    /* type definition for DTC inrormation types */
  typedef uint8_t tDEH_DTCInfoType;

    /* type definition for DTC status masks */
  typedef uint8_t tDEH_DTCStatusMask;

    /* type definition for numbers of extend data records */
  typedef uint8_t tDEH_RecordNumber;
#endif


  /* type definition for read  all detectable events */
typedef struct
{
  tDEH_DTC DTC;
  uint8_t StatusOfDTC;
} tDEH_DTCAndStatus;  


  /* Type definition for all event parameter */
typedef struct
{
  tDEH_DTC       DTC;              /* Diagnostic Trouble Code */
  uint8_t        EventParameter;   /* time or cycle/erasable/chg state detect.*/
  uint8_t        EventSymPrio;     /* faulttype and priority */
  tDEH_QualET    QualEvent;        /* time or  cycle for defect detection */
  tDEH_QualET    DeQualEvent;      /* time or  cycle for defect detection */
  tDEH_LampInfos LampParameter;    /* Parameter to control a warning lamp */
  uint8_t        IndexFFrameTable; /* index for Freeze Frame Table */
} tDEH_EventPathParameter;


  /* Type definition Standard Freeze Frame Block for KWP2000*/
typedef struct
{
    uint8_t StatusOfDTC;
    uint8_t Priority;
    uint8_t OCC;
    uint8_t UnlearnCounter;
    uint8_t Km[3];
    uint8_t TimeIndicationBit;
    uint8_t TimeStamp[5];
} tDEH_StdFreezeFrameData;


  /* type definition for DEH version information */
typedef struct
{
    uint16_t  vendorID;
    uint8_t   moduleID;
    uint8_t   sw_major_version;
    uint8_t   sw_minor_version;
    uint8_t   sw_patch_version;
} tDEH_StdVersionInfo;

/* ========================================================================== */
/* Global Prototypes                                                          */
/* ========================================================================== */

tDEH_Return DEH_ClearEvent(tDEH_EventNumber EventNumber_uT);
tDEH_Return DEH_Init(void);
void        DEH_ProcessDiagEventHandler(void);
tDEH_Return DEH_StM_Event(tDEH_EventNumber EventNum_uT, _Bool status_b, uint8_t ExtEventFkt_u8);
void        DEH_GetVersionInfo(tDEH_StdVersionInfo *pVersionInfo);

  /* Prototypes for KWP2000 and UDS interface*/
tDEH_Return DEH_SetDiagnosticSession (tDEH_DiagnosticType DiagnosticType);
tDEH_Return DEH_ClearDiagnosticInformation(tDEH_DTCGroup DTCGroup);
tDEH_Return DEH_ControlDTCSetting (tDEH_DTCSetType DTCSetType, tDEH_TypeOfDTC TypeOfDTC);

#ifdef DEH_SUP_KWP2000
  tDEH_Return DEH_ReadStdFreezeFrameData(tDEH_DTC Dtc, tDEH_StdFreezeFrameData *pStdFFrames);
  tDEH_Return DEH_ReadAddFreezeFrameData(tDEH_DTC Dtc, tDEH_FreezeFrameNumber FF_Number,uint8_t *pNumOfBytes, tDEH_AddFreezeFrame *pAddFFrames);
  tDEH_Return DEH_ReadIdentifiedEvents (tDEH_DTCGroup DTCGroup, tDEH_DTCAndStatus* pDestination, uint8_t EventMode);
  tDEH_Return DEH_ReadSupportedEvent (tDEH_DTCGroup DTCGroup, tDEH_DTCAndStatus* pDestination);
  tDEH_Return DEH_GetNumberOfIdentEvents (tDEH_DTCGroup DTCGroup, tDEH_EventNumber* EventNum);
  tDEH_Return DEH_GetNumberOfSupEvents (tDEH_DTCGroup DTCGroup, tDEH_EventNumber* EventNum);
#endif

#if (defined(DEH_TEST))
  uint8_t DEH_SUiF_StatusOfDTC(uint16_t EventNumber_u16, uint8_t mode_u8);
#endif
  /* Prototypes for UDS interface*/
#ifdef DEH_SUP_UDS
tDEH_Return DEH_ReadDTCByStatus(tDEH_DTCInfoType DTCInfoType, tDEH_DTCStatusMask DTCStatusMask, tDEH_DTCAndStatus *pDTCAndStatus);
  #ifdef DEH_SUP_SNAP_SHOT_DATA
    tDEH_Return DEH_ReadDTCSnapshotRecord(tDEH_DTC DTC, uint8_t *pStatusOfDTC, uint16_t BuffSize, uint16_t *pNumOfBytes, uint8_t *pDestBuffer);
  #endif
tDEH_Return DEH_ReadDTCExtDataRecord(tDEH_DTC DTC, uint8_t *pStatusOfDTC, tDEH_RecordNumber RecordNumber, uint16_t BuffSize, uint16_t *pNumOfBytes, uint8_t *pDestBuffer);
/* test for UDS Status Of DTC */  
#endif
tDEH_Return DEH_StoreStatusOfDTCInfos(void);

  /* external functions */
extern tDEH_Return APL_DEH_ReadStdFF(tDEH_StdFreezeFrame *pStdDestination);

#if (defined(DEH_SUP_KWP2000))

  extern tDEH_Return APL_DEH_ReadAddFF(tDEH_FreezeFrameNumber FF_Number, tDEH_AddFreezeFrame *pAddDestination);

#elif (defined(DEH_SUP_UDS))

  #ifdef DEH_READ_ADD_FF
    extern tDEH_Return APL_DEH_ReadAddFF( tDEH_RecordNumber RecordNumber, uint8_t BuffSize_u8,uint8_t *pNumOfBytes, uint8_t *pDataBuffer_u8);
  #endif
  #ifdef DEH_SUP_SNAP_SHOT_DATA
    extern tDEH_Return APL_DEH_ReadDTCSnapshotRecord(tDEH_DTC DTC, uint16_t BuffSize_u16, uint16_t *pNumOfBytes_u16, uint8_t *pDestBuffer);
    extern void        APL_DEH_SnapShotDatenInit(uint8_t mode);
  #endif
#endif

extern tDEH_Return APL_DEH_ReadStatusOfDTC(uint16_t BlockSize_u16, uint8_t *Buffer_u8);
extern tDEH_Return APL_DEH_WriteStatusOfDTC(uint16_t BlockSize_u16, uint8_t *Buffer_u8);
extern tDEH_Return APL_DEH_EEPReadEventData ( uint16_t  BlockPosition_u16, uint16_t BlockSize_u16, uint8_t *Buffer_u8);
extern tDEH_Return APL_DEH_EEPWriteEventData( uint16_t  BlockPosition_u16, uint16_t BlockSize_u16, uint8_t *Buffer_u8);
extern uint8_t     APL_DEH_ReadUnlearnCounter(void);

#ifdef DEH_SUP_DETECT_CHANGE_EV_STATE
  extern void APL_DEH_ReportChgEventState(tDEH_EventNumber EventNum_uT, uint8_t state_u8);
#endif 

    /* array for all actual event states */
extern tDEH_EventBufferState DEH_EventBuffer[DEH_MAX_NUMBER_OF_EVENTPATHS];

   /* array to store the parameters for all events */
extern const tDEH_EventPathParameter DEH_EventPathParameter[DEH_MAX_NUMBER_OF_EVENTPATHS];

  /* array with all Freeze Frame Definitions */
#if ((defined(DEH_READ_ADD_FF)) || (defined(DEH_READ_STD_FF)))
  extern uint8_t DEH_FreezeFrameTable[DEH_MAX_NUMBER_FF_IDX][DEH_MAX_NUMBER_FF]; 
#endif

  /* message for CAN-Event Bit for the system */ 
extern _Bool DEH_stCANEventBit_mb;

  /* global variable for the actual warnlamp state */
extern uint8_t DEH_stWarnLamp_mu8;

// Function declarations

#if defined(__cplusplus)
}
#endif

  /* avoid double INCLUDES */
#endif



