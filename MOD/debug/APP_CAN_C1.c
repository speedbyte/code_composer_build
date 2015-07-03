/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_CAN_C1.c
*-----------------------------------------------------------------------------
* Module Name:    APP_CAN 
*-----------------------------------------------------------------------------
* Description:    Handle CAN related doings for the applications
*-----------------------------------------------------------------------------
* $Date: 2011-05-13 09:39:35 +0200 (Fr, 13 Mai 2011) $
* $Rev: 19667 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_CAN/trunk/src/APP_CAN_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*---------------------------------------------------------------------------- */
#include "Common.h"
#include "APP_CAN.h"
#include "can_inc.h"
#include "UHV_NAR.h"
#include "n_vnmdef.h"
#include "NM_CB.h"
#include "ERM.h"
#include "deh.h"
#include "DEH_DTC_CFG.h"
#include "APP_CAN_Speed.h"
#include "MFL_MGR.h"
#include "APP_PHONE_CAN.h"
#include "UDS.h"
#include "sds_ext.h"
#include "CONTEXT_MAN.h"
#include "FBL_Data.h"
#include "CSHDL.h"
#include "APP_DEVICE_AudioManager.h"
#include "ADC.h"
#include "Pwrmgr.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "APP_CAN_C1.id"
#include "TRACE.h"
#include "TRACE.h"


/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/
typedef union
{
    Uint32Type  instr;
    struct
    {
        REG_32 phoneOn              :1;
        REG_32 muteUnmute           :1;
        REG_32 muteSource           :4;
        REG_32 callActive           :1;
        REG_32 ringActive           :1;
        REG_32 providerName         :12;
        REG_32 fieldStrength        :4;
        REG_32 fieldStrengthError   :1;
        REG_32 registrationStatus   :3;
        REG_32 btStatus             :1;
        REG_32                      :3;
     }bits;
}APP_CAN_CanTelephoneStatus;

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
static CanSignalMessageType    canSignalInfo;

MissedMessageType missedMessages;
Uint32Type   WatchMessageTimer;
APP_CAN_MsgStates canMsgStates;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/
#define SLEEPDELAY      430
#define TIMEOUT_NACHLAUFZEIT ((PREPARE_CK5050_TIMEOUT+UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF_TIMEOUT) / 10)
/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the type
*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfSetTelefonStates(void);
static SuccessFailType APP_CAN_sfGWKombi(void);
static SuccessFailType APP_CAN_sfMFLTasten(void);
static SuccessFailType APP_CAN_sfMFLTastenKon_1(void);
static SuccessFailType APP_CAN_sfBSG2(void);
static SuccessFailType APP_CAN_sfBSG3(void);
static SuccessFailType APP_CAN_sfRadio4(void);
static SuccessFailType APP_CAN_sfLSM1(void);
static SuccessFailType APP_CAN_sfSystemInfo1(void);
static SuccessFailType APP_CAN_sfSollverbauNeu(void);
static SuccessFailType APP_CAN_sfGateway3(void);
static SuccessFailType APP_CAN_sfBSGKombi(void);
static SuccessFailType APP_CAN_sfDiagnose_1(void);
static Uint8Type _APP_CAN_bGetCanEventBit(void);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
                              
/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
static APP_CAN_CanTelephoneStatus  canTelStatus;
static APP_CAN_ENUM_MARKE _APP_CAN_enMarke = APP_CAN_NoMarke;
static APP_CAN_ENUM_DERIVAT _APP_CAN_enDerivat = APP_CAN_NoDerivat;
static APP_CAN_ENUM_MFL_VARIANTE _APP_CAN_MflVariante = APP_CAN_MFL_NoVariante;
static Uint8Type s_NWDFdet = TRUE;
static Uint8Type Diagnose_1_FirstTime  = FALSE;
static LSMmessageType lsm;
static Uint16Type _u16ActualSpeed = 0U;
static Diagnose1MessageType diagnose_1;
static Uint8Type    _APP_CAN_u8KombiLanguage = 0xFF;
static Uint8Type    BSG3Timer;
static Uint8Type    eventZASState       = 0U;
static Uint16Type   SleepDelayTimer     = SLEEPDELAY;
static Uint16Type   NachlaufzeitTimer   = TIMEOUT_NACHLAUFZEIT;
static Uint8Type    kl15Store           = OFF;
static Uint8Type    Radio4Timer;
static Uint8Type    LsmTimer;
static Uint8Type    DIAG1Timer = 0;
static Uint8Type    SysInfoTimer;
static Uint8Type    GW3Timer;
static Uint8Type    GWKombiTimer;
static Uint8Type    VBNTimer;
static BooleanType  bOnOffInfotainment = BT_FALSE;
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfInit                                               */
/** 
    \brief      Initialization of  Mdul CAN-Application
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_CAN_sfInit(void)
{
    SuccessFailType sF = SUCCESS;
    
    canTelStatus.instr              = 0;
    canSignalInfo.instr             = 0;
	missedMessages.instr            = 0;
	WatchMessageTimer               = WEICHKOMCTR;
    Radio4Timer                     = 0U;
    LsmTimer                        = 0U; 
    SysInfoTimer                    = 0U; 
    GW3Timer                        = 0U; 
    GWKombiTimer                    = 0U; 
    VBNTimer                        = 0U; 
    BSG3Timer                       = 0U;
    DIAG1Timer                      = 0U;
    _APP_CAN_u8KombiLanguage        = 0xFF;
    bOnOffInfotainment              = BT_FALSE;


    (void)memset(&canMsgStates, 0, sizeof(APP_CAN_MsgStates));
    (void)memset(&canSignalInfo, 0, sizeof(canSignalInfo));
    
    SleepDelayTimer                 = SLEEPDELAY;       // #1231
    NachlaufzeitTimer               = TIMEOUT_NACHLAUFZEIT;
    
    //(void)EEPROM_enGetSingleByteFromRamMirror(&_APP_CAN_u8KombiLanguage, EEPROM_LAYOUT_ENUM_LastSaveLanguageSelection);
    //DEBUG_VALUE1(APP_CAN_SWI_TRC, MOD_APP_CAN, "sfInit LastSavedLanguageSelection %d", _APP_CAN_u8KombiLanguage);
    
    (void)EEPROM_enGetSingleByteFromRamMirror((Uint8Type *)&_APP_CAN_enMarke, EEPROM_LAYOUT_ENUM_LastSaveMarkeSelection);
	DEBUG_VALUE1(APP_CAN_SWI_TRC, MOD_APP_CAN, "sfInit LastSaveMarkeSelection %d", _APP_CAN_enMarke);
    // Marc: 10.09.2009
    // we have read the value successfully and are now able to compare an incoming 
    // SYS1 Marke indication with a valid value.
    switch(_APP_CAN_enMarke)
    {
        case APP_CAN_VW_Volkswagen: 
        case APP_CAN_AU_Audi:      
        case APP_CAN_SE_Seat:
        case APP_CAN_SK_Skoda:
        case APP_CAN_VN_VWNutzfzg:
        case APP_CAN_Bugatti:
        case APP_CAN_Lamborghini:
        case APP_CAN_Bentley:
        case APP_CAN_RollsRoyce:
        case APP_CAN_Ford:
        case APP_CAN_NoMarke: 
        case APP_CAN_Porsche:
            /* detect the value at the init state - inform application. */
            (void)CONTEXT_MAN_sfSetFahrzeugTyp(_APP_CAN_enMarke);
            break;
        default:
            _APP_CAN_enMarke = APP_CAN_NoMarke;
            break;
    }
    _APP_CAN_enDerivat = APP_CAN_NoDerivat;
	return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_vResetLanguageDatasetdownload                        */
/** 
    \brief      Reset _APP_CAN_u8KombiLanguage to default value
               
    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_CAN_vResetLanguageDatasetdownload(void)
{
    DEBUG_TEXT(APP_CAN_SWI_DBG, MOD_APP_CAN, "APP_CAN_vResetLanguageDatasetdownload to 0xFF");
    _APP_CAN_u8KombiLanguage = 0xFF;
}



/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfInit                                               */
/** 
    \brief      Re-Initialization of timeout CAN variables
               
    \return     void
*/
/*----------------------------------------------------------------------------*/
void APP_CAN_sfReInitTOVariables(void)
{
	missedMessages.instr            = 0;
	WatchMessageTimer               = WEICHKOMCTR;
    Radio4Timer                     = 0U;
    LsmTimer                        = 0U; 
    SysInfoTimer                    = 0U; 
    GW3Timer                        = 0U; 
    GWKombiTimer                    = 0U; 
    VBNTimer                        = 0U; 
    BSG3Timer                       = 0U;
    DIAG1Timer                      = 0U;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_vHandleEvents                                        */
/** 
    \brief      Task Function to handle events for MDI Application 
  
    \brief      Description:\n
                Waits for incoming events and if one arrives function \n
                reads Messages received From BAP-Dispatcher and \n 
                AT_Cmd_Parser and handles them. \n

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_CAN_vHandleEvents(EventMaskType event)
{
    APP_PHONE_CanDataType receiveButtonData;
      
    /* start Mute, Demute request telefon */
    if( event & evHostTaskTelefonStates )
    {
        (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_HOST_TASK_CYCLIC_CAN_TX_MSG);
        (void)ReceiveMessage(msgTelefonStates ,&receiveButtonData);
        
        switch (receiveButtonData.common.id)
        {
            case APP_PHONE_CAN_PHONE_ON:
                canTelStatus.bits.phoneOn = (Uint8Type)receiveButtonData.phoneOn.isOn;
                break;
            case APP_PHONE_CAN_BTCONNECTIONSTATUS:
            {
                canTelStatus.bits.btStatus = (Uint8Type)receiveButtonData.BTstate.IsConnected;
                DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "Received Event BTstate");
                break;
            }
            case APP_PHONE_CAN_PHONE_MUTE:
                DEBUG_VALUE2(APP_CAN_SWI_TRC, MOD_APP_CAN, "HandleEvents: mute: %d source %d", 
                                    receiveButtonData.muteBehavior.muteUnmute,
                                    receiveButtonData.muteBehavior.signalSource);
                canTelStatus.bits.muteUnmute = (Uint8Type)receiveButtonData.muteBehavior.muteUnmute;
                canTelStatus.bits.muteSource = (Uint8Type)receiveButtonData.muteBehavior.signalSource;
                
                if (0 == canTelStatus.bits.muteUnmute)
                {
                    canTelStatus.bits.muteSource = (Uint8Type)0;
                }
                break;
                
            case APP_PHONE_CAN_ACTIVE_CALL:
                canTelStatus.bits.callActive = (Uint8Type)receiveButtonData.activeCall.isActive;
                break;
                
            case APP_PHONE_CAN_PHONE_RING:
                canTelStatus.bits.ringActive = (Uint8Type)receiveButtonData.phoneRing.isRinging;
                break;
                
            case APP_PHONE_CAN_PROVIDER:
//                canTelStatus.bits.providerName = receiveButtonData.provider.networkProvider.name;
                break;
            case APP_PHONE_CAN_FIELD_STRENGTH:
                DEBUG_VALUE1(APP_CAN_SWI_DBG, MOD_APP_CAN, "HandleEvents: signal strength %d", 
                             receiveButtonData.fieldStrength.value);
                if (0xFF == receiveButtonData.fieldStrength.value)
                {
                    canTelStatus.bits.fieldStrengthError = (Uint8Type)1;
                    canTelStatus.bits.fieldStrength = (Uint8Type)0;
                }
                else
                {
                    if(receiveButtonData.fieldStrength.value < 6U)
                    {
                        canTelStatus.bits.fieldStrength = (Uint8Type)(receiveButtonData.fieldStrength.value * 3);
                        canTelStatus.bits.fieldStrengthError = (Uint8Type)0;
                    }
                    else
                    {
                        canTelStatus.bits.fieldStrength = (Uint8Type)0;
                        canTelStatus.bits.fieldStrengthError = (Uint8Type)1;
                    }
                }    
                break;
                
            case APP_PHONE_CAN_REGISTER_STATE:
                canTelStatus.bits.registrationStatus = (Uint8Type)receiveButtonData.registerState.state;
                break;
                
            default:
                break;
    
        }
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_HOST_TASK_CYCLIC_CAN_TX_MSG, MSEC(10), 0);
        (void)ClearEvent(evHostTaskTelefonStates);
    }
    if( event & evHostTaskOnOffInfoSleepInd )
    {
        DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "HandleEvents: InfoOnOff set sleep indication");
        GotoMode(BusSleep);
        (void)ClearEvent(evHostTaskOnOffInfoSleepInd);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfSetTelefonStates                                   */
/** 
    \brief      Set Telefon Mute Demute Request over Can 
  
    \brief      Description: Set Telefon Mute Demute Request over Can \n
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfSetTelefonStates(void)
{
    TF3_PHONE_ON             = canTelStatus.bits.phoneOn;
    TF3_PHONE_INIT           = canTelStatus.bits.phoneOn;
    TF3_PHONE_MUTE           = canTelStatus.bits.muteUnmute;
    TF3_MUTE_MSG_CODE        = canTelStatus.bits.muteSource;
    TF3_ACTIVE_CALL          = canTelStatus.bits.callActive;
    TF3_PHONE_RING           = canTelStatus.bits.ringActive;
    TF3_FELDSTAERKE          = canTelStatus.bits.fieldStrength;
    TF3_FELDSTAERKE_FEHLER   = canTelStatus.bits.fieldStrengthError;
    TF3_REGISTRIERUNGSSTATUS = canTelStatus.bits.registrationStatus;
    TF3_BT_CONNECTIONSTATE   = canTelStatus.bits.btStatus;
    TF3_PHONE_ERROR_FLAG     = _APP_CAN_bGetCanEventBit();
    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_CAN_bGetCanEventBit                                      */
/**
    \brief      Get the current CAN State

    \return     BooleanType
*/
/*----------------------------------------------------------------------------*/
static Uint8Type _APP_CAN_bGetCanEventBit(void)
{
	Uint8Type u8GetKundenDienstErrorBit = 0;
	u8GetKundenDienstErrorBit = DEH_GET_CAN_EVENT_BIT;

	if ( u8GetKundenDienstErrorBit == 0x0 )
	{
		;
	}
	else
	{
		u8GetKundenDienstErrorBit = SDS_UDS_u8GetKundenDienstBit();
        DEBUG_VALUE1( APP_CAN_SWI_TRC, MOD_APP_CAN,"Kunden Dienst Bit %d", u8GetKundenDienstErrorBit);
	}
	
	return ( u8GetKundenDienstErrorBit );
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_enGetMuteStatus                                      */
/** 
    \brief      Get the current CAN mute status 
  
    \return     APP_DEVICE_AM_AudioStatusType
*/
/*----------------------------------------------------------------------------*/
APP_CAN_enAudioStatusType APP_CAN_enGetMuteStatus(void)
{
    return((APP_CAN_enAudioStatusType)canTelStatus.bits.muteUnmute);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sf10msCyclicCheckReceivedCANMessages                                */
/** 
    \brief      Check for received CAN messages. 
  
    \brief      The received CAN messages are evaluated and corresponding \n
                actions are taken. Following received CAN messages are checked:
                1. Check for Speed border
                2. MFL
                3. B2 Infotainment aus
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_CAN_sf10msCyclicCheckReceivedCANMessages(void)
{     
	SuccessFailType sF = SUCCESS;

	sF = APP_CAN_sfGWKombi();
	
	sF = APP_CAN_sfSystemInfo1();
	
	sF = APP_CAN_sfSollverbauNeu();
	
	sF = APP_CAN_sfGateway3();

    sF = APP_CAN_sfBSG2();
    
    sF = APP_CAN_sfBSG3();
    
    sF = APP_CAN_sfRadio4();
    
    sF = APP_CAN_sfLSM1();
    
    sF = APP_CAN_sfDiagnose_1();
    
    sF = APP_CAN_sfBSGKombi();

    sF = APP_CAN_sfMFLTasten();
    
    sF = APP_CAN_sfMFLTastenKon_1();
    
	return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfBSGKombi                                           */
/** 
    \brief      Check for new information on mBSGKombi message.
  
    \brief      Check for remote start bit from CAN message mBSGKombi.
                Can ID: 0x470, DLC : 8, Time : 50ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfBSGKombi(void)
{
    if ( BSGKOMBI_MESSAGE_RECEIVE )
    {
		if (canSignalInfo.CanSignalMessageBit.Remote.onOffState != BSGKOMBI_REMOTESTART)
		{
		    canSignalInfo.CanSignalMessageBit.Remote.onOffState  = BSGKOMBI_REMOTESTART;
		    
		    (void)SendMessage(msgCANInfotainmentInfo, &canSignalInfo);
		}
		DisableAllInterrupts();
		BSGKOMBI_MESSAGE_RECEIVE = 0;
		EnableAllInterrupts(); 

    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfGWKombi                                        */
/** 
    \brief      Check for new information on mGWKombi message. 
  
    \brief      Check if the current car speed and take \n
                corresponding actions.
                Monitoring the message.
                Can ID: 0x527, DLC : 8, Time : 200ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfGWKombi(void)
{     
   tDEH_Return retVal_u8 = DEH_OK;
    
    /* Get Car speed information from Can message mGW_Kombi */ 
    if( GWK_MESSAGE_RECEIVE )
    {       
        GWKombiTimer = 0;
        DisableAllInterrupts();
        GWK_MESSAGE_RECEIVE = 0;
        EnableAllInterrupts(); 
        missedMessages.MessageBits.GwKombi = 0;
        _u16ActualSpeed = b_GWK_FzgGeschw_b; 
        if(_u16ActualSpeed >= 0x7FFF)
        {
            _u16ActualSpeed = 0;
        }
        APP_CAN_Speed_vSpeedHandler(_u16ActualSpeed); 
        if( 0x00 == missedMessages.instr )
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, FALSE);
        }
        if (( TRUE == GWK_ALT_KOMBI ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_C15500_SG_SCHALTTAFELEINSATZ_KEINEKOMMUNIKATION, TRUE);
        }
        else if( FALSE == GWK_ALT_KOMBI )
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_C15500_SG_SCHALTTAFELEINSATZ_KEINEKOMMUNIKATION, FALSE);
        }
        else
        {
            /* do nothing */
        }
    }
    if( 0UL == WatchMessageTimer )
    {
        if(( SELF_DIAG_TIMER <= GWKombiTimer ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mGW_Kombi DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL");
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, TRUE);
            GWKombiTimer = 0;
            missedMessages.MessageBits.GwKombi = 1;
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetCurrentScopeEigenDiag: %d", Pwrmgr_GetCurrentScopeEigenDiag());
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetRapidPwrmgrVoltage: %d",Pwrmgr_GetRapidPwrmgrVoltage());
        }
        else
        {
            GWKombiTimer++;
        }
    }
    retVal_u8 = retVal_u8;
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfSystemInfo1                                        */
/** 
    \brief      Check for new information on mSystemInfo_1 message. 
  
    \brief      Check if the NWDF bit for start/stop and take \n
                corresponding actions.
                Monitoring the message.
                Can ID: 0x651, DLC : 8, Time : 100ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfSystemInfo1(void)
{
    SuccessFailType sF = SUCCESS;
    tDEH_Return retVal_u8 = DEH_OK;
    CommonResponseCodeType rC = COMMON_RSP_OK;
        
    if( SY1_MESSAGE_RECEIVE )
    {
        missedMessages.MessageBits.Systeminfo1 = 0;
        SysInfoTimer = 0U;

        if(TRUE == SY1_NWDF_GUELTIG)
        {
            DEBUG_TEXT(APP_CAN_SWI_DBG, MOD_APP_CAN, "mSystemInfo_1: NWDF gueltige Bit set to 1");
            if((FALSE == s_NWDFdet) && (TRUE == SY1_NWDF_BIT))
            {
                DEBUG_TEXT(APP_CAN_SWI_DBG, MOD_APP_CAN, "mSystemInfo_1: NWDF Bit set to 1");
                APP_CAN_sfReInitTOVariables();
            }
            s_NWDFdet = SY1_NWDF_BIT;
        }
        else
        {
            s_NWDFdet = TRUE;
        }

        /* check for valid Verbauliste */
        if (TRUE == SY1_VERBAULISTE_GUELTIG)
        {
            if ((Uint8Type)_APP_CAN_enMarke != SY1_MARKE)
            {
                _APP_CAN_enMarke = (APP_CAN_ENUM_MARKE)SY1_MARKE;
                switch(_APP_CAN_enMarke)
                {
                    case APP_CAN_VW_Volkswagen: 
                    case APP_CAN_AU_Audi:      
                    case APP_CAN_SE_Seat:
                    case APP_CAN_SK_Skoda:
                    case APP_CAN_VN_VWNutzfzg:
                    case APP_CAN_Bugatti:
                    case APP_CAN_Lamborghini:
                    case APP_CAN_Bentley:
                    case APP_CAN_RollsRoyce:
                    case APP_CAN_Ford:
                    case APP_CAN_NoMarke: 
                    case APP_CAN_Porsche:  
                        DEBUG_VALUE1( APP_CAN_SWI_TRC, MOD_APP_CAN,"mSystemInfo_1: Marke received %x", _APP_CAN_enMarke);
                        /* set function to the used modules */
                        /* changed the value at the runtime - call these function. */
                        (void)CONTEXT_MAN_sfSetFahrzeugTyp(_APP_CAN_enMarke);
                        /* check for coding in the new car manufacture */
                        canMsgStates.common = MARKE;
                        canMsgStates.marke.marke = _APP_CAN_enMarke;
                        (void)SendMessage(msgAppCanStates, &canMsgStates);                        
                        break;
                    default:
                        DEBUG_VALUE1( APP_CAN_SWI_ERR, MOD_APP_CAN, "mSystemInfo_1: Marke received %x is invalid", _APP_CAN_enMarke);
                        break;
                }
            }
            if((Uint8Type)_APP_CAN_enDerivat != SY1_DERIVAT)
            {
                _APP_CAN_enDerivat = (APP_CAN_ENUM_DERIVAT)SY1_DERIVAT;
            }
        }
        else
        {
            /* no Verbauliste valid 
               - readout the last received information or default value from EEPROM*/
        	(void)EEPROM_enGetSingleByteFromRamMirror((Uint8Type *)&_APP_CAN_enMarke, EEPROM_LAYOUT_ENUM_LastSaveMarkeSelection);
        }

        DisableAllInterrupts();
        SY1_MESSAGE_RECEIVE = 0;
        EnableAllInterrupts(); 

        if( 0x00 == missedMessages.instr)
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, FALSE);
        }
    }

    /* no mSysteminfo_1 message received */
    if( 0UL == WatchMessageTimer )
    {
        if(( SELF_DIAG_TIMER <= SysInfoTimer ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mSystemInfo_1 DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL");
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, TRUE);
            SY1_NWDF_GUELTIG = FALSE;
            s_NWDFdet = TRUE;
            SysInfoTimer = 0U;
            missedMessages.MessageBits.Systeminfo1 = 1;
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetCurrentScopeEigenDiag: %d", Pwrmgr_GetCurrentScopeEigenDiag());
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetRapidPwrmgrVoltage: %d",Pwrmgr_GetRapidPwrmgrVoltage());
        }
        else
        {
            SysInfoTimer++;
        }
    }
    retVal_u8 = retVal_u8;
    rC = rC;
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfGateway3                                           */
/** 
    \brief      Check for new information on mGateway_3 message. 
  
    \brief      Check if the language is changed and take \n
                corresponding actions.
                Monitoring the message.
                Can ID: 0x653, DLC : 3, Time : 500ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfGateway3(void)
{
    SuccessFailType sF = SUCCESS;
    tDEH_Return retVal_u8 = DEH_OK;
    CommonResponseCodeType rC = COMMON_RSP_OK;
    
    if( GW3_MESSAGE_RECEIVED )
    {
        GW3Timer = 0U;
        missedMessages.MessageBits.Gateway3 = 0;
        DisableAllInterrupts();
        GW3_MESSAGE_RECEIVED = 0;
        EnableAllInterrupts(); 
        
        /* check to receive speech type */
        if (TRUE == GW3_LAND_SPRACH_EMPF)
        {
            if (_APP_CAN_u8KombiLanguage != GW3_SPRACHVARIANTE)
            {
                _APP_CAN_u8KombiLanguage = GW3_SPRACHVARIANTE;
                canMsgStates.common = SD_LANGUAGE;
                switch(_APP_CAN_u8KombiLanguage)
                {
                    case APP_CAN_Deutsch:
                    case APP_CAN_Englisch:
                    case APP_CAN_Franzoesisch:
                    case APP_CAN_Italienisch:
                    case APP_CAN_Spanisch:
                    case APP_CAN_Portugiesisch:
                    case APP_CAN_Tschechisch:
                    case APP_CAN_Chinesisch:
                    case APP_CAN_US_Englisch:
                    case APP_CAN_Niederlaendisch:
                    case APP_CAN_Japanisch:
                    case APP_CAN_Russisch:
                    case APP_CAN_Koreanisch:
                    case APP_CAN_Franko_Kanadisch:
                    case APP_CAN_Schwedisch:
                    case APP_CAN_Polnisch:
                    case APP_CAN_Tuerkisch:
                    case APP_CAN_US_Spanisch:
                    case APP_CAN_Norwegisch:
                        canMsgStates.sd_lang.sd_language = (APP_CAN_enLang)_APP_CAN_u8KombiLanguage;
                        (void)EEPROM_sfRamWriteOnly(
                                                (Uint8Type*)&_APP_CAN_u8KombiLanguage,
                                                EEPROM_LAYOUT_ENUM_LastSaveLanguageSelection);
                        DEBUG_VALUE1( APP_CAN_SWI_TRC, MOD_APP_CAN, "mGateway_3: Language received %x", _APP_CAN_u8KombiLanguage);
                        break;
                    default:
                        canMsgStates.sd_lang.sd_language = APP_CAN_keine_Sprachvar;
                        DEBUG_VALUE1( APP_CAN_SWI_ERR, MOD_APP_CAN, "mGateway_3: Language received %x is invalid", _APP_CAN_u8KombiLanguage);
                        break;
                }
                (void)SendMessage(msgAppCanStates, &canMsgStates);
            }
        }
        else
        {
            /* no speech type supported */
        	(void)EEPROM_enGetSingleByteFromRamMirror(&_APP_CAN_u8KombiLanguage, EEPROM_LAYOUT_ENUM_LastSaveLanguageSelection);
        }
        
        if( 0x00 == missedMessages.instr )
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, FALSE);
        }
    }

    /* no mGateway_3 message received */    
    if( 0UL == WatchMessageTimer )
    {
        if(( SELF_DIAG_TIMER <= GW3Timer ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mGateway_3 DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL");
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, TRUE);
            GW3Timer = 0U;
            missedMessages.MessageBits.Gateway3 = 1;
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetCurrentScopeEigenDiag: %d", Pwrmgr_GetCurrentScopeEigenDiag());
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetRapidPwrmgrVoltage: %d",Pwrmgr_GetRapidPwrmgrVoltage());
        }
        else
        {
            GW3Timer++;
        }
    }
    retVal_u8 = retVal_u8;
    rC = rC;
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfSollverbauNeu                                      */
/** 
    \brief      Check for new information on mSollverbau_neu message. 
  
    \brief      Monitoring the message.
                Can ID: 0x655, DLC : 8, Time : 500ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfSollverbauNeu(void)
{
    SuccessFailType sF = SUCCESS;
    tDEH_Return retVal_u8 = DEH_OK;
    
    if( VBN_MESSAGE_RECEIVE )
    {
        missedMessages.MessageBits.Sollverbau = 0;
        DisableAllInterrupts();
        VBN_MESSAGE_RECEIVE = 0;
        EnableAllInterrupts(); 

        if( 0x00 == missedMessages.instr )
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, FALSE);
        }
        VBNTimer = 0U;
    }
    /* no mSollverbau_neu message received */
    if( 0UL == WatchMessageTimer )
    {
        if(( SELF_DIAG_TIMER <= VBNTimer ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mSollverbau_neu DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL");
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, TRUE);
            VBNTimer = 0U;
            missedMessages.MessageBits.Sollverbau = 1;
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetCurrentScopeEigenDiag: %d", Pwrmgr_GetCurrentScopeEigenDiag());
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetRapidPwrmgrVoltage: %d",Pwrmgr_GetRapidPwrmgrVoltage());
        }
        else
        {
            VBNTimer++;
        }
    }
    retVal_u8 = retVal_u8;
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfMFLTasten                                          */
/** 
    \brief      Check for new information on MFL Taste. 
  
    \brief      Check if any button was pressed on the MFL Taste and take \n
                corresponding actions.
                Can ID: 0x5C1, DLC : 4.
    
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfMFLTasten(void)
{   
    SuccessFailType sF = SUCCESS;
    
    if( ON == canSignalInfo.CanSignalMessageBit.BSG3.klemme15 )
    {
        /* Check for new message */
      	if( MFL_TASTEN_MESSAGE_RECEIVED )
    	{
    		MFL_vParameterAnalysis((Uint8Type)TYPE_TASTEN, 
    		                       (MFL_ENUM_TASTENSYMBOL)MLT_SYMBOL, 
    		                       (Uint8Type)MLT_TICKZAEHLER, 
    		                       (MFL_ENUM_MARKE)MLT_MARKE, 
    		                       (Uint8Type)MLT_VARIANTE);
    		_APP_CAN_MflVariante = (APP_CAN_ENUM_MFL_VARIANTE)MLT_VARIANTE;
    		DisableAllInterrupts();
    		MFL_TASTEN_MESSAGE_RECEIVED = 0;
    		EnableAllInterrupts();     
       	}
    }
	return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfMFLTastenKon_1                                           */
/** 
    \brief      Check for new information on MFL Taste. 
  
    \brief      Check if any button was pressed on the MFL Taste and take \n
                corresponding actions.
                Can ID: 0x5BF, DLC : 4
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfMFLTastenKon_1(void)
{   
    SuccessFailType sF = SUCCESS;
    
    if( ON == canSignalInfo.CanSignalMessageBit.BSG3.klemme15 )
    {
        /* Check for new message */
	    if( MFL_TASTEN_KON_1_MESSAGE_RECEIVED )
	    {
	        if (APP_CAN_Offroad == _APP_CAN_enDerivat)
	        {
	            MFL_vParameterAnalysis((Uint8Type)TYPE_TASTEN_KON, 
        	                            (MFL_ENUM_TASTENSYMBOL)MLT_SYMBOL, 
        	                            (Uint8Type)MLT_TICKZAEHLER, 
        	                            (MFL_ENUM_MARKE)MLT_MARKE, 
        	                            (Uint8Type)MLT_VARIANTE);	
            }
	    	DisableAllInterrupts();
	    	MFL_TASTEN_KON_1_MESSAGE_RECEIVED = 0;
	    	EnableAllInterrupts();     
	    }
    }
	return sF;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfBSG2                                               */
/** 
    \brief      Check for new information on mBSG_2 message. 
  
    \brief      Check if any ausInfotainment/warnInfotainment and take \n
                corresponding actions.
                Can ID: 0x571, DLC : 6, Time : 500ms   
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfBSG2(void)
{
    SuccessFailType sF = SUCCESS;
    if ( BSG2_MESSAGE_RECEIVED )
    {
		if ((canSignalInfo.CanSignalMessageBit.BSG2.ausInfo  != BSG2_BS2AUSINFO) || 
		    (canSignalInfo.CanSignalMessageBit.BSG2.warnInfo != BSG2_BS2WARNINFO))
		{
		    canSignalInfo.CanSignalMessageBit.BSG2.ausInfo  = BSG2_BS2AUSINFO;
		    canSignalInfo.CanSignalMessageBit.BSG2.warnInfo = BSG2_BS2WARNINFO;
		    
            if((BSG3_KLEMME_15 == OFF) && (BSG2_BS2AUSINFO == ON))
            { 
                DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mBSG_2 Infotainment OnOff Spec. active");
                bOnOffInfotainment = BT_TRUE;
                /* Set Infotainment On Off timer to set sleep indication */
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_HOST_ONOFF_INFO_SLEEP_IND, MSEC(15000), 0);
                //GotoMode(BusSleep);
            }
            else if ((BT_TRUE == bOnOffInfotainment) &&
                     ((BSG3_KLEMME_15 == ON) ||
                     (BSG2_BS2AUSINFO == OFF)))
            {   
                DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mBSG_2 Infotainment OnOff cancel timer");
                bOnOffInfotainment = BT_FALSE;
                /* Cancel Infotainment On Off timer*/
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_HOST_ONOFF_INFO_SLEEP_IND);
            }
		    (void)SendMessage(msgCANInfotainmentInfo, &canSignalInfo);
		}
		DisableAllInterrupts();
		BSG2_MESSAGE_RECEIVED = 0;
		EnableAllInterrupts(); 
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfRadio4                                */
/**     
    \brief      Check for new information on Radio4 message. 
  
    \brief      Check if the radio is OFF or ON and take \n
                corresponding actions.
                Monitoring the message timeout. If timeout occurs we assume
                radio is off.
                Can ID: 0x661, DLC : 8, Time : 1000ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfRadio4(void)
{
    SuccessFailType sF = SUCCESS;
    tDEH_Return retVal_u8 = DEH_OK;
    Uint8Type informAppTask = 0;

    if ( RADIO4_MESSAGE_RECEIVE )
    {
        Radio4Timer = 0U;
        retVal_u8 = DEH_AdditionalReportEvent(DTC_000016_SG_RADIO_KEINSIGNAL, FALSE);

        if (canSignalInfo.CanSignalMessageBit.Radio.onOffState  != RADIO4_ONOFF)
        {
            canSignalInfo.CanSignalMessageBit.Radio.onOffState  = RADIO4_ONOFF;
            (void)SendMessage(msgCANInfotainmentInfo, &canSignalInfo);
            informAppTask = 1;
            canMsgStates.radio.onOff = RADIO4_ONOFF;
        }
        if(canMsgStates.radio.mute != RADIO4_MUTE)
        {
            informAppTask = 1;
            canMsgStates.radio.mute = RADIO4_MUTE;
        }

        if (informAppTask == 1)
        {
            canMsgStates.common = RADIO;
            (void)SendMessage(msgAppCanStates, &canMsgStates);                        
            informAppTask = 0;
        }
        DisableAllInterrupts();
        RADIO4_MESSAGE_RECEIVE = 0;
        EnableAllInterrupts();
    }
    /* no mRAdio_4u message received */
    if( 0UL == WatchMessageTimer )
    {
        if(( SELF_DIAG_TIMER <= Radio4Timer ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            DEBUG_TEXT(APP_CAN_SWI_DBG, MOD_APP_CAN, "DTC_000016_SG_RADIO_KEINSIGNAL");
            retVal_u8 = DEH_AdditionalReportEvent(DTC_000016_SG_RADIO_KEINSIGNAL, TRUE);
            Radio4Timer = 0U;
            /* changed by MM: ticket 10367 */
            canSignalInfo.CanSignalMessageBit.Radio.onOffState  = OFF;
            (void)SendMessage(msgCANInfotainmentInfo, &canSignalInfo);
        }
        else
        {
            Radio4Timer++;
        }
    }
    retVal_u8 = retVal_u8;
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfBSG3                                               */
/** 
    \brief      Check for new information on mBSG_3 message. 
  
    \brief      Check if the KL15and KLS is OFF or ON and take \n
                corresponding actions.
                Monitoring the message.
                Can ID: 0x575, DLC : 4, Time : 200ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfBSG3(void)
{
    SuccessFailType sF = SUCCESS;
    StatusType NmState = 0;
    tDEH_Return retVal_u8 = DEH_OK;

    if(IsBusOff() == FALSE)
    {
        NmState = NmGetStatus();
        /* check if mBSG_3 messages was received */
        if( BSG3_MESSAGE_RECEIVE )
        {
            eventZASState = 0;
            BSG3Timer = 0U;
            if((NmStateLimphome( NmState ) != 1) && 
               ((BSG3_KLEMME_15 == ON) || ((BSG3_KLEMME_S == ON) && (canSignalInfo.CanSignalMessageBit.Radio.onOffState == ON))))
            {
            	if (BSG3_KLEMME_15 == ON)
                {
                    GotoMode(Awake);
                    if (BT_TRUE == bOnOffInfotainment)
                    {   
                        DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mBSG_3 Infotainment OnOff cancel timer");
                        bOnOffInfotainment = BT_FALSE;
                        /* Cancel Infotainment On Off timer*/
                        (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_HOST_ONOFF_INFO_SLEEP_IND);
                    }
                }
                NachlaufzeitTimer   = TIMEOUT_NACHLAUFZEIT;
            }
            retVal_u8 = DEH_AdditionalReportEvent(DTC_C14000_BODYCOMPUTER1_KEINEKOMMUNIKATION, FALSE);

    		if ((canSignalInfo.CanSignalMessageBit.BSG3.klemme15 != BSG3_KLEMME_15) || 
    		    (canSignalInfo.CanSignalMessageBit.BSG3.klemmeS  != BSG3_KLEMME_S))
    		{
    		    canSignalInfo.CanSignalMessageBit.BSG3.klemme15 = BSG3_KLEMME_15;
    		    canSignalInfo.CanSignalMessageBit.BSG3.klemmeS  = BSG3_KLEMME_S;
    		    
    		    (void)SendMessage(msgCANInfotainmentInfo, &canSignalInfo);
    		}
           
            if((kl15Store == OFF) && (BSG3_KLEMME_15 == ON))
            {
                /* reset the CAN Eigen-Diagnose time */ 
                (void)APP_CAN_sfReInitTOVariables();  /* additional for start/stopp */
                kl15Store = ON;
            }
            else if((kl15Store == ON) && (BSG3_KLEMME_15 == OFF))
            {
                /* reset the Sleep delay timer */
                SleepDelayTimer = SLEEPDELAY;
                kl15Store = OFF;
            }
            else
            {
                /* do nothing */
            }
            DisableAllInterrupts();
            BSG3_MESSAGE_RECEIVE = 0;
            EnableAllInterrupts();
        }
        /* if watch message timer is over check if the mBsg_3 message is come in */ 
        if( 0UL == WatchMessageTimer )
        {
            if(( SELF_DIAG_TIMER <= BSG3Timer ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
            {
                DEBUG_TEXT(APP_CAN_SWI_DBG, MOD_APP_CAN, "mBSG_3: DTC_C14000_BODYCOMPUTER1_KEINEKOMMUNIKATION");
                retVal_u8 = DEH_AdditionalReportEvent(DTC_C14000_BODYCOMPUTER1_KEINEKOMMUNIKATION, TRUE);
                if(eventZASState >= 5)
                {
                    /* lost the mBSG3 message, reset all signals and send to the CSHDL */
                    BSG3_KLEMME_15 = OFF;
                    BSG3_KLEMME_S = OFF;
                    
        		    canSignalInfo.CanSignalMessageBit.BSG3.klemme15 = BSG3_KLEMME_15;
        		    canSignalInfo.CanSignalMessageBit.BSG3.klemmeS  = BSG3_KLEMME_S;
        		    
        		    (void)SendMessage(msgCANInfotainmentInfo, &canSignalInfo);
        		    DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mBSG_3 Timeout KL15 = OFF, KLS = OFF, set bus to sleep");
                    GotoMode(BusSleep);
                }
                
                eventZASState++;
                BSG3Timer = 0U;
            }
            else
            {
                BSG3Timer++;
            }
        }
    }
    retVal_u8 = retVal_u8;
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_u8GetMarke                                           */
/** 
    \brief      Get the current Marke
               
    \return     APP_CAN_ENUM_MARKE
    			all values
*/
/*----------------------------------------------------------------------------*/
APP_CAN_ENUM_MARKE APP_CAN_enGetMarke(void)
{
    return(_APP_CAN_enMarke);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_enGetDerivat                                         */
/** 
    \brief      Get the current Derivat
               
    \return     APP_CAN_ENUM_DERIVAT
    			all values
*/
/*----------------------------------------------------------------------------*/
APP_CAN_ENUM_MARKE APP_CAN_enGetDerivat(void)
{
    return (APP_CAN_ENUM_MARKE)_APP_CAN_enDerivat;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_u8GetActiveLSM1Device                                */
/** 
    \brief      Get the current Device on LS1: MFA or MFL
               
    \return     APP_CAN_ENUM_LSM_VERBAU
    			enAppCanMflAvailable : MFL ist eingebaut
    			enAppCanLssAvailable : MFA ist eingebaut
    			Important: The two can never be together.
*/
/*----------------------------------------------------------------------------*/
APP_CAN_ENUM_LSM_VERBAU APP_CAN_u8GetActiveLSM1Device(void)
{
	APP_CAN_ENUM_LSM_VERBAU enVerbauListe = enAppCanNothingAvailable;
	
	if (( 0x01 == lsm.MFLvorhanden) && (0x01 == lsm.MFAvorhanden))
    {
		enVerbauListe = enAppCanLssAndMflAvailable;
    }
    else if ( 0x01 == lsm.MFLvorhanden)
    {
        enVerbauListe = enAppCanMflAvailable;
    }
    else if ( 0x01 == lsm.MFAvorhanden)
    {
    	enVerbauListe = enAppCanLssAvailable;
    }
  return enVerbauListe; 
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_SleepDelayTimer                                      */
/** 
    \brief      Get the current SleepDelayTimer
               
    \return     Uint16Type
    			all values
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_CAN_SleepDelayTimer(void)
{
    return SleepDelayTimer;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_ResetSleepDelayTimer                                      */
/** 
    \brief      Reset the current SleepDelayTimer
               
*/
/*----------------------------------------------------------------------------*/
void APP_CAN_ResetSleepDelayTimer(void)
{
    SleepDelayTimer = SLEEPDELAY;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfLSM1                                       */
/** 
    \brief      Check for new information on mLSM_1 message. 
  
    \brief      Monitoring the message.
                Can ID: 0x2C1, DLC : 6, Time : 100ms
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfLSM1(void)
{
    SuccessFailType sF = SUCCESS;
    tDEH_Return retVal_u8 = DEH_OK;
    
    
    if ( LSM1_MESSAGE_RECEIVED )
    {
        LsmTimer = 0U;
		lsm.MFAvorhanden    = LSM1_MFA_VORHANDEN; // Byte 5, Bit 2, 0x2C1
		lsm.MFLvorhanden    = LSM1_MFL_VORHANDEN; // Byte 5, Bit 1, 0x2C1
		lsm.MFAtasten       = LSM1_MFA_TASTEN;
		lsm.MFLtyp          = LSM1_MFL_TYP;

        /* remove warning of not used */
        lsm = lsm;
		
		retVal_u8 = DEH_AdditionalReportEvent(DTC_C21200_SG_LENKSAEULENELEKTRONIK_KEINEKOMMUNIKATION, FALSE);
		DisableAllInterrupts();
		LSM1_MESSAGE_RECEIVED = 0;
		EnableAllInterrupts();   
    }
    /* no mLSM_1 message received */
    if( 0UL == WatchMessageTimer )
    {
        if(( SELF_DIAG_TIMER <= LsmTimer) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            DEBUG_TEXT(APP_CAN_SWI_DBG, MOD_APP_CAN, "mLSM_1: DTC_C21200_SG_LENKSAEULENELEKTRONIK_KEINEKOMMUNIKATION");
            retVal_u8 = DEH_AdditionalReportEvent(DTC_C21200_SG_LENKSAEULENELEKTRONIK_KEINEKOMMUNIKATION, TRUE);
            LsmTimer = 0U;
        }
        else
        {
            LsmTimer++;
        }
    }
    retVal_u8 = retVal_u8; 
    return sF;   
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfDiagnose_1                                         */
/** 
    \brief      Check for received Diagnose 1 CAN messages. 
  
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType APP_CAN_sfDiagnose_1(void)
{
    tDEH_Return retVal_u8 = DEH_OK;
    
    if( DIAGNOSE_MESSAGE_RECEIVED )
    {
        missedMessages.MessageBits.Diagnose1 = 0;
        DIAG1Timer = 0;
        if((DIAGNOSE_ALT_ZEIT == TRUE) || (DIAGNOSE_ALT_KILOMETER == TRUE))
        {
            Diagnose_1_FirstTime = FALSE;
        }
        else
        {
            Diagnose_1_FirstTime = TRUE;
        }
        diagnose_1.jear  = DIAGNOSE_JAHR_0 + (DIAGNOSE_JAHR_1 * 16U)+2000U;
        diagnose_1.month = DIAGNOSE_MONAT;
        diagnose_1.day   = (DIAGNOSE_TAG_1 << 1) | DIAGNOSE_TAG_0;
        diagnose_1.verlernzaehler  = DIAGNOSE_VERLERNZAEHLER;
        diagnose_1.kilometer = ((DIAGNOSE_KM_STAND_2 & 0x0F) << 16) | (DIAGNOSE_KM_STAND_1 << 8) | (DIAGNOSE_KM_STAND_0);
        
        if( 0x00 == missedMessages.instr )
        {
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, FALSE);
            //missedMessages.MessageBits.Diagnose1 = 0;
        }
        DisableAllInterrupts();
        DIAGNOSE_MESSAGE_RECEIVED = 0;
        EnableAllInterrupts(); 
    }

    if( 0UL == WatchMessageTimer )
    {
        if(( SELF_DIAG_TIMER <= DIAG1Timer ) && (TRUE == APP_CAN_u8EigenDiagDTCStore()))
        {
            DEBUG_TEXT(APP_CAN_SWI_TRC, MOD_APP_CAN, "mDiagnose_1 DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL");
            retVal_u8 = DEH_AdditionalReportEvent(DTC_D00900_DI_DATENBUS_J533_UNPLAUSIBLESSIGNAL, TRUE);
            DIAG1Timer = 0;
            missedMessages.MessageBits.Diagnose1 = 1;
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetCurrentScopeEigenDiag: %d", Pwrmgr_GetCurrentScopeEigenDiag());
            DEBUG_VALUE1( APP_CAN_SWI_DBG, MOD_APP_CAN, "Pwrmgr_GetRapidPwrmgrVoltage: %d",Pwrmgr_GetRapidPwrmgrVoltage());
        }
        else
        {
            DIAG1Timer++;
        }
    }
    retVal_u8 = retVal_u8;
    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfCheckBusOff                                        */
/** 
    \brief      Check for bus off. 
  
    \brief      Check if any node go to bus off and waite 4 sec before \n
                we go to bus off.
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_CAN_sfCheckBusOff(void)
{
    SuccessFailType sF = SUCCESS;
    Uint8Type canActiveCheck = IsCanActive();
    Uint8Type canDiagSession = UDS_ubGetSession();
  
    if( (IsBusOff() == FALSE) && 
        ((canSignalInfo.CanSignalMessageBit.BSG3.klemme15 == ON) ||
        ((canSignalInfo.CanSignalMessageBit.BSG3.klemmeS == ON) && (canSignalInfo.CanSignalMessageBit.Radio.onOffState == ON))))
    {
        if(WatchMessageTimer > 0UL)
        {
            WatchMessageTimer--;
        }
        if(SleepDelayTimer > 0)
        {
            SleepDelayTimer--;
        }
    }
    else
    {
        if(SleepDelayTimer > 0)
        {
            SleepDelayTimer--;
        }
        else if((TRUE == canActiveCheck) && 
                (UDScub_DEFAULT_SESSION == canDiagSession) && 
                (IsBusOff() != TRUE))
        {
            if((NachlaufzeitTimer > 0) && (1 == CSHDL_vGetCkOnOffStatus()))
            {
                NachlaufzeitTimer--;
            }
            else
            {   /* set sleep indication */
                GotoMode(BusSleep);
            }
        }
        else
        {
            /* nothing to do */
        }
        APP_CAN_sfReInitTOVariables();
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_CAN_cbNachlaufzeitTimeout                                 */
/**
  \brief        Alarm callback to trigger the nachlaufzeit timeout

  \return       void

 */
/*----------------------------------------------------------------------------*/
ALARMCALLBACK(APP_CAN_cbNachlaufzeitTimeout)
{
    GotoMode(BusSleep);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfSendCyclicCanMessage                               */
/** 
    \brief      Task Function to handle transmit message for Host Task. 
  
    \brief      Waits of function call for cyclic or spontane \n
                set message values and transmit these message to CAN \n 

    \param      event
                EventMaskType
                Bitmask containing all unhandled events
               
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_CAN_sfSendCyclicCanMessage(EventMaskType event)     
{  
    SuccessFailType sF = SUCCESS;
    
    if (event & evHostTaskCyclicTxMsg)
    {        
        c1_mTelefon_3_c = 0x00;
        c2_mTelefon_3_c = 0x00;
        c3_mTelefon_3_c = 0x00;
        c4_mTelefon_3_c = 0x00;
        c5_mTelefon_3_c = 0x00;
        c6_mTelefon_3_c = 0x00;
        c7_mTelefon_3_c = 0x00;

        (void)APP_CAN_sfSetTelefonStates();
        
        /* send the DSP_01 */
        if(CanTransmit( CanTxmTelefon_3 ) == kCanTxOk)
        {
            DisableAllInterrupts();
            mTelefon_3_conf_b = 0;
            EnableAllInterrupts();
        }
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_HOST_TASK_CYCLIC_CAN_TX_MSG, MSEC(1000), 0);
        (void)ClearEvent(evHostTaskCyclicTxMsg);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_u8Klemme15Status                                     */
/** 
    \brief      Function to check Klemme 15 status. 
                               
    \return     Uint8Type
    			Status of Klemme 15
    			ON or OFF
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_CAN_u8Klemme15Status(void)
{
    return (canSignalInfo.CanSignalMessageBit.BSG3.klemme15);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_u8DTCStore                                        */
/** 
    \brief      Function to check the Klemme 15 status, the NWDF status \n
                and outside of the voltage range for set the DTC. 
               
    \return     Uint8Type
    			TRUE, FALSE
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_CAN_u8DTCStore(void)
{
    FlagType    retVal = FALSE;

    if((OFF == APP_CAN_u8Klemme15Status()) || 
       (FALSE == Pwrmgr_GetCurrentScope()) || (FALSE == s_NWDFdet))
    {
    	retVal = FALSE;
    }
    else
    {
        retVal = TRUE;
    }

    return (retVal);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_u8EigenDiagDTCStore                                        */
/**
    \brief      Function to check the Klemme 15 status, the NWDF status \n
                and outside of the voltage range for set the Eigen Diagnose DTC.

    \return     Uint8Type
    			TRUE, FALSE
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_CAN_u8EigenDiagDTCStore(void)
{
    FlagType    retVal = FALSE;

    if((OFF == APP_CAN_u8Klemme15Status()) ||
       (FALSE == Pwrmgr_GetCurrentScopeEigenDiag()) || (FALSE == s_NWDFdet))
    {
    	retVal = FALSE;
    }
    else
    {
        retVal = TRUE;
    }


    return (retVal);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_u8GetNWDF                                           */
/** 
    \brief      Function to check the NWDF status
    
    \return     Uint8Type
    			TRUE, FALSE
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_CAN_u8GetNWDF(void)
{
    return(s_NWDFdet);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_u8GetMflVariante                                     */
/** 
    \brief      Function get the MFL variante back
    
    \return     APP_CAN_ENUM_MFL_VARIANTE
    			0x06 MFL NORMAL, 
    			0x09 MFL LOW
*/
/*----------------------------------------------------------------------------*/
APP_CAN_ENUM_MFL_VARIANTE APP_CAN_u8GetMflVariante(void)
{
    return(_APP_CAN_MflVariante);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_sfDiagnose_1_MessageReceived                         */
/** 
    \brief      
   
               
    \return     Uint8Type
    			Status of function run
    			TRUE, FALSE
*/
/*----------------------------------------------------------------------------*/
Uint8Type APP_CAN_sfDiagnose_1_MessageReceived(void)
{
    return (Diagnose_1_FirstTime);
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_CAN_vDiagnose_1_Data                                     */
/** 
    \brief       
  
    \return     Diagnose1MessageType
    			value limits
*/
/*----------------------------------------------------------------------------*/
Diagnose1MessageType APP_CAN_vDiagnose_1_Data(void)
{
    return (diagnose_1);
}
/* End Of File APP_CAN_C1.c */
