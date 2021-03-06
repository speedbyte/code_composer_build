/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_CHSDL_CI.h
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

#ifndef _APP_CHSDL_CI_H
#define _APP_CHSDL_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "osek.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/


typedef struct
{
    BooleanType                 bKL15On;
    BooleanType                 bPrepareCk5050Shutdown;
    Uint32Type                  unhandledShutdown_AppBitmask;
    APP_CSHDL_NotificationType  cshdlState;
}AppCshdlContext;


#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__NONE               (0 << 0)
#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_DEVICE         (1 << 0)
#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_SD             (1 << 1)
#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__KPB                (1 << 2)
#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__ERM                (1 << 3)
#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONEBOOK_BAP  (1 << 4)
#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONE_BAP      (1 << 5)
#define APP_CSHDL_CK5050_SHUTDOWN_UNHANDLED__APP_PHONE          (1 << 6)

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/



#endif /* _APP_CHSDL_CI_H */

/* End Of File APP_CSHDL_CI.h */

