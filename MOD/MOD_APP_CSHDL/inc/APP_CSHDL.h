/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_CHSDL.h
*-----------------------------------------------------------------------------
* Module Name:  APP_CHSDL
*-----------------------------------------------------------------------------
* Description:  manage KL15 on / off and CK5050 shutdown preparation
*-----------------------------------------------------------------------------
* $Date$
* $Rev$
* $Author$
* $URL$
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_CHSDL_H
#define _APP_CHSDL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

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
typedef enum
{
/* 0 */    APP_CSHDL_NOTIFY_KL_15_OFF = 0,
/* 1 */    APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_OFF,
/* 2 */    APP_CSHDL_NOTIFY_UHV_ACTIVE_KL_15_ON, // former APP_CSHDL_NOTIFY_KL_15_ON,
/* 3 */    APP_CSHDL_NOTIFY_UHV_ACTIVE_KL15_OFF_BEFORE_CK5050_OFF,  // this state is only for playing Disconnect Jingle
/* 4 */    APP_CSHDL_NOTIFY_PREPARE_CK5050_SHUTDOWN,
/* 5 */    APP_CSHDL_NOTIFY_WAIT_FOR_SHUTDOWN,
/* 6 */    APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON,                  //  Trigger for Screen S010
/* 7 */    APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_OFF,                 //
/* 8 */    APP_CSHDL_NOTIFY_CRADLE_RESTRICTION_ON_DISCONNECT_DEVICES   // Shutdown started because Cradle removed
}APP_CSHDL_NotificationType;

typedef enum
{
    APP_CSHDL_APP_DEVICE = 0,
    APP_CSHDL_APP_SD,
    APP_CSHDL_KPB,
    APP_CSHDL_ERM,
    APP_CSHDL_APP_PHONEBOOK_BAP,
    APP_CSHDL_APP_PHONE_BAP,
    APP_CSHDL_APP_PHONE
}APP_CSHDL_NotifiedApp;

typedef enum
{
    APP_CSHDL_CRADLE_RESTRICTION_UNKNOWN=0,
    APP_CSHDL_CRADLE_RESTRICTION_OFF,
    APP_CSHDL_CRADLE_RESTRICTION_ON
}APP_CSHDL_enCradleRestriction_States;     // for external apps

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

SuccessFailType APP_CSHDL_sfInitApp(void);
void APP_CSHDL_vHandleEvents(EventMaskType event);


void APP_CSHDL_vCk5050ShutDownHandled(APP_CSHDL_NotifiedApp app);
APP_CSHDL_NotificationType APP_CSHDL_eGetCshdlState();
APP_CSHDL_enCradleRestriction_States APP_CSHDL_eGetCradleRestrictionState(void);

#endif /* _APP_CSHDL_H */

/* End Of File APP_CSHDL.h */

