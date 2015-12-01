/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     APP_PHONE_AT_PNCC.h
*-----------------------------------------------------------------------------
* Module Name:
*-----------------------------------------------------------------------------
* Description:
*-----------------------------------------------------------------------------
* $Date: 2010-08-04 18:36:07 +0200 (Mi, 04 Aug 2010) $
* $Rev: 14154 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_PHONE/trunk/inc/APP_PHONE_AT_PNCC.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_PHONE_AT_PNCC_H
#define _APP_PHONE_AT_PNCC_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/* include the export configuration file */
#include "APP_PHONE_CE.h"
#include "ERM_CE.h"

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
#define BAP_CALL_STATE_DATA_SIZE    8
#define MAX_PLCC_ELEMENTS           ERM_MAX_NUM_PLCC
/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef enum
{
    APP_PHONE_CALL_INDEX_0 = 0,
    APP_PHONE_CALL_INDEX_1,
    APP_PHONE_CALL_INDEX_2,
    APP_PHONE_CALL_NONE
}APP_PHONE_enCallIndex;

typedef enum
{
    PNCC_INIT = 0,
    WAIT_FOR_PLCC,
    PLCC_COMPLETE,
    PNCC_VALID,
    PNCC_INVALID
} PnccState;

typedef enum
{
    ARRAY_ELEM_UNKNOWN = 0,
    ARRAY_ELEM_NOT_CHANGED,
    ARRAY_ELEM_ADDED,
    ARRAY_ELEM_REMOVED
} ElementChangedState;

typedef struct
{
    /* PNCC / PLCC specific */ 
    Uint8Type               currentPncc;
    PnccState               pnccState;
    ATRspCallStatValType    *plcc[MAX_PLCC_ELEMENTS];
    Uint8Type               arrayHasChanged;
    ElementChangedState     CallInfoHasChanged[MAX_PLCC_ELEMENTS];
    ElementChangedState     changedState[MAX_PLCC_ELEMENTS]; 
}AppPhoneAtPnccContext;

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Global const Declarations                                                  */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/

// public method
const AppPhoneAtPnccContext *APP_PHONE_AT_PNCC_pGetPnccContext();
const ATRspCallStatValType  *APP_PHONE_AT_PNCC_pGetPlccContext(ATCallIndexEnum index);
BooleanType APP_PHONE_AT_PNCC_bAllCallStatesIdle();
BooleanType APP_PHONE_AT_PNCC_bArrayHasChanged();
BooleanType APP_PHONE_AT_PNCC_bCallInfoHasChanged();
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfActiveCalls();
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfTerminatedCalls();
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfHeldCalls();
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfIncomingCalls();
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfWaitingCalls();
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfOutgoingCalls();
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfCallType(ATCallStateEnum callState);
Uint16Type  APP_PHONE_AT_PNCC_u16GetNumberOfCalls(); 
ATCallIndexEnum APP_PHONE_AT_PNCC_GetOutgoingCallIndex();

// module internal methods
SuccessFailType _APP_PHONE_AT_PNCC_sfHandleCallStatusCount(const ATRspCallStatCountType *pPncc);
SuccessFailType _APP_PHONE_AT_PNCC_sfHandleCallStatusIndex(const ATRspCallStatValType *pPlcc);
SuccessFailType _APP_PHONE_AT_PNCC_sfInit();
SuccessFailType _APP_PHONE_AT_PNCC_sfSetPnccDefault();

APP_PHONE_enCallIndex _APP_PHONE_AT_PNCC_GetCurrentActiveCallIndex();
//NumHeldCallsEnum _APP_PHONE_AT_PNCC_GetCurrentHeldCallIndex();
/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/** 
\brief	description of the interface ITEMPLATE
*/
#ifdef ITEMPLATE /* BEGIN DECLARE INTERFACE(ITEMPLATE) */

#endif /* END DECLARE INTERFACE(ITEMPLATE) */


#endif /* _APP_PHONE_AT_PNCC_H */

/* End Of File APP_PHONE_AT_PNCC.h */

