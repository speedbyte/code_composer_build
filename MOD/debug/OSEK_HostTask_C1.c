/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     OSEK_HostTask_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MOD_OSEK
*-----------------------------------------------------------------------------
* Description:    file to provide the Init and Task body for the
*                 HostTask_10ms.
*-----------------------------------------------------------------------------
* $Date: 2011-04-28 16:10:47 +0200 (Do, 28 Apr 2011) $
* $Rev: 19380 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/src/OSEK_HostTask_C1.c $
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
#include "ISRMask_CB.h"
#include "SYS_INIT.h"
#include "OSEK_HostTask_H1.h"
#include "can_inc.h"
#include "n_vnmdef.h"
#include "NM_CB.h"
#include "tp.h"
#include "UDS.h"
#include "deh.h"
#include "deh_cfg.h"
#include "sds.h"
#include "APP_CAN.h"
#include "ASR.h"
#include "sds_ext.h"
#include "bap.h"
#include "FBL_Data.h"
#include "Version.h"
#include "OSEK_HostTask_C1.id"
#include "EEPROM_LAYOUT_CFG.h"
#include "TRACE.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Definitions                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType EEHostControl;
/*----------------------------------------------------------------------------*/
/* Prototypes of private Methods                                              */
/*----------------------------------------------------------------------------*/
//static void vHostTask_PrintStackUsage();
/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/
BooleanType BAPIsShutDown = BT_FALSE;
Uint8Type BusOffDebounceTimeCounter = 0;
/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : getEEHostControl                                             */
/**
    \brief      Register the adress on the structure for EEcontrol for the
                database task

    \return     EEDeviceControlBlockType*

   */
/*----------------------------------------------------------------------------*/
EEDeviceControlBlockType* getEEHostControl(void)
{
    return &EEHostControl;
}

/*----------------------------------------------------------------------------*/
/* Function    : InitHostTask                                                 */
/**
    \brief      Callback function for initialization of OsHostTask_10ms

    \brief      Description:
				Callback function to initialize the OsHostTask_10ms,
				this function will be only called one time during first
                activation of OsHostTask_10ms

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType InitHostTask(void)
{
    SuccessFailType sF = SUCCESS;
    tDEH_Return     retVal_u8 = DEH_OK;
    EEDeviceControlBlockType    *pEEHost;

    /* IMPORTANT: Init Peripherals first !!!! */
    /* initialize the Peripheral parts of the System. */
    sF = SYSINIT_sfPeripheralsInit();
    pEEHost = (EEDeviceControlBlockType *)getEEHostControl();
    getEEHostControl()->priority = EEPromHIGH_PRIORITY;

    sF = EEPROM_sfInit(pEEHost);

    /* initialize the Software parts of the System. */
    sF = SYSINIT_sfSwInit();

    DEBUG_VALUE4(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "*** Copyright 2010 S1nn GmbH & Co. KG *** UHV High Software-Version: %c%c%c%c",
                 SoftwareVersion[0], SoftwareVersion[1], SoftwareVersion[2], SoftwareVersion[3]);

    /* The entire RAM Mirror will be updated after calling EEPOM_vLayoutInit*/
    EEPROM_vLayoutInit();
    EEPROM_vLayoutCfgInit();

	/* activate OSEK-Tasks */
	(void)ActivateTask(OsAppTask_event_100ms);        /* activation for OsAppTask_event_100ms */
	(void)ActivateTask(OsDatabaseTask_event);   /* activation for DatabaseTask_event */
	(void)ActivateTask(OsSystemTask_event);         /* activation for SystemTask_event */

	/* do all BAP init jobs */
	if (BAP_DISP_Initialization()== FAIL)
	{
		sF = FAIL;
		/* todo: error handling                              */
		/* what is to do if BAP not comes up with the system */
	}

	/* initialize interrupt flag register */
	ApplInterruptFlagMask();

	/* update software versions if software was updated */
	(void)FBLDATA_sfUpdateAfterReflash();

	/* Read the unlearncounter from EEProm and Function for initializing the DTC EEPROM area once  */
	InitDEHFaultData();

	retVal_u8 = DEH_Init();
	if(retVal_u8 != DEH_OK)
	{
		DEH_InitEEPROM();
		retVal_u8 = DEH_Init();
	}

	/* call init for modul APPCan */
	(void)APP_CAN_sfInit(); /* todo:  integration */

	(void)ASR_sfInit();

	(void)SetRelAlarm(CTA_HOST_TASK_CYCLIC_CAN_TX_MSG, MSEC(10), 0);

    return(sF);
}


/*----------------------------------------------------------------------------*/
/* Function    : HostTask_10ms                                                */
/**
    \brief      Callback function for OsHostTask_10ms

    \brief      Description:
                Callback function from the OsHostTask_10ms this function will
				be cyclic called all 10ms

    \return     void
   */
/*----------------------------------------------------------------------------*/
void HostTask_10ms(void)
{
    EventMaskType events;
    /* wait for event ... */
	(void)WaitEvent(  evActivateHostTask_10ms |
                evHostTaskPhoneBapReq   |
                evHostTaskPhoneBookBapReq |
                evHostTaskMdiBapReq       |
                evHostTaskKpbBapReq       |
                evHostTaskTelefonStates    |
                evHostTaskDiagnoseResponse |
                evHostTaskCyclicTxMsg |
                evHostTaskASRCyclicTxMsg |
                evHostTaskASRStatus         |
                evHostTaskASRTrigger        |
                evHostTaskMflTimer          |
                evHostTaskOnOffInfoSleepInd 
            );


    (void)GetEvent(OsHostTask_10ms, &events);

    if( events & evActivateHostTask_10ms)
    {
#if defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBTESTMODE)  || defined(UHV_BLUETOOTH_ZERTIFIZIERUNG_PBFREQEX)|| defined(UHV_BLUETOOTH_NO_CAN)
        /*no networkmanagement force UHV to stay on */
#else
        NmTask();	                        /* call the networkmanagement Task all 10 ms    */
#endif

        ApplCheckLimphome();                /* call the limphome handling for NWM           */
        ApplHandleBusOffWaitingTime();      /* call the BusOff timing handling              */
        (void)APP_CAN_sfCheckBusOff();
        /* CAN part */
        /* call Check for speed border, MFL, Infotainment aus */
        (void)APP_CAN_sf10msCyclicCheckReceivedCANMessages();
        /* diagnostic part */
        UDS_vTask();                        /* call UDS                                     */
        TP_vTask();                         /* call ISO-TP                                  */
        SDSext_vTask();                        /* call SDS-UDS                                 */

        /* BAP-Stack part, first check if the bus is avaible */
        if(IsBusOff() == BT_FALSE)
        {
            if((BAPIsShutDown == BT_TRUE) && (BusOffDebounceTimeCounter > BUSOFF_DEBOUNCE_TIME))
            {
                DEBUG_TEXT(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "Start the BAP task again after BusOff ends");
                BAP_DISP_Start();                   /* restart the BAP task after BusOff ends      */
                BAP_DISP_RemoveAllQueueElements();  /* remove all of the elements in the ack queue */
                BAPIsShutDown = BT_FALSE;
                BusOffDebounceTimeCounter=0;
            }
            else if((BAPIsShutDown == BT_TRUE) && (BusOffDebounceTimeCounter <= BUSOFF_DEBOUNCE_TIME))
            {
                BusOffDebounceTimeCounter++;
            }
            else
            {
                BAP_Task();                         /* call BAP-Stack only when bus is active      */
            }
        }
        else
        {
            if(BAPIsShutDown == BT_FALSE)
            {
                DEBUG_TEXT(OSEK_INTERFACE_SWI_TRC, MOD_OSEK_INTERFACE, "Shut down the BAP task when BusOff");
                BAP_DISP_ShutDown();                /* shut down the task in case of BusOff error  */
                BAPIsShutDown = BT_TRUE;
            }

            BusOffDebounceTimeCounter=0;
         }

        DEH_ProcessDiagEventHandler();
        /* Outside handling for DEH EEprom write retry handling */
        APL_DEH_EEPWriteHandleRetries();

        (void)ClearEvent(evActivateHostTask_10ms);
    }

    APP_CAN_vHandleEvents(events);

    (void)APP_CAN_sfSendCyclicCanMessage(events);


    (void)ASR_sfSendCyclicASRMessage(events);

    /* call BAP-Dispatcher */
    BAP_DISP_HandleEvent(events);

   /* call diagnostic handler */
    (void)SDSext_DIAG_vHandleEvents(events);


    return;
}
/* End Of File OSEK_HostTask_C1.c */
