/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file 	CONTEXT_MAN_CI.h
 * \brief	CI - Configuration Internal File.  Detailed description found below.
 * \brief	This file mainly contains of:
 * 			1. constants to switch on or off the internal interfaces. 
 * 			2. module internal constants that can be configured by the module owner.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-04-30 11:16:06 +0200 (Do, 30 Apr 2009) $
* $Rev: 5999 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CONTEXT_MAN/trunk/inc/CONTEXT_MAN_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CONTEXT_MAN_CI_H
#define _CONTEXT_MAN_CI_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "common.h"

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/
/*----------------------------------------------------------------------------*/
/* Synonym adaptions                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of module global methods                                        */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the method
*/

/*----------------------------------------------------------------------------*/
/* Module global code macros                                                         */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the macros
*/
#define sqrt(x) ((x)*(x))

/*----------------------------------------------------------------------------*/
/* Module global variables                                                    */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of the module global variable
*/

/*----------------------------------------------------------------------------*/
/* Module global const definitions                                                  */
/*----------------------------------------------------------------------------*/
typedef enum
{
	enContextManagerFnInfoCall,
	enContextManagerFnServiceCall,
	enContextManagerFnSosCall,
	enContextManagerFnInfoCall_Hangup,
	enContextManagerFnServiceCall_Hangup,
	enContextManagerFnSosCall_Hangup,
	enContextManagerFnHangUpCall,
	enContextManagerFnActiveCall,
	enContextManagerFnNoActiveCall,
	enWaitToAvoidDoubleOkPress,
    enContextManagerConnectLastUsedDevice,
    enContextManagerDeleteAllDevices
} CONTEXT_MAN_ENUM_FN_BUTTONS;

#define CONTEXT_MAN_FN_HANGUP 


#endif /* _CONTEXT_MAN_CI_H */

/* End Of File CONTEXT_MAN_CI.h */
