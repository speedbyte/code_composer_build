/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   _APP_PHONE_AT_REQUEST_Builder.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_PHONE
*-----------------------------------------------------------------------------
* $Date: 2010-06-23 20:35:57 +0530 (Mi, 23 Jun 2010) $
* $Rev: 13677 $
* $Author: gross $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/inc/APP_PHONE_AT_Request_Builder.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef __APP_PHONE_AT_REQUEST_Builder_H
#define __APP_PHONE_AT_REQUEST_Builder_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "UART.h"
#include "APP_COMMON_Utils.h"
#include "ATCommonData.h"


/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
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
SuccessFailType _APP_PHONE_AT_REQUEST_sfDialNumber(const SCI_ST* puart,   
                                         const Uint8Type * numberString,
                                         Uint16Type numberLen );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfDialIndex                             */
/** 
    \brief      builds AT request to dial a pn index  
  
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
                                         Uint16Type pnIndex );

///*----------------------------------------------------------------------------*/
///* Function    : _APP_PHONE_AT_REQUEST_sfDialPbIndex                             */
///** 
//    \brief      builds AT request to dial a pb index  
//  
//    \brief      Description:\n
//                builds AT request to dial a pb index \n 
//
//    \param      puart
//                const SCI_ST* 
//                pointer to uart ressource
//
//    \param      pbIndex
//                Uint16Type
//                pb entry to be dialed
//                
//    \return     SuccessFailType
//                SUCCESS or FAIL
//*/
///*----------------------------------------------------------------------------*/
//SuccessFailType _APP_PHONE_AT_REQUEST_sfDialPbIndex(const SCI_ST* puart,   
//                                         Uint16Type pbIndex );
//

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfDialNumberType(const SCI_ST* puart,   
                                         Uint16Type pbIndex,
                                         ATPhoneNumberTypeEnum numberType );

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfRedial(const SCI_ST* puart);

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
                                         ATCallHangupEnum callIndex); 

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfAcceptCall(const SCI_ST* puart);

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfPhoneStatus(const SCI_ST* puart);

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfCallState(const SCI_ST* puart);

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfHandleCallInfo(const SCI_ST* puart);

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
                                                Uint8Type dtmfChar );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfPlpdRequest(const SCI_ST* puart)       */  
/** 
    \brief      send PLPD Request to Parrot  
  
    \brief      Description:\n
                send PLPD Request to Parrot 
 
    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfPlpdRequest(const SCI_ST* puart);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfCallHold                              */
/** 
    \brief      build call hold AT request 
  
    \brief      Description:\n
                build call hold AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfCallHold(const SCI_ST* puart);

SuccessFailType _APP_PHONE_AT_REQUEST_sfCallReplace(const SCI_ST* puart);


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_REQUEST_sfResumeCall                                 */
/** 
    \brief      build resume call AT request 
  
    \brief      Description:\n
                build resume call AT request \n 

    \param      puart
                const SCI_ST* 
                pointer to uart ressource

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_REQUEST_sfResumeCall(const SCI_ST* puart);

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
                                        ATAudioTransferModeEnum mode);

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
                                        ATAudioGatewayModeEnum mode);

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
                                                      ATMicModeEnum mode );

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfSwitchCalls(const SCI_ST* puart);

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfJoinToConference(const SCI_ST* puart);

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfSplitCallFromConference(
                                         const SCI_ST* puart,
                                         ATCallIndexEnum callIndex);

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfPutCallsThrough(const SCI_ST* puart);

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
SuccessFailType _APP_PHONE_AT_REQUEST_sfRequestHfpVersion(const SCI_ST* puart);


#endif /* __APP_PHONE_AT_REQUEST_Builder_H */

/* End Of File _APP_PHONE_AT_REQUEST_Builder.h */

