/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     _APP_PHONE_AT_REQUEST_Builder_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Telephone Application
*-----------------------------------------------------------------------------
* Description:    builds up all phone specific at requests   
*-----------------------------------------------------------------------------
* $Date: 2008-10-13 14:05:09 +0200 (Mo, 13 Okt 2008) $
* $Rev: 2240 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_PHONE_BAP_C1.c $
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
#include "bap_defines.h"    // vw

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONE_CI.h"
#include "APP_PHONE_AT_Request_Builder.h"
#include "ATCmdHandler.h"
#include "ERM.h"
#include "UART.h"

#include "APP_PHONE_AT_Request_Builder_C1.id"
#include "TRACE.h"
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
/** 
\brief	variable to save the current call management function currently in progress
*/
/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the private variable
*/

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
/* Function    : _APP_PHONE_AT_REQUEST_sfDialNumber                           */
/** 
    \brief      handles dial and redial number  
  
    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      numberString
                const Uint8Type
                string containing the number to be dialed
                
    \param      numberLen
                Uint16Type
                length of number to be dialed
    
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfDialNumber( const SCI_ST* puart,
                                                    const Uint8Type * numberString, 
                                                    Uint16Type numberLen )
{
    // extract number and optional name and put them into struct
    // which can be handled by the ATCmdHandler
    ATReqAllType request;

    // check if number is NULL which indicates a Redial request
    if( (NULL == numberString) || (0 == numberLen) )
    {
        request.common.req_type     = AT_REQ_CALL_LAST_NUMBER;  /* BLDN */ 
        request.callNumber.len      = 0;         
        (void)memset( request.callNumber.number, 0, AT_MAX_PHONE_NUM_LEN);    
    }
    else
    {
        request.common.req_type     = AT_REQ_CALL_PHONE_NUBER;  
     
        if( numberLen > AT_MAX_PHONE_NUM_LEN ) 
        {
            numberLen = AT_MAX_PHONE_NUM_LEN;
        }

        request.callNumber.len      = numberLen; 
        (void)memset(request.callNumber.number, 0, AT_MAX_PHONE_NUM_LEN);    
        (void)memcpy(request.callNumber.number, numberString, numberLen);
    }
 
    return AT_Cmd_Handler_Tel(&request,puart);

}   /* END OF _APP_PHONE_AT_REQUEST_sfDialNumber()                            */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfDialIndex                             */
/** 
    \brief      builds AT request to dial a pn index  
  
    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      pbIndex
                Uint16Type
                pb index within the phonebook
                
    \param      pnIndex
                Uint16Type
                index of the number for the selected pb entry if more than one
                number exists for pb entry
    
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfDialIndex(const SCI_ST* puart,
                                                            Uint16Type pbIndex,
                                                            Uint16Type pnIndex )
{
    ATReqAllType request;
    
    request.common.req_type     = AT_REQ_CALL_PB_INDEX;  
    // check if we dial with pb index only ( default number ) or a special pn
    // index
    request.atdi.pbIndex        = pbIndex;
    request.atdi.pnIndex_avail  = AT_FALSE;
  
    if( 0 != pnIndex )
    {
        request.atdi.pnIndex_avail  = AT_TRUE;
        request.atdi.pnIndex        = pnIndex; 
    }    
 
    return AT_Cmd_Handler_Tel(&request,puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfRedial                                */
/** 
    \brief      builds AT request to redial last dialled number  
  
    \param      puart
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfRedial(const SCI_ST* puart )
{
    ATReqAllType request;
    
    request.common.req_type = AT_REQ_CALL_LAST_NUMBER;  

    return AT_Cmd_Handler_Tel(&request,puart);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfDialNumberType                             */
/** 
    \brief      builds AT request to dial a given numberType of pbIndex  
  
    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      pbIndex
                Uint16Type
                index of pb entry to be called
                
    \param      numberType
                ATPhoneNumberTypeEnum
                category of number of pb entry to be dialed
    
    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfDialNumberType( const SCI_ST* puart,
                                                                 Uint16Type pbIndex,
                                                                 ATPhoneNumberTypeEnum numberType )
{
    ATReqAllType request;

    request.common.req_type     = AT_REQ_CALL_PB_NUMBER_TYPE;  
    request.atdp.pbIndex        = pbIndex;
    request.atdp.numberType     = numberType; 
 
    return AT_Cmd_Handler_Tel(&request,puart);
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfAcceptCall                                */
/** 
    \brief      build accept call AT request 
  
    \brief      Description:\n
                build accept call AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfAcceptCall(const SCI_ST* puart)   
{
    ATReqAllType request;
    
    request.common.req_type = AT_REQ_PICK_UP_CALL; /* ATA */  
    
    return AT_Cmd_Handler_Tel(&request,puart);
}   /* END OF _APP_PHONE_AT_REQUEST_sfAcceptCall() */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfHangUp                               */
/** 
    \brief      send hangup AT request  
  
    \brief      Description:\n
                send hangup AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      callIndex
                ATCallHangupEnum
                index of call to be hung up

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfHangUp(const SCI_ST* puart,   
                                                ATCallHangupEnum callIndex) 
{
    ATReqAllType request;
    
    request.common.req_type = AT_REQ_CALL_HANG_UP;   
    request.chup.mode       = callIndex;     

    return AT_Cmd_Handler_Tel(&request,puart);

}   /* END OF _APP_PHONE_AT_REQUEST_sfHangUp() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfPhoneStatus                               */
/** 
    \brief      build phone status AT request 
  
    \brief      Description:\n
                build phone status AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfPhoneStatus(const SCI_ST* puart)   
{
    ATReqAllType request;
        
    request.common.req_type = AT_REQ_PHONE_STATUS_IND_VAL;
    
    return AT_Cmd_Handler_Tel(&request,puart);
}   /* END OF _APP_PHONE_AT_REQUEST_sfPhoneStatus() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfCallState                                 */
/** 
    \brief      build phone call state AT request 
  
    \brief      Description:\n
                build phone call state AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfCallState(const SCI_ST* puart)   
{
    ATReqAllType request;

    request.common.req_type = AT_REQ_CALL_STATUS_IND_INDEX;
    
    return AT_Cmd_Handler_Tel(&request,puart);
}   
/* END OF _APP_PHONE_AT_REQUEST_sfCallState() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfCallInfo                                  */
/** 
    \brief      build phone call info AT request 
  
    \brief      Description:\n
                build phone call info AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfHandleCallInfo(const SCI_ST* puart)   
{
    ATReqAllType request;
 
    /* send AT request */
    request.common.req_type = AT_REQ_CALL_STATUS_IND_INDEX;
    
    return AT_Cmd_Handler_Tel(&request,puart);
}   /* END OF _APP_PHONE_AT_REQUEST_sfCallInfo() */


 
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfSendDTMF                              */
/** 
    \brief      build sendDTMF AT request 
  
    \brief      Description:\n
                build sendDTMF AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      dtmfChar 
                Uint8Type 
                character to send as dtmf tone

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfSendDTMF(const SCI_ST* puart,
                                                            Uint8Type dtmfChar)   
{
    ATReqAllType request;

    request.common.req_type = AT_REQ_DTMF;
    // copy byte
    request.dtmf.value = dtmfChar;

    return AT_Cmd_Handler_Tel(&request,puart);
}   /* END OF _APP_PHONE_AT_REQUEST_sfCallState() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfPlpdRequest       */  
/** 
    \brief      send PLPD Request to Parrot  
  
    \brief      Description:\n
                send PLPD Request to Parrot 
 
    \return     SuccessFailType
                APP_COMMON_RSP_OK or ... 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfPlpdRequest(const SCI_ST* puart)   
{
    ATReqAllType request;

    request.common.req_type = AT_REQ_GET_PAIRED_DEVICES;
    request.plpd.mode = AT_PLPD_GET_ALL;

    return AT_Cmd_Handler_Tel(&request,puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfCallHold                              */
/** 
    \brief      build call hold AT request 
  
    \brief      Description:\n
                build call hold AT request \n 

    \return     SuccessFailType
                APP_COMMON_RSP_OK or ... 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfCallHold(const SCI_ST* puart)   
{
    ATReqAllType request;

    // PMPC=1, put all active calls on Hold
    request.common.req_type         = AT_REQ_MULTI_PARTY_CALL;
    request.pmpc.action             = AT_MPC_ACTION_ALL_ACTIVE_ON_HOLD;    
    request.pmpc.idx_available      = AT_FALSE;      
    
    return AT_Cmd_Handler_Tel(&request,puart);
}/* END OF _APP_PHONE_sfHandleCallHold() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfCallReplace                              */
/** 
    \brief      build call Replace AT request 
  
    \brief      Description:\n
                build call replace AT request \n 

    \return     SuccessFailType
                APP_COMMON_RSP_OK or ... 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfCallReplace(const SCI_ST* puart)   
{
    ATReqAllType request;

    // PMPC=0, hangup all active calls, knocking call becomes active
    request.common.req_type         = AT_REQ_MULTI_PARTY_CALL;
    request.pmpc.action             = AT_MPC_ACTION_HANGUP_IDX_OR_ALL;    
    request.pmpc.idx_available      = AT_FALSE;      
    
    return AT_Cmd_Handler_Tel(&request,puart);
}/* END OF _APP_PHONE_sfHandleCallReplace() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfResumeCall                                 */
/** 
    \brief      build resume call AT request 
  
    \brief      Description:\n
                build resume call AT request \n 

    \return     SuccessFailType
                APP_COMMON_RSP_OK or ... 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfResumeCall(const SCI_ST* puart)   
{
    ATReqAllType request;
    request.common.req_type         = AT_REQ_MULTI_PARTY_CALL;
    
    /* PMPC=2,0 Resume all Calls on Hold */
    //request.pmpc.action             = AT_MPC_ACTION_PUT_IDX_ACTIVE;  
    //request.pmpc.index              = AT_CALL_INDEX_0;  
    //request.pmpc.idx_available      = AT_TRUE;

    // SUSANNE: we also wanna resume a conference sometimes: try this without an index! 
    // *PMPC=2 
    request.pmpc.action             = AT_MPC_ACTION_PUT_IDX_ACTIVE;  
    request.pmpc.index              = AT_CALL_INDEX_NONE;
    request.pmpc.idx_available      = AT_FALSE;

    
    return AT_Cmd_Handler_Tel(&request,puart);
}/* END OF _APP_PHONE_AT_REQUEST_sfResumeCall() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfHeadsetOnOff                            */
/** 
    \brief      builds headset On / Off AT request 
  
    \brief      Description:\n
                builds headset On / Off AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      mode
                ATAudioGatewayModeEnum  
                mode to be set

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfHeadsetOnOff(const SCI_ST* puart,   
                                                     ATAudioGatewayModeEnum mode)
{
    ATReqAllType request;
    
    request.common.req_type = AT_REQ_BT_AG_MODE;
    request.pagm.mode = mode;
    
    return AT_Cmd_Handler_Tel(&request,puart);
}/* END OF _APP_PHONE_AT_REQUEST_sfHeadsetOnOff() */





/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfHandsFreeOnOff                            */
/** 
    \brief      builds Hands Free On / Off AT request 
  
    \brief      Description:\n
                builds Hands Free On / Off AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      mode
                ATAudioTransferModeEnum
                mode to be set

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfHandsFreeOnOff(const SCI_ST* puart,   
                                                       ATAudioTransferModeEnum mode)
{
    ATReqAllType request;
    
    request.common.req_type = AT_REQ_AUDIO_TRANS_DISCRETE;
    request.patr.mode = mode;
    
    return AT_Cmd_Handler_Tel(&request,puart);
}/* END OF _APP_PHONE_AT_REQUEST_sfHandsFreeOnOff() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfMicroMuteOnOff                        */
/** 
    \brief      build micro mute on/off AT request 
  
    \brief      Description:\n
                build micro mute on/off AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      mode 
                ATMicModeEnum 
                mode to be set

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfMicroMuteOnOff(const SCI_ST* puart,   
                                                                ATMicModeEnum mode )
{
    ATReqAllType request;

    request.common.req_type = AT_REQ_AMGMT_MIC_MUTE;
    request.micMute.mode    = mode; 
    
    return AT_Cmd_Handler_Tel(&request,puart);
}   /* END OF _APP_PHONE_AT_REQUEST_sfMicroMuteOnOff() */
 

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfSwitchCalls                           */
/** 
    \brief      build switch calls AT request 
  
    \brief      Description:\n
                build switch calls AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfSwitchCalls(const SCI_ST* puart)   
{
    ATReqAllType request;

    request.common.req_type = AT_REQ_SWITCH_CALLS;
    
    return AT_Cmd_Handler_Tel(&request,puart);
}   /* END OF _APP_PHONE_AT_REQUEST_sfSwitchCalls() */

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfJoinToConference                                 */
/** 
    \brief      build AT request to join a held call to conference  
  
    \brief      Description:\n
                build AT request to join a held call to conference \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfJoinToConference(const SCI_ST* puart)
{
    ATReqAllType request;

    // AT*PMPC=3
    request.common.req_type         = AT_REQ_MULTI_PARTY_CALL;
    request.pmpc.action             = AT_MPC_ACTION_HELD_CALLS_TO_CONF;    
    request.pmpc.idx_available      = AT_FALSE;      
    
    return AT_Cmd_Handler_Tel(&request,puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfSplitCallFromConference                                 */
/** 
    \brief      builds AT request to split a given call Index from a conference 
  
    \brief      Description:\n
                builds AT request to split a given call Index from a conference \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \param      callIndex
                ATCallIndexEnum 
                call index which shall be splitted from conference 

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfSplitCallFromConference( const SCI_ST* puart,   
                                                                 ATCallIndexEnum callIndex)
{
    ATReqAllType request;

    // AT*PMPC=2,callId
    request.common.req_type         = AT_REQ_MULTI_PARTY_CALL;
    request.pmpc.action             = AT_MPC_ACTION_PUT_IDX_ACTIVE;    
    request.pmpc.idx_available      = AT_TRUE;
    request.pmpc.index              = callIndex;      
    
    return AT_Cmd_Handler_Tel(&request,puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfPutCallsThrough                      */
/** 
    \brief      build AT request to join a held call to conference  
  
    \brief      Description:\n
                build AT request to join a held call to conference \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfPutCallsThrough(const SCI_ST* puart)
{
    ATReqAllType request;

    // AT*PMPC=3
    request.common.req_type         = AT_REQ_MULTI_PARTY_CALL;
    request.pmpc.action             = AT_MPC_ACTION_JOIN_ACTIVE_CALLS;    
    request.pmpc.idx_available      = AT_FALSE;      
    
    return AT_Cmd_Handler_Tel(&request,puart);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfRequestHfpVersion                      */
/** 
    \brief      request hf profile version of active user  
  
    \brief      Description:\n
                request hf profile version of active user  \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfRequestHfpVersion(const SCI_ST* puart)
{
    ATReqAllType request;

    const ERMUserProfileType * profile = ERM_pGetActiveUserProfile();
    if(NULL == profile)
    {
        return FAIL;
    }

    // AT*PBCD=deviceId
    request.common.req_type         = AT_REQ_CLASS_DEVICE_REQUEST;
    request.pbcd.device_id          = profile->deviceIndex;    
    
    return AT_Cmd_Handler_Tel(&request,puart);
}


/* End Of File _APP_PHONE_AT_REQUEST_Builder_C1.c */


