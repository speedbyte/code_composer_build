/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     KPB_Alarm_C1.c
*-----------------------------------------------------------------------------
* Module Name:    KPB 
*-----------------------------------------------------------------------------
* Description:    Handles all KPB specific alarm callbacks 
*-----------------------------------------------------------------------------
* $Date: 2011-04-26 14:04:38 +0200 (Di, 26 Apr 2011) $:
* $Rev: 19307 $:
* $Author: nikolov $:
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_KPB/trunk/src/KPB_Alarm_C1.c $:
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "APP_COMMON_Utils.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>           // vector
#include <osekext.h>        // vector

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "KPB.h"
#include "KPB_CI.h"
#include "KPB_CE.h"
#include "KPB_BAP.h"
#include "KPB_Alarm.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/** 
\brief	variable that contains the module global variables
*/
extern KpbContext gKpbContext;
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
/* Function    : _KPB_vStartAsgSupervision                                     */
/** 
    \brief      Activates a timer for ASG supervision  

    \return     void
   */
/*----------------------------------------------------------------------------*/
void _KPB_vStartAsgSupervision()
{

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ASG_SV_TIMEOUT, 
                                       MSEC(KPB_ASG_SUPERVISION_MS_TIMEOUT), 
                                       0);

    gKpbContext.isAsgResponseOutstanding = BT_TRUE;

    _KPB_vBapSendLsgStatus(BT_TRUE);
}

/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vStopAsgSupervision                                      */
/** 
    \brief      Stops the ASG supervision  mechanism 

    \return     void
   */
/*----------------------------------------------------------------------------*/
void _KPB_vStopAsgSupervision()
{
    (void)CancelAlarm(CTA_ASG_SV_TIMEOUT);

    gKpbContext.isAsgResponseOutstanding = BT_FALSE;
}

/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vStartFrameDataTimer                                     */
/** 
    \brief      Activates the FrameDataAck timeout

    \return     void
   */
/*----------------------------------------------------------------------------*/
void _KPB_vStartFrameDataAckTimer()
{
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_FRAME_DATA_ACK_TIMEOUT, 
                                       MSEC(KPB_FRAME_DATA_ACK_MS_TIMEOUT), 
                                       0);
}

/*----------------------------------------------------------------------------*/
/* Function    : _KPB_vStopFrameDataTimer                                      */
/** 
    \brief      Stops the FrameDataAck timer  

    \return     void
   */
/*----------------------------------------------------------------------------*/
void _KPB_vStopFrameDataAckTimer()
{
    (void)CancelAlarm(CTA_FRAME_DATA_ACK_TIMEOUT);
}

/*----------------------------------------------------------------------------*/
/* Function    : KPB_cbFrameDataAckTimeout                                    */
/** 
    \brief      Callback that is invoked after FrameDataAck timeout  

    \return     void
   */
/*----------------------------------------------------------------------------*/
ALARMCALLBACK(KPB_cbFrameDataAckTimeout)
{
    /* set event since we can not resend BapMessage from here 
     * ( Alloc / Free is not possible from alarm callback ) */
    (void)SetEvent( OsAppTask_event_100ms, evAppTaskKpbPending);
}


/*----------------------------------------------------------------------------*/
/* Function    : KPB_cbDisplayStatusOff                                       */
/** 
    \brief      Callback that is invoked after DisplayStatusOff timeout  

    \return     void
   */
/*----------------------------------------------------------------------------*/
ALARMCALLBACK(KPB_cbDisplayStatusOff)
{
        /* since we are in different task context, set flag to notify to APP_SD */
        gKpbContext.notifyPresentationOff = BT_TRUE;
}

/* End Of File KPB_Alarm_C1.c */


