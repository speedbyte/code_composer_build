/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     BAP_Dispatcher_InitSendBuffer.c
*-----------------------------------------------------------------------------
* Module Name:    BAP Dispatcher
*-----------------------------------------------------------------------------
* Description:    xxx
*                 
*-----------------------------------------------------------------------------
* $Date: 2010-11-30 12:01:33 +0100 (Di, 30 Nov 2010) $
* $Rev: 16597 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_BAP_Dispatcher/trunk/src/BAP_Dispatcher_InitSendBuffer.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"
#include "bap.h"
#include "bap_user_stdtypes.h"
#include "bap_defines.h"

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Bap_Dispatcher.h"
#include "BAP_Dispatcher_InitSendBuffer.h"

#include "BAP_Dispatcher_InitSendBuffer.id"
#include "Trace.h"
/*----------------------------------------------------------------------------*/
/* pragma Directives                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global Variables Definitions (avoid as much as possible!)                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Manifest Constant Definitions                                      */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Types                                                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializeTelefonInt8Sendbuffer(void);
static SuccessFailType _BAP_DISP_InitializeTelefonByteSendbuffer(void);
static SuccessFailType _BAP_DISP_InitializePhonebookInt8Sendbuffer(void);
static SuccessFailType _BAP_DISP_InitializePhonebookByteSendbuffer(void);
static SuccessFailType _BAP_DISP_InitializeMDIInt8Sendbuffer(void);
static SuccessFailType _BAP_DISP_InitializeMDIByteSendbuffer(void);
static SuccessFailType _BAP_DISP_InitializeSDInt8Sendbuffer(void);
static SuccessFailType _BAP_DISP_InitializeSDByteSendbuffer(void);

/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief	description of the private constant
*/
const Uint8Type INT8_INIT_DEFAULT_ZERO          = 0x00;
const Uint8Type INT8_INIT_NOT_SUPPORTED         = 0xFF;

const Uint8Type INT8_INIT_TEL_HANDSFREE_ON_OFF  = (Uint8Type)BAP_HANDSFREE_ON;
const Uint8Type INT8_INIT_TEL_MICROMUTE_ON_OFF  = (Uint8Type)BAP_MICRO_UNMUTE;

const Uint8Type INT8_INIT_MDI_FSG_SETUP         = (Uint8Type)0x02;

/* up to 5 numbers per entry */
const Uint8Type INT8_INIT_PB_FSG_SETUP          = 0x02;

const Uint8Type SEND_BUFFER_DEFAULT_ZERO[BAP_DISP_SEND_BUFFER_enMaxSize] = {0};
const Uint8Type SEND_BUFFER_SD_FRAME_STATUS[]   = 
                                        { 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00 };
const Uint8Type SEND_BUFFER_MDI_TIME_POSITION[]   = 
                                        { 0xFF, 0xFF, 0xFF, 0xFF};
            

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_InitSendbuffer                                      */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions except for BAP-Config,
                FunctionList and Heartbeat.The buffer is initialized for each
                functions as per the specifications in fsg_config.c
                In case of any error condition while initializing the buffer,
                the function exits, thereby not allowing BAP communication.

    \param      aLsgId
                BAP LsgId

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType BAP_DISP_InitSendbuffer(lsgId_t aLsgId)
{
    switch(aLsgId)
    {
    case BapLsg_Telephone:
	    /* Calling BAP_InitSendBufferInt8 for Telefone */
	    if( _BAP_DISP_InitializeTelefonInt8Sendbuffer() != SUCCESS )
        {

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializeTelefonInt8Sendbuffer() failed");

    	    return(FAIL);
        }

    	/* Calling BAP_InitSendBufferByteSequence for Telefone */
    	if( _BAP_DISP_InitializeTelefonByteSendbuffer() != SUCCESS )
    	{

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializeTelefonByteSendbuffer() failed");

    	    return(FAIL);
    	}
        break;
    case BapLsg_Phonebook:
	    /* Calling BAP_InitSendBufferInt8 for Phonebook */
	    if( _BAP_DISP_InitializePhonebookInt8Sendbuffer() != SUCCESS )
        {

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializePhonebookInt8Sendbuffer() failed");

    	    return(FAIL);
        }

    	/* Calling BAP_InitSendBufferByteSequence for Phonebook */
    	if( _BAP_DISP_InitializePhonebookByteSendbuffer() != SUCCESS )
    	{

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializePhonebookByteSendbuffer() failed");

    	    return(FAIL);
    	}
        break;
    case BapLsg_MDI:
	    /* Calling BAP_InitSendBufferInt8 for MDI */
	    if( _BAP_DISP_InitializeMDIInt8Sendbuffer() != SUCCESS )
        {

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializeMDIInt8Sendbuffer() failed");

    	    return(FAIL);
        }

    	/* Calling BAP_InitSendBufferByteSequence for MDI */
    	if( _BAP_DISP_InitializeMDIByteSendbuffer() != SUCCESS )
    	{

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializeMDIByteSendbuffer() failed");

    	    return(FAIL);
    	}
        break;
    case BapLsg_SD:
	    /* Calling BAP_InitSendBufferInt8 for SD */
	    if( _BAP_DISP_InitializeSDInt8Sendbuffer() != SUCCESS )
        {

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializeSDInt8Sendbuffer() failed");

    	    return(FAIL);
        }

    	/* Calling BAP_InitSendBufferByteSequence for SD */
    	if( _BAP_DISP_InitializeSDByteSendbuffer() != SUCCESS )
    	{

            DEBUG_TEXT(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitializeSDByteSendbuffer() failed");

    	    return(FAIL);
    	}
        break;
    default:

            DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_DISP_InitSendbuffer: unknown LSG: %02x", aLsgId);

        return FAIL;
    }

	return SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* Function    : BAP_DISP_InitializeTelefonInt8Sendbuffer                     */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of size Int8.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializeTelefonInt8Sendbuffer(void)
{
	/* to store the return value of BAP_InitSendBufferXXX */
     BapError_et eErr = BapErr_OK;

    /* Calling BAP_InitSendBufferInt8 for LockState */
    eErr = BAP_InitSendBufferInt8( BapLsg_Telephone,
    	                           BapFct_Telephone_LockState,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed Telephone LockState: %x", eErr);

        return(FAIL);
    }

    /* Calling BAP_InitSendBufferInt8 for FSG_OperationState */
    eErr = BAP_InitSendBufferInt8( BapLsg_Telephone,
    	                           BapFct_Telephone_FSG_OperationState,
    	                           (uint8_t)BAP_OPSTATE_PHONE_INIT );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed Telephone OperationState: %x", eErr);

        return(FAIL);
    }

    /* Initializing the send buffer for RegisterState */
    eErr = BAP_InitSendBufferInt8(  BapLsg_Telephone,
    	                            BapFct_Telephone_RegisterState,
    	                            INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed Telephone RegisterState: %x", eErr);

        return(FAIL);
    }

    /* Initializing the send buffer for LINESTATE */
	eErr = BAP_InitSendBufferInt8( BapLsg_Telephone,
		                           BapFct_Telephone_LineState,
		    	    			   INT8_INIT_DEFAULT_ZERO );

	if (BapErr_OK != eErr)
	{

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed Telephone LineState: %x", eErr);

		return(FAIL);
	}

    /* Initializing the send buffer for SignalQuality */
	eErr = BAP_InitSendBufferInt8( BapLsg_Telephone,
		                           BapFct_Telephone_SignalQuality,
		  		    			   0xFF );
	if (BapErr_OK != eErr)
	{

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed Telephone SignalQuality: %x", eErr);

		return(FAIL);
	}

    /* Initializing the send buffer for HandsFreeOnOff */
	eErr = BAP_InitSendBufferInt8( BapLsg_Telephone,
		                           BapFct_Telephone_HandsFreeOnOff,
				    			   INT8_INIT_TEL_HANDSFREE_ON_OFF );
	if (BapErr_OK != eErr)
	{

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed Telephone HandsFreeOnOff: %x", eErr);

		return(FAIL);
	}

    /* Initializing the send buffer for MicroMuteOnOff */
	eErr = BAP_InitSendBufferInt8( BapLsg_Telephone,
		                           BapFct_Telephone_MicroMuteOnOff,
		                           INT8_INIT_TEL_MICROMUTE_ON_OFF );
	if (BapErr_OK != eErr)
	{

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed Telephone MircoMuteOnOff: %x", eErr);

		return(FAIL);
	}

	return(SUCCESS);
}

/*----------------------------------------------------------------------------*/
/* Function    : _BAP_DISP_InitializeTelefonByteSendbuffer                     */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of byte sequence.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializeTelefonByteSendbuffer(void)
{
	BapError_et eErr = BapErr_OK;
    /* Taken maximum size of 8 bytes */
    //Uint8Type initBuffer[] = {0x38,0x3,0xfb,0x7f,0xeb,0x98,0x0,0x0};
    Uint8Type initBuffer[] = {0x1C,0xC0,0xDF,0xFE,0xD7,0x19,0x0,0x0};

    Uint8Type fsgSetupInitBuf[] = {TELEPHONE_DEFAULT_FSG_SETUP__PHONE_CHARACTERISTICS, 
                                   TELEPHONE_DEFAULT_FSG_SETUP__MOBILE_CONNECTION_TYPE, 
                                   TELEPHONE_DEFAULT_FSG_SETUP__SUPP_NUM_USER_PROFILES, 
                                   TELEPHONE_DEFAULT_FSG_SETUP__PHONE_TYPE};

    /* Initializing the send buffer for ActiveUser */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Telephone,
    		                                BapFct_Telephone_ActiveUser,
                                            SEND_BUFFER_DEFAULT_ZERO,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enTelActiveUser);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for Telephone ActiveUser: %x", eErr);

    	return(FAIL);
    }


    /* Initializing the send buffer for NetworkProvider   */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Telephone,
    		                                BapFct_Telephone_NetworkProvider,
                                            SEND_BUFFER_DEFAULT_ZERO,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enTelNetworkProvider);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for Telephone NetworkProvider: %x", eErr);

    	return(FAIL);
    }
    
    /* Initializing the send buffer for FSG Setup */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Telephone,
								            BapFct_Telephone_FSG_Setup,
                                            //SEND_BUFFER_DEFAULT_ZERO,
                                            fsgSetupInitBuf,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enTelFSGSetup);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for Telephone FSG_Setup: %x", eErr);

        return(FAIL);
    }

    /* Initializing the send buffer for MobileServiceSupport   */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Telephone,
    		                                BapFct_Telephone_MobileServiceSupport,
                                            initBuffer,
                                            (uint16_t)BAP_DISP_SEND_BUFFER_enTelMobileServiceSupport);
								            //BAP_TELEPHON_FCTID_MOBILESERVICESUPPORT_SIZE);
    
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for Telephone MobileServiceSupport: %x", eErr);

		return(FAIL);
    }

	/* Initializing the send buffer for CALLSTATE */
	eErr = BAP_InitSendBufferByteSequence( BapLsg_Telephone,
				                           BapFct_Telephone_CallState,
							    		   SEND_BUFFER_DEFAULT_ZERO,
                                           (uint16_t)BAP_DISP_SEND_BUFFER_enTelCallState);
	if (BapErr_OK != eErr)
	{

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for Telephone CallState: %x", eErr);

		return(FAIL);
	}


	return (SUCCESS);
}

/*----------------------------------------------------------------------------*/
/* Function    : _BAP_DISP_InitializePhonebookInt8Sendbuffer                  */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of size Int8.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializePhonebookInt8Sendbuffer(void)
{
	/* to store the return value of BAP_InitSendBufferXXX */
     BapError_et eErr = BapErr_OK;

    /* Calling BAP_InitSendBufferInt8 for FSGOPERATIONSTATE   */
    eErr = BAP_InitSendBufferInt8( BapLsg_Phonebook,
    	                           BapFct_Phonebook_FSG_OperationState,
    	                           INT8_INIT_DEFAULT_ZERO );
	if (BapErr_OK != eErr)
	{

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for PB OpertationState: %x", eErr);

		return(FAIL);
	}

    /* Calling BAP_InitSendBufferInt8 for FSG Setup */
    eErr = BAP_InitSendBufferInt8( BapLsg_Phonebook,
		                           BapFct_Phonebook_FSG_Setup,
    	                           INT8_INIT_PB_FSG_SETUP );  
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for PB FSGSETUP:%x", eErr);

        return(FAIL);
    }

	return(SUCCESS);
}

/*----------------------------------------------------------------------------*/
/* Function    : _BAP_DISP_InitializePhonebookByteSendbuffer                   */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of byte sequence.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializePhonebookByteSendbuffer(void)
{
	BapError_et eErr = BapErr_OK;
    
    /* bits are twisted ( bit order in each byte is changed ) */
    /* sms bits are set during runtime when PMST=1 was received */
    //Uint8Type mobileServiceSupportBuf[] = { 0x38,0x03,0xFB,0xC0,0x00,0x0,0x0,0x0 };
    Uint8Type mobileServiceSupportBuf[] = { 0x1C,0xC0,0xDF,0x07,0x00,0x0,0x0,0x0 };


    /* Initializing the send buffer for MobileServiceSupport   */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Phonebook,
    		                                BapFct_Phonebook_MobileServiceSupport,
                                            mobileServiceSupportBuf,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enPBMobileServiceSupport);
	if (BapErr_OK != eErr)
	{

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for PB MobileServiceSupport: %x", eErr);

		return(FAIL);
	}
    
    /* Initializing the send buffer for MissedCallIndication */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Phonebook,
    		                                BapFct_Phonebook_MissedCallIndication,
                                            SEND_BUFFER_DEFAULT_ZERO,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enPBMissedCallIndication);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for PB MissedCalledIndication:%x", eErr);

		return(FAIL);
    }
	
    /* Initializing the send buffer for PbState */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Phonebook,
    		                                BapFct_Phonebook_PbState,
                                            SEND_BUFFER_DEFAULT_ZERO,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enPBPbState);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for PB PbState:%x", eErr);

		return(FAIL);
    }

    /* Initializing the send buffer for VoiceMailboxNumber */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Phonebook,
    		                                BapFct_Phonebook_VoiceMailboxNumber,
                                            SEND_BUFFER_DEFAULT_ZERO,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enPbVoiceMailBox);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for PB VoiceMailboxNumber:%x", eErr);

		return(FAIL);
    }

    /* Initializing the send buffer for SMSState */
    eErr = BAP_InitSendBufferByteSequence(  BapLsg_Phonebook,
    		                                BapFct_Phonebook_SMSState,
                                            SEND_BUFFER_DEFAULT_ZERO,
								            (uint16_t)BAP_DISP_SEND_BUFFER_enPBSmsState);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for PB SMSState:%x", eErr);

		return(FAIL);
    }
    return (SUCCESS);
}

/*----------------------------------------------------------------------------*/
/* Function    : _BAP_DISP_InitializeMDIInt8Sendbuffer                         */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of size Int8.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializeMDIInt8Sendbuffer(void)
{
	/* to store the return value of BAP_InitSendBufferXXX */
     BapError_et eErr = BapErr_OK;

    /* Calling BAP_InitSendBufferInt8 for FSGOPERATIONSTATE   */
    eErr = BAP_InitSendBufferInt8( BapLsg_MDI,
    	                           BapFct_MDI_FSG_OperationState,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI OperationState:%x", eErr);

        return(FAIL);
    }

    /* Calling BAP_InitSendBufferInt8 for FSG Setup */
    eErr = BAP_InitSendBufferInt8( BapLsg_MDI,
		                           BapFct_MDI_FSG_Setup,
    	                           INT8_INIT_MDI_FSG_SETUP );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI FSGSETUP:%x", eErr);

        return(FAIL);
    }

    /* Calling BAP_InitSendBufferInt8 for PlayStatus */
    eErr = BAP_InitSendBufferInt8( BapLsg_MDI,
		   		                   BapFct_MDI_PlayStatus,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI PlayStatus:%x", eErr);

        return(FAIL);
    }


//    /* Calling BAP_InitSendBufferInt8 for Activate */
//    eErr = BAP_InitSendBufferInt8( BapLsg_MDI,
//		   		                   BapFct_MDI_MDactivate,
//    	                           INT8_INIT_DEFAULT_ZERO );
//    if (BapErr_OK != eErr)
//    {
//        return(FAIL);
//    }


    /* Calling BAP_InitSendBufferInt8 for MediaEvent */
    eErr = BAP_InitSendBufferInt8( BapLsg_MDI,
		   		                   BapFct_MDI_MediaEvent ,
    	                           INT8_INIT_NOT_SUPPORTED );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI MediaEvent:%x", eErr);

        return(FAIL);
    }
#ifdef APP_MDI_BAP_RANDOM_REPEAT_SUPPORTED
    /* Calling BAP_InitSendBufferInt8 for Random */
    eErr = BAP_InitSendBufferInt8( BapLsg_MDI,
		   		                   BapFct_MDI_Random,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI Random:%x", eErr);

        return(FAIL);
    }

    /* Calling BAP_InitSendBufferInt8 for Repeat */
    eErr = BAP_InitSendBufferInt8( BapLsg_MDI,
		   		                   BapFct_MDI_Repeat,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI Repeat:%x", eErr);

        return(FAIL);
    }
#endif
	return(SUCCESS);
}

/*----------------------------------------------------------------------------*/
/* Function    : _BAP_DISP_InitializeMDIByteSendbuffer                   */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of byte sequence.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializeMDIByteSendbuffer(void)
{
	BapError_et eErr = BapErr_OK;

    eErr = BAP_InitSendBufferByteSequence( BapLsg_MDI,
								           BapFct_MDI_MDserviceSupport,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enMDIMDServiceSupport);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI MDserviceSupport: %x", eErr);

        return(FAIL);
    }

    eErr = BAP_InitSendBufferByteSequence( BapLsg_MDI,
								           BapFct_MDI_MDactive,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enMDIMDActive);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI MDactive:%x", eErr);

        return(FAIL);
    }

#ifdef APP_MDI_BAP_TIME_POSITION_SUPPORTED
    eErr = BAP_InitSendBufferByteSequence( BapLsg_MDI,
								           BapFct_MDI_TimePosition,
                                           SEND_BUFFER_MDI_TIME_POSITION,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enMDITimePosition);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI TimePositon:%x", eErr);

        return(FAIL);
    }
#endif

    eErr = BAP_InitSendBufferByteSequence( BapLsg_MDI,
								           BapFct_MDI_CurrentFileName,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enCurrentFileName);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI CurrentFileName:%x", eErr);

        return(FAIL);
    }


    eErr = BAP_InitSendBufferByteSequence( BapLsg_MDI,
								           BapFct_MDI_CurrentFileInfo,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enCurrentFileInfo); 
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for MDI CurrentFileInfo:%x", eErr);

        return(FAIL);
    }

    
    return (SUCCESS);
}


/*----------------------------------------------------------------------------*/
/* Function    : _BAP_DISP_InitializeSDInt8Sendbuffer                         */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of size Int8.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializeSDInt8Sendbuffer(void)
{
	/* to store the return value of BAP_InitSendBufferXXX */
     BapError_et eErr = BapErr_OK;

    /* Calling BAP_InitSendBufferInt8 for FSG_OPERATIONSTATE   */
    eErr = BAP_InitSendBufferInt8( BapLsg_SD,
    	                           BapFct_SD_FSG_OperationState,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD FSG-OperationState:%x", eErr);

        return(FAIL);
    }

    eErr = BAP_InitSendBufferInt8( BapLsg_SD,
    	                           BapFct_SD_FrameDataAck,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD FrameDataAck:%x", eErr);

        return(FAIL);
    }
    
    eErr = BAP_InitSendBufferInt8( BapLsg_SD,
    	                           BapFct_SD_LsgStatus,
    	                           INT8_INIT_DEFAULT_ZERO );
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD LsgStatus:%x", eErr);

        return(FAIL);
    }

	return(SUCCESS);
}

/*----------------------------------------------------------------------------*/
/* Function    : _BAP_DISP_InitializeSDByteSendbuffer                   */
/**
    \brief      This module is responsible for the initialization of the send
                buffer for the property class functions of byte sequence.

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
static SuccessFailType _BAP_DISP_InitializeSDByteSendbuffer(void)
{
	BapError_et eErr = BapErr_OK;

    eErr = BAP_InitSendBufferByteSequence( BapLsg_SD,
								           BapFct_SD_ASG_Capabilities,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enSDAsgCapabilities);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD ASG_Capabilities: %x", eErr);

        return(FAIL);
    }

    eErr = BAP_InitSendBufferByteSequence( BapLsg_SD,
								           BapFct_SD_ASG_Config,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enSDAsgConfig);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD ASG_Capabilities: %x", eErr);

        return(FAIL);
    }


    eErr = BAP_InitSendBufferByteSequence( BapLsg_SD,
								           BapFct_SD_MFL_BlockDefinition,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enSDMflBlockDefinition);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD MFL_BlockDefinition: %x", eErr);

        return(FAIL);
    }

    eErr = BAP_InitSendBufferByteSequence( BapLsg_SD,
								           BapFct_SD_FrameStatus,
                                           SEND_BUFFER_SD_FRAME_STATUS,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enSDFrameStatus);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD FrameStatus: %x", eErr);

        return(FAIL);
    }
    
    eErr = BAP_InitSendBufferByteSequence( BapLsg_SD,
								           BapFct_SD_ScrollBar,
                                           SEND_BUFFER_DEFAULT_ZERO,
								           (uint16_t)BAP_DISP_SEND_BUFFER_enSDScrollBar);
    if (BapErr_OK != eErr)
    {

        DEBUG_VALUE1(BAP_DISPATCHER_SWI_ERR, MOD_BAP_DISPATCHER, "BAP_Init failed for SD ScrollBar: %x", eErr);

        return(FAIL);
    }


    return (SUCCESS);
}

/* End Of File BAP_Dispatcher_InitSendBuffer.c */
