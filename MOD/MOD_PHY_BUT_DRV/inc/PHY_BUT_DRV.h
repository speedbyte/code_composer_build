/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     PHY_BUT_DRV.h
*-----------------------------------------------------------------------------
* Module Name:  PHY_BUT_DRV
*-----------------------------------------------------------------------------
* Description: Driver part (host task) of 3 Button module handling
*-----------------------------------------------------------------------------
* $Date: 2010-01-25 15:04:11 +0100 (Mo, 25 Jan 2010) $
* $Rev: 10732 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_PHY_BUT_DRV/branch/MOD_PHY_BUT_DRV_POLO_branch/inc/PHY_BUT_DRV.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _PHY_BUT_DRV_H
#define _PHY_BUT_DRV_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/

#include "PHY_BUT_DRV_CE.h"
#include "LOGI_BUT_MAN.h"
#include "APP_CSHDL.h"
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

/**
   \brief Button states

   \brief A button can have the state of pressed or not pressed
*/
typedef enum {
    enNoTransition = 0,
    enButtonPressed_PosFlanke,
    enButtonReleased_NegFlanke,
    enDefaultState = 0xFF
} PHY_BUT_DRV_ENUM_DYNAMIC_STATE;

typedef struct {
	LOGI_BUT_MAN_ENUM_BUTTON_NAMES enButtonName;
	LOGI_BUT_MAN_ENUM_DEVICES enDeviceName;
	PHY_BUT_DRV_ENUM_DYNAMIC_STATE enButtonState;
	LOGI_BUT_MAN_ENUM_PRESSTYPE enButtonPressType;
	Uint8Type enTicksReceived;
} PHY_BUT_DRV_ST_MESSAGE;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

#ifdef IPHY_BUT_DRV_INIT

extern SuccessFailType PHY_BUT_DRV_sfInit(void);

#endif 

#ifdef IPHY_BUT_DRV_HOST 

SuccessFailType PHY_BUT_DRV_vhandle (LOGI_BUT_MAN_ENUM_DEVICES enDevice);

#endif 

#ifdef IPHY_BUT_DRV_DIAGNOSE_CRADLE

BooleanType PHY_BUT_DRV_bGetTelephoneConnectivity(void);
TriStateType PHY_BUT_DRV_bGetCradleConnectivity(void);
BooleanType PHY_BUT_DRV_bGetBasePlateConnectivity(void);
SuccessFailType PHY_BUT_DRV_sfSwitchBasePlateVoltageDiagnostics(Uint8Type u8Value);

#endif

#ifdef IPHY_BUT_DRV_APP_CSHDL

void PHY_BUT_DRV_vOnAppCshdlNotification(APP_CSHDL_NotificationType notification);

#endif

Uint8Type PHY_BUT_DRV_u8GetVariantType(void);
void PHY_BUT_DRV_vStartup(void);

#endif /* _PHY_BUT_DRV_H */

/* End Of File PHY_BUT_DRV.h */
