/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     OSEK_AppTask_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_OSEK
*-----------------------------------------------------------------------------
* Description:    file to provide the Init and Task body for the 
*                 AppTask_event_5ms.
*-----------------------------------------------------------------------------
* $Date: 2011-04-26 14:04:38 +0200 (Di, 26 Apr 2011) $
* $Rev: 19307 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/src/OSEK_AppTask_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "OSEK_AppTask_H1.h"
#include "APP_PHONE.h"
#include "APP_PHONE_BAP.h"
#include "APP_SMS.h"
#include "APP_PHONEBOOK.h"
#include "APP_PHONEBOOK_BAP.h"
#include "VCR.h"
#include "KPB.h"
#include "APP_SD.h"
#include "APP_CSHDL.h"
#include "APP_DIAG.h"
#include "ERM.h"
#include "APP_MDI_BAP.h"
#include "OSEK_AppTask_Alarm.h"
#include "APP_COMMON_AT_Dispatcher.h"
#include "APP_DEVICE_AudioManager.h"
#include "CONTEXT_MAN.h"
#include "OSEK_AppTask_C1.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType EEAppTaskControl;
/*----------------------------------------------------------------------------*/
/* Global const Definitions                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of private Methods                                              */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType* getEEAppTaskControl(void);
static void AppTask_vHandleCanEvents(EventMaskType event);
static void AppTask_vHandleEvent(EventMaskType event);

/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/
#define CTA_APP_CYCLIC_TIME 100
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : getEEAppTaskControl                                          */
/** 
    \brief      Description:
                Register the adress on the structure for getEEAppTaskControl
                for the OSEK_AppTask task

    \return     EEDeviceControlBlockType*
   */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType* getEEAppTaskControl(void)
{
    return &EEAppTaskControl;
}

/*----------------------------------------------------------------------------*/
/* Function    : InitAppTask                                                  */
/** 
    \brief      Callback function for initialization of OsAppTask_event_100ms
  
    \brief      Description:
                Interface function to initialize the OsAppTask_event_100ms, this
				function will be only called one time during first activation 
				of OSAppTask_event_5ms 

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType InitAppTask(void)
{
    SuccessFailType sF = SUCCESS;

    getEEAppTaskControl()->priority = EEPromHIGH_PRIORITY;
    (void)EEPROM_sfInit(getEEAppTaskControl());

    _AppTask_vInitAlarm();
    
    /* init applications */
    (void)APP_PHONE_BAP_sfInitApp();
    (void)APP_PHONE_sfInitApp();

    (void)APP_SMS_sfInitApp();
    (void)APP_PHONEBOOK_sfInitApp();
    (void)APP_PHONEBOOK_BAP_sfInitApp();
    (void)APP_DEVICE_sfInitApp();
    
    (void)APP_MDI_BAP_sfInitApp();
    (void)APP_MDI_sfInitApp();
    
    (void)VCR_sfInitApp();
    (void)KPB_sfInitApp();
    
    (void)APP_SD_sfInitApp();
    
    (void)APP_CSHDL_sfInitApp();
    
    (void)LOGI_BUT_MAN_vInit3BM();
    (void)LOGI_BUT_MAN_vInitBaseplateAndAntenna();
    /* init ERM, startup uart */
    sF = ERM_Startup();

    
    (void)SetRelAlarm(CTA_APP, MSEC(CTA_APP_CYCLIC_TIME), 0);
  
    return(sF);
}

/*----------------------------------------------------------------------------*/
/* Function    :    AppTask_vHandleCanEvents                           */
/** 
    \brief      Function for SpeedManager event in application task
      
    \brief      Description:
                Function wait of an incomming event and receive the messsage.
                Check if the massge includes language selection or speed.

    \param      event
                EventMaskType
                evAppTaskPhonebookSpeedBoarder
    
    \return     void

   */
/*----------------------------------------------------------------------------*/
void AppTask_vHandleCanEvents(EventMaskType event)
{
    // check for SpeedManager event used by sd and XX
    if( event & evAppTaskCanStates)
    {
        APP_CAN_MsgStates canState;
        (void)memset(&canState,0x00,sizeof(APP_CAN_MsgStates));
        // read Message
        (void)ReceiveMessage( msgAppCanStates , &canState);
        // now check who should handle the event
        switch ( canState.common )
        { 
            case SD_LANGUAGE:

                DEBUG_VALUE1(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "AppTask: new CAN lang %d",
                             canState.sd_lang.sd_language);
                // Notify APP_SD and VCR about the change of the language on CAN bus
                (void)APP_SD_vOnAppCanLangNotification(canState.sd_lang);
                // needs to be handled here and not via sd, cause if no sd is available, the language change
                // via Can would never have an effect to VCR 
                (void)VCR_sFChangeLanguageViaCan(canState.sd_lang.sd_language);
                break;
        
            case SPEED:
                (void)APP_SD_vOnAppCanSpeedNotification(canState.speed.speed);
                if (canState.speed.speed == overSpeed)
                {
                    ERM_BTVisibilityBarringOn(TRUE);
                }
                else /* if (speedState.speed.speed == underSpeed) */
                {
                    ERM_BTVisibilityBarringOn(FALSE);
                }
                DEBUG_VALUE1(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "AppTask: new CAN speed %d", 
                             canState.speed.speed);
                break;
            
            case MARKE:        
                if (APP_COMMON_RSP_OK == APP_DEVICE_rcBackupEeprom(NULL, canState.marke.marke))
                {
                    DEBUG_VALUE1(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "AppTask: new CAN marke %d",
                                 canState.marke.marke);
                }
                break;

            case RADIO:
                DEBUG_VALUE4(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "AppTask: Radio %d ACK received %d HU OnOff %d HU con %c", 
                             canState.common,
                             canState.radio.mute, 
                             canState.radio.onOff,
                             ' ');
                (void)APP_DEVICE_AM_vRadioInfoReceived(canState.radio);
                break;

            default:        
                DEBUG_VALUE1(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "AppTask: ign. %d", 
                             canState.common);
                break;
        }
        // msg queue is unqueued so always just clear event, cause msg queue
        // will never return that it is empty !!!!!! 
        if( E_COM_NOMSG == GetMessageStatus(msgAppCanStates))
        {
            // clear only if all msg received
            (void)ClearEvent(evAppTaskCanStates);
        }
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : AppTask_vHandleEvent                                         */
/** 
    \brief      Handles evActivateAppTAppTask_event_100msnt

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
   */
/*----------------------------------------------------------------------------*/
void AppTask_vHandleEvent(EventMaskType event)
{
    if( event & evActivateAppTask_event_100ms )
    {
	    (void)ClearEvent(evActivateAppTask_event_100ms);
        _AppTask_vHandleAlarm();
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : AppTask_event_100ms                                            */
/** 
    \brief      Callback function for OsAppTask_event_100ms
  
    \brief      Description:
                Interface function from the OsAppTask_event_100ms this function
				will be activated by event and can be restarted by the task
				self with an static alarm (CTA_APP) which sets an event.
				The application functions are responsible to set an absolute alarm
				( not cyclic alarm ). The alarm should be newly set everytime the Application
				needs it.

    \return     void
   */
/*----------------------------------------------------------------------------*/
void AppTask_event_100ms(void)
{
    EventMaskType events;
    // cyclically called every CTA_APP_CYCLIC_TIME ms
	(void)WaitEvent(  evActivateAppTask_event_100ms         |
                /* events from BAP Dispatcher to Apps */
                evAppTaskPhoneBapInd                |
                evAppTaskPhoneBookBapInd            |
                evAppTaskMdiBapInd                  |
                evAppTaskKpbBapInd                  |
                /* events indicating pending fcts */
                evAppTaskPhoneFctPending            |
                evAppTaskPhoneBookFctPending        |
                evAppTaskMdiFctPending              |
                evAppTaskKpbPending                 |
                evAppTaskDeviceFctPending           |
                /* event from driverTask to Apps */
                evLogiButMan_ButtonInfo             |
                evAppTaskERMBTVisibleTimer          |
                evAppTaskERMPending                 |
                evAppTaskCanStates                  |
                evAppTaskVCRPending                 |
                evAppTaskVCRSupervisor              |
                evAppTaskSmsFctPending              |
                evAppTaskDiagnoseRequest            |
                evAppTaskPhoneBookBapFctPending     |
                evAppTaskSdFctPending               |
                evAppTaskMFLKeys                    |
                evAppTaskSystemStates               |
                evAppTaskApplATResponse             |
                evAppTaskPhoneBapRequestPending     |
                evAppTaskContaxtManHandleCalls		|
				evAppTaskKpbSupervision
                );
    

    (void)GetEvent(OsAppTask_event_100ms, &events);

    AppTask_vHandleEvent(events);

    AppTask_vHandleCanEvents(events);

    APP_COMMON_vHandleEvents(events);
    APP_PHONE_vHandleEvents(events);
    APP_PHONE_BAP_vHandleEvents(events);
    
    APP_SMS_vHandleEvents(events);
    APP_PHONEBOOK_vHandleEvents(events);
    APP_PHONEBOOK_BAP_vHandleEvents(events);

    APP_MDI_vHandleEvents(events);
    APP_MDI_BAP_vHandleEvents(events);
    APP_SD_vHandleEvents(events);
    APP_DEVICE_vHandleEvents(events);

    APP_DIAG_vHandleEvents(events);
    
    ERM_Manager(events);
    VCR_vHandleEvents(events);

    /* call application 3BM, Cradle, MFL */
    LOGI_BUT_MAN_vHandleEvents(events);

    CONTEXT_MAN_vHandleEvents(events);
    LOGI_BUT_MAN_vRollRadDebouncingFunction(events);
    KPB_vHandleEvents(events);
    
    APP_CSHDL_vHandleEvents(events);
   
    return;
}

/* End Of File OSEK_AppTask_C1.c */
