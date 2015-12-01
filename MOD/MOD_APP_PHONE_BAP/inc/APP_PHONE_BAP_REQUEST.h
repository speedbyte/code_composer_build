/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_PHONE_BAP_REQUEST.h
*-----------------------------------------------------------------------------
* Module Name:  APP_PHONE_BAP    
*-----------------------------------------------------------------------------
* Description:  Module Global Interfaces, Enum, Structs for APP_PHONE_BAP
*-----------------------------------------------------------------------------
* $Date: 2011-02-16 13:28:25 +0100 (Mi, 16 Feb 2011) $
* $Rev: 17936 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE_BAP/trunk/inc/APP_PHONE_BAP_REQUEST.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_BAP_REQUEST_H
#define _APP_PHONE_BAP_REQUEST_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "UART.h"
#include "APP_PHONE_BAP_CI.h"

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
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/


/** 
\brief	description of the interface IAPP_PHONE_BAP_INTERNAL
*/
//#ifdef IAPP_PHONE_BAP_REQUEST_INTERNAL /* BEGIN DECLARE
//INTERFACE(IAPP_PHONE_BAP_INTERNAL) */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_sfSendPlpdRequest(const SCI_ST* puart)       */  
/** 
    \brief      send PLPD Request to Parrot  
  
    \brief      Description:\n
                send PLPD Request to Parrot 
 
    \param      puart
                const SCI_ST*
                no limits
    
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_sfSendPlpdRequest(const SCI_ST * puart);


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendNetworkProvider                 */
/** 
    \brief      Update network provider shown in Headunit
  
    \param      opName
                name of network provider

    \param      len
                length of network provider name    

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendNetworkProvider(const Uint8Type * opName,
Uint16Type len );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendSignalQuality                        */
/** 
    \brief      send signal quality property status update
  
    \param      quality
                Uint8Type 
                new value for signal strength. This value is the one received
                from CK5050, so we need to map it to bap range

    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendSignalQuality( Uint8Type quality );

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendRegisterState                        */
/** 
    \brief      update bap's register state via status message

    \param      atServiceValue
                service availability value received from CK5050, needs to be
                mapped to bap range
                
    \param      atRoamingValue
                indicator if phone is searching for a network or not
  
    \return     SuccessFailType
                SUCCESS or FAIL 
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendRegisterState( Uint8Type atServiceValue, 
                                                       Uint8Type atRoamingValue);

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_BAP_REQUEST_sfSendAllCallStatesIdle               */
/** 
    \brief      update bap's call state to all idle. this is done after
                disconnect of active user
  
    \brief      Description:\n
                update bap's call state to all idle. this is done after
                disconnect of active user\n 

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_BAP_REQUEST_sfSendAllCallStatesIdle();



SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendHandsFreeOnOff(HandsFreeMode mode);

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendMicroMuteOnOff(MicroMuteMode mode);

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendUserListStatus(const BapArrayHeaderType * pRecvHeader);

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendUserListChanged();

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendActiveUser();

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfDeactiveUser();

SuccessFailType
_APP_PHONE_BAP_REQUEST_sfSendLineState();

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendCallState();

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendCallInfoStatus(const BapArrayHeaderType * pRecvHeader);

SuccessFailType
_APP_PHONE_BAP_sfUpdateAndSendMobileServiceSupport();

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendMobServSupp(const PhoneMobileServiceSupport *pPhoneMss);

SuccessFailType 
_APP_PHONE_BAP_REQUEST_sfSendFsgOperationState(FsgOperationState newState);


SuccessFailType
_APP_PHONE_BAP_REQUEST_sfSendFsgSetup(BapFsgSetupMobileConnectionType connectionType);

void _APP_PHONE_BAP_vInternalNotification(APP_PHONE_enNotificationId notification);


//#endif /* END DECLARE INTERFACE(IAPP_PHONE_BAP_INTERNAL) */

#endif /* _APP_PHONE_BAP_REQUEST_H */

/* End Of File APP_PHONE_BAP_REQUEST.h */

