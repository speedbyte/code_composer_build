/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_MDI_BAP_C1.c
*-----------------------------------------------------------------------------
* Module Name:    MDI Application
*-----------------------------------------------------------------------------
* Description:    Handles all MDI specific requests from BAP  
*-----------------------------------------------------------------------------
* $Date: 2011-06-15 14:52:02 +0200 (Mi, 15 Jun 2011) $
* $Rev: 20164 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI_BAP/trunk/src/APP_MDI_BAP_C1.c $
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
#include "bap_defines.h"    // vw

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"
#include "BAP_Dispatcher.h"
#include "ATCmdHandler.h"
#include "UART.h"
#include "TRACE.h"
#include "APP_MDI_BAP_C1.id"
#include "OSEK_AppTask_Alarm.h"
#include "APP_DEVICE.h"

#include "APP_MDI_BAP_CE.h"
#include "APP_MDI_BAP_CI.h"
#include "APP_MDI_BAP.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/** 
\brief	variable to save the current call management function currently in progress
*/
AppMdiBapContext gAppMdiBapContext;
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
static SuccessFailType _APP_MDI_BAP_sfParseBapIndication(  const BapIndication *pBapInd, 
                                                           MdiBapIndType *pMdiInd);
static SuccessFailType _APP_MDI_BAP_sfParsePlayStatus(const BapIndication *pBapInd,
                                                            MdiBapIndType *pMdiInd);
static SuccessFailType _APP_MDI_BAP_sfParseSkip(const BapIndication *pBapInd,
                                                      MdiBapIndType *pMdiInd);
static SuccessFailType _APP_MDI_BAP_sfParseMDActive(const BapIndication *pBapInd,
                                                      MdiBapIndType *pMdiInd);
#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
static SuccessFailType _APP_MDI_BAP_sfParseTimePosition(const BapIndication *pBapInd,
                                                     MdiBapIndType *pMdiInd);
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
static SuccessFailType _APP_MDI_BAP_sfParseRandomMode( const BapIndication *pBapInd,
                                                       MdiBapIndType *pMdiInd);
static SuccessFailType _APP_MDI_BAP_sfParseRepeatMode( const BapIndication *pBapInd,
                                                       MdiBapIndType *pMdiInd);
#endif

static SuccessFailType _APP_MDI_BAP_sfParseCurrentFileName( const BapIndication *pBapInd,
                                                            MdiBapIndType *pMdiInd);
static SuccessFailType _APP_MDI_BAP_sfParseCurrentFileInfo( const BapIndication *pBapInd,
                                                            MdiBapIndType *pMdiInd);


static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandlePlayStatus(const MdiBapIndType *pMdiInd);
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleSkip(const MdiBapIndType *pMdiInd);
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleMDactive(const MdiBapIndType *pMdiInd);

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleTimePositon(const MdiBapIndType *pMdiInd);
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleRandomMode(const MdiBapIndType *pMdiInd);
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleRepeatMode(const MdiBapIndType *pMdiInd);
#endif

static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleCurrentFileName(const MdiBapIndType *pMdiInd);
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleCurrentFileInfo(const MdiBapIndType *pMdiInd);

static void _APP_MDI_BAP_vBapSendPlayStatus(const APP_MDI_BAP_enPlayStatusValue playStatusVal);
static void _APP_MDI_BAP_vBapSendPlayStatusMap(const APP_MDI_enActiveStatusValue playStatusVal);
static APP_MDI_BAP_enPlayStatusValue _APP_MDI_BAP_enBapGetPlayStatusMap(const APP_MDI_enActiveStatusValue playStatusVal);
static void _APP_MDI_BAP_vBapSendSkip(fctId_t fctId, APP_MDI_BAP_enBapResult bapResult);
static void _APP_MDI_BAP_vBapSendOPState(APP_MDI_enOpState opState);
static void _APP_MDI_BAP_vBapSendMDServiceSupport( const MdServiceSupport *pMss);
static void _APP_MDI_BAP_vBapSendMDactive( const Uint8Type* string, const Uint16Type length, const Uint8Type mdType);

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
static void _APP_MDI_BAP_vBapSendTimePosition(Uint16Type trackTime, Uint16Type trackTimePosition);
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
static void _APP_MDI_BAP_vBapSendRandomMode(APP_MDI_BAP_enRandomMode randomMode);
static void _APP_MDI_BAP_vBapSendRandomModeMap(APP_MDI_enRandomMode randomMode);
static void _APP_MDI_BAP_vBapSendRepeatMode(APP_MDI_BAP_enRepeatMode repeatMode);
static void _APP_MDI_BAP_vBapSendRepeatModeMap(APP_MDI_enRepeatMode repeatMode);
#endif

static void _APP_MDI_BAP_vBapSendCurrentFileName(const AppMdiFileNameDataType* pFileName);
static void _APP_MDI_BAP_vBapSendCurrentFileInfo( const AppMdiFileInfoDataType *pTitle, 
                                                  const AppMdiFileInfoDataType *pArtist, 
                                                  const AppMdiFileInfoDataType *pAlbum);


static void _APP_MDI_BAP_vHandleMdiIndication(const MdiBapIndType *pMdiInd );
static void _APP_MDI_BAP_vHandleBAPIndicationDefault(const BapIndication *pBapInd, const MdiBapIndType *pMdiInd);
static void _APP_MDI_BAP_vMdiActiveUpdate(void);
static void _APP_MDI_BAP_vReleaseContext(void);
static void _APP_MDI_BAP_vRetryContext(const MdiBapIndType *mdiInd);

static void _APP_MDI_BAP_vBuildAndSendMdActive(void);

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
static void _APP_MDI_BAP_vSendTimePosition();
#endif

static void _APP_MDI_BAP_vSetMdServiceSupport( MdServiceSupport *pMdMss,
                                               APP_MDI_enMetaDataType metaType);
void _APP_MDI_cbActiveMediaPlayer(void);

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
//ATResponseAllType   atRsp;
static APP_MDI_BAP_enPlayStatusValue   enValueBapSent;

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
/* Function    : APP_MDI_BAP_sfInitApp                                         */
/** 
    \brief      Callback function for initialization of App MDI
  
    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
SuccessFailType APP_MDI_BAP_sfInitApp()
{
    (void)memset(&gAppMdiBapContext, 0, sizeof(gAppMdiBapContext));
    enValueBapSent = APP_MDI_BAP_NO_MEDIA_DEVICE_ACTIVE;
    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_vHandleEvents                                        */
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

void APP_MDI_BAP_vHandleEvents(EventMaskType event)
{
    
    // ****************************************************
    // check if we have some additional things to do
    // from the last callFctId triggered by BAP
    // This is important if e.g. more than one AT commands
    // have to be send to process one BAP-Indication
    // ***************************************************
    if( event & evAppTaskMdiBapFctPending )
    {
        (void)ClearEvent(evAppTaskMdiBapFctPending);

        if( gAppMdiBapContext.mdiInd.common.fctType != MDI_BAP_INVALID_FCT_ID)
        {
            _APP_MDI_BAP_vHandleMdiIndication(&gAppMdiBapContext.mdiInd);
        }
    }

    // *****************************************************
    // look for events from BAPDispatcher and process them 
    // check for new indication messages from BAP Dispatcher
    // *****************************************************
	if(event & evAppTaskMdiBapInd )
	{         

        (void)ClearEvent(evAppTaskMdiBapInd);

        if(ERM_bIsStartupDone() == BT_FALSE)
        {
            /* do a default handling */
            BapIndication bapInd;
            MdiBapIndType mdiInd;

            (void)ReceiveMessage( msgMdiBapIndication , &bapInd );

            if( _APP_MDI_BAP_sfParseBapIndication(&bapInd, &mdiInd) == SUCCESS )
            {
                _APP_MDI_BAP_vHandleBAPIndicationDefault(&bapInd, &mdiInd);
            }            
            (void)APP_COMMON_UTILS_sfFreeBapIndication( &bapInd );
        }
        else if( gAppMdiBapContext.mdiInd.common.fctType == MDI_BAP_INVALID_FCT_ID )
        {
            BapIndication bapInd;
            MdiBapIndType mdiInd;

            (void)ReceiveMessage( msgMdiBapIndication , &bapInd);

            if( _APP_MDI_BAP_sfParseBapIndication(&bapInd, &mdiInd) == SUCCESS )
            {
                _APP_MDI_BAP_vHandleMdiIndication(&mdiInd);
            }            
            (void)APP_COMMON_UTILS_sfFreeBapIndication(&bapInd);
        }

        // check queue Status
        if( E_COM_NOMSG != GetMessageStatus(msgMdiBapIndication))
        {
            // there are still some messages
            (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_BAP_IND, 
                                                MSEC(10), 
                                                0);
        }
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_vReleaseContext                                  */
/** 
  \brief        Resets the function specifc context  

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vReleaseContext()
{
    (void)memset(&gAppMdiBapContext.mdiInd, 0, sizeof(gAppMdiBapContext.mdiInd));
} 
    
/*----------------------------------------------------------------------------*/
/* Function :                                      */
/** 
  \brief        Sets an alarm to retry the handling of a bap indication  

  \param        mdiInd
                Pointer to the MDI BAP indication

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vRetryContext(const MdiBapIndType *mdiInd)
{
    (void)memcpy(&gAppMdiBapContext.mdiInd, mdiInd, sizeof(gAppMdiBapContext.mdiInd));

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm( CTA_APP_MDI_BAP_FCT_PENDING, MSEC(10), 0);
} 


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vParseBAPIndications                             */
/** 
    \brief      handle indications from bap dispatcher 
  
    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseBapIndication(  const BapIndication *pBapInd, 
                                                    MdiBapIndType *pMdiInd)
{
    SuccessFailType result = SUCCESS;

    switch(pBapInd->fct_id)
    {
        case BapFct_MDI_PlayStatus:
            result = _APP_MDI_BAP_sfParsePlayStatus(pBapInd, pMdiInd);
            break;
        case BapFct_MDI_SkipForward:
        case BapFct_MDI_SkipBackward:
            result = _APP_MDI_BAP_sfParseSkip(pBapInd, pMdiInd);
            break;
        case BapFct_MDI_MDactive:
            result = _APP_MDI_BAP_sfParseMDActive(pBapInd, pMdiInd);
            break;
#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
        case BapFct_MDI_TimePosition:
            result = _APP_MDI_BAP_sfParseTimePosition(pBapInd, pMdiInd);
            break;
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
        case BapFct_MDI_Random:
            result = _APP_MDI_BAP_sfParseRandomMode(pBapInd, pMdiInd);
            break;
        case BapFct_MDI_Repeat:
            result = _APP_MDI_BAP_sfParseRepeatMode(pBapInd, pMdiInd);
            break;
#endif
        case BapFct_MDI_CurrentFileName:
            result = _APP_MDI_BAP_sfParseCurrentFileName(pBapInd, pMdiInd);
            break;
        case BapFct_MDI_CurrentFileInfo:
            result = _APP_MDI_BAP_sfParseCurrentFileInfo(pBapInd, pMdiInd);
            break;
        default:
            result = FAIL;
            break;
    }

    if(result != SUCCESS)
    {
        //Send BapError message
        APP_COMMON_UTILS_vSendBapError( BapLsg_MDI, 
                                        pBapInd->fct_id,
                                        (enum BapError_t) 0xFF);

        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseBapInd failed fctId: 0x%02x", 
                      pBapInd->fct_id);
    }

    return result;
}


/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_sfParsePlayStatus                                */
/** 
  \brief        Function to parse the PlayStatus BAP message  

    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParsePlayStatus(const BapIndication *pBapInd,
                                               MdiBapIndType *pMdiInd)
{
    Uint8Type playStatusVal = 0xFFU;

    switch(pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pMdiInd->common.fctType = MDI_BAP_PLAY_STATUS;
            pMdiInd->common.opType  = MDI_BAP_OP_TYPE_GET;
            break;

        case BapInd_DataSetGet:
            pMdiInd->common.fctType = MDI_BAP_PLAY_STATUS;
            pMdiInd->common.opType  = MDI_BAP_OP_TYPE_SET_GET;
            break;

        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParsePlayStatus: invalid IndType: 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }


    if(pMdiInd->common.opType == MDI_BAP_OP_TYPE_GET)
    {
        /* nothing else to do */
        return SUCCESS;
    }


    playStatusVal = pBapInd->payload.u8_value;

    switch(playStatusVal)
    {
        case APP_MDI_BAP_NO_MEDIA_DEVICE_ACTIVE: 
        case APP_MDI_BAP_PLAY:
        case APP_MDI_BAP_STOP:
        case APP_MDI_BAP_PAUSE:
        case APP_MDI_BAP_FAST_FORWARD:
        case APP_MDI_BAP_FAST_BACKWARD:
        case APP_MDI_BAP_SEARCH_BACKWARD:
        case APP_MDI_BAP_STEP_BY_STEP:
        case APP_MDI_BAP_NOT_SUPPORTED:
            pMdiInd->playStatus.value = (APP_MDI_BAP_enPlayStatusValue)playStatusVal;
            break;

        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParsePlayStatus: unknown playStatus value 0x%02x", 
                          playStatusVal );
            return FAIL;
    }

    return SUCCESS;
} 

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_sfParseSkip                                      */
/** 
  \brief        Function to parse the Skip BAP message  

    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseSkip(const BapIndication *pBapInd,
                                               MdiBapIndType *pMdiInd)
{
    Uint8Type skipMode      = 0U;
    Uint8Type skipCounter   = 0U;

    if(pBapInd->fct_id == BapFct_MDI_SkipForward)
    {
        pMdiInd->common.fctType = MDI_BAP_SKIP_FORWARD;
    }
    else
    {
        pMdiInd->common.fctType = MDI_BAP_SKIP_BACKWARD;
    }

    switch(pBapInd->ind_type)
    {
        case BapInd_StartResult:
            pMdiInd->common.opType  = MDI_BAP_OP_TYPE_START_RESULT;
            break;

        case BapInd_Abort:
            pMdiInd->common.opType  = MDI_BAP_OP_TYPE_ABORT;
            break;

        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseSkip: invalid IndType: 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }


    if(pMdiInd->common.opType == MDI_BAP_OP_TYPE_ABORT)
    {
        /* nothing else to do */
        return SUCCESS;
    }

    if(pBapInd->payload.byte_seq.length != 2)
    {
        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseSkip: unexpected payload size: %d (2)", 
                      pBapInd->payload.byte_seq.length );
        return FAIL;
    }


    skipMode    = *pBapInd->payload.byte_seq.data;
    skipCounter = *(pBapInd->payload.byte_seq.data+1);

    switch(skipMode)
    {
        case MDI_BAP_SKIP_FILE_TRACK:
        case MDI_BAP_SKIP_DISK_FOLDER_PLAYLIST:
            /* we set the skipMode for skipBackward simultaniously */
            pMdiInd->skipForward.mode = (APP_MDI_BAP_enSkipMode)skipMode;
            break;

        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseSkip: unknown skipMode 0x%02x", 
                          skipMode );
            return FAIL;
    }

    /* we set the skipCounter for skipBackward simultaniously */
    pMdiInd->skipForward.counter = skipCounter;

    return SUCCESS;
} 



/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_sfParseMDActive                                  */
/** 
  \brief        Function to parse the MDActive BAP message  

    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseMDActive(const BapIndication *pBapInd,
                                                   MdiBapIndType *pMdiInd)
{
    pMdiInd->common.fctType = MDI_BAP_MD_ACTIVE;

    switch(pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pMdiInd->common.opType  = MDI_BAP_OP_TYPE_GET;
            break;

        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseMDActive: invalid IndType: 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }

    return SUCCESS;
}


#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_sfParseTimePosition                              */
/** 
  \brief        Function to parse the TimePosition BAP message  

    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseTimePosition(const BapIndication *pBapInd,
                                                       MdiBapIndType *pMdiInd)
{
    switch(pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pMdiInd->common.fctType  = MDI_BAP_TIME_POSITION;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_GET;
            break;
    
        case BapInd_DataSetGet:
        {
            Union32Type payload = {0};
            pMdiInd->common.fctType  = MDI_BAP_TIME_POSITION;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_SET_GET;
        
            if(pBapInd->payload.byte_seq.length != 4)
            {
                DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseTimePosition: invalid data length %d", 
                              pBapInd->payload.byte_seq.length );
                return FAIL;
            }

            (void)memcpy(&payload, pBapInd->payload.byte_seq.data, 4);

            pMdiInd->timePositon.trackTimePosition = payload.u16[1];
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseTimePosition: invalid IndType 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }


    return SUCCESS;
} 
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_sfParseRandomMode                                */
/** 
  \brief        Function to parse the Random BAP message  

    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseRandomMode( const BapIndication *pBapInd,
                                                   MdiBapIndType *pMdiInd)
{
    switch(pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pMdiInd->common.fctType  = MDI_BAP_RANDOM;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_GET;
            break;
    
        case BapInd_DataSetGet:
        {
            pMdiInd->common.fctType  = MDI_BAP_RANDOM;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_SET_GET;
        
            if(pBapInd->data_type != BAP_DT_INT8)
            {
                DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseRandom: invalid data_type %d", 
                              pBapInd->data_type );
                return FAIL;
            }
    
            switch((APP_MDI_BAP_enRandomMode)pBapInd->payload.u8_value)
            {
                case MDI_BAP_RANDOM_OFF:
                case MDI_BAP_RANDOM_ALL:
                case MDI_BAP_RANDOM_FOLDER_PLAYLISTS:
                case MDI_BAP_RANDOM_ALL_PLAYLISTS:
                case MDI_BAP_RANDOM_NOT_SUPPORTED:
                    pMdiInd->random.mode = (APP_MDI_BAP_enRandomMode)pBapInd->payload.u8_value;
                    break;
                default:
                    DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseRandom: invalid rand val %d", 
                                  pBapInd->payload.u8_value );
                    return FAIL;
            }
            

            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseRandom: invalid IndType 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_sfParseRepeatMode                                */
/** 
  \brief        Function to parse the Repeat BAP message  

    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL

 */
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseRepeatMode( const BapIndication *pBapInd,
                                                   MdiBapIndType *pMdiInd)
{
    switch(pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pMdiInd->common.fctType  = MDI_BAP_REPEAT;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_GET;
            break;
    
        case BapInd_DataSetGet:
        {
            pMdiInd->common.fctType  = MDI_BAP_REPEAT;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_SET_GET;
        
            if(pBapInd->data_type != BAP_DT_INT8)
            {
                DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseRepeat: invalid data_type %d", 
                              pBapInd->data_type );
                return FAIL;
            }
    
            switch((APP_MDI_BAP_enRepeatMode)pBapInd->payload.u8_value)
            {
                case MDI_BAP_REPEAT_OFF:
                case MDI_BAP_REPEAT_ALL_MEDIUM:
                case MDI_BAP_REPEAT_FOLDER_PLAYLIST:
                case MDI_BAP_REPEAT_ALL_PLAYLISTS:
                case MDI_BAP_REPEAT_FILE_TRACK:
                case MDI_BAP_REPEAT_TITLE:
                case MDI_BAP_REPEAT_CHAPTER:
                case MDI_BAP_REPEAT_AB:
                case MDI_BAP_REPEAT_NOT_SUPPORTED:
                    pMdiInd->repeat.mode = (APP_MDI_BAP_enRepeatMode)pBapInd->payload.u8_value;
                    break;
                default:
                    DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseRepeat: invalid repeatState val %d", 
                                  pBapInd->payload.u8_value );
                    return FAIL;
            }
            

            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseRepeat: invalid IndType 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }

    return SUCCESS;
}

#endif

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfParseCurrentFileName                          */
/** 
    \brief      Handle indications from bap dispatcher 
  
    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseCurrentFileName( const BapIndication *pBapInd,
                                                            MdiBapIndType *pMdiInd)
{
    switch(pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pMdiInd->common.fctType  = MDI_BAP_CURRENT_FILE_NAME;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_GET;
            break;
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseCurFileName: invalid IndType 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }

    return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfParseCurrentFileInfo                          */
/** 
    \brief      Handle indications from bap dispatcher 
  
    \param      pBapInd
                const BapIndication*
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     SuccessFailType
    			Status of function run
    			SUCCESS, FAIL
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType _APP_MDI_BAP_sfParseCurrentFileInfo( const BapIndication *pBapInd,
                                                            MdiBapIndType *pMdiInd)
{
    switch(pBapInd->ind_type)
    {
        case BapInd_DataGet:
            pMdiInd->common.fctType  = MDI_BAP_CURRENT_FILE_INFO;
            pMdiInd->common.opType   = MDI_BAP_OP_TYPE_GET;
            break;
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "ParseCurFileInfo: invalid IndType 0x%02x", 
                          pBapInd->ind_type );
            return FAIL;
    }

    return SUCCESS;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vHandleMdiIndication                            */
/** 
    \brief      Handle indications from bap dispatcher 
  
    \param      pMdiInd 
                MdiBapIndType *

    \return     void 
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vHandleMdiIndication(const MdiBapIndType *pMdiInd )
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;


    DEBUG_VALUE1( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "vHaMdiInd: BAP fct received: %d",
                  pMdiInd->common.fctType );


    switch( pMdiInd->common.fctType )
    {
        case MDI_BAP_PLAY_STATUS:
            response = _APP_MDI_BAP_sfHandlePlayStatus(pMdiInd);
            break;
        case MDI_BAP_SKIP_FORWARD:
        case MDI_BAP_SKIP_BACKWARD:
            response = _APP_MDI_BAP_sfHandleSkip(pMdiInd);
            break;
        case MDI_BAP_MD_ACTIVE:
            response = _APP_MDI_BAP_sfHandleMDactive(pMdiInd);
            break;
#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
        case MDI_BAP_TIME_POSITION:
            response = _APP_MDI_BAP_sfHandleTimePositon(pMdiInd);
            break;
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
        case MDI_BAP_RANDOM:
            response = _APP_MDI_BAP_sfHandleRandomMode(pMdiInd);
            break;
        case MDI_BAP_REPEAT:
            response = _APP_MDI_BAP_sfHandleRepeatMode(pMdiInd);
            break;
#endif
        case MDI_BAP_CURRENT_FILE_NAME:
            response = _APP_MDI_BAP_sfHandleCurrentFileName(pMdiInd);
            break;
        case MDI_BAP_CURRENT_FILE_INFO:
            response = _APP_MDI_BAP_sfHandleCurrentFileInfo(pMdiInd);
            break;
        default:
            /* Unknown MDI Indication */
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "vHaMdiInd: unsup fct: %d", 
                          pMdiInd->common.fctType );
            break;
    }


    switch(response)
    {
        case APP_COMMON_RSP_OK:
            _APP_MDI_BAP_vReleaseContext();
            break;
        case APP_COMMON_RSP_ERROR_BUSY:
            _APP_MDI_BAP_vRetryContext(pMdiInd);
            DEBUG_VALUE1( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "vHaMdiInd: failed, busy fct: %d", 
                          pMdiInd->common.fctType );
            break;
        case APP_COMMON_RSP_ERROR:
        default:
            _APP_MDI_BAP_vReleaseContext();

            DEBUG_VALUE2( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "vHaMdiInd: error %d for fct: %d", 
                          response,
                          pMdiInd->common.fctType );
            //Send BapError message
            APP_COMMON_UTILS_vSendBapError( BapLsg_MDI, 
                                            (fctId_t)pMdiInd->common.fctType,
                                            (enum BapError_t) 0xFF);
            break;
    }
        

}/* END OF _APP_MDI_vHandleBAPIndication() */



/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vHandleBAPIndicationDefault                     */
/** 
    \brief      Sends default responses in the case that we are not finished with initilization 
  
    \param      pBapInd
                const BapIndication *
    
    \param      pMdiInd 
                MdiBapIndType *

    \return     void 
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vHandleBAPIndicationDefault(const BapIndication *pBapInd, const MdiBapIndType *pMdiInd)
{
    switch( pMdiInd->common.fctType )
    {
//        case MDI_BAP_MD_LIST:
//            (void)APP_COMMON_UTILS_sfSendEmptyBapArray(pBapInd);
//            break;
        case MDI_BAP_PLAY_STATUS:
            (void)_APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_NO_MEDIA_DEVICE_ACTIVE);
            break;
#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
        case MDI_BAP_REPEAT:
            (void)_APP_MDI_BAP_vBapSendRepeatMode(MDI_BAP_REPEAT_OFF);
            break;
#endif
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HaBAPIndDef: no default handling for fct: 0x%02x", 
                          pMdiInd->common.fctType );
            break;
    }
    
}/* END OF _APP_MDI_vHandleBAPIndication() */




/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandlePlayStatus                              */
/** 
    \brief      Handles PlayStatus BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandlePlayStatus(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_GET:
        {
            _APP_MDI_BAP_vBapSendPlayStatusMap(APP_MDI_GetPlayStatus());
            break;
        }
        case MDI_BAP_OP_TYPE_SET_GET:
        {
            DEBUG_VALUE1(APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "BAP SET_GET request %d", pMdiInd->playStatus.value);

            if(APP_MDI_bRemoteControlAvailable() == BT_TRUE)
            {
                SuccessFailType result = SUCCESS;

                switch(pMdiInd->playStatus.value)
                {
                    case APP_MDI_BAP_PLAY:
                        if(IsCK5050ResponseStillOngoing())
                        {
                            APP_MDI_vDeferPlayMode(APP_MDI_RESUME);
                        }
                        else
                        {
                            result = APP_MDI_sfSetPlayMode( APP_MDI_RESUME );
                        }
                        break;
                    case APP_MDI_BAP_STOP:
                    	result = APP_MDI_sfSetPlayMode( APP_MDI_STOP );
                    	break;
                    case APP_MDI_BAP_PAUSE:
                        if (IsCK5050ResponseStillOngoing())
                        {
                            APP_MDI_vDeferPlayMode(APP_MDI_PAUSE);
                        }
                        else
                        {
                            result = APP_MDI_sfSetPlayMode( APP_MDI_PAUSE );
                        }
                        break;
                    case APP_MDI_BAP_FAST_FORWARD:
                        if (IsCK5050ResponseStillOngoing())
                        {
                            APP_MDI_vDeferPlayMode(APP_MDI_FAST_FORWARD);
                        }
                        else
                        {
                            result = APP_MDI_sfSetPlayMode( APP_MDI_FAST_FORWARD );
                        }
                        break;
                    case APP_MDI_BAP_FAST_BACKWARD:
                        if (IsCK5050ResponseStillOngoing())
                        {
                            APP_MDI_vDeferPlayMode(APP_MDI_FAST_BACKWARD);
                        }
                        else
                        {
                            result = APP_MDI_sfSetPlayMode( APP_MDI_FAST_BACKWARD );
                        }
                        break;
                    case APP_MDI_BAP_NO_MEDIA_DEVICE_ACTIVE:
                        _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_NO_MEDIA_DEVICE_ACTIVE);
                        return APP_COMMON_RSP_OK;
                    case APP_MDI_BAP_SEARCH_FORWARD:
                    case APP_MDI_BAP_SEARCH_BACKWARD:
                    case APP_MDI_BAP_STEP_BY_STEP:
                    case APP_MDI_BAP_NOT_SUPPORTED:
                    default:
                        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HaPlayStatus: unsup %d", 
                                      pMdiInd->playStatus.value );

                        _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_NOT_SUPPORTED);
                        return APP_COMMON_RSP_OK;

                }

                if(result == SUCCESS)
                {
                    // The PlayStatus doesn't support processing, so the PlayStatus
                    // must be sent back here at once
                    // Send back the requested play status back, otherwise the HU will call
                    // SETGET (every 50ms) as long as the desired value will be returned.
                    // After play status has changed the update follows automatically
                    _APP_MDI_BAP_vBapSendPlayStatus(pMdiInd->playStatus.value);
                }
                else
                {
                    response = APP_COMMON_RSP_ERROR;
                }
                
            }
            else
            {
                DEBUG_TEXT( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP,"No AVRCP device is connected, no play status supported");
                _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_NOT_SUPPORTED);
            }
            break;
        }
        default:
        {
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HaPlayStatus: invalid opType: %d", 
                          pMdiInd->common.opType );

            response = APP_COMMON_RSP_ERROR;
            break;
        }
    }

    return response;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandleSkip                                    */
/** 
    \brief      Handles HandleSkip BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleSkip(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_START_RESULT:
        {
            fctId_t skipFctId = BapFct_MDI_SkipForward;
            
            if(pMdiInd->common.fctType == MDI_BAP_SKIP_FORWARD)
            {
                skipFctId = BapFct_MDI_SkipForward;
                if(APP_MDI_sfSetSkip(APP_MDI_NEXT_SONG, pMdiInd->skipForward.counter) != SUCCESS)
                {
                    response = APP_COMMON_RSP_ERROR;
                }
            }
            else
            {
                skipFctId = BapFct_MDI_SkipBackward;
                if(APP_MDI_sfSetSkip(APP_MDI_PREVIOUS_SONG, pMdiInd->skipBackward.counter) != SUCCESS)
                {
                    response = APP_COMMON_RSP_ERROR;
                }
            }

            if( response == APP_COMMON_RSP_OK )
            {
                _APP_MDI_BAP_vBapSendSkip( skipFctId, APP_MDI_BAP_enResultOk );
            }
            else
            {
                _APP_MDI_BAP_vBapSendSkip( skipFctId, APP_MDI_BAP_enResultNotOk );
            }
            break;
        }
        case MDI_BAP_OP_TYPE_ABORT:
        {
            DEBUG_TEXT( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleSkip: failed, opType ABORT is not supported");

            response = APP_COMMON_RSP_ERROR;
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleSkip: failed, invalid opType: %d",
                          pMdiInd->common.opType);
            break;
    }

    return response;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandleMDactive                                */
/** 
    \brief      Handles MDactive BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleMDactive(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_GET:
        {
            _APP_MDI_BAP_vBuildAndSendMdActive();
            break;
        }
        default:
        {
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HaMDactive: failed, invalid opType: %d",
                          pMdiInd->common.opType );
            response = APP_COMMON_RSP_ERROR;
            break;
        }
    }

    return response;
}


#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandleTimePositon                             */
/** 
    \brief      Handles TimePositon BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleTimePositon(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;


    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_GET:
        {
            _APP_MDI_BAP_vSendTimePosition();
            break;
        }
        case MDI_BAP_OP_TYPE_SET_GET:
        {
            // TODO: set the given trackTimePositon
            _APP_MDI_BAP_vSendTimePosition();
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleTimePositon: failed, invalid opType: %d",
                          pMdiInd->common.opType );
            break;
    }

    return response;
}
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandleRandom                                  */
/** 
    \brief      Handles Random BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleRandomMode(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_GET:
        {
            _APP_MDI_BAP_vBapSendRandomModeMap(APP_MDI_GetRandomMode());
            break;
        }
        case MDI_BAP_OP_TYPE_SET_GET:
        {
            SuccessFailType result = SUCCESS;   
            APP_MDI_BAP_enRandomMode randMode = pMdiInd->random.mode;
            // set the given random mode
            switch(randMode)
            {
                case MDI_BAP_RANDOM_OFF:
                case MDI_BAP_RANDOM_ALL_PLAYLISTS:
                    result = APP_MDI_sfSetRandomMode(MDI_RANDOM_DISABLE);
                    break;
                case MDI_BAP_RANDOM_ALL:
                    result = APP_MDI_sfSetRandomMode(MDI_RANDOM_ENABLE);
                    randMode = MDI_BAP_RANDOM_ALL;
                    break;
                case MDI_BAP_RANDOM_FOLDER_PLAYLISTS:  
                    result = APP_MDI_sfSetRandomMode(MDI_RANDOM_FOLDER_PLAYLISTS);
                    randMode = MDI_BAP_RANDOM_FOLDER_PLAYLISTS;
                    break;
                default:
                    DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleRandom: invalid BAP random mode %d",
                                  randMode );

                    result = FAIL;
                    break;
            }
            
            if(result == SUCCESS)
            {
                _APP_MDI_BAP_vBapSendRandomMode(randMode);
            }
            else
            {
                response = APP_COMMON_RSP_ERROR;
            }
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleRandom: failed, invalid opType: %d",
                          pMdiInd->common.opType );
            response = APP_COMMON_RSP_ERROR;
            break;
    }

    return response;
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandleRepeat                                  */
/** 
    \brief      Handles Repeat BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleRepeatMode(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_GET:
        {
            _APP_MDI_BAP_vBapSendRepeatMode(MDI_BAP_REPEAT_NOT_SUPPORTED);
            break;
        }
        case MDI_BAP_OP_TYPE_SET_GET:
        {
            APP_MDI_BAP_enRepeatMode repeatMode = pMdiInd->repeat.mode;
            SuccessFailType result = SUCCESS;
            // set the provided repeat mode
            switch(repeatMode)
            {
                case MDI_BAP_REPEAT_OFF:
                    result = APP_MDI_sfSetRepeatMode(MDI_REPEAT_DISABLE);
                    break;        
                case MDI_BAP_REPEAT_FILE_TRACK:
                    result = APP_MDI_sfSetRepeatMode(MDI_REPEAT_ONE_TRACK);
                    break;
                case MDI_BAP_REPEAT_FOLDER_PLAYLIST:
                    result = APP_MDI_sfSetRepeatMode(MDI_REPEAT_GROUP);
                    break;
                case MDI_BAP_REPEAT_ALL_MEDIUM:
                case MDI_BAP_REPEAT_ALL_PLAYLISTS:
                case MDI_BAP_REPEAT_TITLE:
                case MDI_BAP_REPEAT_CHAPTER:
                case MDI_BAP_REPEAT_AB:
                    result = APP_MDI_sfSetRepeatMode(MDI_REPEAT_ALL_TRACK);
                    repeatMode = MDI_BAP_REPEAT_ALL_MEDIUM;
                    break;
                default:
                    DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleRepeat: invalid BAP repeat mode %d",
                                  repeatMode );

                    result = FAIL;
                    break;
            }

            if(result == SUCCESS)
            {
                _APP_MDI_BAP_vBapSendRepeatMode(repeatMode);
            }
            else
            {
                response = APP_COMMON_RSP_ERROR;
            }
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleRepeat: failed, invalid opType: %d",
                          pMdiInd->common.opType );
            response = APP_COMMON_RSP_ERROR;
            break;
    }

    return response;
}

#endif
/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandleCurrentFileName                         */
/** 
    \brief      Handles CurrentFileName BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleCurrentFileName(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_GET:
        {
            _APP_MDI_BAP_vBapSendCurrentFileName(APP_MDI_GetFileName());
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleCurFileName: failed, invalid opType: %d",
                          pMdiInd->common.opType );
            break;
    }

    return response;
}



/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_sfHandleCurrentFileInfo                         */
/** 
    \brief      Handles CurrentFileInfo BAP indication 
  
    \param      pMdiInd
                const MdiBapIndType *
    
    \return     APP_COMMON_enResponseCode
   
*/
/*----------------------------------------------------------------------------*/
static APP_COMMON_enResponseCode _APP_MDI_BAP_sfHandleCurrentFileInfo(const MdiBapIndType *pMdiInd)
{
    APP_COMMON_enResponseCode response = APP_COMMON_RSP_OK;

    switch(pMdiInd->common.opType)
    {
        case MDI_BAP_OP_TYPE_GET:
        {
            _APP_MDI_BAP_vBapSendCurrentFileInfo(APP_MDI_GetTitle(), APP_MDI_GetArtist(), APP_MDI_GetAlbum());
            break;
        }
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "HandleCurFileInfo: failed, invalid opType: %d",
                          pMdiInd->common.opType );
            break;
    }

    return response;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendPlayStatus                              */
/** 
    \brief      Sends a PlayStatus BAP request
  
    \param      playStatusVal
                const APP_MDI_BAP_enPlayStatusValue
    
    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendPlayStatus(const APP_MDI_BAP_enPlayStatusValue playStatusVal)
{
    BapRequest bapReq;

    bapReq.lsg_id    = BapLsg_MDI;
    bapReq.fct_id    = BapFct_MDI_PlayStatus;
    bapReq.req_type  = BapReq_Data;
    bapReq.data_type = BAP_DT_INT8;

    bapReq.payload.u8_value = (Uint8Type)playStatusVal;
    enValueBapSent = playStatusVal;

    DEBUG_VALUE1( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "BapSendPlayStatus: %d",
            playStatusVal );

    (void)APP_COMMON_UTILS_stSendBapRequest(msgMdiBapRequest, &bapReq);

}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendPlayStatusMap                            */
/** 
    \brief      Wrapper to map a APP_MDI play status to BAP play status
  
    \param      playStatusVal
                const APP_MDI_enActiveStatusValue
    
    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendPlayStatusMap(const APP_MDI_enActiveStatusValue playStatusVal)
{
    switch(playStatusVal)
    {
        case APP_MDI_ACT_STATE_PLAY:
            _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_PLAY);
            break;
        case APP_MDI_ACT_STATE_INIT:
            // Don't send a pause state when player state is unknown.
            DEBUG_TEXT( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "vBapSendPlayStatusMap: INIT state");
            break;
        case APP_MDI_ACT_STATE_PAUSE:
            _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_PAUSE);
            break;
        case APP_MDI_ACT_STATE_FAST_FORWARD:
            _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_FAST_FORWARD);
            break;
        case APP_MDI_ACT_STATE_FAST_REWIND:
            _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_FAST_BACKWARD);
            break;
        case APP_MDI_ACT_STATE_STOP:
            _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_PAUSE);
            break;
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "vBapSendPlayStatusMap: invalid playStatus: %d",
                          playStatusVal );
            break;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :                                      */
/**
  \brief        Returns the BAP play status from active MDI play status

  \param        playStatusVal
                Value of MDI play status

  \return       APP_MDI_BAP_enPlayStatusValue

 */
/*----------------------------------------------------------------------------*/
static APP_MDI_BAP_enPlayStatusValue _APP_MDI_BAP_enBapGetPlayStatusMap(const APP_MDI_enActiveStatusValue playStatusVal)
{
    APP_MDI_BAP_enPlayStatusValue ret = APP_MDI_BAP_NOT_SUPPORTED;

    switch(playStatusVal)
    {
        case APP_MDI_ACT_STATE_PLAY:
            ret = APP_MDI_BAP_PLAY;
            break;
        case APP_MDI_ACT_STATE_INIT:
        case APP_MDI_ACT_STATE_PAUSE:
            ret = APP_MDI_BAP_PAUSE;
            break;
        case APP_MDI_ACT_STATE_FAST_FORWARD:
            ret = APP_MDI_BAP_FAST_FORWARD;
            break;
        case APP_MDI_ACT_STATE_FAST_REWIND:
            ret = APP_MDI_BAP_FAST_BACKWARD;
            break;
        case APP_MDI_ACT_STATE_STOP:
            ret = APP_MDI_BAP_PAUSE;
            break;
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "enBapGetPlayStatusMap: invalid playStatus: %d",
                          playStatusVal );
            break;
    }
    return ret;
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendSkip                                    */
/** 
    \brief      Sends a Skip BAP request
  
    \param      fctId
                fctId_t

    \param      bapResult
                APP_MDI_BAP_enBapResult
    
    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendSkip(fctId_t fctId, APP_MDI_BAP_enBapResult bapResult)
{
    BapRequest bapReq;

    bapReq.lsg_id    = BapLsg_MDI;
    bapReq.fct_id    = fctId;
    bapReq.req_type  = BapReq_Result;
    bapReq.data_type = BAP_DT_INT8;

    bapReq.payload.u8_value = (Uint8Type)bapResult;
       
    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &bapReq );
}







/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendMDactive                                */
/** 
    \brief      Sends a MDactive BAP request
  
    \param      string
                const Uint8Type*

    \param      length
                const Uint16Type

    \param      mdType
                const Uint8Type
    
    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendMDactive( const Uint8Type* string, 
                                    const Uint16Type length,
                                    const Uint8Type  mdType)
{
    SuccessFailType result  = SUCCESS;
    Uint32Type tmpLen       = 0U;
    Uint8Type *pWriteBuf    = NULL;
    BapRequest request;

    tmpLen = length > BAP_MD_NAME_MAX_LEN ? BAP_MD_NAME_MAX_LEN : length;
 
    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_MDactive;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_BYTE; 
 
    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&request.payload.byte_seq.data );
    
    if(result != SUCCESS)
    {
        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendMDactive: POOL_sfAllocate() failed %d",
                      result );
        return;
    }
    
    pWriteBuf = request.payload.byte_seq.data;

    /* set the MDName */ 
    tmpLen = APP_COMMON_UTILS_u16WriteBapString(  pWriteBuf, 
                                                  string,
                                                  tmpLen);
    pWriteBuf += tmpLen;

    /* set the MDType */ 
    *pWriteBuf = mdType;
    
    pWriteBuf++;
   
    /* set the length of the byte sequence */ 
    request.payload.byte_seq.length = 
            (Uint16Type)(pWriteBuf - request.payload.byte_seq.data);
   

    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);

}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendMDServiceSupport                        */
/** 
    \brief      Sends a MDServiceSupport BAP request
  
    \param      pMSS
                const MdServiceSupport *

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendMDServiceSupport( const MdServiceSupport *pMSS)
{
    SuccessFailType result  = SUCCESS;

    BapRequest request;

    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_MDserviceSupport;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_BYTE; 
 
    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&request.payload.byte_seq.data );
    
    if(result != SUCCESS)
    {
        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "MDServiceSupport: POOL_sfAllocate() failed %d",
                      result );
        return;
    }

    /* set the length of the byte sequence */ 
    request.payload.byte_seq.length = sizeof(*pMSS);
    
    (void)memcpy( request.payload.byte_seq.data,
            pMSS,
            request.payload.byte_seq.length );

    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendOPState                                 */
/** 
    \brief      Sends a OpState BAP request
  
    \param      opState
                APP_MDI_enOpState

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendOPState(APP_MDI_enOpState opState)
{
    BapRequest request;
    
    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_FSG_OperationState;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_INT8; 

    request.payload.u8_value = (Uint8Type)opState;

    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);
}



#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendTimePosition                            */
/** 
    \brief      Sends a TimePosition BAP request
  
    \param      trackTime

    \param      trackTimePosition

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendTimePosition(Uint16Type trackTime, Uint16Type trackTimePosition)
{
    SuccessFailType result  = SUCCESS;

    BapRequest request;
    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_TimePosition;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_BYTE; 
 
    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&request.payload.byte_seq.data );
    
    if(result != SUCCESS)
    {
        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendTimePos: POOL_sfAllocate() failed %d",
                      result );
        return;
    }

    /* set the length of the byte sequence */ 
    request.payload.byte_seq.length = 4;
    APP_COMMON_UTILS_vSetUint16Type( trackTime, request.payload.byte_seq.data ); 
    APP_COMMON_UTILS_vSetUint16Type( trackTimePosition, request.payload.byte_seq.data + sizeof(Uint16Type) ); 


    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);
}
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendRandomMode                              */
/** 
    \brief      Sends a RandomMode BAP request
  
    \param      randomMode
                APP_MDI_BAP_enRandomMode

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendRandomMode(APP_MDI_BAP_enRandomMode randomMode)
{
    BapRequest request;
    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_Random;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_INT8; 
 
    request.payload.u8_value = (Uint8Type)randomMode;

    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendRandomModeMap                           */
/** 
    \brief      Maps a APP_MDI random Mode to BAP random mode and 
                sends a RandomMode BAP request
  
    \param      randomMode
                APP_MDI_BAP_enRandomMode

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendRandomModeMap(APP_MDI_enRandomMode randomMode)
{
    switch(randomMode)
    {
        case MDI_RANDOM_ENABLE:
            _APP_MDI_BAP_vBapSendRandomMode(MDI_BAP_RANDOM_ALL);
            break;
        case MDI_RANDOM_DISABLE:
            _APP_MDI_BAP_vBapSendRandomMode(MDI_BAP_RANDOM_OFF);
            break;
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendRandModeMap: invalid random mode: %d",
                          randomMode );
            break;
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendRepeatMode                              */
/** 
    \brief      Sends a RepeatMode BAP request
  
    \param      repeatMode
                APP_MDI_BAP_enRepeatMode

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendRepeatMode(APP_MDI_BAP_enRepeatMode repeatMode)
{
    BapRequest request;
    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_Repeat;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_INT8; 
 
    request.payload.u8_value = (Uint8Type)repeatMode;

    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendRepeatModeMap                           */
/** 
    \brief      Maps a APP_MDI repeat Mode to BAP repeat mode and 
                sends a RepeatMode BAP request
  
    \param      repeatMode
                APP_MDI_enRepeatMode

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendRepeatModeMap(APP_MDI_enRepeatMode repeatMode)
{
    switch(repeatMode)
    {
        case MDI_REPEAT_ONE_TRACK:
            _APP_MDI_BAP_vBapSendRepeatMode(MDI_BAP_REPEAT_FILE_TRACK);
            break;
        case MDI_REPEAT_ALL_TRACK:
            _APP_MDI_BAP_vBapSendRepeatMode(MDI_BAP_REPEAT_ALL_MEDIUM);
            break;
        case MDI_REPEAT_DISABLE:
            _APP_MDI_BAP_vBapSendRepeatMode(MDI_BAP_REPEAT_OFF);
            break;
        default:
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP,  "BapSendRepeatModeMap: invalid repeat mode: %d",
                          repeatMode );
            break;
    }
}
#endif
/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendCurrentFileName                         */
/** 
    \brief      Sends a CurrentFileName BAP request
  
    \param      pFileName
                const AppMdiFileNameDataType*

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendCurrentFileName(const AppMdiFileNameDataType* pFileName)
{
    SuccessFailType result = SUCCESS;
    Uint16Type      length = 0;

    BapRequest request;
    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_CurrentFileName;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_BYTE; 

    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&request.payload.byte_seq.data );
    
    if(result != SUCCESS)
    {
        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendCurFileName: POOL_sfAllocate() failed %d",
                      result );
        return;
    }
    
    if(pFileName == NULL)
    {
        request.payload.byte_seq.length = APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data, 
                                                                              NULL,
                                                                              0);
    }
    else
    {
        length = (pFileName->length > APP_MDI_BAP_CURRENT_FILE_NAME_MAX) ? 
                                APP_MDI_BAP_CURRENT_FILE_NAME_MAX : pFileName->length;

        request.payload.byte_seq.length = APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data, 
                                                                              pFileName->data,
                                                                              length);
    }

    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);
}


/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBapSendCurrentFileInfo                         */
/** 
    \brief      Sends a CurrentFileInfo BAP request
  
    \param      pTitle
                const AppMdiFileInfoDataType *

    \param      pArtist
                const AppMdiFileInfoDataType *

    \param      pAlbum
                const AppMdiFileInfoDataType *

    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBapSendCurrentFileInfo( const AppMdiFileInfoDataType *pTitle, 
                                                  const AppMdiFileInfoDataType *pArtist, 
                                                  const AppMdiFileInfoDataType *pAlbum)
{
    SuccessFailType result  = SUCCESS;
    Uint16Type      length = 0;

    BapRequest request;
    request.lsg_id      = BapLsg_MDI;
    request.fct_id      = BapFct_MDI_CurrentFileInfo;
    request.req_type    = BapReq_Data;
    request.data_type   = BAP_DT_BYTE; 

    if(pTitle == NULL)
    {
        DEBUG_TEXT( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendCurFileInfo: pTitle == NULL" );
    }

    if(pArtist == NULL)
    {
        DEBUG_TEXT( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendCurFileInfo: pArtist == NULL");
    }

    if(pAlbum == NULL)
    {
        DEBUG_TEXT( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendCurFileInfo: pAlbum == NULL");
    }
 
    result = POOL_sfAllocate( POOL_enBapReqDataType, 
                              (void**)&request.payload.byte_seq.data );
    
    if(result != SUCCESS)
    {
        DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "BapSendCurFileInfo: POOL_sfAllocate() failed %d",
                      result);
        return;
    }



    /* write the TITLE as BAP-String*/ 
    if(pTitle == NULL)
    {
        request.payload.byte_seq.length = APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data, 
                                                                              NULL,
                                                                              0);
    }
    else
    {
        length = (pTitle->length > APP_MDI_BAP_CURRENT_FILE_INFO_MAX) ? 
                                APP_MDI_BAP_CURRENT_FILE_INFO_MAX : pTitle->length;

        request.payload.byte_seq.length = APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data, 
                                                                              pTitle->data,
                                                                              length);
    }

    /* write the ARTIST as BAP-String*/ 
    if(pArtist == NULL)
    {
        request.payload.byte_seq.length += APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data, 
                                                                               NULL,
                                                                               0);
    }
    else
    {
        length = (pArtist->length > APP_MDI_BAP_CURRENT_FILE_INFO_MAX) ? 
                                APP_MDI_BAP_CURRENT_FILE_INFO_MAX : pArtist->length;

        request.payload.byte_seq.length += APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data +
                                                                               request.payload.byte_seq.length, 
                                                                               pArtist->data,
                                                                               length);
    }

    /* write the ALBUM as BAP-String*/ 
    if(pAlbum == NULL)
    {
        request.payload.byte_seq.length += APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data, 
                                                                               NULL,
                                                                               0);
    }
    else
    {
        length = (pAlbum->length > APP_MDI_BAP_CURRENT_FILE_INFO_MAX) ? 
                                APP_MDI_BAP_CURRENT_FILE_INFO_MAX : pAlbum->length;

        request.payload.byte_seq.length += APP_COMMON_UTILS_u16WriteBapString( request.payload.byte_seq.data +
                                                                               request.payload.byte_seq.length, 
                                                                               pAlbum->data,
                                                                               length);
    }

    (void)APP_COMMON_UTILS_stSendBapRequest( msgMdiBapRequest, &request);
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vBuildAndSendMdActive                           */
/** 
    \brief      Builds and Sends a MdActive BAP request
  
    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vBuildAndSendMdActive()
{
    AT_DeviceId deviceId = ERM_u8GetActivePlayer();

    if(deviceId <= AT_MAX_DEVICE_ID)
    {
        const ERMBTDeviceTableType *pBtDevice = ERM_GetBTDevice(deviceId);
        Uint8Type mdType = BAP_MD_ACTIVE_MD_TYPE_AUX;

        if( (pBtDevice != NULL) && (pBtDevice->avrcp_supported == AT_SUPPORTED) )
        {
            mdType = BAP_MD_ACTIVE_MD_TYPE_MS_LOGIC;
        }

        _APP_MDI_BAP_vBapSendMDactive( pBtDevice->device_name, 
                                       pBtDevice->device_name_len,
                                       mdType);
    }
    else
    {
        _APP_MDI_BAP_vBapSendMDactive( NULL, 0, 0);
    }
}

/*----------------------------------------------------------------------------*/
/* Function    : _APP_MDI_BAP_vMdiActiveUpdate                                */
/** 
    \brief      Is called to update the MDI active state 
  
    \return     void
   
*/
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vMdiActiveUpdate()
{
    AT_DeviceId deviceId = ERM_u8GetActivePlayer();
    
    //pBtDevice = APP_MDI_GetMdiDevice(deviceId);
    _APP_MDI_BAP_vBuildAndSendMdActive();

    if(deviceId <= AT_MAX_DEVICE_ID)
    {
        MdServiceSupport mdServ;

        /* if still no play status known try to mute the player */
        if((APP_MDI_GetPlayStatus() == APP_MDI_ACT_STATE_INIT)||(APP_MDI_GetPlayStatus() == APP_MDI_ACT_STATE_PLAY))
            APP_MDI_sfSetPlayMode(APP_MDI_PAUSE);

        /* player is conected */
        _APP_MDI_BAP_vBapSendOPState(APP_MDI_enReadyToOperate);

        /* send MDservicesupport always after A2DP is connected */
        _APP_MDI_BAP_vSetMdServiceSupport(&mdServ, APP_MDI_GetMetadataType());
        _APP_MDI_BAP_vBapSendMDServiceSupport(&mdServ);
    }
    else
    {
        /* no player connected - player is disconnected */
        MdServiceSupport mdServ;

        (void)memset(&mdServ, 0, sizeof(mdServ));
        _APP_MDI_BAP_vBapSendMDServiceSupport(&mdServ);

        _APP_MDI_BAP_vBapSendOPState(APP_MDI_enNoMediaDeviceActive); 

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
        _APP_MDI_BAP_vBapSendTimePosition(BAP_MD_TRACK_TIME_UNAVAILABLE, BAP_MD_TRACK_TIME_UNAVAILABLE);
#endif

        _APP_MDI_BAP_vBapSendCurrentFileName(NULL);
        _APP_MDI_BAP_vBapSendCurrentFileInfo(NULL, NULL, NULL);
    }
}


/*----------------------------------------------------------------------------*/
/* Function    : APP_MDI_ERM_vHandleErmNotification                           */
/** 
    \brief      Handle State change of ERM to Update e.g. FSG OperationState
  
    \param      new_notification
                ERMNotificationType

    \return     void 
*/
/*----------------------------------------------------------------------------*/
void APP_MDI_BAP_vHandleErmNotification(const ERMNotificationType * pNotification)
{
    switch(pNotification->common.id)
    {
        case ERM_NOTIFY_PBSN_A2DP:
            _APP_MDI_BAP_vMdiActiveUpdate();
            break;
        case ERM_NOTIFY_PBSN_AVRCP:
            if (pNotification->pbsn_avrcp.result == AT_SERVRES_STOP_SUCCEED)
            {
                /* send MDservicesupport always after AVRCP is disconnected */
                MdServiceSupport mdServ;
                _APP_MDI_BAP_vSetMdServiceSupport(&mdServ, APP_MDI_GetMetadataType());
                _APP_MDI_BAP_vBapSendMDServiceSupport(&mdServ);
            }
            if ((pNotification->pbsn_avrcp.result == AT_SERVRES_START_SUCCEED)
                /* if a2dp is already started before avrcp started */
                &&(APP_DEVICE_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK) != AT_INVALID_DEVICE_ID)
                /* player status is still init */
                &&(APP_MDI_GetPlayStatus() == APP_MDI_ACT_STATE_INIT)
                )
            {/* if still no play status known try to mute the player, changed by nikolov, 15.02.2011(RT:9613) */
                if( APP_MDI_sfSetPlayMode( APP_MDI_PAUSE ) != SUCCESS)
                {
                    DEBUG_TEXT( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "HandleErmNotification: SetPlayMode( APP_MDI_PAUSE ) FAILED");
                }
                /* force the pause command to the HU, HU need defined play status after start up(see mail communication in the ticket) */
                DEBUG_TEXT( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "HandleErmNotification: force the pause state to HU after avrcp connected");
                _APP_MDI_BAP_vBapSendPlayStatus(APP_MDI_BAP_PAUSE);
            }
            break;
        default:
            /* no handling */
            return;
    }
}

/*----------------------------------------------------------------------------*/
/* Function :   APP_MDI_BAP_vOnAppMdiNotification                             */
/** 
  \brief        Is called to handle a MDI notification  

  \param        event
                APP_MDI_enNotificationId

  \return       void

 */
/*----------------------------------------------------------------------------*/
void APP_MDI_BAP_vOnAppMdiNotification(APP_MDI_enNotificationId event)
{

    DEBUG_VALUE1( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "OnAppMdiNoti: event %d rcvd",
                  event);

    switch(event)
    {
        case APP_MDI_PLAY_STATUS_CHANGED:
        {
            APP_MDI_enActiveStatusValue actPlayStatus = APP_MDI_GetPlayStatus();
            APP_MDI_BAP_enPlayStatusValue bapPlayStatus = _APP_MDI_BAP_enBapGetPlayStatusMap(actPlayStatus);
            // send BAP response only if play status has changed after last send
            if(( enValueBapSent != bapPlayStatus )&&( APP_MDI_bIsSeekingInProgress() == BT_FALSE))
            {
                _APP_MDI_BAP_vBapSendPlayStatusMap(actPlayStatus);
            }
            break;
        }
#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
        case APP_MDI_TIME_POSITION_CHANGED:
        case APP_MDI_ABS_TIME_CHANGED:
            _APP_MDI_BAP_vSendTimePosition();
            break;
#endif

#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
        case APP_MDI_REPEAT_CHANGED:
            _APP_MDI_BAP_vBapSendRepeatModeMap(APP_MDI_GetRepeatMode());
            break;
        case APP_MDI_RANDOM_CHANGED:
            _APP_MDI_BAP_vBapSendRandomModeMap(APP_MDI_GetRandomMode());
            break;
#endif
        case APP_MDI_PATHNAME_CHANGED:
            _APP_MDI_BAP_vBapSendCurrentFileName(APP_MDI_GetFileName());
            break;
        //case APP_MDI_TITLE_CHANGED:
        //case APP_MDI_ARTIST_CHANGED:
        case APP_MDI_ALBUM_CHANGED:
            _APP_MDI_BAP_vBapSendCurrentFileInfo( APP_MDI_GetTitle(),
                                                  APP_MDI_GetArtist(),
                                                  APP_MDI_GetAlbum());
            break;
        case APP_MDI_METADATATYPE_CHANGED:
        {
            MdServiceSupport mdServ;    
            _APP_MDI_BAP_vSetMdServiceSupport(&mdServ, APP_MDI_GetMetadataType());
            _APP_MDI_BAP_vBapSendMDServiceSupport(&mdServ);
            break;
        }   
        default:
            /* ignore the event */
            DEBUG_VALUE1( APP_MDI_BAP_SWI_TRC, MOD_APP_MDI_BAP, "OnAppMdiNoti: event %d not handled",
                          event);
            break;
    }
}
 
/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_vSetMdServiceSupport                             */
/** 
  \brief        Sets the default bits of MdServiceSupport message  

  \param        pMdMss
                MdServiceSupport *

  \param        APP_MDI_enMetaDataType
                metaType

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vSetMdServiceSupport( MdServiceSupport *pMdMss,
                                               APP_MDI_enMetaDataType metaType)
{
    (void)memset(pMdMss, 0, sizeof(*pMdMss));

    /* the follow values are common for all profiles */
    pMdMss->Heartbeat = 1U;
    pMdMss->FunctionList = 1U;
    pMdMss->BapConfig = 1U;
    pMdMss->GetAll = 1U;
    pMdMss->FsgOpState = 1U;
    pMdMss->FsgSetup = 1U;
    pMdMss->MdServiceSupport  = 1U;
    pMdMss->MediaEvent = 1U;
    pMdMss->MDactive = 1U;

    DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "SetMdServSupp: metaType %d ",
                          metaType);

    switch(metaType)
    {
        case APP_MDI_ONLY_A2DP:
            /* nothing more to set for A2DP only */
            break;
        case APP_MDI_AVRCP_BASE:
        case APP_MDI_SONY_ERICCSON:
            pMdMss->PlayStatus = 1U;
            pMdMss->SkipBackward = 1U;
            pMdMss->SkipForward = 1U;
            break;
        case APP_MDI_AVRCP_1_3:
        case APP_MDI_AVRCP_1_4:    
            pMdMss->PlayStatus = 1U;
            pMdMss->SkipBackward = 1U;
            pMdMss->SkipForward = 1U;
#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
            pMdMss->TimePosition = 1U;
#endif
            pMdMss->CurrentFileInfo = 1U;
            pMdMss->CurrentFileName = 1U;
#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
            pMdMss->Repeat = 1U; // // see ticket 7424
            pMdMss->Random = 1U; // see ticket 7424
#endif
            break;
        case APP_MDI_INIT:
        default:
             /* do nothing for other meta data types */
            (void)memset(pMdMss, 0, sizeof(*pMdMss));
            DEBUG_VALUE1( APP_MDI_BAP_SWI_ERR, MOD_APP_MDI_BAP, "SetMdServSupp: metaType %d not supported",
                          metaType);
            break;
    }
}

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
/*----------------------------------------------------------------------------*/
/* Function :   _APP_MDI_BAP_vSendTimePosition                                */
/** 
  \brief         Sends the current time postion as BAP message 

  \param        pMdMss
                MdServiceSupport *

  \return       void

 */
/*----------------------------------------------------------------------------*/
static void _APP_MDI_BAP_vSendTimePosition()
{
    Uint32Type trackTimePositionInSec = APP_MDI_GetTrackTimePosition();
    Uint32Type trackTimeInSec         = APP_MDI_GetTrackTime();

    if(trackTimePositionInSec != APP_MDI_TOTAL_TRACK_TIME_INVALID)
    {
        trackTimePositionInSec = (trackTimePositionInSec+500)/1000;
    }
    else
    {
        // RNS315 wit SW version X320 doesn't support 0xFFFF for track position,
        // so fake it here to zero until it is fixed in HU
        trackTimePositionInSec = 0;
    }

    if(trackTimeInSec != APP_MDI_TOTAL_TRACK_TIME_INVALID)
    {
        trackTimeInSec = (trackTimeInSec+500)/1000;
    }

    if(trackTimeInSec > BAP_MD_TRACK_TIME_UNAVAILABLE)
    {
        trackTimeInSec = BAP_MD_TRACK_TIME_UNAVAILABLE;
    }
    if(trackTimePositionInSec > BAP_MD_TRACK_TIME_UNAVAILABLE)
    {
        trackTimePositionInSec = BAP_MD_TRACK_TIME_UNAVAILABLE;
    }

    _APP_MDI_BAP_vBapSendTimePosition((Uint16Type)trackTimeInSec, (Uint16Type)trackTimePositionInSec);
}
#endif

void _APP_MDI_cbActiveMediaPlayer(void)
{
    _APP_MDI_BAP_vMdiActiveUpdate();
}

/* End Of File APP_MDI_BAP_C1.c */
