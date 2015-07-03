/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_AT_PNCC_C1.c
*-----------------------------------------------------------------------------
* Module Name:    Telephone Application
*-----------------------------------------------------------------------------
* Description:    Handles all call specific responses from
                  AT Cmd Parser
*-----------------------------------------------------------------------------
* $Date: 2008-08-21 10:54:08 +0200 (Do, 21 Aug 2008) $
* $Rev: 857 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/src/APP_PHONE_AT_C1.c $
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
#include "ATParser.h"
#include "POOL.h"
#include "VCR.h"
#include "CONTEXT_MAN.h"
#include "APP_SD.h"
#include "APP_DEVICE_AudioManager.h"


#include "APP_PHONE_AT_PNCC_C1.id"
#include "TRACE.h"

#include "APP_PHONE_CI.h"
#include "APP_PHONE.h"
#include "APP_PHONE_AT.h"
#include "APP_PHONE_AT_PNCC.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/**
\brief	variable to save the current call management function currently in progress
*/
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
static SuccessFailType _APP_PHONE_AT_PNCC_sfHandlePnccValidState(void);
static SuccessFailType _APP_PHONE_AT_PNCC_sfHandlePlccCompleteState(void);
//static SuccessFailType _APP_PHONE_AT_PNCC_sfSetAudioMute(void);
static SuccessFailType _APP_PHONE_AT_PNCC_sfSetMicroMute(void);
//static SuccessFailType _APP_PHONE_AT_PNCC_sfSetHandsFree();
static SuccessFailType _APP_PHONE_AT_PNCC_sfSetHeadSetOff(void);

static BooleanType _APP_PHONE_AT_PNCC_vSearchSDBMHashTable(Uint32Type newHash);
static SuccessFailType _APP_PHONE_AT_PNCC_vAddToSDBMHashTable(Uint32Type newHash);

static void _APP_PHONE_AT_PNCC_vReplaceSpecialNumber(ATRspCallStatValType *plcc);

static SuccessFailType
_APP_PHONE_AT_PNCC_vSearchAndReplaceSpecialNumber(ATRspCallStatValType *pPlcc, const Uint8Type *pSearchNum, Uint16Type searchNumLen,
                                                                               const Uint8Type *pText,      Uint16Type textLen );


/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the private variable
*/
static AppPhoneAtPnccContext gPnccContext;
extern AppPhoneContext gAppPhoneContext;

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_sfInit                              */
/**
    \brief      init plcc context structure

    \brief      init plcc context structure

    \return     SuccessFailType
                SUCCESS or FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_PNCC_sfInit()
{
    (void)memset(&gPnccContext, 0, sizeof(gPnccContext));

    return SUCCESS;
}
/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_vHandleCallStatusCount                              */
/**
    \brief      handle call status count response msg from AT Cmd Parser

    \brief      Description:\n
                handles call status count response msgs from AT Cmd Parser \n

    \param      pPncc
                ATRspCallStatCountType *
                pointer to at response

    \return     SuccessFailType
                SUCCESS or FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_PNCC_sfHandleCallStatusCount(const ATRspCallStatCountType *pPncc)
{
    SuccessFailType result = SUCCESS;
    Uint16Type i = 0U;
    BooleanType bCallStatusChanged = BT_FALSE;

    if( gPnccContext.currentPncc > pPncc->value )//PNCC value decreased
    	bCallStatusChanged = BT_TRUE;

    /* Update the global PNCC value */
    gPnccContext.currentPncc = pPncc->value;

    for(i=0U; i<MAX_PLCC_ELEMENTS; i++)
    {
        gPnccContext.changedState[i] = ARRAY_ELEM_UNKNOWN;
        gPnccContext.CallInfoHasChanged[i] = ARRAY_ELEM_UNKNOWN;
    }

    if( gPnccContext.currentPncc == 0U )
    {
        gPnccContext.pnccState = PLCC_COMPLETE;
        result = _APP_PHONE_AT_PNCC_sfHandlePlccCompleteState();
    }
    else
    {
        gPnccContext.pnccState = WAIT_FOR_PLCC;
    }

    if( bCallStatusChanged == BT_TRUE ) //one call decrease
    	//inform about hang up one call successfully done
    	APP_PHONE_HangUp_CallStatusChange();

    return result;

}/* END OF _APP_PHONE_vHandleCallStatusCount() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_vHandleCallStatusIndex                              */
/**
    \brief      handle call status index response msg from AT Cmd Parser

    \brief      Description:\n
                handles call status index response msgs from AT Cmd Parser \n

    \param      pPlcc
                ATRspCallStatValType *
                pointer to at response

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_PNCC_sfHandleCallStatusIndex(const ATRspCallStatValType *pConstPlcc)
{
    // PLCC is sent after an AT*PLCC request ( CallStatus / CallInfo )
    // PLCC will be sent also when there is an incoming call
    ATRspCallStatValType Plcc;
    ATRspCallStatValType *pPlcc;
    SuccessFailType result  = SUCCESS;

    // make a local copy of the const data, contained in plcc
    pPlcc = &Plcc;

    memcpy(pPlcc, pConstPlcc, sizeof(*pConstPlcc));

    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "Size of Plcc %d", sizeof(*pConstPlcc));

    DEBUG_VALUE2(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "[PLCC] recvd index: %d len: %d",
                    pPlcc->index,
                    pPlcc->caller_id_len);

    _APP_PHONE_AT_PNCC_vReplaceSpecialNumber(pPlcc);

    switch(gPnccContext.pnccState)
    {
    case WAIT_FOR_PLCC:
        if(pPlcc->index >= MAX_PLCC_ELEMENTS)
        {
            return FAIL;
        }

        if( gPnccContext.plcc[pPlcc->index] == NULL )
        {
            /* this position is empty, so just allocate memory and copy the
             * element */
            gPnccContext.changedState[pPlcc->index] = ARRAY_ELEM_ADDED;
            gPnccContext.CallInfoHasChanged[pPlcc->index] = ARRAY_ELEM_ADDED;

            result = POOL_sfAllocate( POOL_enATRspCallStatValType,
                                      (void**)&gPnccContext.plcc[pPlcc->index]);

            if(result != SUCCESS)
            {
                return FAIL;
            }

            (void)memcpy(gPnccContext.plcc[pPlcc->index], pPlcc, sizeof(*pPlcc));

            DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "added");
        }
        else
        {
            /* there is already one element at this position, so first check if the call info changed */
           Sint32Type ret;

           if((pPlcc->caller_id_len == gPnccContext.plcc[pPlcc->index]->caller_id_len)&&
                   ((memcmp(gPnccContext.plcc[pPlcc->index]->caller_id,
                           pPlcc->caller_id,
                           pPlcc->caller_id_len)) == 0))
           {
               gPnccContext.CallInfoHasChanged[pPlcc->index] = ARRAY_ELEM_NOT_CHANGED;
               DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, " Call Info not changed");
           }
           else
           {
               gPnccContext.CallInfoHasChanged[pPlcc->index] = ARRAY_ELEM_ADDED;
               DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, " Call Info changed");
           }

            /* there is already one element at this position, so check if it has
             * changed */
            ret = memcmp(gPnccContext.plcc[pPlcc->index], pPlcc, sizeof(*pPlcc));

            /* the ringing in progress is not directly mapped in BAP */
            if((ret == 0U)||(pPlcc->state == AT_CS_OUTCALL_RING_IPR)||
              ((pPlcc->state == gPnccContext.plcc[pPlcc->index]->state)&&(gPnccContext.CallInfoHasChanged[pPlcc->index] == ARRAY_ELEM_ADDED)))
            {
                /* the elements are the same */
                gPnccContext.changedState[pPlcc->index] = ARRAY_ELEM_NOT_CHANGED;
                (void)memcpy(gPnccContext.plcc[pPlcc->index], pPlcc, sizeof(*pPlcc));
                DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, " Call Array not changed");
            }
            else
            {
                /* the elements are not the same */

                /* first check if state changed from incoming / ringing to terminating
                 * This is an indication for a missed call */
                if((AT_CS_INCOMING        == gPnccContext.plcc[pPlcc->index]->state ||
                    AT_CS_WAITING_CALL    == gPnccContext.plcc[pPlcc->index]->state ) &&
                    AT_CS_CALL_TERMINATED == pPlcc->state )
                {
                    // *** BEGIN Susanne: 06.08.2009: fixed this, now counting all during uhv active
                    //gAppPhoneContext.u16MissedCallsDuringKl15Off++;

                    // SUSANNE: 28.08.2009 BEGIN
                    // first check for unknown caller id ( empty or e.g. '+' sent by some phones when number is supressed )
                    // for these cases we will not indicate a new missed call!!!
                    if(BT_TRUE == APP_PHONE_bIsUnknownCallerId(pPlcc->caller_id,pPlcc->caller_id_len))
                    {
                        // ignore this call
                    }
                    else
                    {
                        // SUSANNE: 28.08.2009 END

                        // check if number already registered !!!!
                        Uint32Type newHash = APP_COMMON_u32GetSDBM_Hash(pPlcc->caller_id, pPlcc->caller_id_len);
                        if( BT_FALSE == _APP_PHONE_AT_PNCC_vSearchSDBMHashTable(newHash))
                        {
                            // number not yet registered
                            // try to add it to the table
                            _APP_PHONE_AT_PNCC_vAddToSDBMHashTable(newHash);
                            // increment counter
                            gAppPhoneContext.u16MissedNumbers++;
                        }

                        // now send the notificationm but make sure we have at least one entry ( pb size )
                        // so the hmis are able to request something
                        //if(0U < APP_PHONEBOOK_u16GetCurrentPBSize(APP_PHONEBOOK_enMissedCalls))
                        //{
                        //    // TODO: move the notification to APP_PB when pb size has been updated
                        //    _APP_PHONE_vNotifyApp(APP_PHONE_MISSED_CALLS);
                        //}

                        // increase the missed calls in any case
                        gAppPhoneContext.u16MissedCalls++;
                        // *** END Susanne: 06.08.2009: fixed this, now counting all during uhv active
                    }
                }
                else if( (AT_CS_INCOMING    == gPnccContext.plcc[pPlcc->index]->state) &&
                         (AT_CS_ACTIVE_CALL == pPlcc->state) )
                {
                    if(gAppPhoneContext.isHandsFreeOff == BT_TRUE)
                    {
                        // RT:3258 per default we assume the HANDSFREE mode,
                        // if no PATR:1 is received within APP_PHONE_PATR_TIMEOUT ms we set the mode back
                        // to PRIVATE-MODE by the alarm callback
                        DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE,
                                            "sfHaCallStatIdx: incom -> act call, def HANDSFREE" );

                        gAppPhoneContext.isHandsFreeOff = BT_FALSE;
                        _APP_PHONE_vNotifyApp(APP_PHONE_HANDSFREE_MODE_CHANGED);

                        (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_PHONE_PATR_WORKAROUND_TIMER,
                                                                  MSEC(APP_PHONE_PATR_TIMEOUT), 0);
                    }
                }

                /* copy new plcc data */
                gPnccContext.changedState[pPlcc->index] = ARRAY_ELEM_ADDED;
                (void)memcpy(gPnccContext.plcc[pPlcc->index], pPlcc, sizeof(*pPlcc));

                DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "overwritten");
            }
        }


        if(_APP_PHONE_AT_PNCC_u16GetValidPlccNumber() == gPnccContext.currentPncc)
        {
            gPnccContext.pnccState = PLCC_COMPLETE;
            result = _APP_PHONE_AT_PNCC_sfHandlePlccCompleteState();
        }
        break;

    default:
        return FAIL;
    }

    return result;
}/* END OF _APP_PHONE_vHandleCallStatusCount() */


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_vSearchSDBMHashTable                              */
/**
    \brief      searches SDBM Hash Table for a given hash value
                Function returns true, if it is already included in table, else FALSE

    \brief      Description:\n
                handles call status index response msgs from AT Cmd Parser \n

    \param      newHash
                Uint32Type
                new hash value to be searched for

    \return     BooleanType
                BT_TRUE or BT_FALSE
*/
/*----------------------------------------------------------------------------*/
BooleanType _APP_PHONE_AT_PNCC_vSearchSDBMHashTable(Uint32Type newHash)
{
    // search table
    Uint16Type idx      = 0;
    // we support maximum APP_PHONE_MAX_CALL_STACK_SIZE entries but number of missed numbers might
    // be greater, so to avoid
    Uint16Type maxIdx   = gAppPhoneContext.u16MissedNumbers % APP_PHONE_MAX_CALL_STACK_SIZE;

    for(idx=0; idx < maxIdx; idx++)
    {
        if(newHash == gAppPhoneContext.u32MissedCallerIdHashTable[idx])
        {
            return BT_TRUE;
        }
    }
    return BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_vAddToSDBMHashTable                              */
/**
    \brief      tries to add a new hash to the table

    \param      newHash
                Uint32Type
                new hash value to be added

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_PNCC_vAddToSDBMHashTable(Uint32Type newHash)
{
    // current number of missed numbers gives the index. If the table is full, we
    // start at the beginning, so the 20 newest numbers are always within the table
    Uint16Type idx = ( gAppPhoneContext.u16MissedNumbers ) % APP_PHONE_MAX_CALL_STACK_SIZE;
    gAppPhoneContext.u32MissedCallerIdHashTable[idx] = newHash;

    DEBUG_VALUE2(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "AddedHash: 0x%08x at idx %d", newHash, idx);

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_sfHandlePlccCompleteState                              */
/**
    \brief      handle state when all plccs have been received from CK5050

    \brief      Description:\n
                handle state when all plccs have been received from CK5050 \n

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_AT_PNCC_sfHandlePlccCompleteState()
{
    SuccessFailType result = SUCCESS;
    Sint16Type i;

    /* Look for unhandled previous plcc'c */
    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.changedState[i] == ARRAY_ELEM_UNKNOWN)
        {
            if( gPnccContext.plcc[i] == NULL)
            {
                gPnccContext.changedState[i] = ARRAY_ELEM_NOT_CHANGED;
                gPnccContext.CallInfoHasChanged[i] = ARRAY_ELEM_NOT_CHANGED;

                DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "[PLCC Complete] index %d not changed", i);
            }
            else
            {
                gPnccContext.changedState[i] = ARRAY_ELEM_REMOVED;
                gPnccContext.CallInfoHasChanged[i] = ARRAY_ELEM_REMOVED;
                (void)POOL_sfFree(gPnccContext.plcc[i]);
                gPnccContext.plcc[i] = NULL;

                DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "[PLCC Complete] index %d freed", i);
            }
        }
    }

    gPnccContext.pnccState = PNCC_VALID;
    result = _APP_PHONE_AT_PNCC_sfHandlePnccValidState();

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_sfHandlePnccValidState                    */
/**
    \brief      handles the pncc valid state

    \brief      Description:\n
                On pncc valid we have all information (plcc's) to send out
                the depending messages \n

    \return     SuccessFailType
                SUCCESS or FAIL

*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_AT_PNCC_sfHandlePnccValidState()
{
    SuccessFailType result = SUCCESS;


    if(APP_PHONE_AT_PNCC_bAllCallStatesIdle())
    {
        // unmute audio sources if there is no active call
        DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPnccValState: unmute phone" );
        APP_DEVICE_AM_vReleaseMute(NULL, APP_CAN_enTelephoneCall, __LINE__);
        
        gAppPhoneContext.isHangUpSent = BT_FALSE;
        gAppPhoneContext.isHandsFreeOff = BT_TRUE;
    }
    else
    {
        if (APP_PHONE_AT_PNCC_u16GetNumberOfCalls() != APP_PHONE_AT_PNCC_u16GetNumberOfTerminatedCalls())
        { 
            /* make sure UHV is active mute source with reason phone call, even if we have it already */
            if (APP_PHONE_AT_PNCC_u16GetNumberOfIncomingCalls())
            {
                if (APP_SD_getCurrentCtx() == APP_SD_CONTEXT_S642)
                {
                    const ERMUserProfileType *pActProfile = ERM_pGetActiveUserProfile();
                    DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPnccValState: Set active profile ringtone RING mute phone" );                
                    APP_DEVICE_rcStartRingtoneNonBlock(pActProfile->ringtone);
                }
                else
                {
                    DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPnccValState: RING mute phone" );
                    (void)APP_DEVICE_AM_sfGetMute(NULL, APP_CAN_enRing, __LINE__ );
                }
            }
            else
            {
                DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPnccValState: CALL mute phone" );
                (void)APP_DEVICE_AM_sfGetMute(NULL, APP_CAN_enTelephoneCall, __LINE__ );
            }
        }
        else
        {
            DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPnccValState: call is terminated" );
        }
    }

    if(APP_PHONE_AT_PNCC_GetOutgoingCallIndex() != AT_CALL_INDEX_NONE)
    {
        (void)APP_COMMON_UTILS_stCancelOsekRelAlarm(CTA_APP_PHONE_OUTGOING_CALL_INDICATION_TIMER);
        DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "HaPnccValState: cancel alarm" );
    }

    // When PATR:0 has been received, we set an alarm to avoid indicating a PATR:0 / PATR:1 toggling
    // which sometimes occurrs
    // When we get an indication for a terminating call or a hold call shortly after receiving PATR:0
    // we will not indicate the change
    if( APP_PHONE_AT_PNCC_u16GetNumberOfHeldCalls() > 0 ||
        APP_PHONE_AT_PNCC_u16GetNumberOfTerminatedCalls() > 0 )
    {
        (void)APP_COMMON_UTILS_stCancelOsekRelAlarm( CTA_APP_PHONE_PATR_WORKAROUND_TIMER );
    }

    if( APP_PHONE_AT_PNCC_u16GetNumberOfCallType(AT_CS_OUTCALL_RING_IPR) > 0 )
    {
        if(gAppPhoneContext.isHandsFreeOff == BT_TRUE)
        {
            /* RT:3231 we simulate the PATR:0 event, default state of the CK5050 */
            DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "sim patr:0, isHandsFreeOff %d",
                                gAppPhoneContext.isHandsFreeOff );

            _APP_PHONE_vNotifyApp(APP_PHONE_HANDSFREE_MODE_CHANGED);
        }
    }



    // reset MicroMute to NotMuted if all Call states are idle again cause user might
    // have changed MicroMuteOnOff during last call
    result = _APP_PHONE_AT_PNCC_sfSetMicroMute();

    // reset the audio state to handsfree and not to Headset
    result = _APP_PHONE_AT_PNCC_sfSetHeadSetOff();			// RT 2276

    //_APP_PHONE_AT_PNCC_vReplaceSpecialNumber();             // RT 3636

    // notify apps about plccs
    _APP_PHONE_vNotifyApp(APP_PHONE_CALL_STATE_CHANGED);

    gPnccContext.pnccState = PNCC_INIT;

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_vReplaceSpecialNumber                     */
/**
    \brief      handles the special handling for info / service / SOS and Mailbox

    \brief      Description:\n
                The Plcc phone routine change the name and all application \n
                screened the same name \n

    \return     none

*/
/*----------------------------------------------------------------------------*/
static void _APP_PHONE_AT_PNCC_vReplaceSpecialNumber(ATRspCallStatValType *plcc)
{
    ATPhoneNumberType u8SpecialNumber;
    SuccessFailType result              = FAIL;
    Uint16Type phoneNumLen              = AT_MAX_PHONE_NUM_LEN;
    const ERMUserProfileType * profile  = ERM_pGetActiveUserProfile();
    Uint16Type  textLen                 = 0;
    const Uint8Type *pText              = NULL;
    Uint8Type u8Roaming                 = APP_PHONE_GetRoaming();

    // replace unknown callerids ('','+', ';' with unknown text )
    if (BT_TRUE == APP_PHONE_bIsUnknownCallerId(plcc->caller_id, plcc->caller_id_len))
    {
        result = APP_SD_sfGetTextForNumberType(APP_SD_NUMBER_TYPE_UNKNOWN_CALLER_ID, &pText, &textLen);
        if ((SUCCESS == result) && (pText != NULL))
        {
            plcc->id_avail = AT_TRUE;
            (void)memcpy(plcc->caller_id, pText, textLen);
            plcc->caller_id_len = textLen;
            DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "repl unknown callerids ('','+', ';')" );
            //continue; // only replace once, so continue with next plcc
        }
    }
    // check if callerId is number saved for mailbox for active user, if so, show
    // 'Mailbox' instead of number or other 'Mailbox' names e.g. Voicebox
    result = APP_SD_sfGetTextForNumberType(APP_SD_NUMBER_TYPE_MAILBOX, &pText, &textLen);
    if( (SUCCESS == result) && (pText != NULL) && (NULL != profile) && (0 < profile->voicemailbox_len))
    {
        // we have a mailbox number and an associated text, check if we have a caller_id matchin the mailbox number and replace it
        if(SUCCESS == _APP_PHONE_AT_PNCC_vSearchAndReplaceSpecialNumber(plcc, profile->voicemailbox, profile->voicemailbox_len,
                                                                       pText, textLen ))
        {
            DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "ReplSpecNumb: Mailbox");
            //continue; // only replace once, so continue with next plcc
        }
    }

    /* check for info call first, differ between romaing or not */
    if(0 == u8Roaming)  // roaming not active
    {
        result = CONTEXT_MAN_sfGetInfoCallNumber(&u8SpecialNumber, &phoneNumLen);
    }
    else
    {   // romaing active
        result = CONTEXT_MAN_sfGetInfoCallNumberRoaming(&u8SpecialNumber, &phoneNumLen);
    }

    if( (SUCCESS == result) && (0 < phoneNumLen))
    {
        result = APP_SD_sfGetTextForNumberType(APP_SD_NUMBER_TYPE_INFO, &pText, &textLen);
        if( (SUCCESS == result) && NULL != pText)
        {
            // we have a mailbox number and an associated text, check if we have a caller_id matchin the mailbox number and replace it
            if(SUCCESS == _APP_PHONE_AT_PNCC_vSearchAndReplaceSpecialNumber(plcc, u8SpecialNumber, phoneNumLen,
                                                                           pText, textLen ))
            {
                DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "ReplSpecNumb: Info, roam: %d" , u8Roaming);
                //continue; // only replace once, so continue with next plcc
            }
        }
    }

    /* now check for service call, only for VW, differ between romaing or not */
    if (APP_CAN_VW_Volkswagen == CONTEXT_MAN_sfGetFahrzeugTyp())
    {
        if(0 == u8Roaming)  // roaming not active
        {
            result = CONTEXT_MAN_sfGetServiceCallNumber(&u8SpecialNumber, &phoneNumLen);
        }
        else
        {   // roaming active
            result = CONTEXT_MAN_sfGetServiceCallNumberRoaming(&u8SpecialNumber, &phoneNumLen);
        }

        if( (SUCCESS == result) && (0 < phoneNumLen))
        {
            result = APP_SD_sfGetTextForNumberType(APP_SD_NUMBER_TYPE_SERVICE, &pText, &textLen);
            if( (SUCCESS == result) && NULL != pText)
            {
                // we have a mailbox number and an associated text, check if we have a caller_id matchin the mailbox number and replace it
                if(SUCCESS == _APP_PHONE_AT_PNCC_vSearchAndReplaceSpecialNumber(plcc, u8SpecialNumber, phoneNumLen,
                                                                               pText, textLen ))
                {
                    DEBUG_VALUE1(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "ReplSpecNumb: Serv, roam: %d", u8Roaming);
                    //continue; // only replace once, so continue with next plcc
                }
            }
        }
    }
    /* now check for SOS call, only for Skoda */
    else if (APP_CAN_SK_Skoda == CONTEXT_MAN_sfGetFahrzeugTyp())
    {
        /* check if call a SOS call */
        result = CONTEXT_MAN_sfGetSOSCallNumber(&u8SpecialNumber, &phoneNumLen);

        if( (SUCCESS == result) && (0 < phoneNumLen))
        {
            result = APP_SD_sfGetTextForNumberType(APP_SD_NUMBER_TYPE_SOS, &pText, &textLen);
            if( (SUCCESS == result) && NULL != pText)
            {
                // we have a mailbox number and an associated text, check if we have a caller_id matchin the mailbox number and replace it
                if(SUCCESS == _APP_PHONE_AT_PNCC_vSearchAndReplaceSpecialNumber(plcc, u8SpecialNumber, phoneNumLen,
                                                                               pText, textLen ))
                {
                    DEBUG_TEXT(APP_PHONE_SWI_TRC, MOD_APP_PHONE, "ReplSpecNumb: SOS" );
                    //continue; // only replace once, so continue with next plcc
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_vSearchAndReplaceSpecialNumber            */
/**
    \brief      handles the special handling for info / service / SOS and Mailbox

    \brief      Description:\n
                The Plcc phone routine change the name and all application \n
                show the same name \n

    \return     SuccessFailType
                FAIL meand, not found or error, no replacment
                SUCCESS means number has been found and replaced by pText

*/
/*----------------------------------------------------------------------------*/
static SuccessFailType
_APP_PHONE_AT_PNCC_vSearchAndReplaceSpecialNumber(ATRspCallStatValType *pPlcc, const Uint8Type *pSearchNum, Uint16Type searchNumLen,
                                                                               const Uint8Type *pText,      Uint16Type textLen )
{
    BooleanType bNeedToReplace = BT_FALSE;

    // check if pPlcc, pText and pSearchNum not NULL
    if(NULL == pPlcc        ||
       NULL == pSearchNum   || NULL == pText )
    {
        return FAIL;
    }

    // 1.1 compare if length are the same
    if( searchNumLen == pPlcc->caller_id_len)
    {
        // 1.2. compare caller_id with pSearchNum
        if ( 0 == memcmp(pSearchNum, pPlcc->caller_id, searchNumLen ))
        {   // numbers match, so replace it with pText and return SUCCESS
            bNeedToReplace = BT_TRUE;
        }
    }
    // 2.1 compare if length are the same, but only if caller_id_ext is available
    else if(pPlcc->ext_avail && searchNumLen == pPlcc->caller_id_ext_len)
    {
        // 1.2. compare caller_id with pSearchNum
        if( 0 == memcmp(pSearchNum, pPlcc->call_id_ext, searchNumLen) )
        {   // numbers match, so replace it with pText and return SUCCESS
            // WE COPY THE TEXT into caller_id , too
            bNeedToReplace = BT_TRUE;
        }
    }
    if(BT_TRUE == bNeedToReplace )
    {
        pPlcc->caller_id_len = textLen > sizeof(pPlcc->caller_id) ?
                                    sizeof(pPlcc->caller_id) : textLen;

        (void)memcpy(pPlcc->caller_id, pText, textLen);
        return SUCCESS;
    }

    return FAIL;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_pGetPnccContext                          */
/**
    \brief      returns pointer to current pncc / Plcc context

    \brief      Description:\n
                returns pointer to current pncc / plcc context

    \return     const AppPhoneAtPnccContext *
                pointer to plcc context structure

*/
/*----------------------------------------------------------------------------*/
const AppPhoneAtPnccContext * APP_PHONE_AT_PNCC_pGetPnccContext()
{
    return /*(const AppPhoneAtPnccContext*)*/&gPnccContext;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_pGetPlccContext                          */
/**
    \brief      returns pointer to current pncc / Plcc context

    \brief      Description:\n
                returns pointer to current pncc / plcc context

    \param      index
                ATCallIndexEnum
                plcc which shall be returned, valid are 0 to
                (MAX_PLCC_ELEMENTS - 1)

    \return     const AppPhoneAtPnccContext *
                pointer to specific plcc index in context structure
*/
/*----------------------------------------------------------------------------*/
const ATRspCallStatValType *APP_PHONE_AT_PNCC_pGetPlccContext(ATCallIndexEnum index)
{
    if((Uint8Type)index < MAX_PLCC_ELEMENTS)
    {
        return gPnccContext.plcc[index];
    }

    return NULL;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_sfSetPnccDefaultState                     */
/**
    \brief      reset plcc elements to default values

    \brief      Description:
                reset plcc elements to default values \n

    \return     SuccessFailType
                SUCCESS or FAIL

*/
/*----------------------------------------------------------------------------*/
SuccessFailType _APP_PHONE_AT_PNCC_sfSetPnccDefault()
{
    SuccessFailType result = SUCCESS;
    Uint16Type i = 0U;

    /* Update the global PNCC value */
    gPnccContext.currentPncc = 0;

    for(i=0U; i<MAX_PLCC_ELEMENTS; i++)
    {
        gPnccContext.changedState[i] = ARRAY_ELEM_UNKNOWN;
    }

    gPnccContext.pnccState = PLCC_COMPLETE;
    result = _APP_PHONE_AT_PNCC_sfHandlePlccCompleteState();

    return result;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_u16GetValidPlccNumber                          */
/**
    \brief      returns the number of ongoing calls

    \brief      returns the number of ongoing calls

    \return     Uint16Type
                number of ongoing calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type _APP_PHONE_AT_PNCC_u16GetValidPlccNumber()
{
    Sint16Type i;
    Uint16Type count = 0U;

    for(i=0U; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i] && ( ARRAY_ELEM_UNKNOWN != gPnccContext.changedState[i] ))
        {
            count++;
        }
    }
    return count;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_bArrayHasChanged                           */
/**
    \brief      checks if plcc-Array has changed since last update

    \brief      Description:\n
                checks if plcc-Array has changed since last update

    \return     BooleanType
                BT_FALSE = unchanged, BT_TRUE = changed
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_AT_PNCC_bArrayHasChanged()
{
    Sint16Type i;
    BooleanType hasChanged = BT_FALSE;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.changedState[i] != ARRAY_ELEM_NOT_CHANGED)
        {
            hasChanged = BT_TRUE;
        }
    }
    return hasChanged;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_bCallInfoHasChanged                           */
/**
    \brief      checks if the call info has changed since last update

    \brief      Description:\n
                checks if the call info has changed since last update

    \return     BooleanType
                BT_FALSE = unchanged, BT_TRUE = changed
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_AT_PNCC_bCallInfoHasChanged()
{
    Sint16Type i;
    BooleanType hasChanged = BT_FALSE;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.CallInfoHasChanged[i] != ARRAY_ELEM_NOT_CHANGED)
        {
            hasChanged = BT_TRUE;
        }
    }
    return hasChanged;
}
//APP_PHONE_enCallIndex _APP_PHONE_AT_PNCC_GetCurrentHeldCallIndex()
//{
//    APP_PHONE_enCallIndex heldCalls = HOLD_CALLS_NONE;
//
//    if( gPnccContext.plcc[0] != NULL )
//    {
//        if( AT_CS_HELD_CALL == gPnccContext.plcc[0]->state )
//        {
//            
//            heldCalls = HOLD_CALLS_INDEX_0;
//        }
//    }
//    if( gPnccContext.plcc[1] != NULL )
//    {
//        if( AT_CS_HELD_CALL == gPnccContext.plcc[1]->state )
//        {
//            if( HOLD_CALLS_INDEX_0 == heldCalls )
//            {
//                
//                // seems that both calls are onHold, so chup both of them
//                heldCalls = HOLD_CALLS_BOTH;
//            }
//            else
//            {
//                
//                heldCalls = HOLD_CALLS_INDEX_1;
//            }
//        }
//    }
//    
//    return heldCalls;
//}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_GetCurrentActiveCallIndex                           */
/**
    \brief      returns the first plcc index which has active state

    \brief      Description:\n
                returns the first plcc index which has active state

    \return     APP_PHONE_enCallIndex
                active call index, if any
*/
/*----------------------------------------------------------------------------*/
APP_PHONE_enCallIndex _APP_PHONE_AT_PNCC_GetCurrentActiveCallIndex()
{
    APP_PHONE_enCallIndex activeCall = APP_PHONE_CALL_NONE;
    Sint32Type i = 0;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if( gPnccContext.plcc[i] != NULL )
        {
            if( AT_CS_ACTIVE_CALL == gPnccContext.plcc[i]->state )
            {
                return (APP_PHONE_enCallIndex)i;
            }
        }
    }

    return activeCall;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_GetOutgoingCallIndex                           */
/**
    \brief      returns the first plcc index which has outgoing state

    \brief      Description:\n
                returns the first plcc index which has outgoing state

    \return     ATCallIndexEnum
                outgoing call index, if any
*/
/*----------------------------------------------------------------------------*/
ATCallIndexEnum APP_PHONE_AT_PNCC_GetOutgoingCallIndex()
{
    ATCallIndexEnum outgoingCall = AT_CALL_INDEX_NONE;
    Uint32Type i = 0;

    for(i=0; i<MAX_PLCC_ELEMENTS && i<AT_CALL_INDEX_NONE; i++)
    {
        if( gPnccContext.plcc[i] != NULL )
        {
            if( (AT_CS_OUTCALL_DIAL_IPR == gPnccContext.plcc[i]->state) ||
                (AT_CS_OUTCALL_RING_IPR == gPnccContext.plcc[i]->state) )
            {
                return (ATCallIndexEnum)i;
            }
        }
    }

    return outgoingCall;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_bAllCallStatesIdle                           */
/**
    \brief      returns whether all call indexes are in idle state

    \brief      Description:\n
                returns whether all call indexes are in idle state

    \return     BooleanType
                BT_FALSE or BT_TRUE
*/
/*----------------------------------------------------------------------------*/
BooleanType APP_PHONE_AT_PNCC_bAllCallStatesIdle()
{
    //Sint32Type i = 0;

    if( gPnccContext.currentPncc > 0)
    {
        return BT_FALSE;
        //for(i=0; i<MAX_PLCC_ELEMENTS; i++)
        //{
        //    if(gPnccContext.plcc[i])
        //    {
        //        if( AT_CS_CALL_TERMINATED != gPnccContext.plcc[i]->state )
        //        {
        //            // there is at least one call not in idle / terminating state.
        //            // do not reset HandsFreeMode
        //            return BT_FALSE;
        //        }
        //    }
        //}
    }
    return BT_TRUE;
}
// RS: not longer needed
//*----------------------------------------------------------------------------*/
//* Function    : _APP_PHONE_AT_PNCC_sfSetAudioMute                           */
//**
//    \brief      if all calls are idle again, Audio Mute for Phone Call is unmuted
//
//    \brief      Description:\n
//                if all calls are idle again, Audio Mute for Phone Call is unmuted
//
//    \return     SuccessFailType
//                SUCCESS or FAIL
//*/
//*----------------------------------------------------------------------------*/
//static SuccessFailType _APP_PHONE_AT_PNCC_sfSetAudioMute()
//{
//    // TODO: save last state and update only if state changes!!!
//
//    if( BT_TRUE == APP_PHONE_AT_PNCC_bAllCallStatesIdle() )
//    {
//        // all call states are idle
//        // unmute AudioSources
//        return MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioUnmute, enMuteManagerTelephoneCall , __FILE__, __LINE__);
//    }
//
//    // if there is an active, pending, incoming call or whatever
//    // mute the audio source
//
//    return SUCCESS;//MUTE_MANAGER_sfSetAudioMute(enMuteManagerAudioMute, enMuteManagerTelephoneCall, __FILE__, __LINE__ ); //TB
//}

#if 0
SuccessFailType _APP_PHONE_AT_PNCC_sfSetHandsFree()
{
    if( APP_PHONE_AT_PNCC_bAllCallStatesIdle() )
    {
        /* check if HandsFree was set to off */
        if( BT_TRUE == APP_PHONE_bIsHandsFreeOff())
        {
            /* we need to reset HandsFree to on */
            if( APP_COMMON_RSP_ERROR_BUSY == APP_PHONE_rcHandsFreeOnOff(NULL, AT_AUDIO_HANDSFREE_MODE))
            {
                // we are busy, try again if current fct has been finished
                gAppPhoneContext.pending.resetHandsfreeToOn = BT_TRUE;
            }
        }
        /* else do nothing */
    }
    /* else do nothing */

    return SUCCESS;
}
#endif

SuccessFailType _APP_PHONE_AT_PNCC_sfSetHeadSetOff(void)			// RT 2276
{
    if( APP_PHONE_AT_PNCC_bAllCallStatesIdle() )
    {
        /* check if Headset was set to off */
        if( BT_TRUE == APP_PHONE_bIsHeadsetOn())
        {
            /* we need to reset HandsFree to on */
            if( APP_COMMON_RSP_ERROR_BUSY == APP_PHONE_rcHeadsetOnOff(NULL, AT_AUDIO_GW_MODE_OFF))
            {
                // we are busy, try again if current fct has been finished
                gAppPhoneContext.pending.resetHeadsetToOff = BT_TRUE;
            }
        }
        /* else do nothing */
    }
    /* else do nothing */

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_PHONE_AT_PNCC_sfSetMicroMute                           */
/**
    \brief      if all calls are idle again, Micro Mute is unmuted

    \brief      Description:\n
                if all calls are idle again, Micro Mute is unmuted

    \return     SuccessFailType
                SUCCESS or FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_PHONE_AT_PNCC_sfSetMicroMute()
{
    if( APP_PHONE_AT_PNCC_bAllCallStatesIdle() )
    {
        /* check if Micro was muted */
        if( BT_TRUE == APP_PHONE_bIsMicMuted())
        {
            /* we need to reset Micro not muted */
            if( APP_COMMON_RSP_ERROR_BUSY == APP_PHONE_rcMicroMuteOnOff(NULL, AT_MIC_UNMUTE))
            {
                // we are busy, try again if current fct has been finished
                gAppPhoneContext.pending.unmuteMicro = BT_TRUE;
            }
        }
        /* else do nothing */
    }
    /* else do nothing */

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfActiveCalls                  */
/**
    \brief      returns number of calls currently in active state

    \brief      Description:\n
                returns number of calls currently in active state

    \return     Uint16Type
                number of active calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfActiveCalls()
{
    int i;
    Uint16Type numActiveCalls = 0U;
    //CallState callState = BAP_IDLE;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            //callState   = AT2BapCallStateMatrix[gPnccContext.plcc[i]->state];
            if( AT_CS_ACTIVE_CALL == gPnccContext.plcc[i]->state )
            {
                numActiveCalls++;
            }
        }
    }
    return numActiveCalls;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfCalls                        */
/**
    \brief      returns number of calls 


    \return     Uint16Type
                number of calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfCalls()
{
    Uint16Type i = 0;
    Uint16Type numCalls = 0U;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            numCalls++;
        }
    }
    return numCalls;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfTerminatedCalls              */
/**
    \brief      returns number of calls currently in terminated state


    \return     Uint16Type
                number of terminated calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfTerminatedCalls()
{
    Uint16Type i = 0;
    Uint16Type numCalls = 0U;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            if( AT_CS_CALL_TERMINATED == gPnccContext.plcc[i]->state )
            {
                numCalls++;
            }
        }
    }
    return numCalls;
}
/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfHeldCalls                           */
/**
    \brief      returns number of calls currently in held state

    \brief      Description:\n
                returns number of calls currently in held state

    \return     Uint16Type
                number of held calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfHeldCalls()
{
    int i = 0;
    Uint16Type numHeldCalls = 0U;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            if( AT_CS_HELD_CALL == gPnccContext.plcc[i]->state )
            {
                numHeldCalls++;
            }
        }
    }
    return numHeldCalls;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfIncomingCalls                           */
/**
    \brief      returns number of calls currently in incoming state

    \brief      Description:\n
                returns number of calls currently in incoming state

    \return     Uint16Type
                number of incoming calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfIncomingCalls()
{
    int i = 0;
    Uint16Type numIncomCalls = 0U;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            if( AT_CS_INCOMING == gPnccContext.plcc[i]->state )
            {
                numIncomCalls++;
            }
        }
    }
    return numIncomCalls;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfWaitingCalls                           */
/**
    \brief      returns number of calls currently in waiting state

    \brief      Description:\n
                returns number of calls currently in waiting state

    \return     Uint16Type
                number of waiting calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfWaitingCalls()
{
    int i = 0;
    Uint16Type numWaitCalls = 0U;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            if( AT_CS_WAITING_CALL == gPnccContext.plcc[i]->state )
            {
                numWaitCalls++;
            }
        }
    }
    return numWaitCalls;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfOutgoingCalls                           */
/**
    \brief      returns number of calls currently in outgoing state

    \brief      Description:\n
                returns number of calls currently in outgoing state

    \return     Uint16Type
                number of outgoing calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfOutgoingCalls()
{
    int i = 0;
    Uint16Type numOutgoingCalls = 0U;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            if( (AT_CS_OUTCALL_DIAL_IPR == gPnccContext.plcc[i]->state) ||
                (AT_CS_OUTCALL_RING_IPR == gPnccContext.plcc[i]->state))
            {
                numOutgoingCalls++;
            }
        }
    }
    return numOutgoingCalls;
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_PHONE_AT_PNCC_u16GetNumberOfCallType                     */
/**
    \brief      returns number of calls currently in specified state

    \return     Uint16Type
                number of calls
*/
/*----------------------------------------------------------------------------*/
Uint16Type APP_PHONE_AT_PNCC_u16GetNumberOfCallType(ATCallStateEnum callState)
{
    int i = 0;
    Uint16Type numCalls = 0U;

    for(i=0; i<MAX_PLCC_ELEMENTS; i++)
    {
        if(gPnccContext.plcc[i])
        {
            if( callState == gPnccContext.plcc[i]->state )
            {
                numCalls++;
            }
        }
    }
    return numCalls;
}


/* End Of File APP_PHONE_AT_PNCC_C1.c */


