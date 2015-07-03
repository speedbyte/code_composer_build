/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_COMMON_UTILS_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Common Application
*-----------------------------------------------------------------------------
* Description:    contains some helper functions common for all applications   
*-----------------------------------------------------------------------------
* $Date: 2010-03-03 16:10:23 +0100 (Mi, 03 Mrz 2010) $
* $Rev: 11318 $
* $Author: brust $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_COMMON/trunk/src/APP_COMMON_Utils_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
//#include "bap_types.h"
/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "APP_COMMON_AT_Dispatcher.h"
#include "ERM.h"
#include "APP_MDI.h"
#include "APP_SMS.h"
#include "VCR_MessageHandler.h"
#include "APP_PHONE.h"
#include "APP_PHONEBOOK.h"
#include "APP_DEVICE.h"

#include "APP_COMMON_AT_Dispatcher_C1.id"
#include "Trace.h"
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
/* Function    : APP_COMMON_UTILS_u16GetBapString                             */
/** 
    \brief      Extracts a BAP string from a byte sequence 
  

    \param      pBufferBegin
                const Uint8Type*

    \param      pStringBegin
                Uint8Type**

    \return     Uint16Type 
*/
/*----------------------------------------------------------------------------*/
void APP_COMMON_vHandleEvents( EventMaskType event )
{
    ATRspMessageType  atRsp;
    (void)memset(&atRsp,0x00,sizeof(ATRspMessageType));
    if( event & evAppTaskApplATResponse )
    {
	    // check for new response messages from AT_Cmd_Parser
        (void)ReceiveMessage(msgApplATResponse ,&atRsp);
        switch( atRsp.common.receiver )
        {
            case APP_SUB_ERM:
            {
                (void)ERM_vHandleATResponse(&atRsp);
                break;
            }
            case APP_SUB_TEL:
            {
                (void)APP_PHONE_vHandleATResponse(&atRsp);
                break;
            }
            case APP_SUB_PHONEBOOK:
            {
                (void)APP_PHONEBOOK_vHandleATResponse(&atRsp);
                break;
            }
            case APP_SUB_SMS:
            {
                (void)APP_SMS_vHandleATResponse(&atRsp);
                break;
            }
            case APP_SUB_MDI:
            {
                (void)APP_MDI_vHandleATResponse(&atRsp);
                break;
            }
            case APP_SUB_VCR:
            {
                (void)VCR_sFHandleATMessage(&atRsp);
                break;
            }
            case APP_SUB_DEVICE:
            {
                (void)APP_DEVICE_vHandleATResponse(&atRsp);
                break;
            }
            default:
            {

                DEBUG_VALUE1(APP_COMMON_SWI_ERR, MOD_APP_COMMON, "APP_COMMON_vHandleEvents: wrong receiver %d", atRsp.common.receiver);
                
            break;
            }
        }
        (void)APP_COMMON_UTILS_sfFreeAtResponse(&atRsp);
        if( E_COM_NOMSG == GetMessageStatus(msgApplATResponse))
        {
            // clear only if all msg received
            (void)ClearEvent(evAppTaskApplATResponse);
        }
    }
}

/* End Of File APP_COMMON_UTILS_C1.c */


