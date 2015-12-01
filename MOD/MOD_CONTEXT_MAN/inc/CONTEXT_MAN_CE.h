/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/
/** 
 * \file	CONTEXT_MAN_CE.h
 * \brief	CE - Configuration External File. Detailed description found below.  
 * \brief	This file comprises of constants to switch on or off the interfaces that
 * can be used by other modules. It also contains various constants that can be 
 * configured by external people.
*/
/*-----------------------------------------------------------------------------
* $Date: 2009-11-20 16:20:09 +0100 (Fr, 20 Nov 2009) $
* $Rev: 9948 $
* $Author: agrawal $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_CONTEXT_MAN/trunk/inc/CONTEXT_MAN_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _CONTEXT_MAN_CE_H
#define _CONTEXT_MAN_CE_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "KPB.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/
/** 
    \brief	interface definition to initialise Context Manager.
*/
#define ICONTEXT_MAN_INIT 100
/** 
    \brief	interface definition to resolve functions calls when a button is pressed.
*/
#define ICONTEXT_MAN_BUTTON_PRESS 100
/** 
    \brief	interface definition to set the type of car being used.
*/
#define ICONTEXT_MAN_FAHRZEUG_MARKE 100

#define ICONTEXT_MAN_DIAG    100

#define ICONTEXT_MAN_APP_NOTIFICATIONS 100
/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/
/** 
\brief	description of configured interface
*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/
#endif /* _CONTEXT_MAN_CE_H */

/* End Of File CONTEXT_MAN_CE.h */
