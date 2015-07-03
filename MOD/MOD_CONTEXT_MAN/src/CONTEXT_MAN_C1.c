/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/**
 * \file    CONTEXT_MAN_C1.c
 * \brief   The button presses on Cradle and 3 button module are forwarded to the
 * application depending on the context in which the system is. Detailed description found below.
 * \brief   This file fulfills the following requirements:
 *          1. Requirement 1
 *          2. Requirement 2
 *          3. Requirement x
*/
/*-----------------------------------------------------------------------------
* $Date: 2012-02-06 14:53:12 +0100 (Mo, 06 Feb 2012) $
* $Rev: 25556 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CONTEXT_MAN/trunk/src/CONTEXT_MAN_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h" // includes the 3bm enums
#include "LOGI_BUT_MAN.h"
#include "APP_PHONE.h"  /* APP_CPMMON_enResponseCodes */
#include "APP_PHONE_AT_PNCC.h" /* APP_PHONE_AT_PNCC_pGetPlccContext */
#include "ATParser.h"
#include "VCR.h"
#include "APP_CAN.h"
#include "APP_SD.h"
#include "APP_CSHDL.h"
#include "TRACE.h"
#include "EEPROM_LAYOUT_CFG.h"
#include "ERM.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "CONTEXT_MAN.h"
#include "CONTEXT_MAN_CI.h"
#include "CONTEXT_MAN_C1.id"

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
\brief  description of the type
*/

/*-------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the method
*/
APP_COMMON_enResponseCode _CONTEXT_MAN_vCallStateHandling(const ATRspCallStatValType *pstPropertyCall, ATCallIndexEnum enCurrentCall);

static void _CONTEXT_MAN_vHandleCalls(void);
static APP_COMMON_enResponseCode _CONTEXT_MAN_rcReconnectLastUsedDevice(void);

static Uint16Type _CONTEXT_MAN_u16TransferPhoneNumberToASCII(Uint8Type *data,
        Uint16Type length);
static void CONTEXT_MAN_cDeleteAllProfiles(void);
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/
#define CONTEXT_MAN_TIMEOUT_500MS   500
/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
static CONTEXT_MAN_ENUM_FN_BUTTONS _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
static BooleanType CONTEXT_MAN_bInfoCallInProgress = BT_FALSE;
static BooleanType CONTEXT_MAN_bSosCallInProgress = BT_FALSE;
static APP_CAN_ENUM_MARKE _CONTEXT_MAN_enFahrzeugTyp;

static Uint8Type   _CONTEXT_MAN_u8InfoCallNumber[AT_MAX_PHONE_NUM_LEN+1];
static Uint8Type   _CONTEXT_MAN_u8InfoCallNumberRoaming[AT_MAX_PHONE_NUM_LEN+1];
static Uint16Type  _CONTEXT_MAN_u16InfoCallNumberLen;
static Uint16Type  _CONTEXT_MAN_u16InfoCallNumberLenRoaming;
static Uint8Type   _CONTEXT_MAN_u8ServiceCallNumber[AT_MAX_PHONE_NUM_LEN+1];
static Uint8Type   _CONTEXT_MAN_u8ServiceCallNumberRoaming[AT_MAX_PHONE_NUM_LEN+1];
static Uint16Type  _CONTEXT_MAN_u16ServiceCallNumberLen;
static Uint16Type  _CONTEXT_MAN_u16ServiceCallNumberLenRoaming;
static Uint8Type   _CONTEXT_MAN_u8SOSCallNumber[AT_MAX_PHONE_NUM_LEN+1];
static Uint16Type  _CONTEXT_MAN_u16SOSCallNumberLen;

static BooleanType _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_TRUE;
static Uint8Type _CONTEXT_MAN_u8RetryTimeOut = 0;
static BooleanType _CONTEXT_MAN_phoneActivatedOnce = BT_FALSE;
static Uint32Type  _CONTEXT_MAN_DoubleClick_StartTime = 0;
/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfGetInfoCallNumber                              */
/**
 \brief       function for a request to get the Info Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfGetInfoCallNumber(ATPhoneNumberType* number, Uint16Type* number_len)
{
    *number_len = _CONTEXT_MAN_u16InfoCallNumberLen > AT_MAX_PHONE_NUM_LEN
                        ? AT_MAX_PHONE_NUM_LEN : _CONTEXT_MAN_u16InfoCallNumberLen;
    (void)memcpy (&number[0],_CONTEXT_MAN_u8InfoCallNumber,*number_len);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfGetServiceCallNumber                              */
/**
 \brief       function for a request to get the Service Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfGetServiceCallNumber(ATPhoneNumberType* number, Uint16Type* number_len)
{
    *number_len = _CONTEXT_MAN_u16ServiceCallNumberLen > AT_MAX_PHONE_NUM_LEN
                        ? AT_MAX_PHONE_NUM_LEN : _CONTEXT_MAN_u16ServiceCallNumberLen;
    (void)memcpy (&number[0],_CONTEXT_MAN_u8ServiceCallNumber,*number_len);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfGetSOSCallNumber                              */
/**
 \brief       function for a request to get the SOS Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfGetSOSCallNumber(ATPhoneNumberType* number, Uint16Type* number_len)
{
    *number_len = _CONTEXT_MAN_u16SOSCallNumberLen > AT_MAX_PHONE_NUM_LEN
                        ? AT_MAX_PHONE_NUM_LEN : _CONTEXT_MAN_u16SOSCallNumberLen;
    (void)memcpy (&number[0],_CONTEXT_MAN_u8SOSCallNumber,*number_len);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfGetInfoCallNumberRoaming                              */
/**
 \brief       function for a request to get the Info Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfGetInfoCallNumberRoaming(ATPhoneNumberType* number, Uint16Type* number_len)
{
    *number_len = _CONTEXT_MAN_u16InfoCallNumberLenRoaming > AT_MAX_PHONE_NUM_LEN
                        ? AT_MAX_PHONE_NUM_LEN : _CONTEXT_MAN_u16InfoCallNumberLenRoaming;
    (void)memcpy (&number[0],_CONTEXT_MAN_u8InfoCallNumberRoaming,*number_len);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfGetServiceCallNumberRoaming                              */
/**
 \brief       function for a request to get the Service Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfGetServiceCallNumberRoaming(ATPhoneNumberType* number, Uint16Type* number_len)
{
    *number_len = _CONTEXT_MAN_u16ServiceCallNumberLenRoaming > AT_MAX_PHONE_NUM_LEN
                        ? AT_MAX_PHONE_NUM_LEN : _CONTEXT_MAN_u16ServiceCallNumberLenRoaming;
    (void)memcpy (&number[0],_CONTEXT_MAN_u8ServiceCallNumberRoaming,*number_len);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfSetInfoCallNumber                              */
/**
 \brief       function for a request to Set the Info Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfSetInfoCallNumber(Uint8Type* number, Uint8Type number_len)
{
    _CONTEXT_MAN_u16InfoCallNumberLen = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
    (void)memcpy (_CONTEXT_MAN_u8InfoCallNumber, number ,_CONTEXT_MAN_u16InfoCallNumberLen);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfSetServiceCallNumber                              */
/**
 \brief       function for a request to Set the Service Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfSetServiceCallNumber(Uint8Type* number, Uint8Type number_len)
{
    _CONTEXT_MAN_u16ServiceCallNumberLen = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
    (void)memcpy (_CONTEXT_MAN_u8ServiceCallNumber, number, _CONTEXT_MAN_u16ServiceCallNumberLen);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfSetSOSCallNumber                              */
/**
 \brief       function for a request to Set the SOS Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfSetSOSCallNumber(Uint8Type* number, Uint8Type number_len)
{
    _CONTEXT_MAN_u16SOSCallNumberLen = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
    (void)memcpy (_CONTEXT_MAN_u8SOSCallNumber,number, _CONTEXT_MAN_u16SOSCallNumberLen);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfSetInfoCallNumberRoaming                              */
/**
 \brief       function for a request to Set the Info Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfSetInfoCallNumberRoaming(Uint8Type* number, Uint8Type number_len)
{
    _CONTEXT_MAN_u16InfoCallNumberLenRoaming = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
    (void)memcpy (_CONTEXT_MAN_u8InfoCallNumberRoaming,number, _CONTEXT_MAN_u16InfoCallNumberLenRoaming);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfSetServiceCallNumberRoaming                              */
/**
 \brief       function for a request to Set the Service Call Number

 \param         number

 \param         number_len

 \return        SuccessFailType
 */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfSetServiceCallNumberRoaming(Uint8Type* number, Uint8Type number_len)
{
    _CONTEXT_MAN_u16ServiceCallNumberLenRoaming = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
    (void)memcpy (_CONTEXT_MAN_u8ServiceCallNumberRoaming,number, _CONTEXT_MAN_u16ServiceCallNumberLenRoaming);
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/

/* Function    : _CONTEXT_MAN_cbResponseCode                                                    */
/**
 \brief      to convert the aplication response codes into Diagnose response codes

 \brief

 \param      rsp
             APP_COMMON_enResponseCode

 \return     void
 */
/*----------------------------------------------------------------------------*/
static void _CONTEXT_MAN_cbResponseCode(APP_COMMON_enResponseCode rsp)
{
    ;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_vHandleEvents                                    */
/**
    \brief      Task Function to handle events for Telephone Application Task

    \brief      Description:
                Waits for incoming events

    \param      event
                EventMaskType
                Bitmask containing all unhandled events

    \return     void
*/
/*----------------------------------------------------------------------------*/
void CONTEXT_MAN_vHandleEvents(EventMaskType event)
{
    if( event & evAppTaskContaxtManHandleCalls )
    {
        (void)ClearEvent(evAppTaskContaxtManHandleCalls);
        _CONTEXT_MAN_vHandleCalls();
    }
}

/*----------------------------------------------------------------------------*/

/* Function    : _CONTEXT_MAN_cbResponseCodeCyclic                                                    */
/**
 \brief      to convert the aplication response codes into Diagnose response codes

 \brief

 \param      rsp
             APP_COMMON_enResponseCode

 \return     void
 */
/*----------------------------------------------------------------------------*/
static void _CONTEXT_MAN_cbResponseCodeCyclic(APP_COMMON_enResponseCode rsp)
{
    // do retry and in success case execute info call oder sos call oder service call.
    _CONTEXT_MAN_u8RetryTimeOut++;
    if ( (rsp == APP_COMMON_RSP_OK) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 ) )
    {
        _CONTEXT_MAN_u8RetryTimeOut = 0;
        _CONTEXT_MAN_enPendingWork = enContextManagerFnActiveCall;
    }
    //in case of not OK print out a debug msg
    if ( (rsp != APP_COMMON_RSP_OK)  )
    {
        DEBUG_VALUE2(CONTEXT_MAN_SWI_ERR, MOD_CONTEXT_MAN, "Received Error in cbResponseCodeCyclic : _CONTEXT_MAN_u8RetryTimeOut %d, rsp %d",
                                _CONTEXT_MAN_u8RetryTimeOut, rsp );
    }
}

/*----------------------------------------------------------------------------*/

/* Function    : _CONTEXT_MAN_cbResponseCodeCyclicHangup                                                    */
/**
 \brief      by Hang up to execute retries and in case of success forward to call execution (info, sos or service call)

 \param      rsp
             APP_COMMON_enResponseCode

 \return     void
 */
/*----------------------------------------------------------------------------*/
static void _CONTEXT_MAN_cbResponseCodeCyclicHangup(APP_COMMON_enResponseCode rsp)
{
    // do retry and in success case execute info call oder sos call oder service call.
    _CONTEXT_MAN_u8RetryTimeOut++;
    if ( (rsp == APP_COMMON_RSP_OK) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 ) )
    {
        _CONTEXT_MAN_u8RetryTimeOut = 0;
        switch ( _CONTEXT_MAN_enPendingWork )
        {
            //INFO call
            case enContextManagerFnInfoCall_Hangup:
                _CONTEXT_MAN_enPendingWork = enContextManagerFnInfoCall;
                break;
            // SOS call
            case enContextManagerFnSosCall_Hangup:
                _CONTEXT_MAN_enPendingWork = enContextManagerFnSosCall;
                break;
            // SERVICE call
            case enContextManagerFnServiceCall_Hangup:
                _CONTEXT_MAN_enPendingWork = enContextManagerFnServiceCall;
                break;
            // default
            default:
            _CONTEXT_MAN_u8RetryTimeOut = 0;
            _CONTEXT_MAN_enPendingWork = enContextManagerFnActiveCall;
        }
    }
    //set alarm back to original values after hang up all calls execution
    if( BT_TRUE == APP_COMMON_UTILS_bIsAlarmActive(CTA_CONTEXT_MAN_HANDLE_CALLS) )
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS, MSEC(250), MSEC(500));

    //in case of not OK print out a debug msg
    if ( (rsp != APP_COMMON_RSP_OK)  )
    {
        DEBUG_VALUE2(CONTEXT_MAN_SWI_ERR, MOD_CONTEXT_MAN, "Received Error in cbResponseCodeCyclicHangup : _CONTEXT_MAN_u8RetryTimeOut %d, rsp %d",
                        _CONTEXT_MAN_u8RetryTimeOut, rsp );
    }
}
/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfInit                                                    */
/**
    \brief      initialise Mute manager to default

    \brief      this function initialise all Mute manager variables back to\n
                the specific default settings.

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfInit(void)
{
    SuccessFailType sF              = SUCCESS;
    _CONTEXT_MAN_enFahrzeugTyp = APP_CAN_enGetMarke();
    _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_TRUE;
    return sF;
}
/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MANAGER_sfResolveFunctionCall                                                    */
/**
    \brief      Call corresponding application function according to the Context.
                Call Accept
                Call Hang
                Call Reject
    \brief      Button Press BT: do nothing.
                Button Press Mic Mute: only activate/dactivate during active call.

                AT_CS_ACTIVE_CALL      = 0,
                AT_CS_HELD_CALL        = 1,
                AT_CS_OUTCALL_DIAL_IPR ( dial in progress ) = 2,
                AT_CS_OUTCALL_RING_IPR ( ringing in progress ) = 3,
                AT_CS_INCOMING         = 4,
                AT_CS_WAITING_CALL     = 5,
                AT_CS_CALL_TERMINATED  = 6

                HangUp
                This command allows to :
                - Reject an incoming call.
                - Hang up the active call.
                - Hang up the held call during a three-way call, if it is supported by the phone.

                Button Press VCR: call corresponding function according to if system
                1. is in active call or not.
                2. is the button mode configured for dual functions of the button by the diagnose

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MANAGER_sfResolveFunctionCall(LOGI_BUT_MAN_ENUM_BUTTON_NAMES enButton, LOGI_BUT_MAN_ENUM_PRESSTYPE enPressType)
{
    APP_COMMON_enResponseCode res = APP_COMMON_RSP_OK;
    /* TODO : to analyse the response from phone function calls like Hangup and DialNumber()*/
    Uint8Type u8RooflineMode;
    static BooleanType InitDone = BT_FALSE;
    const ATRspCallStatValType *plcc1 = APP_PHONE_AT_PNCC_pGetPlccContext(AT_CALL_INDEX_0);
    const ATRspCallStatValType *plcc2 = APP_PHONE_AT_PNCC_pGetPlccContext(AT_CALL_INDEX_1);
    const ATRspCallStatValType *activePlcc = NULL;
    const ATRspCallStatValType *heldPlcc = NULL;
    const ATRspCallStatValType *unknownPlcc = NULL;
    LOGI_BUT_MAN_ENUM_BUTTON_NAMES enPendingWork = enLogiButManNoButton;
    //DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ResolveFunctionCall: Call 1 %d, Call 2: %d", plcc1->state, plcc2->state);
    DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ResolveFunctionCall: Button: %d Press Type: %d",
                       enButton, enPressType);

    //RT #5424
    switch(enButton)
    {
        case CONTEXT_MAN_MFL_HOOK_BUTTON:
        case CONTEXT_MAN_LSS_DOWN_BUTTON:
        case CONTEXT_MAN_LSS_UP_BUTTON:
            break;// Never Block
        default:
            if( APP_CSHDL_eGetCradleRestrictionState() == APP_CSHDL_CRADLE_RESTRICTION_ON)
            {
                DEBUG_TEXT(CONTEXT_MAN_SWI_WNG, MOD_CONTEXT_MAN, "Alle buttons sperren" );
                return SUCCESS;
            }
            break;
    }

    if ( InitDone == BT_FALSE )
    {
        Uint8Type number[9];
        Uint8Type number_len = ID_NUMBERS_INFO_SERVICE_SOS_CALL_COMMON_MAX_LEN;

        EEPROM_enGetByteSequenceFromRamMirror(number, number_len, EEPROM_LAYOUT_ENUM_SetNumberRoamingForInfoCall);
        _CONTEXT_MAN_u16InfoCallNumberLenRoaming = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
        (void)memcpy (_CONTEXT_MAN_u8InfoCallNumberRoaming,number, _CONTEXT_MAN_u16InfoCallNumberLenRoaming);

        EEPROM_enGetByteSequenceFromRamMirror(number, number_len, EEPROM_LAYOUT_ENUM_SetNumberRoamingForServiceCall);
        _CONTEXT_MAN_u16ServiceCallNumberLenRoaming = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
        (void)memcpy (_CONTEXT_MAN_u8ServiceCallNumberRoaming,number, _CONTEXT_MAN_u16ServiceCallNumberLenRoaming);

        EEPROM_enGetByteSequenceFromRamMirror(number, number_len, EEPROM_LAYOUT_ENUM_SetNumberForInfoCall);
        _CONTEXT_MAN_u16InfoCallNumberLen = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
        (void)memcpy (_CONTEXT_MAN_u8InfoCallNumber, number ,_CONTEXT_MAN_u16InfoCallNumberLen);

        EEPROM_enGetByteSequenceFromRamMirror(number, number_len, EEPROM_LAYOUT_ENUM_SetNumberForServiceCall);
        _CONTEXT_MAN_u16ServiceCallNumberLen = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
        (void)memcpy (_CONTEXT_MAN_u8ServiceCallNumber, number, _CONTEXT_MAN_u16ServiceCallNumberLen);

        EEPROM_enGetByteSequenceFromRamMirror(number, number_len, EEPROM_LAYOUT_ENUM_EmergencyNumber);
        _CONTEXT_MAN_u16SOSCallNumberLen = _CONTEXT_MAN_u16TransferPhoneNumberToASCII (number,number_len);
        (void)memcpy (_CONTEXT_MAN_u8SOSCallNumber,number, _CONTEXT_MAN_u16SOSCallNumberLen);

        InitDone = BT_TRUE;
    }
    if( (plcc1!=NULL) && (plcc2!=NULL) )
    {
        /* we have callStates on both lines */
        if(plcc1->state == AT_CS_ACTIVE_CALL)
        {
            activePlcc  = plcc1;
            unknownPlcc = plcc2;
        }
        else if(plcc2->state == AT_CS_ACTIVE_CALL)
        {
            activePlcc  = plcc2;
            unknownPlcc = plcc1;
        }
        else if(plcc1->state == AT_CS_HELD_CALL)
        {
            /* no active plcc available */
            heldPlcc    = plcc1;
            unknownPlcc = plcc2;
        }
        else if(plcc2->state == AT_CS_HELD_CALL)
        {
            heldPlcc    = plcc2;
            unknownPlcc = plcc1;
        }
        else
        {
            activePlcc  = NULL;
            heldPlcc    = NULL;
            unknownPlcc = NULL;
        }
    }

    switch(enButton)
    {
        case CONTEXT_MAN_MFL_BACK_BUTTON:
        {
            if ( BT_FALSE == KPB_bIsAsgAvailable())
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON;
            }
            else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda)
            {
                DEBUG_STATE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "$LOGI_BUT_MAN_ENUM_BUTTON_NAMES$ %d->SD,%d",enButton, enPressType);
                APP_SD_vOnMflButton(enButton, enPressType);
            }
            else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_VW_Volkswagen)
            {
                DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "VW,BACK:Do nothing");
            }
            break;
        }
        case  LOGI_BUT_MAN_3BM_BTVISIBLE_BUTTON:
        {
            EEPROM_LAYOUT_vGetRooflineMode(&u8RooflineMode);
            if ( u8RooflineMode == APP_DIAG_ROOFLINE_MODULE_TYPE_NAR)
            {
                if (enPressType == enLogiButManShortPress)
                {
                    DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->ERM,%d",enButton, enPressType);
                    ERM_vCb3BM_BTVisibility();
                }
                else if (enPressType == enLogiButManLongPressTenSeconds)
                {
                    DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "BT 10Seconds long press");
                    CONTEXT_MAN_cDeleteAllProfiles();
                }
            }
            else if ( u8RooflineMode == APP_DIAG_ROOFLINE_MODULE_TYPE_RDW)
            {
                if ( enPressType == enLogiButManShortPressAtSingleFunction ||
                        ( enPressType == enLogiButManLongPress ))
                {
                    if (CONTEXT_MAN_bInfoCallInProgress == BT_TRUE )
                    {
                        enButton = LOGI_BUT_MAN_NO_BUTTON;
                    }
                    else
                    {
                        enButton = LOGI_BUT_MAN_CRADLE_INFO_BUTTON ;
                    }
                }
            }
            break;
        }
        case LOGI_BUT_MAN_3BM_MICMUTE_BUTTON:
        {
            EEPROM_LAYOUT_vGetRooflineMode(&u8RooflineMode);
            if ( u8RooflineMode == APP_DIAG_ROOFLINE_MODULE_TYPE_NAR)
            {
                if ( plcc1 != NULL ||
                     plcc2 != NULL
                     )
                {
                    if ( enPressType == enLogiButManShortPress )
                    {
                        DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "$LOGI_BUT_MAN_ENUM_BUTTON_NAMES$ %d->PH,%d",enButton, enPressType);
                        APP_PHONE_vCbMicMute();
                    }
                    else if ( enPressType == enLogiButManLongPress )
                    {
                        DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "LongMicPressed: Do nothing");
                    }
                }
                else if ( ( enPressType == enLogiButManShortPress ) && ( enPressType == enLogiButManLongPress ) )
                {
                    DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Do nothing. All calls are terminated.");
                }
            }
            else if ( u8RooflineMode == APP_DIAG_ROOFLINE_MODULE_TYPE_RDW)
            {
                if ( enPressType == enLogiButManShortPressAtSingleFunction ||
                        ( enPressType == enLogiButManLongPress ))
                {
                    if (CONTEXT_MAN_bSosCallInProgress == BT_TRUE )
                    {
                        enButton = LOGI_BUT_MAN_NO_BUTTON;
                    }
                    else
                    {
                        enButton = LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON ;
                    }
                }
            }
            break;
        }
        case LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON:
        case LOGI_BUT_MAN_CRADLE_BOTH_BUTTON:
        {
            if ( enPressType == enLogiButManShortPress)
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON;
            }
            else
            {
                if (CONTEXT_MAN_bSosCallInProgress == BT_TRUE )
                {
                    enButton = LOGI_BUT_MAN_NO_BUTTON;
                }
                else
                {
                    enButton = LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON ;
                }
            }
            break;
        }
        case LOGI_BUT_MAN_CRADLE_INFO_BUTTON:
        {
            if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda)
            {
                // The Skoda Info button has the same functionality as 3BM Ptt button,.
                enButton = LOGI_BUT_MAN_3BM_PTT_BUTTON;
            }
            else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_VW_Volkswagen)
            {
                if ( enPressType == enLogiButManShortPress)
                {
                    enButton = LOGI_BUT_MAN_3BM_PTT_BUTTON ;
                }
                else
                {
                    if (CONTEXT_MAN_bInfoCallInProgress == BT_TRUE )
                    {
                        enButton = LOGI_BUT_MAN_NO_BUTTON;
                    }
                    else
                    {
                        enButton = LOGI_BUT_MAN_CRADLE_INFO_BUTTON ;
                    }
                }

            }
            break;
        }
        case CONTEXT_MAN_MFL_HOOK_BUTTON:
        {
            _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
            if ( plcc1 != NULL ||
                 plcc2 != NULL
                 )  // the handling when a call is in progress is the same for both Hook and 3BM.
                // only in the case, when the context is not telephone, the hook button shrt press should reactivate the telephone context
                // but the 3BM should hang up the call. hook long press should do nothing.
            {
                //DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->%d",enButton, enPressType);
                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ResolveFunctionCall: Call 1 %d, Call 2: %d", plcc1->state, plcc2->state);
                if ( enPressType == enLogiButManShortPress )
                {
                      if((KPB_bIsAsgPresentation() != BT_TRUE))
                      {
                        if ( BT_FALSE == KPB_bIsAsgAvailable())
                        {
                            enButton = LOGI_BUT_MAN_3BM_PTT_BUTTON;
                        }
                        else
                        {
                            DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                            APP_SD_vOnMflButton(enButton, enPressType);
                              enButton = LOGI_BUT_MAN_NO_BUTTON;
                        }
                      }
                      else  // WE are in presentation.
                      {
                        enButton = LOGI_BUT_MAN_3BM_PTT_BUTTON;
                      }
                }
                else if ( enPressType == enLogiButManLongPress ) // Longpress Hook
                {
                    if((KPB_bIsAsgPresentation() != BT_TRUE))
                    {
                        if ( BT_FALSE == KPB_bIsAsgAvailable())
                        {
                            enButton = LOGI_BUT_MAN_3BM_PTT_BUTTON;
                        }
                        else
                        {
                            enButton = LOGI_BUT_MAN_NO_BUTTON;
                        }
                    }
                    else  // WE are in presentation.
                    {
                        enButton = LOGI_BUT_MAN_3BM_PTT_BUTTON;
                    }
                }
            }
            else   // All calls are terminated
            {
                if ( enPressType == enLogiButManShortPress )
                {
                    if((osGetSystemCounter() - _CONTEXT_MAN_DoubleClick_StartTime) < CONTEXT_MAN_TIMEOUT_500MS)
                    {
                        if (BT_FALSE == ERM_bPhoneAvailable())
                        {
                            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "DoubleClick detected, no phone connected");
                            ERM_vCb3BM_BTVisibility();
                        }
                    }
                    else
                    {
                        _CONTEXT_MAN_DoubleClick_StartTime = osGetSystemCounter();
                        if (( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda) &&
                            (APP_CAN_MFL_VarianteLow == APP_CAN_u8GetMflVariante()))
                        {
                            if (BT_TRUE == VCR_bCheckSIVRIsActive())
                            {
                                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->VCR,%d",enButton, enPressType);
                                VCR_vCb3BM_PTT();
                            }
                            else if ( BT_TRUE == KPB_bIsAsgAvailable())
                            {
                                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                                APP_SD_vOnMflButton(enButton, enPressType);
                            }
                        }
                        else if (BT_TRUE == VCR_bCheckSIVRIsActive())
                        {
                            if ((BT_FALSE == KPB_bIsAsgPresentation()) &&
                                (BT_TRUE == KPB_bIsAsgAvailable()))
                            {
                                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                                APP_SD_vOnMflButton(enButton, enPressType);
                            }
                            else
                            {
                                DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "SIVR is active: VCR_vEventCall()");
                                VCR_vEventCall();
                            }
                        }
                        else if ( BT_TRUE == KPB_bIsAsgAvailable())
                        {
                            DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                            APP_SD_vOnMflButton(enButton, enPressType);
                        }
                    }
                }
                else if ( enPressType == enLogiButManLongPress )
                {
                    if (( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda) &&
                        (APP_CAN_MFL_VarianteLow == APP_CAN_u8GetMflVariante()))
                    {
                        DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->VCR,%d",enButton, enPressType);
                        VCR_vCb3BM_PTT();
                    }
                    else
                    {
                        if(AT_INVALID_DEVICE_ID != APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_PHONE))
                        {
                            DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Redial=%d", res);
                            res =  APP_PHONE_rcRedial(_CONTEXT_MAN_cbResponseCode );
                        }
                        else if ( BT_TRUE == KPB_bIsAsgAvailable())
                        {
                            /* Idle Screen is active and currently no phone connected but in current driving cycle */
                            if(BT_TRUE == APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ACTIVE_PHONE))
                            {
                                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                                APP_SD_vOnMflButton(enButton, enPressType);
                            }
                        }
                        else
                        {
                            if (BT_TRUE == _CONTEXT_MAN_phoneActivatedOnce)
                            {
                                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ResolveFunctionCall: reconnect last active device %d->%d",enButton, enPressType);
                                _CONTEXT_MAN_u8RetryTimeOut = 0;
                                (void)_CONTEXT_MAN_rcReconnectLastUsedDevice();
                            }
                            else
                            {
                                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ResolveFunctionCall: do nothing %d->%d",enButton, enPressType);
                            }
                        }
                    }
                }
                /* Idle Screen is active and no phone connected in the current driving cycle */
                else if ( enPressType == enLogiButManLongPressTenSeconds )
                {
                    CONTEXT_MAN_cDeleteAllProfiles();
                }
            }
            break;
        }
        case CONTEXT_MAN_LSS_UP_BUTTON:
        {
            if ( (BT_FALSE == KPB_bIsAsgAvailable()) || (KPB_bIsAsgPresentation() == BT_FALSE))
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON;
            }
            else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda)
            {
                if ((enPressType == enLogiButManLongPress)||(enPressType == enLogiButManLongPressRepeat) )
                {
                    DEBUG_STATE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "$LOGI_BUT_MAN_ENUM_BUTTON_NAMES$ %d->SD,%d",enButton, enPressType);
                    APP_SD_vOnMflButton(enButton, enPressType);// send LSS Info for Rücksprung ins haupt menü
                }
                else if ( enPressType == enLogiButManShortPress)
                {
                    enPressType = enLogiButManShortPressAtSingleFunction; // i have to simulate because the
                    // mfl up button is mapped to a wiederhol function type
                    enButton = CONTEXT_MAN_MFL_UP_BUTTON; // All other presses are identical to MFL UP.
                }
            }
            else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_VW_Volkswagen)
            {
                if ( (enPressType == enLogiButManLongPress) || (enPressType == enLogiButManLongPressRepeat) )
                {
                    /* Maennel changed for CR-E74: Leave Call and Idle Screen via LSS */
                    if(((APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ALL) == BT_TRUE)\
                            ||(APP_SD_bIsCallScreenActive() == BT_TRUE))\
                            && ((enPressType == enLogiButManLongPress)||(enPressType == enLogiButManLongPressRepeat)))
                    {
                        DEBUG_STATE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "$LOGI_BUT_MAN_ENUM_BUTTON_NAMES$ %d->SD,%d",enButton, enPressType);
                        APP_SD_vOnMflButton(enButton, enLogiButManLongPress);// send LSS Info for Rücksprung ins haupt menü
                    }
                    else
                    {
                        enPressType = enLogiButManLongPress;
                        enButton = CONTEXT_MAN_MFL_UP_BUTTON; // when not in idle screen, send mfl up.
                    }

                }
                if ( (enPressType == enLogiButManShortPress) || (enPressType == enLogiButManShortPressAtSingleFunction) )
                {
                    if((APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ALL) == BT_TRUE) && (enPressType == enLogiButManShortPressAtSingleFunction))
                    {
                        enButton = LOGI_BUT_MAN_NO_BUTTON; // when not in idle screen, send mfl up.
                    }
                    else if((APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ALL) == BT_TRUE) && (enPressType == enLogiButManShortPress))
                    {
                        enPressType = enLogiButManShortPressAtSingleFunction;// i have to simulate because the
                        // mfl up button is mapped to a wiederhol function type
                        enButton = CONTEXT_MAN_MFL_UP_BUTTON; // when not in idle screen, send mfl up.
                    }
                    else if ( enPressType == enLogiButManShortPressAtSingleFunction)
                    {
                        enPressType = enLogiButManShortPressAtSingleFunction;// i have to simulate because the
                        // mfl up button is mapped to a wiederhol function type
                        enButton = CONTEXT_MAN_MFL_UP_BUTTON; // when not in idle screen, send mfl up.
                    }
                }
            }
            break;
        }
        case CONTEXT_MAN_LSS_DOWN_BUTTON:
        {
            if ( (BT_FALSE == KPB_bIsAsgAvailable()) || (KPB_bIsAsgPresentation() == BT_FALSE))
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON;
            }
            else if ( (enPressType == enLogiButManLongPress) || (enPressType == enLogiButManLongPressRepeat) )
            {
                /* Maennel changed for CR-E74: Leave Call and Idle Screen via LSS */
                if(((APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ALL) == BT_TRUE)\
                        ||(APP_SD_bIsCallScreenActive() == BT_TRUE))\
                        && (enPressType == enLogiButManLongPress))
                {
                    DEBUG_STATE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "$LOGI_BUT_MAN_ENUM_BUTTON_NAMES$ %d->SD,%d",enButton, enPressType);
                    APP_SD_vOnMflButton(enButton, enLogiButManLongPress);// send LSS Info for Rücksprung ins haupt menü
                }
                else
                {
                    enPressType = enLogiButManLongPress;
                    enButton = CONTEXT_MAN_MFL_DOWN_BUTTON; // when not in idle screen, send mfl down.
                }
            }
            else if ( (enPressType == enLogiButManShortPress) || (enPressType == enLogiButManShortPressAtSingleFunction) )
            {
                if((APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ALL) == BT_TRUE) && (enPressType == enLogiButManShortPressAtSingleFunction))
                {
                    enButton = LOGI_BUT_MAN_NO_BUTTON; // when not in idle screen, send mfl up.
                }
                else if((APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ALL) == BT_TRUE) && (enPressType == enLogiButManShortPress))
                {
                    enPressType = enLogiButManShortPressAtSingleFunction;// i have to simulate because the
                    // mfl up button is mapped to a wiederhol function type
                    enButton = CONTEXT_MAN_MFL_DOWN_BUTTON; // when not in idle screen, send mfl up.
                }
                else if ( enPressType == enLogiButManShortPressAtSingleFunction)
                {
                    enPressType = enLogiButManShortPressAtSingleFunction;// i have to simulate because the
                    // mfl up button is mapped to a wiederhol function type
                    enButton = CONTEXT_MAN_MFL_DOWN_BUTTON; // when not in idle screen, send mfl up.
                }
            }
        }
            break;
        case CONTEXT_MAN_LSS_OK_BUTTON:
        {
            // The LSS Functionality for OK is Doppel Function
            if ( (BT_FALSE == KPB_bIsAsgAvailable()) || (KPB_bIsAsgPresentation() == BT_FALSE))
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON; //NO press forwarded to app.
            }
            else if ( enPressType == enLogiButManLongPress)
            {
                if((APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_ALL) == BT_TRUE))
                {
                    enButton = LOGI_BUT_MAN_NO_BUTTON; // when not in idle screen, do not do anything.
                }
                else
                {
                    DEBUG_STATE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "$LOGI_BUT_MAN_ENUM_BUTTON_NAMES$ %d->SD,%d",enButton, enPressType);
                    APP_SD_vOnMflButton(enButton, enPressType);// send LSS Info for Rücksprung ins IDLE SCREEN
                }
            }
            else if ( enPressType == enLogiButManShortPress)
            {
                if((_CONTEXT_MAN_bLastScreenWasNotTelephone == BT_TRUE))
                {
                    enButton = LOGI_BUT_MAN_NO_BUTTON;
                    _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
                    DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ThrowOK");
                    /* The LSS has a menu structure. When telephone is selected in this menu structure then
                     * the first button press will be evaluated from the kombi instrument and the button release will
                     * be evaluated from APP_SD. This leads to a problem that two button presses will be evaulated.
                     * To avoid this the variable _CONTEXT_MAN_bLastScreenWasNotTelephone is added which ignores the
                     * button release. */
                }
                else
                {
                    enButton = CONTEXT_MAN_MFL_OK_BUTTON; // All other presses are identical to MFL OK.
                    DEBUG_STATE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "$LOGI_BUT_MAN_ENUM_BUTTON_NAMES$ %d->SD,%d",enButton, enPressType);
                    APP_SD_vOnMflButton(enButton, enPressType);
                    enButton = LOGI_BUT_MAN_NO_BUTTON; // we do not want to get in the mfl func again coded below.
                }
            }
            else if ( (enPressType == enLogiButManShortPressAtSingleFunction)  )
            {
                Uint32Type timeout = 0;
                if (_CONTEXT_MAN_enPendingWork == enWaitToAvoidDoubleOkPress)
                {
                    if( APP_COMMON_UTILS_stGetTimeoutLeft( CTA_CONTEXT_MAN_HANDLE_CALLS, &timeout ) != E_OK)
                    {
                        DEBUG_TEXT(CONTEXT_MAN_SWI_ERR, MOD_CONTEXT_MAN, "Wrongtimervalue");
                    }
                    else
                    {
                        DEBUG_VALUE1(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "TimerVal = %d", timeout);
                        if ( timeout > 1900 )
                        {
                            _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
                            DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "Illiegal Pos Flanke");
                        }
                        else
                        {
                            _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
                            _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
                            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "DoNotThrowOK");

                        }
                    }
                }
                else
                {
                    if ( _CONTEXT_MAN_enPendingWork != enContextManagerFnNoActiveCall)
                    {
                        _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
                    }
                    DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "NormalSequence");
                }
            }
            break;
        }
        case CONTEXT_MAN_ROLL_BUTTON_UP:
        {
            if ( (BT_FALSE == KPB_bIsAsgAvailable()) || (KPB_bIsAsgPresentation() == BT_FALSE))
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON; //NO press forwarded to app.
            }
            else
            {
                enButton = CONTEXT_MAN_MFL_UP_BUTTON; // All  presses are identical to MFL UP.
                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Roll Up %d->SD,%d",enButton, enPressType);
                APP_SD_vOnMflButton(enButton, enPressType);
            }
            enButton = LOGI_BUT_MAN_NO_BUTTON; //NO press forwarded to app.
            break;
        }
        case CONTEXT_MAN_ROLL_BUTTON_DOWN:
        {
            if ( (BT_FALSE == KPB_bIsAsgAvailable()) || (KPB_bIsAsgPresentation() == BT_FALSE))
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON; //NO press forwarded to app.
            }
            else
            {
                enButton = CONTEXT_MAN_MFL_DOWN_BUTTON; // All presses are identical to MFL DOWN.
                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Roll Down %d->SD,%d",enButton, enPressType);
                APP_SD_vOnMflButton(enButton, enPressType);
            }
            enButton = LOGI_BUT_MAN_NO_BUTTON; //NO press forwarded to app.
            break;
        }
        case CONTEXT_MAN_MFL_PTT:
        {
            if ( BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle())
            {
                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN,"%d->VCR,%d",enButton, enPressType);
                VCR_vCb3BM_PTT();
            }
            break;
        }
        default:
            break;
    }
    switch(enButton)
    {
        case LOGI_BUT_MAN_3BM_PTT_BUTTON:
        {
            CONTEXT_MAN_bInfoCallInProgress = BT_FALSE;
            CONTEXT_MAN_bSosCallInProgress = BT_FALSE;
            if ( BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()) // call in progress
            {
                DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Call handling");
                if( (plcc1!=NULL) && (plcc2!=NULL) )
                {
                    DEBUG_VALUE4(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "Active %d, Held :%d Unknown :%d %d", activePlcc, heldPlcc, unknownPlcc, ' ');
                    if(activePlcc != NULL)
                    {
                        DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "Active: More than 1 call");
                        switch(unknownPlcc->state)
                        {
                            case AT_CS_ACTIVE_CALL:
                                if ( enPressType == enLogiButManShortPress )
                                {
                                    if ( activePlcc->state == AT_CS_ACTIVE_CALL )
                                    {
                                        res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode,(ATCallHangupEnum)AT_HANGUP_NO_INDEX );
                                        DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang Up Conference = %d",res);
                                    }
                                    else
                                    {
                                        res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)unknownPlcc->index);
                                        DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang Up Outgoing Call = %d",res);
                                    }
                                }
                                else if ( enPressType == enLogiButManLongPress )
                                {
                                    if( BT_FALSE == APP_PHONE_bIsHandsFreeOff())
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_DISCRETE_MODE );
                                    }
                                    else
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_HANDSFREE_MODE );
                                    }
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "AG to HF Routing = %d", res);
                                }
                                break;
                            case AT_CS_HELD_CALL:
                                // one call is on hold and the other is active.
                                if ( enPressType == enLogiButManShortPress )
                                {
                                    res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)activePlcc->index);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang up Active Call = %d", res);
                                }
                                else if ( enPressType == enLogiButManLongPress )
                                {
                                    if( BT_FALSE == APP_PHONE_bIsHandsFreeOff())
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_DISCRETE_MODE );
                                    }
                                    else
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_HANDSFREE_MODE );
                                    }
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "AG to HF Routing = %d",res);
                                }
                                break;
                            case AT_CS_WAITING_CALL:
                                /* we have one waiting call */
                                if ( enPressType == enLogiButManShortPress )
                                {
                                    res = APP_PHONE_rcAcceptCall(_CONTEXT_MAN_cbResponseCode);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Incoming Call Accepted = %d",res);
                                }
                                else if ( enPressType == enLogiButManLongPress )
                                {
                                    res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)unknownPlcc->index);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang up Active Call = %d",res);
                                }
                                break;

                            default:
                                /* ignore other states */
                                break;
                        }
                    }
                    else if(heldPlcc != NULL)
                    {
                        DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "Held: More than 1 call");
                        switch(unknownPlcc->state)
                        {
                            case AT_CS_HELD_CALL:
                                /* held conference */
                                if ( enPressType == enLogiButManShortPress )
                                {
                                    res = APP_PHONE_rcResumeCall(_CONTEXT_MAN_cbResponseCode);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Resume Conf = %d", res);
                                }
                                else if ( enPressType == enLogiButManLongPress )
                                {
                                    if( BT_FALSE == APP_PHONE_bIsHandsFreeOff())
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_DISCRETE_MODE );
                                    }
                                    else
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_HANDSFREE_MODE );
                                    }
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "AG to HF Routing = %d", res);
                                }
                                break;
                            case AT_CS_OUTCALL_DIAL_IPR:
                            case AT_CS_OUTCALL_RING_IPR:
                                if ( enPressType == enLogiButManShortPress )
                                {
                                    res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)unknownPlcc->index);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang up Active Call = %d",res);
                                }
                                else if ( enPressType == enLogiButManLongPress )
                                {
                                    if( BT_FALSE == APP_PHONE_bIsHandsFreeOff())
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_DISCRETE_MODE );
                                    }
                                    else
                                    {
                                        res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_HANDSFREE_MODE );
                                    }
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "AG to HF Routing = %d", res);
                                }
                                break;
                            case AT_CS_ACTIVE_CALL:
                            {
                                if ( enPressType == enLogiButManShortPress )
                                {
                                    res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)unknownPlcc->index);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang up Active Call = %d",res);
                                }
                                break;
                            }
                            case AT_CS_INCOMING:
                            case AT_CS_WAITING_CALL:
                                /* we have one waiting call */
                                if ( enPressType == enLogiButManShortPress )
                                {
                                    res = APP_PHONE_rcAcceptCall(_CONTEXT_MAN_cbResponseCode);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Incoming Call Accepted = %d", res);
                                }
                                else if ( enPressType == enLogiButManLongPress )
                                {
                                    res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)unknownPlcc->index);
                                    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang up Active Call = %d",res);
                                }
                                break;
                            case AT_CS_CALL_TERMINATED:
                            default:
                                /* ignore other states */
                                break;
                        }
                    }
                }
                else if ( (plcc1 != NULL) || (plcc2 != NULL) )
                {
                    /* we have callState on one line */
                    const ATRspCallStatValType *plcc = (plcc1 != NULL) ? plcc1 : plcc2;
                    DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "Only 1 Call");
                    switch(plcc->state)
                    {
                        case AT_CS_ACTIVE_CALL:
                        case AT_CS_OUTCALL_DIAL_IPR:
                        case AT_CS_OUTCALL_RING_IPR:
                            if ( enPressType == enLogiButManShortPress )
                            {
                                res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)plcc->index);
                                DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Hang up Active Call = %d",res);
                            }
                            else if ( enPressType == enLogiButManLongPress )
                            {
                                if( BT_FALSE == APP_PHONE_bIsHandsFreeOff())
                                {
                                    res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_DISCRETE_MODE );
                                }
                                else
                                {
                                    res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_HANDSFREE_MODE );
                                }
                                DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "AG to HF Routing = %d",res);
                            }
                            break;
                        case AT_CS_CALL_TERMINATED:
                            break;
                        case AT_CS_INCOMING:
                            if ( enPressType == enLogiButManShortPress )
                            {
                                res = APP_PHONE_rcAcceptCall(_CONTEXT_MAN_cbResponseCode);
                                DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Incoming Call Accepted = %d",res);
                            }
                            else if ( enPressType == enLogiButManLongPress )
                            {
                                res = APP_PHONE_rcHangUp(_CONTEXT_MAN_cbResponseCode, (ATCallHangupEnum)plcc->index);
                                DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Incoming Call Rejected = %d",res);
                            }
                            break;
                        case AT_CS_HELD_CALL:
                            if ( enPressType == enLogiButManShortPress )
                            {
                                res = APP_PHONE_rcResumeCall( NULL );
                                DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Held Call Resumed = %d",res);
                            }
                            else if ( enPressType == enLogiButManLongPress )
                            {
                                if( BT_FALSE == APP_PHONE_bIsHandsFreeOff())
                                {
                                    res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_DISCRETE_MODE );
                                }
                                else
                                {
                                    res = APP_PHONE_rcHandsFreeOnOff( _CONTEXT_MAN_cbResponseCode, AT_AUDIO_HANDSFREE_MODE );
                                }
                                DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "AG to HF Routing = %d",res);
                            }
                            break;
                        default:
                            DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "sfHandlePhoneEvent: plcc state %d ignored",
                                         plcc->state);
                            break;
                    }
                }
            }
            else// call not in progress or telefon not active.
            {
                DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "No call active");
                if ( enPressType == enLogiButManShortPress )
                {
                    DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->VCR,%d",enButton, enPressType);
                    VCR_vCb3BM_PTT();
                }
                else if ( enPressType == enLogiButManLongPress )
                {
                    DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Long PTT Pressed: Do nothing");
                }
            }
            break;
        }
        case LOGI_BUT_MAN_CRADLE_INFO_BUTTON:
        case LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON:
        {
            if ( AT_INVALID_DEVICE_ID  == ERM_u8GetActivePhone() )
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON;
            }
            else if ( enPressType == enLogiButManShortPressAtSingleFunction)
            {
                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
                if ((enButton == LOGI_BUT_MAN_CRADLE_INFO_BUTTON) )
                {
                    APP_SD_vOnMflButton(LOGI_BUT_MAN_CRADLE_INFO_CALL, enPressType); // call APP_SD function to display message.
                }
                else if ( enButton == LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON )
                {
                    if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_VW_Volkswagen)
                    {
                        APP_SD_vOnMflButton(LOGI_BUT_MAN_CRADLE_SERVICE_CALL, enPressType); // call APP_SD function to display message.
                    }
                    else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda)
                    {
                        APP_SD_vOnMflButton(LOGI_BUT_MAN_CRADLE_SOS_CALL, enPressType); // call APP_SD function to display message.
                    }
                }
                break;
            }
            else
            {
                if ( BT_FALSE == APP_PHONE_AT_PNCC_bAllCallStatesIdle()) // call in progress
                {
                    DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Call handling");
                    DEBUG_VALUE4(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "Active %d, Held :%d Unknown :%d More than 1 call", activePlcc, heldPlcc, unknownPlcc, ' ');
                    enPendingWork = enButton;
                }
                else// call not in progress or telefon not active.
                {
                    DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "No call active");
                    switch(enButton)
                    {
                    case LOGI_BUT_MAN_CRADLE_INFO_BUTTON:
                        if ( enPressType == enLogiButManLongPress )
                        {
                            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "LongInfoPressed: Call InfoRuf");
                            enPendingWork = enButton;
                        }
                        break;
                    case LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON:
                        if ( enPressType == enLogiButManLongPress )
                        {
                            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "LongSosPressed: Call Sosruf");
                            enPendingWork = enButton;
                        }
                        break;
                    default:
                        break;
                    }
                }
                switch ( enPendingWork )
                {
                    case LOGI_BUT_MAN_CRADLE_INFO_BUTTON:
                        APP_SD_vOnMflButton(LOGI_BUT_MAN_CRADLE_INFO_CALL, enPressType); // call APP_SD function to display message.
                        _CONTEXT_MAN_enPendingWork = enContextManagerFnInfoCall_Hangup;
                        _CONTEXT_MAN_u8RetryTimeOut = 0;
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS, MSEC(250), MSEC(APP_PHONE_TIMEOUT_HANGUP_CALL));
                        break;
                    case LOGI_BUT_MAN_CRADLE_SOS_SERVICE_BUTTON:
                        if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_VW_Volkswagen)
                        {
                            APP_SD_vOnMflButton(LOGI_BUT_MAN_CRADLE_SERVICE_CALL, enPressType); // call APP_SD function to display message.
                            _CONTEXT_MAN_enPendingWork = enContextManagerFnServiceCall_Hangup;
                        }
                        else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda)
                        {
                            APP_SD_vOnMflButton(LOGI_BUT_MAN_CRADLE_SOS_CALL, enPressType); // call APP_SD function to display message.
                            _CONTEXT_MAN_enPendingWork = enContextManagerFnSosCall_Hangup;
                        }
                        _CONTEXT_MAN_u8RetryTimeOut = 0;
                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS, MSEC(250), MSEC(APP_PHONE_TIMEOUT_HANGUP_CALL));
                        break;
                    default:
                        DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Cancel the Alarm");
                        (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
                        break;
                }
            }
            break;
        }
        case CONTEXT_MAN_MFL_UP_BUTTON:
        case CONTEXT_MAN_MFL_DOWN_BUTTON:
        {
            if ( (BT_FALSE == KPB_bIsAsgAvailable()) || (KPB_bIsAsgPresentation() == BT_FALSE))
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON; //NO press forwarded to app.
            }
            else if ( enPressType == enLogiButManShortPressAtSingleFunction )
            {
                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                APP_SD_vOnMflButton(enButton, enLogiButManShortPress);
            }
            else if ( (enPressType == enLogiButManLongPress) || (enPressType == enLogiButManLongPressRepeat) )
            {
                DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                APP_SD_vOnMflButton(enButton, enLogiButManLongPress);
            }
            break;
        }
        case CONTEXT_MAN_MFL_OK_BUTTON:
        {
            if ( (BT_FALSE == KPB_bIsAsgAvailable()) || (KPB_bIsAsgPresentation() == BT_FALSE))
            {
                enButton = LOGI_BUT_MAN_NO_BUTTON; //NO press forwarded to app.
            }
            else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_SK_Skoda)
            {
                if ( enPressType == enLogiButManShortPress)
                {
                    if((_CONTEXT_MAN_bLastScreenWasNotTelephone == BT_TRUE) )
                    {
                        enButton = LOGI_BUT_MAN_NO_BUTTON;
                        _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
                        DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ThrowOK");
                        /* The MFL Skoda High has a menu structure. When telephone is selected in this menu structure then
                         * the first button press will be evaluated from the kombi instrument and the button release will
                         * be evaluated from APP_SD. This leads to a problem that two button presses will be evaulated.
                         * To avoid this the variable _CONTEXT_MAN_bLastScreenWasNotTelephone is added which ignores the
                         * button release. */
                    }
                    else
                    {
                        DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                        APP_SD_vOnMflButton(enButton, enPressType);
                    }
                }
                else if ( (enPressType == enLogiButManShortPressAtSingleFunction)  )
                {
                    Uint32Type timeout = 0;
                    if (_CONTEXT_MAN_enPendingWork == enWaitToAvoidDoubleOkPress)
                    {
                        if( APP_COMMON_UTILS_stGetTimeoutLeft( CTA_CONTEXT_MAN_HANDLE_CALLS, &timeout ) != E_OK)
                        {
                            DEBUG_TEXT(CONTEXT_MAN_SWI_ERR, MOD_CONTEXT_MAN, "Wrongtimervalue");
                        }
                        else
                        {
                            DEBUG_VALUE1(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "TimerVal = %d", timeout);
                            if ( timeout > 1900 )
                            {
                                _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
                                DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "Illiegal Pos Flanke");
                            }
                            else
                            {
                                _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
                                _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
                                DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "DoNotThrowOK");
                            }
                        }
                    }
                    else
                    {
                        if ( _CONTEXT_MAN_enPendingWork != enContextManagerFnNoActiveCall)
                        {
                            _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
                        }
                        DEBUG_TEXT(CONTEXT_MAN_SWI_DBG, MOD_CONTEXT_MAN, "NormalSequence");
                    }
                }
            }
            else if ( _CONTEXT_MAN_enFahrzeugTyp == APP_CAN_VW_Volkswagen)
            {
                if ( enPressType == enLogiButManShortPressAtSingleFunction )
                { // remove the check for OK button if VCR is active handling was changed to SD. TB 19.10.2009
                    DEBUG_VALUE2(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "%d->SD,%d",enButton, enPressType);
                    APP_SD_vOnMflButton(enButton, enPressType);
                }
            }
            break;
        }
        default:
            break;
    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _CONTEXT_MAN_vHandleCalls                                                    */
/**
    \brief      to handle retries for call management.

    \brief      The timer is activated till a OK is received from the application
                when a call is made or the phone needs to be hanged up.

    \return     void
   */
/*----------------------------------------------------------------------------*/
static void _CONTEXT_MAN_vHandleCalls(void)
{
    Uint8Type *au8Number;
    Uint16Type u16NumberLength = 0;
    APP_COMMON_enResponseCode res = APP_COMMON_RSP_OK;
    switch ( _CONTEXT_MAN_enPendingWork )
    {
        case enContextManagerFnInfoCall:
            if ( APP_PHONE_GetRoaming() == 0) // not roaming
            {
                au8Number = _CONTEXT_MAN_u8InfoCallNumber;
                u16NumberLength = _CONTEXT_MAN_u16InfoCallNumberLen;
            }
            else if ( APP_PHONE_GetRoaming() == 1) // roaming
            {
                au8Number = _CONTEXT_MAN_u8InfoCallNumberRoaming;
                u16NumberLength = _CONTEXT_MAN_u16InfoCallNumberLenRoaming;
            }
            res = APP_PHONE_rcDialNumber(_CONTEXT_MAN_cbResponseCodeCyclic,(const Uint8Type *)au8Number,u16NumberLength );
            if ( res == APP_COMMON_RSP_OK )
            {
                CONTEXT_MAN_bInfoCallInProgress = BT_TRUE;
                CONTEXT_MAN_bSosCallInProgress = BT_FALSE;
                ; // noActiveCall case will take care of the alarm to be canceled.
            }
            else if (  (res != APP_COMMON_RSP_ERROR_BUSY) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 )  )
            {
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
            }
            else
            {
                _CONTEXT_MAN_u8RetryTimeOut++;
            }
            break;

        case enContextManagerFnServiceCall:
            if ( APP_PHONE_GetRoaming() == 0)
            {
                au8Number = _CONTEXT_MAN_u8ServiceCallNumber;
                u16NumberLength = _CONTEXT_MAN_u16ServiceCallNumberLen;
            }
            else if ( APP_PHONE_GetRoaming() == 1)
            {
                au8Number = _CONTEXT_MAN_u8ServiceCallNumberRoaming;
                u16NumberLength = _CONTEXT_MAN_u16ServiceCallNumberLenRoaming;

            }
            res = APP_PHONE_rcDialNumber(_CONTEXT_MAN_cbResponseCodeCyclic,(const Uint8Type *)au8Number,u16NumberLength );
            if ( res == APP_COMMON_RSP_OK )
            {
                CONTEXT_MAN_bSosCallInProgress = BT_TRUE;
                CONTEXT_MAN_bInfoCallInProgress = BT_FALSE;
                ; // noActiveCall case will take care of the alarm to be canceled.
            }
            else if (  (res != APP_COMMON_RSP_ERROR_BUSY) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 )  )
            {
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
            }
            else
            {
                _CONTEXT_MAN_u8RetryTimeOut++;
            }
            break;
        case enContextManagerFnSosCall:
            if ( APP_PHONE_GetRoaming() == 0)
                {
                    au8Number = _CONTEXT_MAN_u8SOSCallNumber;
                    u16NumberLength = _CONTEXT_MAN_u16SOSCallNumberLen;
                }
                else if ( APP_PHONE_GetRoaming() == 1)
                {
                    au8Number = _CONTEXT_MAN_u8SOSCallNumber;
                    u16NumberLength = _CONTEXT_MAN_u16SOSCallNumberLen;
                }
            res = APP_PHONE_rcDialNumber(_CONTEXT_MAN_cbResponseCodeCyclic,(const Uint8Type *)au8Number,u16NumberLength );
            if ( res == APP_COMMON_RSP_OK )
            {
                CONTEXT_MAN_bSosCallInProgress = BT_TRUE;
                CONTEXT_MAN_bInfoCallInProgress = BT_FALSE;
                ; // noActiveCall case will take care of the alarm to be canceled.
            }
            else if (  (res != APP_COMMON_RSP_ERROR_BUSY) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 )  )
            {
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
            }
            else
            {
                _CONTEXT_MAN_u8RetryTimeOut++;
            }
            break;
        case enContextManagerFnInfoCall_Hangup:
        case enContextManagerFnSosCall_Hangup:
        case enContextManagerFnServiceCall_Hangup:
            //HANGUP all calls;
            res = APP_PHONE_rcHangUpAll( _CONTEXT_MAN_cbResponseCodeCyclicHangup );
            if ( res == APP_COMMON_RSP_OK )
            {
                ; // noActiveCall case will take care of the alarm to be canceled.
            }
            else if (  (res != APP_COMMON_RSP_ERROR_BUSY) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 )  )
            {
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
                DEBUG_TEXT(CONTEXT_MAN_SWI_ERR, MOD_CONTEXT_MAN, "ERROR in hang up, retries exceeded !!!");
            }
            else
            {
                _CONTEXT_MAN_u8RetryTimeOut++;
            }
            break;

        case enContextManagerFnActiveCall:
        {
            _CONTEXT_MAN_u8RetryTimeOut++;
            if ( BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle() && (_CONTEXT_MAN_u8RetryTimeOut > 10)) // call not in progress
            {
                _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
                CONTEXT_MAN_bInfoCallInProgress = BT_FALSE;
                CONTEXT_MAN_bSosCallInProgress = BT_FALSE;
                _CONTEXT_MAN_u8RetryTimeOut = 0;
            }
            break;
        }
        case enContextManagerFnNoActiveCall:
        {
            (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
            CONTEXT_MAN_bInfoCallInProgress = BT_FALSE;
            CONTEXT_MAN_bSosCallInProgress = BT_FALSE;
            _CONTEXT_MAN_u8RetryTimeOut = 0;
            break;
        }
        case enWaitToAvoidDoubleOkPress:
        {
            if  (KPB_bIsAsgPresentation() == BT_TRUE )
            {
                _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
            }
            _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
            CONTEXT_MAN_bInfoCallInProgress = BT_FALSE;
            CONTEXT_MAN_bSosCallInProgress = BT_FALSE;
            _CONTEXT_MAN_u8RetryTimeOut = 0;
            break;
        }
        case enContextManagerConnectLastUsedDevice:
        {
            res = _CONTEXT_MAN_rcReconnectLastUsedDevice();
            if ( (res != APP_COMMON_RSP_ERROR_BUSY) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 )  )
            {
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
                _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
            }
            else
            {
                _CONTEXT_MAN_u8RetryTimeOut++;
            }
            break;
        }
        case enContextManagerDeleteAllDevices:
        {
            res = APP_DEVICE_rcDeleteAllProfiles(NULL);
            if ( (res != APP_COMMON_RSP_ERROR_BUSY) || ( _CONTEXT_MAN_u8RetryTimeOut > 10 )  )
            {
                (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
                _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
            }
            else
            {
                _CONTEXT_MAN_u8RetryTimeOut++;
            }
            break;
        }
        default:
            (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS);
            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfSetFahrzeugTyp                                 */
/**
    \brief      stores the manufacturer of the car.

    \brief      The timer is activated till a OK is received from the application
                when a call is made or the phone needs to be hanged up.

    \param      enMarke

    \return     SuccessFailType
   */
/*----------------------------------------------------------------------------*/
SuccessFailType CONTEXT_MAN_sfSetFahrzeugTyp(APP_CAN_ENUM_MARKE enMarke)
{
    /* special handling for contact manager:
       if we receive the car information SEAT to set the variable to VW
       because the handling is the same for VW and Seat and VW Nutz.
    */
    if ((APP_CAN_SE_Seat == enMarke) || (APP_CAN_VN_VWNutzfzg == enMarke))
    {
        _CONTEXT_MAN_enFahrzeugTyp = APP_CAN_VW_Volkswagen;
    }
    else
    {
        _CONTEXT_MAN_enFahrzeugTyp = enMarke;
    }
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : CONTEXT_MAN_sfGetFahrzeugTyp                                 */
/**
    \brief      get the manufacturer of the car.

    \return     APP_CAN_ENUM_MARKE
   */
/*----------------------------------------------------------------------------*/
APP_CAN_ENUM_MARKE CONTEXT_MAN_sfGetFahrzeugTyp(void)
{
    return _CONTEXT_MAN_enFahrzeugTyp;
}


/*----------------------------------------------------------------------------*/
/* Function :   CONTEXT_MAN_vOnKpbNotification                                     */
/**
  \brief        Is called to notify about a KPB event

  \param        notification
                KPB notification

  \return       void

 */
/*----------------------------------------------------------------------------*/
void CONTEXT_MAN_vOnKpbNotification(KPB_enNotificationId notification)
{
    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "vOnKPBNot: %d", notification);

    switch( notification )
    {
        case KPB_NOTIFY_VISIBLE_ON:
        {
            if (( _CONTEXT_MAN_bLastScreenWasNotTelephone == BT_TRUE) && (_CONTEXT_MAN_enPendingWork != enWaitToAvoidDoubleOkPress))
            {
                if ( _CONTEXT_MAN_enPendingWork == enContextManagerFnNoActiveCall)
                {
                    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS, MSEC(2000), MSEC(0));
                    _CONTEXT_MAN_enPendingWork = enWaitToAvoidDoubleOkPress;
                    DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "VISIBLE ON, Timer Started to avoid double press" );
                }
            }
            break;
        }
        case KPB_NOTIFY_VISIBLE_OFF:
        {
            /* reduce the event load */
            if ( _CONTEXT_MAN_bLastScreenWasNotTelephone == BT_FALSE )
            {
                DEBUG_VALUE1( CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "VISIBLE OFF: Current Status %d",_CONTEXT_MAN_enPendingWork);
                _CONTEXT_MAN_bLastScreenWasNotTelephone = BT_TRUE;
                if (_CONTEXT_MAN_enPendingWork == enWaitToAvoidDoubleOkPress)
                {
                    _CONTEXT_MAN_enPendingWork = enContextManagerFnNoActiveCall;
                }
            }
            break;
        }
        case KPB_NOTIFY_ASG_AVAILABLE:
        case KPB_NOTIFY_ASG_INIT_DONE:
        case KPB_NOTIFY_ASG_INIT_DONE_ACTIVE:
        {
            //_CONTEXT_MAN_bLastScreenWasNotTelephone = BT_FALSE;
            break;
        }
        default:
        {
            DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "vOnKPBNot: ignored %d",
                                notification);
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _CONTEXT_MAN_u16TransferPhoneNumberToASCII                          */
/**
 \brief      transfer the phone numbers in a normal ASCII format

 \param      data

 \param      length

 \return     Uint16Type
 */
/*----------------------------------------------------------------------------*/
Uint16Type _CONTEXT_MAN_u16TransferPhoneNumberToASCII(Uint8Type *data,
        Uint16Type length)
{
    Uint8Type u8tmpdata[AT_MAX_PHONE_NUM_LEN+1];
    Uint16Type PhoneLen = 0;
    int i = 0;

    // Achtung: data wird als BCD übergeben, so das ein Byte = zwei Bytes ASCII sind, somit ist length nur halb so groß!
    if (length > (AT_MAX_PHONE_NUM_LEN >> 1))
        length = (AT_MAX_PHONE_NUM_LEN >> 1);

    while (i < length) {
        if ((data[i] & 0xF0) == 0xF0) {
            break;
        } else {
            if ((data[i] & 0xC0) == 0xC0) {
                if (PhoneLen == 0) {
                    u8tmpdata[PhoneLen] = '+';
                    PhoneLen++;
                } else {
                    // das Sonderzeichen '+' ist nicht am Anfang, sondern irgendwo => einfach ignorieren...
                }
            } else {
                u8tmpdata[PhoneLen] = ((data[i] & 0xF0) >> 4) + 0x30;
                PhoneLen++;
            }
        }

        if ((data[i] & 0x0F) == 0x0F) {
            break;
        } else {
            if ((data[i] & 0x0C) == 0x0C) {
                // das Sonderzeichen '+' ist nicht am Anfang, sondern irgendwo => einfach ignorieren...
            } else {
                u8tmpdata[PhoneLen] = (data[i] & 0x0F) + 0x30;
                PhoneLen++;
            }
        }
        i++;
    }

    for (i=0; i<PhoneLen; i++)
        data[i] = u8tmpdata[i];

    data[i] = 0; // end with a null character to determine a string.
    //PhoneLen++;

    DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "Transfer phone number to ASCII, len = %d",
                    PhoneLen);
    return PhoneLen;

}


/*----------------------------------------------------------------------------*/
/* Function :   _CONTEXT_MAN_rcReconnectLastUsedDevice                        */
/**
  \brief        connect last connected device

  \param        void

  \return       APP_COMMON_enResponseCode

 */
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _CONTEXT_MAN_rcReconnectLastUsedDevice(void)
{
    Uint8Type profileId = ERM_GetLastActiveProfileId( 0, APP_DEVICE_enTYPE_PHONE);
    const ERMUserProfileType *pUserProfile = ERM_pGetUserProfile(profileId);
    APP_COMMON_enResponseCode rsp = APP_DEVICE_sfActivatePhoneDevice( pUserProfile->deviceIndex, NULL);
    if (APP_COMMON_RSP_ERROR_BUSY == rsp)
    {
        _CONTEXT_MAN_enPendingWork = enContextManagerConnectLastUsedDevice;
        _CONTEXT_MAN_u8RetryTimeOut = 0;
        (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS, MSEC(100), MSEC(100));
    }
    return rsp;
}


/*----------------------------------------------------------------------------*/
/* Function :   APP_SD_vOnErmNotification                                     */
/**
  \brief        Is called to notify about a ERM event

  \param        notification
                ERM notification

  \return       void

 */
/*----------------------------------------------------------------------------*/
void CONTEXT_MAN_vOnContextManNotification(const ERMNotificationType * pNotification)
{
    switch( pNotification->common.id )
    {
        case ERM_NOTIFY_PBSN_PHONE:
        {
            /* Phone was still activated in current session */
            if (AT_SERVRES_START_SUCCEED == pNotification->pbsn_phone.result)
            {
                DEBUG_VALUE1(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "ContextManNotification pNotification result %d: ", pNotification->pbsn_phone.result);
                _CONTEXT_MAN_phoneActivatedOnce = BT_TRUE;
            }
            break;
        }
        default:
            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function :   CONTEXT_MAN_cDeleteAllProfiles                                */
/**
  \brief        Function call to delete all profiles handled by BT-3BM and Hook

  \param        void

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void CONTEXT_MAN_cDeleteAllProfiles(void)
{
    APP_COMMON_enResponseCode res = APP_COMMON_RSP_OK;
    if ( BT_TRUE == KPB_bIsAsgAvailable())
    {
        if (BT_TRUE == APP_SD_bIsIdleScreenActive(APP_SD_IDLE_SCREEN_INACTIVE_PHONE))
        {
            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "DeleteAllProfiles: ASG avail., delete all profile");
            res = APP_DEVICE_rcDeleteAllProfiles(NULL);
            if (APP_COMMON_RSP_ERROR_BUSY == res)
            {
                _CONTEXT_MAN_enPendingWork = enContextManagerDeleteAllDevices;
                _CONTEXT_MAN_u8RetryTimeOut = 0;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS, MSEC(100), MSEC(100));
            }
        }
        else
        {
            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "DeleteAllProfiles: ASG avail., dev connected at this cycle, do nothing");
        }
    }
    else
    {
        if (BT_FALSE == _CONTEXT_MAN_phoneActivatedOnce)
        {
            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "DeleteAllProfiles: no ASG, delete all profile");
            res = APP_DEVICE_rcDeleteAllProfiles(NULL);
            if (APP_COMMON_RSP_ERROR_BUSY == res)
            {
                _CONTEXT_MAN_enPendingWork = enContextManagerDeleteAllDevices;
                _CONTEXT_MAN_u8RetryTimeOut = 0;
                (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_CONTEXT_MAN_HANDLE_CALLS, MSEC(100), MSEC(100));
            }
        }
        else
        {
            DEBUG_TEXT(CONTEXT_MAN_SWI_TRC, MOD_CONTEXT_MAN, "DeleteAllProfiles: no ASG, dev connected at this cycle, do nothing");
        }
    }
}
/* End Of File CONTEXT_MAN_C1.c */
