/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONEBOOK_ERM_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Phonebook Application
*-----------------------------------------------------------------------------
* Description:    Handles events received from ERM  
*-----------------------------------------------------------------------------
* $Date: 2010-03-31 11:20:17 +0200 (Mi, 31 Mrz 2010) $
* $Rev: 11913 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONEBOOK/trunk/src/APP_PHONEBOOK_ERM_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "Pool.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_PHONEBOOK.h"
#include "ATCmdHandler.h"
#include "APP_PHONEBOOK_CI.h"
#include "APP_PHONEBOOK_AT.h"
#include "APP_PHONEBOOK_ERM.h"
#include "APP_COMMON_Utils.h"
#include "ERM.h"
#include "KPB.h"


#include "APP_PHONEBOOK_ERM_C1.id"
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
extern AppPhoneBookContext gAppPBContext;
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
static void APP_PHONEBOOK_vHandleErmATResult(ATResponseEnum rsp);
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

static void APP_PHONEBOOK_vHandleErmATResult(ATResponseEnum rsp)
{
    if( ERM_u8GetActivePhone() == AT_INVALID_DEVICE_ID)
    {
        DEBUG_TEXT(APP_PB_SWI_ERR, MOD_APP_PB, "HandleErmATRes: no user connected, sim. AT ERROR");
        //SUSANNE: 25.10.2009
        // do not ignore OK / ERROR here, cause application might trigger 
        // request to Ck5050 although we are not connected ( anymore ) 
        // due to a queued up event e.g. in APP_SD
        // just finish the function with an error
        // pb access is not allowed when no user is connected
        _APP_PHONEBOOK_vSendResultReleaseContext(APP_COMMON_RSP_ERROR);
        return;
    }

    switch( gAppPBContext.fct.common.fctId )
    {
        case APP_PB_enUpdatePbSizes:
            _APP_PHONEBOOK_vOnUpdatePbResult(rsp);
            break;
        case APP_PB_enUpdateSinglePbSize:
            _APP_PHONEBOOK_vOnUpdateSinglePbSize(rsp);
            break;
        case APP_PB_enActivatePpds:
            _APP_PHONEBOOK_vOnActivatePpdsResult(rsp);
            break;
        case APP_PB_enReadCompletePbEntries:
            _APP_PHONEBOOK_vOnReadCompletePbResult(rsp);
            break;
        case APP_PB_enSearchPb:
            _APP_PHONEBOOK_vOnSearchPbResult(rsp);
            break;
        case APP_PB_enManualPbSync:
            _APP_PHONEBOOK_vOnSyncPbResult(rsp);
            break;
        case APP_PB_enDeleteCallStacks:
            _APP_PHONEBOOK_vOnCallStackResetResult(rsp);
            break;
        case APP_PB_enSetPbSortOrder:
        case APP_PB_enGetPbSortOrder:
            _APP_PHONEBOOK_vOnPbSortOrderResult(rsp);
            break;
        case APP_PB_enAddPbEntry:
            _APP_PHONEBOOK_vOnAddPbEntryResult(rsp);
            break;
        case APP_PB_enRequestNumVoiceTagsForContact:    
            _APP_PHONEBOOK_vOnReqNumTagsResult(rsp);
            break;
        default:
            DEBUG_VALUE2(APP_PB_SWI_ERR, MOD_APP_PB, "HandleErmATRes: invalid fctId %d (rsp %d)", 
                           gAppPBContext.fct.common.fctId, rsp);

            _APP_PHONEBOOK_vReleaseFctContext();
            break;
    }

}
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vHandleErmATOk                                       */
/** 
    \brief      handle event from ERM when AT Parser sent OK 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT OK \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_vHandleErmATOk(void)
{
    DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "HandleErmATOk: fctId %d", 
                   gAppPBContext.fct.common.fctId);
    
    APP_PHONEBOOK_vHandleErmATResult(AT_RSP_OK);

}/* END OF APP_PHONEBOOK_vHandleErmATOkt() */

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vHandleErmATError                                    */
/** 
    \brief      handle event from ERM when AT Parser sent ERROR 
  
    \brief      Description:\n
                handles event from ERM when AT Parser sent AT ERROR \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_vHandleErmATError(void)
{
    DEBUG_VALUE1(APP_PB_SWI_TRC, MOD_APP_PB, "vHandleErmATError: fctId %d", 
                   gAppPBContext.fct.common.fctId);
    
    APP_PHONEBOOK_vHandleErmATResult(AT_RSP_ERROR);
}/* END OF APP_SMS_vHandleErmATErrorEvent() */


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_vOnErmNotification                                 */
/** 
    \brief      handle Notifications of ERM
  
    \brief      Description:\n
                handle Notifications of ERM \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_vOnErmNotification(const ERMNotificationType * pNotification)
{
    switch(pNotification->common.id)
    {
        //case ERM_NOTIFY_PHONE_ACTIVE_STATE_CHANGED:
        case ERM_NOTIFY_PBSN_PHONE:
            if( ERM_u8GetActivePhone() != AT_INVALID_DEVICE_ID )
            {
                const ERMUserProfileType * userProfile = NULL;

                if(KPB_bIsAsgAvailable() == BT_TRUE)
                {
                    (void)_APP_PHONEBOOK_sfActivatePpds(BT_TRUE);
                }

                // do not try to update PbSizes if there is no user profile
                userProfile = ERM_pGetActiveUserProfile();
                if((NULL != userProfile) && (BT_TRUE == userProfile->bPbLoadedCompletly) )
                {
                    // SUSANNE: 15.10.2009
                    // Skip this here cause Parrot recommends to request pb sizes not before 
                    // PPBU:0 has been received.    
                    
                    DEBUG_TEXT(APP_PB_SWI_TRC, MOD_APP_PB, "ErmNot: SkipUpdPbSizes");
                    gAppPBContext.bFirstPPBU0ReceivedAlready = BT_FALSE;
                    //(void)_APP_PHONEBOOK_sfUpdatePbSizes();
                }
                break;
            }
            else
            {
                /* reinit after phone disconnect */
                (void)_APP_PHONEBOOK_vReInitApp();
            }
            break;
        default:
            /* ignore notification */
            break;
    }
}



/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONEBOOK_ERM_vHandleErmStateChange                             */
/** 
    \brief      handle State change of ERM 
  
    \brief      Description:\n
                handle State change of ERM \n

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_PHONEBOOK_ERM_vHandleErmStateChange(ERMStateType newState)
{
#if 0
    switch( newState )
    {
        case ERM_STATE_CK5050_BT_ACTIVE:
        {
            if(KPB_bIsAsgAvailable() == BT_TRUE)
            {
                _APP_PHONEBOOK_sfActivatePpds(BT_TRUE);
            }

            _APP_PHONEBOOK_sfUpdatePbSizes();
            break;
        }
        default:
        {
            APP_PHONEBOOK_vReInitApp();
            break;
        }
    }
#endif

}/* END OF APP_PHONE_vHandleErmStateChange() */

/* End Of File APP_PHONEBOOK_ERM_C1.c */


