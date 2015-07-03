/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ERM_Utils.c
*-----------------------------------------------------------------------------
* Module Name:    External resource manager - utilities
*-----------------------------------------------------------------------------
* Description:    External resource manager global functions.
*                 the ERM is responsible for control of the UART resource
*                 towards the different other application modules
*-----------------------------------------------------------------------------
* $Date: 2010-05-14 08:10:39 +0200 (Fr, 14 Mai 2010) $
* $Rev: 13069 $
* $Author: weber $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_ERM/trunk/src/ERM_Utils.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "common.h"
#include "UART.h"
#include "ATParser.h"
#include "ATCmdHandler.h"
#include "APP_PHONE_ERM.h"
#include "APP_PHONE_BAP_ERM.h"
#include "APP_PHONEBOOK_ERM.h"
#include "APP_MDI_ERM.h"
#include "APP_MDI_BAP.h"
#include "APP_COMMON_Utils.h"
#include "ATCommonData.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "ERM.h"
#include "ERM_CE.h"
#include "ERM_AudioSets.h"
#include "ERM_DeviceHandler.h"
#include "ERM_Utils.h"
#include "ERM_Utils.id"
#include "TRACE.h"


/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
extern ERMContextType        _ERM_gContext;
/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the method
*/
void _ERM_vStartUartSupervision(void);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Methods                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_ReleaseUart                                             */
/**
    \brief      release UART ownership

    \brief      this function release the UART ownership. The function will\n
                be used by application modules if previous UART ownership\n
                was requested and granted but now applciation is not able\n
                to send request via UART due to application internal problems.
                In this case the application need to release the UART to avoid\n
                that no other application can get it anymore.

    \param      requestingApp
                APPSubModuleIdEnum
                valid member of SubModule enumeration

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ERM_ReleaseUart( APPSubModuleIdEnum requestingApp )
{
    SuccessFailType sF = FAIL;

    if( requestingApp == _ERM_gContext.eUartOwner )
    {   /* release UART ownership and return success */
        _ERM_gContext.eUartOwner = APP_SUB_NONE;
        sF = SUCCESS;

        _ERM_vStopUartSupervision();

        DEBUG_VALUE1(ERM_SWI_DBG, MOD_ERM, "ERM: Uart rel %d", requestingApp);

    }
    else if (APP_SUB_NONE == _ERM_gContext.eUartOwner)
    {   /* oops - UART was already released indicate warning but return SUCCESS */
        sF = SUCCESS;

        _ERM_vStopUartSupervision();


        DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ERM: Uart no owner %d", requestingApp);
    }
    else
    {   /* warning - different applciation is owner - something failed here */
        /* write trace and return default FAIL */

        DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "ERM: Uart inco owner %d= req %d",_ERM_gContext.eUartOwner,requestingApp);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ERM_GetUart                                                 */
/**
    \brief      try to reserve the resource UART for the given application

    \brief      the function check if ERM is in active state and UART pointer is
                valid (not NULL). Then if no other SW submodule owns the UART
                at the moment the function return the UART pointer so that this
                can be passed to requesting application

    \param      requestingApp
                APPSubModuleIdEnum
                valid member of SubModule enumeration to indicate the calling app

    \return     const SCI_ST* pointer
                NULL or const pointer to UART
   */
/*----------------------------------------------------------------------------*/
const SCI_ST* _ERM_GetUart( APPSubModuleIdEnum requestingApp )
{
    SCI_ST * pRet = NULL;
    static APPSubModuleIdEnum lastReqError = APP_SUB_NONE;

    /* check if ERM is in active state and UART available */
    if ((Uint8Type)TRUE    == _ERM_gContext.u8Active &&
        NULL    != _ERM_gContext.pUartPtr)
    {   /* check if resource is already occupied */
        if (APP_SUB_NONE == _ERM_gContext.eUartOwner )
        {   /* during ERM initial startup only allow ERM to access */
            if (BT_TRUE == _ERM_gContext.bStartupDone ||
                APP_SUB_ERM == requestingApp)
            {
                pRet           = _ERM_gContext.pUartPtr;
                _ERM_gContext.eUartOwner = requestingApp;

                DEBUG_VALUE1(ERM_SWI_DBG, MOD_ERM, "ERM: Uart alloc: %d", requestingApp);

                _ERM_vStartUartSupervision();
            }
            else
            {
                if (lastReqError != requestingApp)
                {

                    DEBUG_VALUE1(ERM_SWI_TRC, MOD_ERM, "ERM: Uart req: %d", requestingApp);
                    DEBUG_VALUE2(ERM_SWI_TRC, MOD_ERM, "ERM: Uart ............ blocked: %d (%d)", _ERM_gContext.eUartOwner, _ERM_gContext.bStartupDone);
                    lastReqError = requestingApp;
                }
            }
        }
        else
        {
            if (lastReqError != requestingApp)
            {
                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "ERM: Uart req: %d, blocked %d", requestingApp,_ERM_gContext.eUartOwner);
                DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "ERM: Uart .................... (%d %d)", _ERM_gContext.reqERMRequest, _ERM_gContext.lastERMRequest);
 
                lastReqError = requestingApp;
            }

        }
    }
    else
    {   /* give NULL ptr back - write trace here */
        if (lastReqError != requestingApp)
        {

            DEBUG_VALUE2(ERM_SWI_WNG, MOD_ERM, "ERM: Uart req: %d, failed: %d", requestingApp, _ERM_gContext.u8Active);
            lastReqError = requestingApp;
        }

    }
    return pRet;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vStartUartSupervision                                     */
/**
    \brief
    \brief      Description:\n

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_vStartUartSupervision(void)
{

    DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM, "Start uartSup %d ( %d sec )", _ERM_gContext.eUartOwner, ERM_UART_SUPERVISION);

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_UART_SUPERVISION,
                                       SEC(ERM_UART_SUPERVISION),
                                       0u);
}

/*----------------------------------------------------------------------------*/
/* Function    : _ERM_vStopUartSupervision                                      */
/**
    \brief
    \brief      Description:\n

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
void _ERM_vStopUartSupervision()
{
    DEBUG_VALUE2(ERM_SWI_DBG, MOD_ERM, "Stop uartSup %d ( %d sec )", _ERM_gContext.eUartOwner, ERM_UART_SUPERVISION);
    (void)CancelAlarm(CTA_UART_SUPERVISION);
}

/*----------------------------------------------------------------------------*/
/* Function    : ERM_UartSupervision                                         */
/**
    \brief
    \brief      Description:\n

    \param      none

    \return     none
   */
/*----------------------------------------------------------------------------*/
ALARMCALLBACK(ERM_UartSupervision)
{

    _ERM_gContext.eResetState = ERM_PENDING_RESET;

    /* start Timer to ensure that debug is written before RESET */

    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), 0u);

}


void ERM_Execute_HW_Reset (void)
{
    _ERM_gContext.eResetState = ERM_EXECUTE_RESET_VR_WATCHDOG;
    /* start Timer to ensure that debug is written before RESET */
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), 0u);
}


void ERM_PWNG4_Execute_HW_Reset (void)
{
    _ERM_gContext.eResetState = ERM_EXECUTE_RESET_PWNG4;
    /* start Timer to ensure that debug is written before RESET */
    (void)APP_COMMON_UTILS_stSetOsekRelAlarm(CTA_ERM_PENDING, MSEC(10), 0u);
}


/* End Of File ERM_Utils.c */


