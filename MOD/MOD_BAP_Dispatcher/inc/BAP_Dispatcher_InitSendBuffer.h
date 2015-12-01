/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     BAP_Dispatcher_InitSendBuffer.h
*-----------------------------------------------------------------------------
* Module Name:    BAP Dispatcher
*-----------------------------------------------------------------------------
* Description:    xxx 
*                  
*-----------------------------------------------------------------------------
* $Date: 2009-07-31 17:10:23 +0200 (Fr, 31 Jul 2009) $
* $Rev: 7574 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_BAP_Dispatcher/trunk/inc/BAP_Dispatcher_InitSendBuffer.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _BAP_DISPATCHER_INIT_SEND_BUFFER_H
#define _BAP_DISPATCHER_INIT_SEND_BUFFER_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

typedef enum
{
    BAP_DISP_SEND_BUFFER_enCurrentFileName          = 1,
    BAP_DISP_SEND_BUFFER_enTelNetworkProvider       = 1,
    BAP_DISP_SEND_BUFFER_enPbVoiceMailBox           = 1,
    BAP_DISP_SEND_BUFFER_enSDAsgCapabilities        = 2,
    BAP_DISP_SEND_BUFFER_enSDAsgConfig              = 2,
    /* we send per default an empty BAP-String and mdType set to 0 */
    BAP_DISP_SEND_BUFFER_enMDIMDActive              = 2,
    BAP_DISP_SEND_BUFFER_enCurrentFileInfo          = 3,
    BAP_DISP_SEND_BUFFER_enPBPbState                = 3,
    BAP_DISP_SEND_BUFFER_enTelActiveUser            = 3,
    BAP_DISP_SEND_BUFFER_enTelFSGSetup              = 4,
    BAP_DISP_SEND_BUFFER_enPBMissedCallIndication   = 4,
    BAP_DISP_SEND_BUFFER_enPBSmsState               = 4,
    BAP_DISP_SEND_BUFFER_enSDScrollBar              = 4,
    BAP_DISP_SEND_BUFFER_enMDITimePosition          = 4,
    BAP_DISP_SEND_BUFFER_enPBMobileServiceSupport   = 6,
    BAP_DISP_SEND_BUFFER_enSDFrameStatus            = 6,
    BAP_DISP_SEND_BUFFER_enTelMobileServiceSupport  = 8,
    BAP_DISP_SEND_BUFFER_enTelCallState             = 8,
    BAP_DISP_SEND_BUFFER_enMDIMDServiceSupport      = 8,
    BAP_DISP_SEND_BUFFER_enSDMflBlockDefinition     = 8,

    /* this element specifies the max buffer size + 1 */
    BAP_DISP_SEND_BUFFER_enMaxSize
}BAP_DISP_SEND_BUFFER_enSizes;

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

SuccessFailType BAP_DISP_InitSendbuffer(lsgId_t aLsgId);

#endif /* _BAP_DISPATCHER_INIT_SEND_BUFFER_H */

/* End Of File BAP_Dispatcher_InitSendBuffer.h */
