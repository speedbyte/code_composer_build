/*==============================================================================
                      Diagnostic Event Handler  (DEH)
                     ---------------------------------
                      Version 1.2.5
                      Author: Ingo Gensert/ IAV GmbH
                      Date:   29.01.2008
                      Modul:  deh_cfg.h

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

#ifndef _DEH_CFG_H_
  #define  _DEH_CFG_H_

#include "stdint.h"
#include "stdbool.h"
/*==========================================================================*/
/* Project Includes                                                         */
/*==========================================================================*/

/*==========================================================================*/
/* Defines                                                                  */
/*==========================================================================*/

//#define DEH_TEST
    /* -------------------------------------------------------------------- */
    /* flag to enable the KWP2000 variant of diagnostic event handler       */
    /* If this flag is enabled then the DEH supports service functions,     */
    /* application functions and data type definitions of KWP2000           */
    /* diagnostic protocol                                                  */
    /* -------------------------------------------------------------------- */
//#define DEH_SUP_KWP2000

/* ========================================================================== */
/*               U D S - DEFINE Block S T A R T                               */
/*  1  - DEH_SUP_UDS                --> ENABLE UDS OR NOT                     */
/*  2  - DEH_SUP_UDS_OBD_STATUS     --> ENABLE OBD SUPPORT                    */
/*  3  - DEH_SUP_SNAP_SHOT_DATA     --> ENABLE SUPPORT OF SNAPSHOT DATAS      */
/*  4  - DEH_MAX_SNAPSHOT_DATA_LEN  --> DEFINE MAXIMUM SNAPSHOT DATA SIZE     */
/*  5  - DEH_MAX_LEN_ADD_FF         --> DEFINE MAXIMUM FOR ADDITIONAL FF DATAS*/
/* ========================================================================== */
    /* -------------------------------------------------------------------- */
    /* flag to enable the UDS variant of diagnostic event handler           */
    /* If this flag is enabled then the DEH supports service functions,     */
    /* application functions and data type definitions of UDS diagnostic    */
    /* protocol                                                             */
    /* -------------------------------------------------------------------- */
#define DEH_SUP_UDS

    /* -------------------------------------------------------------------- */
    /* flag to enable OBD status bits in the UDS variant of DEH             */
    /* This flag should be only defined if the flag EH_SUP_UDS is defined.  */
    /* -------------------------------------------------------------------- */
#define DEH_SUP_UDS_OBD_STATUS

    /* -------------------------------------------------------------------- */
    /* flag to enable support for snap shot data                            */
    /* This flag should be only defined if the flag EH_SUP_UDS is defined.  */
    /* -------------------------------------------------------------------- */
// #define DEH_SUP_SNAP_SHOT_DATA

    /* -------------------------------------------------------------------- */
    /* define for maximum data length for the snapshot datas                */
    /* This define is only relevant if  snapshot datas are activated        */
    /* -------------------------------------------------------------------- */

//#define DEH_MAX_SNAPSHOT_DATA_LEN 20

    /* -------------------------------------------------------------------- */
    /* Define for maximum length of additional freeze frames                */
    /* The maximum legth should be only defined if the flag                 */
    /* DEH_SUP_UDS is defined.                                              */
    /* -------------------------------------------------------------------- */
#define DEH_MAX_LEN_ADD_FF 10

/* ========================================================================== */
/*               U D S - DEFINE Block E N D                                   */
/* ========================================================================== */

    /* -------------------------------------------------------------------- */
    /*  ### check the plausibility of DEH variant settings ###              */
    /*  Force compiler error in folowing cases:                             */
    /*  I)  flags DEH_SUP_KWP2000 and DEH_SUP_UDS are both defined          */
    /*  II) flags DEH_SUP_KWP2000 and DEH_SUP_UDS are not defined           */
    /* -------------------------------------------------------------------- */
#if ( defined(DEH_SUP_KWP2000) && defined(DEH_SUP_UDS) )
    #error "ERROR :: The flags DEH_SUP_KWP2000 and DEH_SUP_UDS may not be defined at the same time!"
#elif ( (!(defined(DEH_SUP_KWP2000))) && (!(defined(DEH_SUP_UDS))) )
 #error "ERROR :: DEH variant not set! Define the flag DEH_SUP_KWP2000 or DEH_SUP_UDS!"
#endif

    /* -------------------------------------------------------------------- */
    /*  ### check the plausibility of variant configuration ###             */
    /*  Force compiler error in folowing cases:                             */
    /*  I)  flag DEH_SUP_KWP2000 is defined and the flags                   */
    /*      DEH_SUP_UDS_LAMP_STATUS or DEH_SUP_UDS_OBD_STATUS are defined   */
    /* -------------------------------------------------------------------- */

#if (defined(DEH_SUP_KWP2000) && (defined(DEH_SUP_UDS_OBD_STATUS)))
    #error "ERROR :: UDS configuration flags may not be defined in KWP2000 variant!"
#endif


    /* -------------------------------------------------------------------- */
    /* maximum of defined events which are present in the system            */
    /* every event has special parameters (see typedef                      */
    /* tDEH_EventPathParameter                                              */
    /* -------------------------------------------------------------------- */
#define DEH_MAX_NUMBER_OF_EVENTPATHS  30

    /* -------------------------------------------------------------------- */
    /* maximum of possible events which could stored into the event memory  */
    /* all events are normally stored in a EEPROM                           */
    /* The maximum Number of possible events wich can be provided by DEH in */
    /* a EEPROM is 254 !!!                                                  */
    /* The minimum should be 5 entries. (VW 80114 Spec.)                    */
    /* -------------------------------------------------------------------- */
#define DEH_MAX_NUMBER_EV_MEM  5

    /* -------------------------------------------------------------------- */
    /* flag to read and store standard freeze frames                        */
    /* standard freeze frames are normally received via GATEWAY COMFORT 1   */
    /* message and are 7 Bytes. The internal storage are 8 Byte with the    */
    /* 3 Byte kilometers and 5 Bytes timestamp.                             */
    /* -------------------------------------------------------------------- */
#define DEH_READ_STD_FF 

    /* -------------------------------------------------------------------- */
    /* flag to read and store additional freeze frames                      */
    /* -------------------------------------------------------------------- */
#define DEH_READ_ADD_FF 

    /* -------------------------------------------------------------------- */
    /* Define for maximum number of additional freeze frames per stored     */
    /* event.                                                               */
    /* -------------------------------------------------------------------- */
#define DEH_MAX_NUMBER_ADD_FF 3

    /* -------------------------------------------------------------------- */
    /*  ### check the plausibility of parameters  ###                       */
    /*  Force compiler error in folowing cases:                             */
    /*  I)  ADD FREEZE FRAMES are defined but the value of                  */
    /*      DEH_MAX_NUMBER_ADD_FF or DEH_MAX_LEN_ADD_FF is not correct      */
    /*  II) ADD FREEZE FRAMES are NOT defined  AND the value of             */
    /*      DEH_MAX_NUMBER_ADD_FF or of DEH_MAX_LEN_ADD_FF is not correct   */
    /*  III)DEH_MAX_LEN_ADD_FF is defined in KWP2000 variant                   */
    /* -------------------------------------------------------------------- */

#if (defined(DEH_READ_ADD_FF))
    #if ((DEH_MAX_NUMBER_ADD_FF == 0) || (!(defined(DEH_MAX_NUMBER_ADD_FF))))
      #error "ERROR :: The number of additional freeze frames is NOT DEFINED or zero!"
    #elif ((defined(DEH_SUP_UDS)) && ((DEH_MAX_LEN_ADD_FF == 0) || (!(defined(DEH_MAX_LEN_ADD_FF)))))
      #error "ERROR :: The maximum length of additional freeze frames is NOT DEFINED or zero!"
    #endif
#elif (DEH_MAX_NUMBER_ADD_FF > 0)
   #error "ERROR :: Additional freeze frames are not defined BUT the number of DEH_MAX_NUMBER_ADD_FF is not zero!"
#elif (!(defined(DEH_MAX_NUMBER_ADD_FF)))
  #error "ERROR :: Additional freeze frames has to be DEFINED with value zero!"
#elif ((defined(DEH_SUP_UDS)) && (DEH_MAX_LEN_ADD_FF > 0))
  #error "ERROR :: Additional freeze frames are not defined BUT the value of DEH_MAX_LEN_ADD_FF is not zero!"
#elif ((defined(DEH_SUP_UDS)) && (!(defined(DEH_MAX_LEN_ADD_FF))))
  #error "ERROR :: The maximum length of additional freeze frames has to be DEFINED with value zero!"
#endif
#if ((defined(DEH_SUP_KWP2000)) && (defined(DEH_MAX_LEN_ADD_FF)))
  #error "ERROR :: The length of additional freeze frames can not be defined in KWP2000 variant!"
#endif

    /* -------------------------------------------------------------------- */
    /* Maximum numbers  of stored freeze frames. This number is the summary */
    /* of Standard freeze frames and further additional freeze frame.       */
    /* -------------------------------------------------------------------- */

#ifdef DEH_READ_STD_FF

    #ifdef DEH_READ_ADD_FF
      #ifdef  DEH_MAX_NUMBER_ADD_FF
        #define DEH_MAX_NUMBER_FF  (DEH_MAX_NUMBER_ADD_FF+1)
      #endif
    #else
       #define DEH_MAX_NUMBER_FF 1
    #endif 

#else

    #if (DEH_MAX_NUMBER_ADD_FF > 0)
       #define DEH_MAX_NUMBER_FF  DEH_MAX_NUMBER_ADD_FF
    #endif

#endif

    /* --------------------------------------------------------------------- */
    /* constants for no standard freeze frames available                     */
    /* two possible reasons :                                                */
    /* I)  read standard FF is not configured                                */
    /* II) for this event is no FF stored (FF buffer was full)               */
    /* --------------------------------------------------------------------- */
#define DEH_STDFF_KM_NOT_AVAILABLE   0xFF
#define DEH_STDFF_TIME_NOT_AVAILABLE 0x00

    /* --------------------------------------------------------------------- */
    /* constants for additional freeze frames available                      */
    /* two possible reasons :                                                */
    /* I)  read additional FF is not configured                                 */
    /* II) for this event is no FF stored (FF buffer was full)               */
    /* --------------------------------------------------------------------- */
#define DEH_FFBUF_INIT_NORMV     ((uint8_t)   0)
#define DEH_FFBUF_INIT_FORM      ((uint8_t)  37)
#define DEH_FFBUF_INIT_MEAS      ((uint8_t) 115)

    /* -------------------------------------------------------------------- */
    /* Define for lines in Freeze Frame Table                               */
    /* Every line has a different combination of Freeze Frames to storage.  */
    /* Every event has a parameter to address the freeze frame table.       */
    /* -------------------------------------------------------------------- */
#define DEH_MAX_NUMBER_FF_IDX 2

    /* -------------------------------------------------------------------- */
    /* flag to enable the function to report all state changes.             */
    /* If this flag enabled then would be called the function               */
    /* APL_DEH_ReportChgEventState (eventNumber_uT; newState )              */
    /* default is this function disable                                     */
    /* -------------------------------------------------------------------- */
//#define DEH_SUP_DETECT_CHANGE_EV_STATE 

    /* -------------------------------------------------------------------- */
    /* Buffer size for temporary storage of freeze frames. If the system    */
    /* should be stored freeze frames this parameter should be minimum      */
    /* one entry.                                                           */
    /* The freeze frame buffer stored the standard freeze frames (8 Bytes ) */
    /* and all needed additional freeze frames ( 3 Byte per Add. FF).       */
    /* This buffer is needed to catch the freeze frames after the first     */
    /* occurance of a event (fault) until this event is stored.             */
    /* Is the event not stored then release the recerve buffer              */
    /* -------------------------------------------------------------------- */
#define DEH_FF_BUFFER_SIZE 5

  /* --------------------------------------------------------------------- */
  /* period definition for Update Event Memory                             */
  /* --------------------------------------------------------------------- */
#define DEH_EVENT_MEM_UPD_PERIOD 10 /* -> with task period 10 ms => 100 ms */

  /* --------------------------------------------------------------------- */
  /* period definition for read Freeze Frames                              */
  /* --------------------------------------------------------------------- */
#define DEH_READ_FF_PERIOD 15 /* -> with task period 10 ms => 150 ms */

  /* --------------------------------------------------------------------- */
  /* period definition for analze the warning lamp state and CAN-Event Bit */
  /* --------------------------------------------------------------------- */
#define DEH_CANBIT_WARNLAMP_PERIOD 100 /* -> with task period 10 ms => 1 sec.*/

  /* --------------------------------------------------------------------- */
  /* period definition for wait after Unlearn Algorithm got an BUSSY from  */
  /* function APL_DEH_EEPWriteEventData (normaly EEPROM access)            */
  /* --------------------------------------------------------------------- */
#define DEH_WAIT_UNLEARN_PERIOD 20 /* -> with task period 10 ms => 200 msec.*/

  /* --------------------------------------------------------------------- */
  /* max numbers of possible unlearn cycles after receiving a new unlearn  */
  /* counter (default is 5)                                                */
  /* --------------------------------------------------------------------- */
#define DEH_MAX_NUM_OF_UC_CYCLES 10

  /* ======================================================================= */
  /* definition for memory block to store the status of DTC infos            */
  /* ======================================================================= */


#if ( ((DEH_MAX_NUMBER_OF_EVENTPATHS/8)*8) == DEH_MAX_NUMBER_OF_EVENTPATHS )
  #define STOFDTC_ONE_BLOCK_SIZE (DEH_MAX_NUMBER_OF_EVENTPATHS/8)
#else 
  #define STOFDTC_ONE_BLOCK_SIZE ((DEH_MAX_NUMBER_OF_EVENTPATHS/8)+1)
#endif 

#if ((defined(DEH_SUP_UDS)) && (defined(DEH_SUP_UDS_OBD_STATUS)))
  #define STOFDTC_BLOCK_SIZE            ((uint16_t)(STOFDTC_ONE_BLOCK_SIZE*4))
  #define  STOFDTC_POS_TEST_NC          ((uint16_t)  0)
  #define  STOFDTC_POS_TEST_FLS         ((uint16_t)  STOFDTC_ONE_BLOCK_SIZE)
  #define  STOFDTC_POS_PENDING_DTC      ((uint16_t) (STOFDTC_ONE_BLOCK_SIZE*2))
  #define  STOFDTC_POS_TEST_FAILED_SLC  ((uint16_t) (STOFDTC_ONE_BLOCK_SIZE*3))

#elif ((defined(DEH_SUP_UDS)) && (!(defined(DEH_SUP_UDS_OBD_STATUS))))
  #define  STOFDTC_BLOCK_SIZE           ((uint16_t) (STOFDTC_ONE_BLOCK_SIZE*2))
  #define  STOFDTC_POS_TEST_NC          ((uint16_t)  0)
  #define  STOFDTC_POS_TEST_FLS         ((uint16_t)  STOFDTC_ONE_BLOCK_SIZE)

#elif (defined(DEH_SUP_KWP2000))
  #define  STOFDTC_BLOCK_SIZE           ((uint16_t) (STOFDTC_ONE_BLOCK_SIZE))
  #define  STOFDTC_POS_TEST_NC          ((uint16_t)  0)
#endif

#ifdef DEH_SUP_UDS
  #define  DEH_MAX_EXT_REC_BUFFER_DATA ((uint8_t) (DEH_MAX_LEN_ADD_FF+2))
#endif

#define  STOFDTC_STORAGE_BLOCK_SIZE  ((uint16_t) (STOFDTC_BLOCK_SIZE+1))
#define  STOFDTC_POS_STATUS_BYTE     ((uint16_t) (STOFDTC_BLOCK_SIZE))

#define BYTE_POS(event_number)       ((event_number==0) ?((uint8_t) 0 ):((uint8_t) (event_number/8)))
#define BIT_POS(event_number)        ((event_number==0) ?((uint8_t) 0 ):((uint8_t) (event_number%8)))



/*==========================================================================*/
/* Typedefs                                                                 */
/*==========================================================================*/

  /* Struct definition for additional Freeze Frame */
#ifdef DEH_SUP_KWP2000
typedef struct 
{ 
  uint8_t formula;
  uint8_t normValue;
  uint8_t measurement;
} tDEH_AddFreezeFrame;
#endif

typedef struct
{
    uint8_t StdFF[8];

} tDEH_StdFreezeFrame;

#ifdef DEH_SUP_KWP2000
  /* type definition for Freeze Frame Controller to store FF*/
typedef struct
{
    uint16_t            EventNumber;
    tDEH_StdFreezeFrame StdFFrames;

#ifdef DEH_READ_ADD_FF
      tDEH_AddFreezeFrame  AddFFrame[DEH_MAX_NUMBER_ADD_FF];
#endif

} tDEH_FFBuffer;
#endif


#ifdef DEH_SUP_UDS
typedef struct
{
    uint16_t            EventNumber;
    tDEH_StdFreezeFrame StdFFrames;
#ifdef DEH_READ_ADD_FF
    uint8_t             ExtDataRec_u8[DEH_MAX_NUMBER_ADD_FF][DEH_MAX_EXT_REC_BUFFER_DATA];
#endif
} tDEH_FFBuffer;
#endif

/*==========================================================================*/
/* Global Variables                                                         */
/*==========================================================================*/

/*==========================================================================*/
/* Global  Prototypes                                                       */
/*==========================================================================*/

/*==========================================================================*/

  /* avoid double INCLUDES */
#endif 


