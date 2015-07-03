/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name: 	OSEK_HookFunctions_C1.c
*-----------------------------------------------------------------------------
* Module Name:	MOD_OSEK
*-----------------------------------------------------------------------------
* Description:	file to provide the Hook Function's StartupHook(), PreTaskHook(),
* 				ErrorHook().
*-----------------------------------------------------------------------------
* $Date: 2011-11-08 11:56:21 +0100 (Di, 08 Nov 2011) $
* $Rev: 23158 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/SYS_OSEK_INTERFACE/trunk/src/OSEK_HookFunctions_C1.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include <osek.h>

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "SYS_INIT.h"
#include "PORT.h"

/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/
/**
\brief	variable to check if we get any Osek-Error code
*/
// Do not Init these variables here, variables will be init with Osek fatal error
Uint32Type FatalOsekErrorOccured;
Uint16Type FatalOsekErrorCode;
Uint16Type FatalOsekguiError;

/*----------------------------------------------------------------------------*/
/* Global const Definitions                                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of private Methods                                              */
/*----------------------------------------------------------------------------*/
static void setOsStarted(void);
static SuccessFailType sfHandleError_E_OS_NOFUNC(void);
static SuccessFailType sfHandleError_E_OS_STATE(void);
static SuccessFailType sfHandleErrorCode(StatusType errorCode);
/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Variables                                                          */
/*----------------------------------------------------------------------------*/
static Uint8Type osStarted = FALSE;
/*----------------------------------------------------------------------------*/
/* Private const Definitions                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/****************************************************************************/
/***********                      OSEK functions                  ***********/
/****************************************************************************/
/*----------------------------------------------------------------------------*/
/* Function    : setOsStarted                                                    */
/** 
    \brief      sets variable indicating OSEK has started
  
    \brief      Description:\n
                sets variable indicating OSEK has started\n
    
    \return     void
   */
/*----------------------------------------------------------------------------*/
/**
 * As there is no possibility to check whether the operation system is already
 * running, we have to take care of that in the user callback
 */
static void setOsStarted(void)
{
    osStarted = TRUE;
}
/*----------------------------------------------------------------------------*/
/* Function    : isOsStarted                                                    */
/** 
    \brief      returns whether OSEK has been started or not
  
    \brief      Description:\n
                returns whether OSEK has been started or not \n
    
    \return     int
                TRUE or FALSE
   */
/*----------------------------------------------------------------------------*/
/**
 * \brief Returns whether the os has been started or not
 */
int isOsStarted(void)
{
    return osStarted;
}

/*----------------------------------------------------------------------------*/
/* Function    : sfHandleError_E_OS_NOFUNC                                    */
/** 
    \brief      OSEK Error handler for E_OS_NOFUNC
  
    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType sfHandleError_E_OS_NOFUNC(void)
{
    SuccessFailType result = SUCCESS;
 
    switch( OSErrorGetServiceId() )
    {
        case OSServiceId_GetAlarm:
            /* this error indicates that the specified alarm does not exist */
            result = SUCCESS;
            break;
        case OSServiceId_CancelAlarm:
            /* this error indicates that the alarm to cancel was not active */
            result = SUCCESS;
            break;
        default:
            result = FAIL;
            break;
    }            
   
    return result; 
}

/*----------------------------------------------------------------------------*/
/* Function    : sfHandleError_E_OS_STATE                                     */
/** 
    \brief      OSEK Error handler for E_OS_STATE
  
    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType sfHandleError_E_OS_STATE(void)
{
    SuccessFailType result = SUCCESS;
 
    switch( OSErrorGetServiceId() )
    {
        case OSServiceId_SetRelAlarm:
            /* this error indicates that the alarm to set was already active */
            result = SUCCESS;
            break;
        default:
            result = FAIL;
            break;
    }            
   
    return result; 
}

/*----------------------------------------------------------------------------*/
/* Function    : sfHandleErrorCode                                                    */
/** 
    \brief      OSEK Error Code handler
  
    \brief      Description:\n

    \param      errorCode
                StatusType

    \return     SuccessFailType
*/
/*----------------------------------------------------------------------------*/
static SuccessFailType sfHandleErrorCode(StatusType errorCode)
{
    SuccessFailType result = SUCCESS;
    volatile uint16 guiError;
    TaskStateType HostState;
    TaskStateType MonitorState;
    TaskStateType DriverSTate;
    TaskStateType AppState;
    TaskStateType DatabaseState;
    TaskStateType SystemState;
    
    guiError = OSErrorGetosCANError();

    (void)GetTaskState(OsHostTask_10ms, &HostState);
    (void)GetTaskState(OsMonitorTask_50ms, &MonitorState);
    (void)GetTaskState(OsDriverTask_1ms, &DriverSTate);
    (void)GetTaskState(OsAppTask_event_100ms, &AppState);
    (void)GetTaskState(OsDatabaseTask_event, &DatabaseState);
    (void)GetTaskState(OsSystemTask_event, &SystemState);

    switch( errorCode )
    {
        case E_OS_NOFUNC:
            result = sfHandleError_E_OS_NOFUNC();
            break;
        case E_OS_STATE:
            result = sfHandleError_E_OS_STATE();
            break;
        default:
        FatalOsekErrorOccured = 0xAEEAAEEA;
	    FatalOsekErrorCode = errorCode;
	    FatalOsekguiError = guiError;

            result = FAIL;
            break;
    }            
   
    return result; 
}

/*----------------------------------------------------------------------------*/
/* Function    : ErrorHook                                                    */
/** 
    \brief      OSEK hook routine for reporting OSEK errors
  
    \brief      Description:\n
                This function will be calle by the operation system if \n
                an error occured \n

    \param      ErrorCode
                StatusType
                value limits
    \return     void
*/
/*----------------------------------------------------------------------------*/
void ErrorHook(StatusType ErrorCode)
{
    volatile StatusType gErrorCode;
    
    gErrorCode = ErrorCode;
    

    if( sfHandleErrorCode( ErrorCode ) != SUCCESS )
    {
        ShutdownOS(ErrorCode);
    }
    gErrorCode = gErrorCode;
}/* END OF ErrorHook() */


/*----------------------------------------------------------------------------*/
/* Function    : StartupHook                                                  */
/** 
    \brief      OSEK hook routine which was called before operation system runs
  
    \brief      Description:\n
                This function can be used to make initialization which must done \n
                only by the system start and then never again \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void StartupHook(void)
{
    /* tells the os has been started.                                         */
    (void)SYSINIT_sfHwInit();	/* initialize the Hardware parts of the System (Port's and watchdog). */
    
    /* tells the os has been started.                                         */
    setOsStarted();
    
    /* start OSEK-COM support                                               */
    (void)StartCOM();
    
}/* END OF StartupHook() */


/*----------------------------------------------------------------------------*/
/* Function    : PreTaskHook                                                  */
/** 
    \brief      OSEK hook routine which was called after a kontext switch 
  
    \brief      Description:\n
                This function can be used for Task run time meassurement \n

    \return     void
   */
/*----------------------------------------------------------------------------*/
void PreTaskHook(void)
{

}/* END OF PreTaskHook() */


/* End Of File OSEK_HookFunctions_C1.c */
