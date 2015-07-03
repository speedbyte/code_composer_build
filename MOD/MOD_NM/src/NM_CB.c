/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     NM_CB.c
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2009-12-10 15:37:28 +0100 (Do, 10 Dez 2009) $
* $Rev: 10242 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_CALLBACK/trunk/src/NM_CB.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "can_inc.h"
#include "UHV_NAR.h"
#include "n_vnmdef.h"
#include "NM_CB_cfg.h"
#include "GIO.h"
#include "deh.h"
#include "DEH_DTC_CFG.h"
#include "Port.h"
#include "ADC.h"
#include "APP_CAN.h"
#include "uds.h"
#include "CAN_CB.h"
#include "spi.h"
#include "TRACE.h"
#include "NM_CB.id"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
static void _NM_CB_vCheckUnderVoltageIgnitionImPulse(void);

static void _NM_CB_vLookForCanTranceiverStates();

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/
#define HANDLEBUSOFFWAITINGTIMECOUNTERTIME      50  /* 500ms */
/** 
\brief	description of the private constant
*/
#define NM_LIMPHOME_COUNTER                     395     /* Limphome counter 395 -> 395*10ms=3,95s for shutdown*/
/** 
\brief	description of the private constant
*/
#define LIMPHOMEFAILURESAVETIME                 192     /* 2 sec */

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/
static Uint8Type     canActive      = FALSE;
/** 
\brief	description of the private variable
*/
static Uint8Type     nmBusOff       = FALSE;
/** 
\brief	description of the private variable
*/
static Uint16Type    aLimphomeCntr  = NM_LIMPHOME_COUNTER;
/** 
\brief	description of the private variable
*/
static Uint8Type    StartHandleBusOffWaitingTimeCounter     = FALSE;
/** 
\brief	description of the private variable
*/
static Uint16Type   HandleBusOffWaitingTimeCounter          = 0;  /* 500ms */
/** 
\brief	description of the private variable
*/
static Uint8Type    busOffCounter                           = TRUE;
/**
\brief	description of the private variable
*/
static Uint8Type _NM_CB_u8UnderVoltageIgnitionImPulse = FALSE;
/**
\brief	description of the private variable
*/
static Uint8Type NM_BusOff_DTC_Waiting_Time_Weichekommunikation = TRUE;


/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Function    : ApplNmWriteUserData                                          */
/** 
    \brief      Callback function for network management
  
    \brief      Description:
                function to write the user data in the Network
                management message (ring)

    \param      data
                NM_CHANNEL_APPLTYPE_FIRST vuint8*
    \return     void
   */
/*----------------------------------------------------------------------------*/
void NM_API_CALLBACK_TYPE ApplNmWriteUserData(NM_CHANNEL_APPLTYPE_FIRST vuint8* data)
{   
    Uint8Type nmDiagSession     = (Uint8Type)UDS_ubGetSession();
    Uint8Type nmKlemme15        = (Uint8Type)APP_CAN_u8Klemme15Status();
    Uint8Type nmCurrentScope    = (Uint8Type)Pwrmgr_GetCurrentScopeEigenDiag();
    
    tDEH_EventBufferState eventStateRadio                   = DEH_EventBuffer[DTC_000016_SG_RADIO_KEINSIGNAL];
    tDEH_EventBufferState eventStateBodyComputer            = DEH_EventBuffer[DTC_C14000_BODYCOMPUTER1_KEINEKOMMUNIKATION];
    tDEH_EventBufferState eventStateLenksaeulenElektronik   = DEH_EventBuffer[DTC_C21200_SG_LENKSAEULENELEKTRONIK_KEINEKOMMUNIKATION];
    tDEH_EventBufferState eventStateUnplausibleSignal       = DEH_EventBuffer[DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL];
    tDEH_EventBufferState eventStateKomfort                 = DEH_EventBuffer[DTC_000021_DATENBUSKOMFORT_DEFEKT];
    tDEH_EventBufferState eventStateInfotainment            = DEH_EventBuffer[DTC_C06400_INFOTAIMENTCAN_DEFEKT];

    /* set NW-Bytes 3-8 with default values                                 */
    data[0]= NM_SIGNAL_BYTE_DEFAULT;
    data[1]= NM_SIGNAL_BYTE_DEFAULT;
    data[2]= NM_SIGNAL_BYTE_DEFAULT;
    data[3]= NM_SIGNAL_BYTE_DEFAULT;
    data[4]= NM_SIGNAL_BYTE_DEFAULT;
    data[5]= NM_SIGNAL_BYTE_DEFAULT;

    /* NWM wakeup - Byte 3  */
    if(wakeupReason == NO_RESET)
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! Wake Up No Reset!!!!!")
    	data[0] = NM_WAKE_UP_NO_RESET;
    }
    else if(wakeupReason == KLEMME30_RESET)
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! Wake Up KL30 Reset!!!!!")
    	data[0] = NM_WAKE_UP_KLEMME_30_RESET_VALUE;
    }
    else if(wakeupReason == CAN_WAKE_UP)
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! Wake Up Can Wake!!!!!")
    	data[0] = NM_WAKE_UP_CAN_WAKE_UP;
    }

    /* NWM after-running reason - Byte4                                     */
    /* Byte4,Bit0 - Klemme15 on                                             */
    if(ON == nmKlemme15)
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! KL15 ON!!!!!")
    	data[1] |= NM_WAKE_UP_KLEMME_15_ON;
    }
    
    /* Byte4,Bit1 - Diagnose on with Klemme15 off                           */
    if((UDScub_DEFAULT_SESSION != nmDiagSession)&& (OFF == nmKlemme15))
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! DIAG ON KL15 Off!!!!!")
    	data[1] |= NM_DIAGNOSE_ON_WO_KL15;
    }
    
    /* Byte4,Bit2 - minimal active time for OSEK-NM                           */
    if(APP_CAN_SleepDelayTimer() > 0)
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! Sleep Delay Abgelaufen!!!!!")
    	data[1] |= NM_OSEKNM_MIN_ACTIVETIME;
    }
       
    /* NWM systeminfo - Byte 6                                              */
    /* Can-TimeOut Fehler - set bit 1                                       */
    if(((eventStateRadio & DEH_EVENT_ACTIVE)            == DEH_EVENT_ACTIVE)            ||
      ((eventStateRadio & DEH_EVENT_DEBOUNCE_ACTIVE)    == DEH_EVENT_DEBOUNCE_ACTIVE)   ||
      ((eventStateBodyComputer & DEH_EVENT_ACTIVE)           == DEH_EVENT_ACTIVE)            ||
      ((eventStateBodyComputer & DEH_EVENT_DEBOUNCE_ACTIVE)  == DEH_EVENT_DEBOUNCE_ACTIVE)   ||  
      ((eventStateLenksaeulenElektronik & DEH_EVENT_ACTIVE)         == DEH_EVENT_ACTIVE)            ||
      ((eventStateLenksaeulenElektronik & DEH_EVENT_DEBOUNCE_ACTIVE)== DEH_EVENT_DEBOUNCE_ACTIVE)   ||
      ((eventStateUnplausibleSignal & DEH_EVENT_ACTIVE)  == DEH_EVENT_ACTIVE)            ||
      ((eventStateUnplausibleSignal & DEH_EVENT_DEBOUNCE_ACTIVE) == DEH_EVENT_DEBOUNCE_ACTIVE) ||
      ((eventStateKomfort & DEH_EVENT_ACTIVE)  == DEH_EVENT_ACTIVE)            ||
      ((eventStateKomfort & DEH_EVENT_DEBOUNCE_ACTIVE) == DEH_EVENT_DEBOUNCE_ACTIVE) ||
      ((eventStateInfotainment & DEH_EVENT_ACTIVE)  == DEH_EVENT_ACTIVE)            ||
      ((eventStateInfotainment & DEH_EVENT_DEBOUNCE_ACTIVE) == DEH_EVENT_DEBOUNCE_ACTIVE))
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! ATleast one message is missing !!!!!")
    	data[3] |= NM_TIME_OUT_ERROR_STORED;
    }
    
    /* Can bezogene Eigendiagnose - set bit 2                               */
    if((FALSE == nmCurrentScope) || (FALSE == APP_CAN_u8GetNWDF()))
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! No Eigen Diag allowed !!!!!")
    	data[3] |= NM_SELF_DIAG_DEACTIVATED;
    }    

    /* Eindrahtbetrieb Can  - set bit 7                                     */
    if(LOW == GIO_u8PinRead(PORT_CAN_ERROR, GIOA))
    {
    	DEBUG_TEXT(NM_SWI_DBG, MOD_NM, " !!! One Line CAN betrieb!!!!!")
    	data[3] |= NM_ONE_WIRE_MODE;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : ApplNmCanNormal                                              */
/** 
    \brief      Callback function for network management
  
    \brief      Description:
                set the transceiver in the normal mode
                wake up the CAN driver

    \return     void
   */
/*----------------------------------------------------------------------------*/
void NM_API_CALLBACK_TYPE ApplNmCanNormal(NM_CHANNEL_APPLTYPE_ONLY)
{
    /* Signal internal that the CAN is now in active state */ 
    canActive = TRUE;
    
#ifdef NM_SUPPORT_CAN_TRANSCEIVER
	/* switch the CAN transceiver to normal operation */
	NM_CAN_TRANSCEIVER_NORMAL();
#endif

#ifdef NM_SUPPORT_CAN
	/* set CAN to normal operation mode */
	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!!ApplNmCanNormal : Switch CAN to Wake up!!!!!")
	NM_CAN_WAKE_UP();
#endif
}/* END OF ApplNmCanNormal() */


/*----------------------------------------------------------------------------*/
/* Function    : ApplNmCanSleep                                               */
/** 
    \brief      Callback function for network management
  
    \brief      Description:
                set the transceiver to standby mode
                stop the CAN driver

    \return     void
   */
/*----------------------------------------------------------------------------*/
void NM_API_CALLBACK_TYPE ApplNmCanSleep(NM_CHANNEL_APPLTYPE_ONLY)
{   
#ifdef NM_SUPPORT_CAN
	Uint8Type returnCode = FALSE;

    /* set CAN to stop operation mode */
	returnCode = NM_CAN_STOP();

    if(returnCode == kCanFailed)
    {
        /* Signal internal that the CAN is now in active state */ 
    	DEBUG_TEXT(NM_SWI_ERR, MOD_NM, " !!! ERROR : Switch CAN to StandBy!!!!!")
    	canActive = TRUE;
    }
    else
    {
        /* Signal internal that the CAN is now switched off */ 
        canActive = FALSE;
#ifdef NM_SUPPORT_CAN_TRANSCEIVER
    	/* switch the CAN transceiver to standby mode */
        DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! Switch CAN to StandBy!!!!!")
        NM_CAN_TRANSCEIVER_STANDBY();
#endif
    }
#endif
}/* END OF ApplNmCanSleep() */


/*----------------------------------------------------------------------------*/
/* Function    : ApplNmCanBusSleep                                            */
/** 
    \brief      Callback function for network management
  
    \brief      Description:
                set the transceiver to sleep mode

    \return     void
   */
/*----------------------------------------------------------------------------*/
void NM_API_CALLBACK_TYPE ApplNmCanBusSleep(NM_CHANNEL_APPLTYPE_ONLY)
{   
#ifdef NM_SUPPORT_CAN
    if(canActive == TRUE)
    {
        /* Signal internal that the CAN is now switched off */ 
        canActive = FALSE;
        DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplNmCanBusSleep : Wake UP CAN !!!!!")
        GotoMode(Awake);
    }
    else
    {
#ifdef NM_SUPPORT_CAN_TRANSCEIVER
    	/* switch the CAN transceiver to sleep mode */
    	NM_CAN_TRANSCEIVER_SLEEP();
#endif
        /* switch the CAN AMP OFF (with ShutdownEvent) */
        
        /* Set event to System Task to switch Power supply off */
    	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplNmCanBusSleep : Shut CAN !!!!!")
    	(void)SetEvent(OsSystemTask_event, evSystemTaskShutdown);
        
    }
#endif
}/* END OF ApplNmCanBusSleep() */


/*----------------------------------------------------------------------------*/
/* Function    : ApplNmBusOffEnd                                              */
/** 
    \brief      Callback function for network management
  
    \brief      Description:
                function to request the end from the BUSOFF state

    \return     void
   */
/*----------------------------------------------------------------------------*/
void NM_API_CALLBACK_TYPE ApplNmBusOffEnd(NM_CHANNEL_APPLTYPE_ONLY)
{   
    /* Signal the NM Application that the Bus state is on */
	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplNmBusOffEnd !!!!!")
	nmBusOff = FALSE;
}/* END OF ApplNmBusOffEnd() */


/*----------------------------------------------------------------------------*/
/* Function    : ApplNmBusOff                                                 */
/** 
    \brief      Callback function for network management
  
    \brief      Description:
                function for reinitialize the CAN-driver

    \return     void
   */
/*----------------------------------------------------------------------------*/
void NM_API_CALLBACK_TYPE ApplNmBusOff(NM_CHANNEL_APPLTYPE_ONLY)
{   
    tDEH_Return retVal_u8 = DEH_OK;
    
    /* Signal the NM Application that the Bus state is on */
    nmBusOff = TRUE;

    /* DEBUG_TEXT(NM_SWI_ERR, MOD_NM, " !!! BUS OFF Error !!!!!") - Print not work */
    _NM_CB_vCheckUnderVoltageIgnitionImPulse();
    CanResetBusOffStart(kNmCanPara);
    StartHandleBusOffWaitingTimeCounter=TRUE; 	
    HandleBusOffWaitingTimeCounter=0;

    if(  TRUE == APP_CAN_u8EigenDiagDTCStore() && \
    	(FALSE == _NM_CB_u8UnderVoltageIgnitionImPulse)&& \
    	(FALSE == NM_BusOff_DTC_Waiting_Time_Weichekommunikation) )
    {
        if (b_SY1_Infotainment_b == TRUE)        
        { 
            retVal_u8 = DEH_AdditionalReportEvent(DTC_C06400_INFOTAIMENTCAN_DEFEKT,TRUE);      
        }
        else 
        {        
            retVal_u8 = DEH_AdditionalReportEvent(DTC_000021_DATENBUSKOMFORT_DEFEKT,TRUE);     
        } 
    }
    retVal_u8 = retVal_u8;
}/* END OF ApplNmBusOff() */


/*----------------------------------------------------------------------------*/
/* Function    : ApplCheckLimphome                                            */
/** 
    \brief      Callback function for test if NM is in limphome state
  
    \brief      Description:
                function for limphome handling

    \return     void
   */
/*----------------------------------------------------------------------------*/
void ApplCheckLimphome(void)
{
    tDEH_Return retVal_u8 = DEH_OK;
    StatusType NmState = 0;

    NmState = NmGetStatus();

    if(NmStateLimphome( NmState ) == 1)
    {
        if((aLimphomeCntr == LIMPHOMEFAILURESAVETIME) && \
           (StartHandleBusOffWaitingTimeCounter == FALSE) && \
           (FALSE == _NM_CB_u8UnderVoltageIgnitionImPulse)&& \
           (FALSE == NM_BusOff_DTC_Waiting_Time_Weichekommunikation) && \
           (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            if (b_SY1_Infotainment_b == TRUE)
            {
                retVal_u8 = DEH_AdditionalReportEvent(DTC_C06500_INFOTAIMENTCAN_KEINEKOMMUNIKATION,TRUE);
            }
            else
            {
                retVal_u8 = DEH_AdditionalReportEvent(DTC_000022_DATENBUSKOMFORT_KEINEKOMMUNIKATION,TRUE);
            }
            aLimphomeCntr--;
        }
        else if((aLimphomeCntr == 0) && (b_BS3_Klemme_15_b == OFF))
        {
        	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! Bus Sleep in Limp Home !!!!!")
        	GotoMode(BusSleep);
            aLimphomeCntr = NM_LIMPHOME_COUNTER;
        }
        else if(aLimphomeCntr == 0)
        {
            aLimphomeCntr = NM_LIMPHOME_COUNTER;
        }
        else
        {
            aLimphomeCntr--;
        }
    }
    else
    {
        aLimphomeCntr = NM_LIMPHOME_COUNTER;

        if(busOffCounter == TRUE)   /* bug fix for AUDI KPM Nr.4662820 */
        {
        	if (b_SY1_Infotainment_b == TRUE)
            {
                retVal_u8 = DEH_AdditionalReportEvent(DTC_C06400_INFOTAIMENTCAN_DEFEKT,FALSE);
            }
            else
            {
                retVal_u8 = DEH_AdditionalReportEvent(DTC_000021_DATENBUSKOMFORT_DEFEKT,FALSE);
            }
            busOffCounter = FALSE;
            _NM_CB_u8UnderVoltageIgnitionImPulse = FALSE;
        }
        
        if (b_SY1_Infotainment_b == TRUE)
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_C06500_INFOTAIMENTCAN_KEINEKOMMUNIKATION,FALSE);
        }
        else
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_000022_DATENBUSKOMFORT_KEINEKOMMUNIKATION,FALSE);
        }
    }

    /* BusOFF detected */
    if(NmStateBusError( NmState ) == 1)
    {
    	/* UnderVoltageIgnitionImPulse */
    	_NM_CB_vCheckUnderVoltageIgnitionImPulse();
    }

    _NM_CB_vLookForCanTranceiverStates();

    retVal_u8 = retVal_u8;
}/* END OF ApplCheckLimphome() */

/*----------------------------------------------------------------------------*/
/* Function    : _NM_CB_vLookForCanTranceiverStates                           */
/**
    \brief      Look for transceiver state CAN_INHIBIT in normal Can transceiver mode

    \brief      Description:
                function look for transceiver state CAN_INHIBIT in normal
                Can transceiver mode . If a LOW is detected this is a
                hint that can transceiver has detect under voltage lower 2,5V.
                Necessary to get information about quick under voltage pulse not
                detectable over normal ADC UBatt handling. All this is necessary
                because we detect a wrong Busoff (Error flag set on Pin ERR_ at Can Transceiver)
                error in case of quick under voltage pulses.
                For Can transceiver mode NORMAL Mode READ_CANTRANSCEIVER_EN
                and READ_CANTRANSCEIVER_STB have to be HIGH

    \return     void
   */
/*----------------------------------------------------------------------------*/
static void _NM_CB_vLookForCanTranceiverStates()
{
	static Uint16Type WatchMessageTimerDTCBusoffNotDetected  = 0;

	if((READ_CANTRANSCEIVER_INHIBIT == BT_FALSE) &&\
       (READ_CANTRANSCEIVER_EN == BT_TRUE) &&\
       (READ_CANTRANSCEIVER_STB == BT_TRUE))
    {
    	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, "INHIBIT is Low in normal Can transceiver mode")
        NM_BusOff_DTC_Waiting_Time_Weichekommunikation = TRUE;
    	WatchMessageTimerDTCBusoffNotDetected  = 0;
    }

	if(WatchMessageTimerDTCBusoffNotDetected < WEICHKOMCTR)
	{
		WatchMessageTimerDTCBusoffNotDetected++;
	}
	else
	{
		if (TRUE == NM_BusOff_DTC_Waiting_Time_Weichekommunikation)
		{
			DEBUG_TEXT(NM_SWI_TRC, MOD_NM, "End INHIBIT BusOff DTC Waiting Time")
		}
		NM_BusOff_DTC_Waiting_Time_Weichekommunikation = FALSE;
	}

}

/*----------------------------------------------------------------------------*/
/* Function    : ApplHandleBusOffWaitingTime                                  */
/** 
    \brief      Callback function for handling the bus off waiting time
  
    \brief      Description:
                function for bus off handling

    \return     void
   */
/*----------------------------------------------------------------------------*/
void ApplHandleBusOffWaitingTime(void)
{
    if (StartHandleBusOffWaitingTimeCounter == TRUE)
    {
        if (HandleBusOffWaitingTimeCounter == 0)
        {
        	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplHandleBusOffWaitingTime Starts!!!!!")
        }
    	HandleBusOffWaitingTimeCounter++;
        if (HandleBusOffWaitingTimeCounter >= HANDLEBUSOFFWAITINGTIMECOUNTERTIME)
        {
        	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplHandleBusOffWaitingTime Ends!!!!!")
        	busOffCounter = TRUE;
            HandleBusOffWaitingTimeCounter=0;
            StartHandleBusOffWaitingTimeCounter=FALSE;
            CanOnline();
        }
    }
}/* END OF ApplHandleBusOffWaitingTime() */

/*----------------------------------------------------------------------------*/
/* Function    : _NM_CB_vCheckUnderVoltageIgnitionImPulse                     */
/**
    \brief      Check UnderVoltage Ignition Impulse

    \brief      Description:
                function for check direct a undervoltage

    \return     void
   */
/*----------------------------------------------------------------------------*/
static void _NM_CB_vCheckUnderVoltageIgnitionImPulse(void)
{
	/* UnderVoltageIgnitionImPulse */
	if (BATTERY_VOLTAGE_EIGENDIAG_LOW_9_15 > ADC_U16GETADCVALUE(ADC_UBATT))
	{
		_NM_CB_u8UnderVoltageIgnitionImPulse = TRUE;
		DEBUG_VALUE1(NM_SWI_TRC, MOD_NM, "ADC DirektBatt = %d .Detect UnderVoltage in case of BusOff", ADC_U16GETADCVALUE(ADC_UBATT));
	}
	else
	{
		DEBUG_VALUE1(NM_SWI_DBG, MOD_NM, "ADC DirektBatt = %d in case of BusOff", ADC_U16GETADCVALUE(ADC_UBATT));
	}

	/*  s_NWDFdet */
	DEBUG_VALUE1(NM_SWI_DBG, MOD_NM, "NWDFdet = %d in case of BusOff", APP_CAN_u8GetNWDF());

}

/*----------------------------------------------------------------------------*/
/* Function    : IsBusOff                                                     */
/** 
    \brief      Callback function to get the bus off state
  
    \brief      Description:
                function to get the actual CAN bus state

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type IsBusOff(void)
{
    return(nmBusOff);
}/* END OF IsBusOff() */


/*----------------------------------------------------------------------------*/
/* Function    : IsCanActive                                                    */
/** 
    \brief      Callback function to get the CAN state
  
    \brief      Description:
                function to get the actual CAN driver state

    \return     Uint8Type
   */
/*----------------------------------------------------------------------------*/
Uint8Type IsCanActive(void)
{
    return(canActive);
}/* END OF IsCanActive() */

/****************************************************************************
| NAME:             ApplNmWaitBusSleep
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      dummy function for Network management
****************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmWaitBusSleep(NM_CHANNEL_APPLTYPE_ONLY)
{
	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplNmWaitBusSleep !!!!!")
}

/****************************************************************************
| NAME:             ApplNmWaitBusSleepCancel
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      dummy function for Network management
****************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmWaitBusSleepCancel(NM_CHANNEL_APPLTYPE_ONLY)
{
	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplNmWaitBusSleepCancel !!!!!")
	APP_CAN_ResetSleepDelayTimer();
    APP_CAN_sfReInitTOVariables();
    NmOsekInit(NM_NORMAL);
}

/****************************************************************************
| NAME:             ApplNmBusStart
| INPUT PARAMETERS: none
| RETURN VALUES:    none
| DESCRIPTION:      dummy function for Network management
****************************************************************************/
void NM_API_CALLBACK_TYPE ApplNmBusStart(NM_CHANNEL_APPLTYPE_ONLY)
{
	DEBUG_TEXT(NM_SWI_TRC, MOD_NM, " !!! ApplNmBusStart !!!!!")
}


/* End Of File NM_CB.c */
