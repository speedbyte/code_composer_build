/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_SMS_ERM_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Sms Application
*-----------------------------------------------------------------------------
* Description:    Handles events received from ERM  
*-----------------------------------------------------------------------------
* $Date: 2010-03-31 11:20:17 +0200 (Mi, 31 Mrz 2010) $
* $Rev: 11913 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_SMS/trunk/src/APP_SMS_ERM_C1.c $
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
/*----------------------------------------------------------------------------*/
#include "ATCmdHandler.h"
#include "APP_COMMON_Utils.h"
#include "ERM.h"

#include "APP_SMS.h"
#include "APP_SMS_CI.h"
#include "APP_SMS_ERM.h"

#include "APP_SMS_ERM_C1.id"
#include "TRACE.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/** 
\brief	variable to save the current pb function currently in progress
*/
extern AppSmsContext gAppSmsContext;
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
/* Function    : APP_SMS_vHandleErmAtResult                                   */
/** 
    \brief      handle event from ERM when AT Parser sent OK or ERROR
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT OK or ERROR\n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleErmAtResult(APP_SMS_enResult result)
{
    switch( gAppSmsContext.fct.common.fctId )
    {
        case APP_SMS_enReadSms:
            _APP_SMS_vOnReadSmsResult(result);
            break;
        case APP_SMS_enReadSmsPreview:
            _APP_SMS_vOnReadSmsPreviewResult(result);
            break;
        case APP_SMS_enSendSms:
            _APP_SMS_vOnSendSmsResult(result);
            break;
        case APP_SMS_enSendConcSms:
            _APP_SMS_vOnSendConcSmsResult(result);
            break;
        case APP_SMS_enDeleteSms:
            _APP_SMS_vOnDeleteSmsResult(result);
            break;
        case APP_SMS_enSmsInit:
            _APP_SMS_vOnSmsInitResult(result);
            break;
        case APP_SMS_enMarkAsRead:
            _APP_SMS_vOnMarkAsReadResult(result);
            break;
        default:
            DEBUG_VALUE2( APP_SMS_SWI_ERR, MOD_APP_SMS, "HandleErmAtResult: unknown fctId %d result %d", gAppSmsContext.fct.common.fctId, result );

            _APP_SMS_vReleaseFctContext();
            break;
    }

}/* END OF _APP_SMS_vHandleErmATOkt() */
/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleErmATOk                                       */
/** 
    \brief      handle event from ERM when AT Parser sent OK 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT OK \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleErmATOk(void)
{
    DEBUG_TEXT( APP_SMS_SWI_TRC, MOD_APP_SMS, "HandleErmATOk");
    
    APP_SMS_vHandleErmAtResult(APP_SMS_enResultOk);

}/* END OF _APP_SMS_vHandleErmATOkt() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vHandleErmATError                                    */
/** 
    \brief      handle event from ERM when AT Parser sent ERROR 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT ERROR \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vHandleErmATError(void)
{
    DEBUG_TEXT( APP_SMS_SWI_TRC, MOD_APP_SMS, "HandleErmATError");

    APP_SMS_vHandleErmAtResult(APP_SMS_enResultError);

}/* END OF APP_SMS_vHandleErmATErrorEvent() */


/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_ERM_vHandleErmStateChange                             */
/** 
    \brief      handle State change of ERM 
  
    \brief      Description:\n
                handle State change of ERM \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_ERM_vHandleErmStateChange(ERMStateType newState)
{
    switch( newState )
    {
        case ERM_STATE_CK5050_ACTIVE:
            _APP_SMS_vReInitApp();
            break;
        
    }
}/* END OF APP_SMS_vHandleErmStateChange() */


/*----------------------------------------------------------------------------*/
/* Function    : APP_SMS_vOnErmNotification                                   */
/** 
    \brief      handle Notifications of ERM
  
    \brief      Description:\n
                handle Notifications of ERM \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_SMS_vOnErmNotification(const ERMNotificationType * pNotification)
{
    switch(pNotification->common.id)
    {
        //case ERM_NOTIFY_PHONE_ACTIVE_STATE_CHANGED:
        case ERM_NOTIFY_PBSN_PHONE:
            if( ERM_u8GetActivePhone() == AT_INVALID_DEVICE_ID )
            {
                /* reinit after phone disconnect */
                _APP_SMS_vReInitApp();
            }
            break;
        default:
            /* ignore notification */
            break;
    }
}

/* End Of File APP_SMS_ERM_C1.c */


