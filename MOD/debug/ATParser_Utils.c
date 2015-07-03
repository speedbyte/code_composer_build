/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATCmdHandler_Utils.c
*-----------------------------------------------------------------------------
* Module Name:    AT Command Handler Utilities
*-----------------------------------------------------------------------------
* Description:    The AT Command Handler manage and built the different
                  message for the Parrot module.
*-----------------------------------------------------------------------------
* $Date: 2011-12-05 15:03:36 +0100 (Mo, 05 Dez 2011) $
* $Rev: 24429 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Parser/trunk/src/ATParser_Utils.c $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/
#include "osek.h"


/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
#include "Common.h"
#include "ATParser_CI.h"
#include "ATParser_Utils.h"
#include "UART.h"
#include "APP_PHONE.h"

#include "ATParser_Utils.id"
#include "TRACE.h"



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
/**
\brief  description of the type
*/
typedef enum
{
    AT_PARSER_enNoQuotes,
    AT_PARSER_enBeginQuote,
    AT_PARSER_enEndQuote

}ATParseQuotedParamsEnum;

/*----------------------------------------------------------------------------*/
/* Prototypes of file local Methods                                           */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the method
*/
/*----------------------------------------------------------------------------*/
/* File local Code Macros                                                        */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the macros
*/

/*----------------------------------------------------------------------------*/
/* File local Variables                                                       */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private variable
*/


/*----------------------------------------------------------------------------*/
/* File local const Definitions                                               */
/*----------------------------------------------------------------------------*/
/**
\brief  description of the private constant
*/

/*----------------------------------------------------------------------------*/
/* Private Address Calculations                                               */
/*----------------------------------------------------------------------------*/


/****************************************************************************/
/*  Method:     ATParseFirstToken                                          */
/****************************************************************************/
Uint16Type _ATParser_FirstToken(Uint8Type *p_msg)
{
    Uint8Type *inPtr  = p_msg;
    Uint16Type length = 0;

    /* Now extract the token with colon or end-of-string delimiters         */
    while((*inPtr != ':') && (*inPtr != 0) && (length <= PAT_MAX_TOKEN_LENGTH))
    {
        inPtr++;
        length++;
    }
    /* take colon into account for token itself */
    if (*inPtr == ':')
    {
        length++;
    }
    /* Flag token exceeding maximum length                                  */
    if (length > PAT_MAX_TOKEN_LENGTH)
        length = 0;
    return(length);
}


/****************************************************************************/
/*  Method:     ATParseOtherToken                                          */
/****************************************************************************/
Uint16Type _ATParser_OtherToken(Uint8Type *p_msg)
{
    Uint8Type *inPtr  = p_msg;
    Uint16Type length = 0;
    ATParseQuotedParamsEnum enQuoteState = AT_PARSER_enNoQuotes;

    /* Now extract the token with comma or end-of-string delimiters         */
    while( /* (*inPtr != ',') &&*/ (*inPtr != 0) && (length <= PAT_MAX_TOKEN_LENGTH))
    {
        if( enQuoteState == AT_PARSER_enEndQuote &&
            *inPtr != ',')
        {
            // last char was a quote, but current char is not the trailing
            // comma, so it seems that the quote is wihtin the leading and
            // trailing quote
            enQuoteState = AT_PARSER_enBeginQuote;
        }
        if( (enQuoteState != AT_PARSER_enBeginQuote) &&
             *inPtr == ',')
        {
            // we never had a quote ( numeric param, AT_PARSER_enNoQuotes ) or
            // we got trailing quote immediately followed by comma
            break;
        }
        if( *inPtr == AT_ASCII_QUOTE)
        {
            if( enQuoteState == AT_PARSER_enNoQuotes ||
                enQuoteState == AT_PARSER_enEndQuote)
            {
                enQuoteState = AT_PARSER_enBeginQuote;
            }
            else if( enQuoteState == AT_PARSER_enBeginQuote )
            {
                enQuoteState = AT_PARSER_enEndQuote;
            }
        }
        inPtr++;
        length++;
    }

    /* take comma or end limiter into account for token itself */
    if (*inPtr == ',' || *inPtr == '\0')
    {
        length++;
    }

 /* Flag token exceeding maximum length                                  */
    if (length > PAT_MAX_TOKEN_LENGTH)
        length = 0;
    return(length);
}


/****************************************************************************/
/*  Method:     _ATParser_u8GetFromAsciiAndCheck                            */
/****************************************************************************/
Uint8Type _ATParser_u8GetFromAsciiAndCheck(Uint8Type *p_msg,
                                           Uint8Type max,
                                           Uint8Type set_default)
{
    Uint8Type u8ret   = 0;

    u8ret = atoi ((const char *)p_msg);
    if (u8ret > max )
    {
        u8ret = set_default;
    }
    return(u8ret);
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_GetDeviceAndValue                                  */
/**
    \brief      Extract device id and following value from given string

    \brief      this function extracts device id Px and one follow up value\n
                from given string

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      Pointer to device id response value to be filled
                Uint8Type *
                value within device id range
    \param      Pointer to value response to be filled
                Uint32Type *
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType  _ATParser_GetDeviceAndValue(Uint8Type  *p_msg,
                                            Uint8Type  *device_id,
                                            Uint32Type *value,
                                            Uint8Type  maxvalue,
                                            Uint32Type string_len)
{
    SuccessFailType sF = SUCCESS;
    Uint16Type count = 0;

    /* first check if at all other token exists */
    count = _ATParser_OtherToken(p_msg);
    if (count > 0 && count < string_len) /* there is another token plus value - process */
    {   /* check if pointer is equal to 'P' for device*/
        if (SUCCESS == _ATParser_ExtDeviceId( &p_msg,device_id))
        {
            /* now read out value */
            *value = atoi((const char *)p_msg);
            if (*value > maxvalue)
            {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,
                        "Value %d out of range 0..%d, set to 0",*value,maxvalue);
#endif
                DEBUG_VALUE2(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"Value %d out of range 0..%d, set to 0",*value,maxvalue);

                *value = 0;
            }
        }
        else
        {
            sF = FAIL;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"AT_PARSER invalid device id");
#endif
            DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "AT_PARSER invalid device id");
        }
    }
    else
    {
        sF = FAIL;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,
                "_ATParser_GetDeviceAndValue no token ");
#endif
        DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "AT_PARSER invalid device id");

    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPlpdInfo                                        */
/**
    \brief      Extract handler for PLPD info elements

    \brief      this function extracts from given string the elements of\n
                PLPD and also do the necessary range and value checks if\n
                necessary

    \param      pointer to message for data elements to be filled in
                ATRspPlpdInfoType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPlpdInfo(ATRspPlpdInfoType * p_info,
                                      Uint8Type         * p_msg,
                                      Uint32Type          string_len)
{
    SuccessFailType sF           = FAIL;
    Uint16Type      count        = 0;
    Uint8Type *     p_tmp        = p_msg;
    Uint8Type *     p_tmp_helper = p_msg;
    Uint8Type       avail_param  = 0;
    /* check if amount of required parameters exists */
    if (SUCCESS == _ATParser_CheckNumberParam(p_msg,
                                              AT_PARSER_NUM_PLPD_INFO_PARAM,
                                              &avail_param,
                                              string_len)  )
    {
        /* first element shall be device id with format Px where x is in range 1..5 */
        /* first search next comma in the string */
        count = _ATParser_OtherToken(p_msg);
        if (count > 0 && count < string_len) /* found -> extract device */
        {   /* */
            if (SUCCESS == _ATParser_ExtDeviceId(&p_tmp, &(p_info->device_id)) &&
                SUCCESS == _ATParser_ExtName(&p_tmp, (AT_DeviceNameString *)(p_info->device_name),
                                             &(p_info->device_name_len),
                                             AT_DEVICE_NAME_MAX_LEN)       &&
                SUCCESS == _ATParser_ExtBTAddress(&p_tmp, (AT_BT_Address *)(p_info->bt_addr))&&
                SUCCESS == _ATParser_ExtCoD(&p_tmp,&(p_info->bt_ClassOfDevice))&&
                SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->support_dial))&&
                SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->support_pb_access))&&
                SUCCESS == _ATParser_ExtBool(&p_tmp,&(p_info->is_connected))&&
                SUCCESS == _ATParser_ExtBool(&p_tmp,&(p_info->is_active))
                )
            {   /* all params filled -> return success */
                sF = SUCCESS;
            }
            else
            {
                /* invalid return code from functions */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPlpdMsg SetPrtToStart SpezHandl");
#endif
                DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPlpdMsg SetPrtToStart SpezHandl");

                p_tmp = p_tmp_helper;
            }
        }
        /* else return default failure */
    }
    if (SUCCESS != sF)
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPlpdMsg spezPlpdHandl");
#endif
        DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPlpdMsg spezPlpdHandl");


        if ((SUCCESS == _ATParser_ExtDeviceId(&p_tmp, &(p_info->device_id))) &&
           (SUCCESS == _ATParser_ExtLastBool(&p_tmp,&(p_info->is_active))) &&
           (SUCCESS == _ATParser_ExtLastBool(&p_tmp,&(p_info->is_connected))) &&
           (SUCCESS == _ATParser_ExtLastSupport(&p_tmp,&(p_info->support_pb_access)))&&
           (SUCCESS == _ATParser_ExtLastSupport(&p_tmp,&(p_info->support_dial)))&&
           (SUCCESS == _ATParser_ExtLastCoD(&p_tmp,&(p_info->bt_ClassOfDevice)))&&
           (SUCCESS == _ATParser_ExtLastBTAddress(&p_tmp, (AT_BT_Address *)(p_info->bt_addr))) &&
           (SUCCESS == _ATParser_ExtLastName(&p_tmp, (AT_DeviceNameString *)(p_info->device_name),
                                             &(p_info->device_name_len),AT_DEVICE_NAME_MAX_LEN)))
        {   /* all params filled -> return success */
            sF = SUCCESS;
        }
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPlccInfo                                        */
/**
    \brief      Extract handler for PLCC info elements

    \brief      this function extracts from given string the elements of\n
                PLCC and also do the necessary range and value checks if\n
                necessary

    \param      pointer to message for data elements to be filled in
                ATRspPlpdInfoType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPlccInfo(ATRspCallStatValType * p_info,
                                      Uint8Type            * p_msg,
                                      Uint32Type           string_len)
{
    SuccessFailType sF    = FAIL;
    Uint8Type  onlyCallerId = 0;
    Uint8Type *     p_tmp = p_msg;
    Uint8Type * save_p_tmp  = p_msg;
    Uint8Type       avail_param = 0;

    /* first check amount of available parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_NUM_PLCC_PARAM,
                               &avail_param, string_len);
    if (AT_PARSER_MIN_NUM_PLCC_PARAM <= avail_param  &&
        AT_PARSER_MAX_NUM_PLCC_PARAM >= avail_param)
    {   /* first extrace index and state which are mandatory */
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->index),
                         AT_PARSER_MIN_PLCC_PARAM,AT_PARSER_MAX_PLCC_PARAM) &&
            SUCCESS == _ATParser_ExtCallState(&p_tmp,&(p_info->state)) )
        {   /* now go on processing the other parameters if required */
            avail_param -= 2;
            p_info->id_avail        = AT_FALSE;
            p_info->type_avail      = AT_FALSE;
            p_info->ext_avail       = AT_FALSE;

            sF = SUCCESS; /* treat anyway as success as mandatory elements filled */

            if (0 < avail_param) /* call direction exists */
            {   /* now read call direction */
                Uint8Type tmpVal = 0;
                if (SUCCESS == _ATParser_sFExtu8RangeValue( &p_tmp,
                                                            &tmpVal,
                                                            (Uint8Type)AT_CD_OUTGOING,
                                                            (Uint8Type)AT_CD_UNKNOWN) )
                {
                    if(AT_CD_UNKNOWN == (ATCallDirTypeEnum)tmpVal)
                    {   // if call direction will be signed as unknown (HFP1.0) IOP issue,
                        // we will mapped this as incoming call 18.10.2010 clearified with LK
                        p_info->call_dir_type   = (ATCallDirTypeEnum)AT_CD_INCOMING;
                    }
                    else
                    {
                        p_info->call_dir_type   = (ATCallDirTypeEnum)tmpVal;
                    }
                    avail_param--;
                }
                else
                {   /* an error occured, nevertheless send data so far
                       but stop processing further parameters */
                    avail_param = 0;
                    sF = FAIL;
                }
            }

            if (0 < avail_param) /* call conference exists */
            {   /* now read call conference */
                Uint8Type tmpVal = 0;
                if (SUCCESS == _ATParser_sFExtu8RangeValue( &p_tmp,
                                                            &tmpVal,
                                                            (Uint8Type)AT_CC_NOT_CONFERENCE,
                                                            (Uint8Type)AT_CC_CONFERENCE) )
                {
                    p_info->call_conf_type  = (ATCallConfTypeEnum)tmpVal;
                    avail_param--;
                }
                else
                {   /* an error occured, nevertheless send data so far
                       but stop processing further parameters */
                    avail_param = 0;
                    sF = FAIL;
                }
            }

            if (0 < avail_param) /* call mode exists */
            {   /* now read call mode */
                Uint8Type tmpVal = 0;
                if (SUCCESS == _ATParser_sFExtu8RangeValue( &p_tmp,
                                                            &tmpVal,
                                                            (Uint8Type)AT_CM_VOICE_CALL,
                                                            (Uint8Type)AT_CM_FAX_CALL) )
                {
                    p_info->call_mode_type  = (ATCallModeTypeEnum)tmpVal;
                    avail_param--;
                }
                else
                {   /* an error occured, nevertheless send data so far
                       but stop processing further parameters */
                    avail_param = 0;
                    sF = FAIL;
                }
            }

            if (0 < avail_param) /* optional caller id exists */
            {   /* now read caller id */
            DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo p_info->callerid_len = %d",p_info->caller_id_len); 
                if (SUCCESS == _ATParser_ExtCallerID(&p_tmp,
                                        (AT_CallerId*)(p_info->caller_id),
                                        &(p_info->caller_id_len),
                                        AT_CALLER_ID_STRING_MAX_LEN) )
                {
                    p_info->id_avail = AT_TRUE;
                    avail_param--;
                }
                else
                {   /* an error occured, nevertheless send data so far
                       but stop processing further parameters */
                    avail_param = 0;
                    sF = FAIL;
                }
            }
            if (0 < avail_param)  /* optional PN type exists*/
            {   /* now read type */
                if (SUCCESS == _ATParser_sFExtPNTypeSpecialValue(&p_tmp,
                        &(p_info->call_type),AT_PNT_GENERAL,AT_PNT_OTHER))
                {
                    p_info->type_avail = AT_TRUE;
                    avail_param--;
                }
                else
                {   /* an error occured, nevertheless send data so far
                       but stop processing further parameters */
                    avail_param = 0;
                    sF = FAIL;
                }
            }
            if (0 < avail_param)  /* optional caller id extended exists */
            {   /* now read caller id extended */
            DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo p_info->callerid_len = %d",p_info->caller_id_len); 
                if (SUCCESS == _ATParser_ExtCallerID(&p_tmp,(AT_CallerId*)(p_info->call_id_ext),
                                                &(p_info->caller_id_ext_len),
                                                AT_CALLER_ID_STRING_MAX_LEN) )
                {
                    p_info->ext_avail = AT_TRUE;
                    avail_param--;
                }
                else
                {   /* an error occured, nevertheless send data so far
                       but stop processing further parameters */
                    avail_param = 0;
                    sF = FAIL;
                }
            }
        }
    }
    if (sF != SUCCESS)
    {
        Uint8Type tmpVal = 0;
        p_tmp = save_p_tmp;
        /* invalid range !!! */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_TRACE,UHV_AT_PARSER,"ExtPlccInfo %d, SpezPlccHandl started",avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPlccInfo %d, SpezPlccHandl started",avail_param);

        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->index),
                         AT_PARSER_MIN_PLCC_PARAM,AT_PARSER_MAX_PLCC_PARAM) &&
            SUCCESS == _ATParser_ExtCallState(&p_tmp,&(p_info->state)) )
        {   /* now go on processing the other parameters if required */
            p_info->id_avail        = AT_FALSE;
            p_info->type_avail      = AT_FALSE;
            p_info->ext_avail       = AT_FALSE;
            /* now read call direction */
            tmpVal = 0;
            if (SUCCESS == _ATParser_sFExtu8RangeValue( &p_tmp,
                                                        &tmpVal,
                                                        (Uint8Type)AT_CD_OUTGOING,
                                                        (Uint8Type)AT_CD_INCOMING) )
            {
                p_info->call_dir_type   = (ATCallDirTypeEnum)tmpVal;
                /* now read call conference */
                tmpVal = 0;
                if (SUCCESS == _ATParser_sFExtu8RangeValue( &p_tmp,
                                                            &tmpVal,
                                                            (Uint8Type)AT_CC_NOT_CONFERENCE,
                                                            (Uint8Type)AT_CC_CONFERENCE) )
                {
                    p_info->call_conf_type  = (ATCallConfTypeEnum)tmpVal;
                    /* now read call mode */
                    tmpVal = 0;
                    if (SUCCESS == _ATParser_sFExtu8RangeValue( &p_tmp,
                                                                &tmpVal,
                                                                (Uint8Type)AT_CM_VOICE_CALL,
                                                                (Uint8Type)AT_CM_FAX_CALL) )
                    {
                        p_info->call_mode_type  = (ATCallModeTypeEnum)tmpVal;
                        /* check variable parameters */
                        save_p_tmp = p_tmp;
                        /* set pointer at last position */
                        p_tmp = p_tmp + strlen((const char *)p_tmp) - 1;

                        if (AT_ASCII_QUOTE == *p_tmp)
                        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                            (void)DEBUG_Printf(UHV_TRACE,UHV_AT_PARSER,"ExtPlccInfo SpezPlccHandl QUOTE");
#endif
                            DEBUG_TEXT(AT_PARSER_SWI_TRC, MOD_AT_PARSER,"ExtPlccInfo SpezPlccHandl QUOTE");

                            /* set the pointer to next to last */
                            p_tmp = p_tmp - 1;
                            /* start searching of next quoted at the end of the string */
                            while(*p_tmp != AT_ASCII_QUOTE)
                            {
                                p_tmp = p_tmp - 1;
                            }
                            /* copy caller id extended (phone number) */
                            DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo p_info->callerid_len = %d",p_info->caller_id_len); 
                            if ( SUCCESS == _ATParser_ExtLastCallerID(&p_tmp,(AT_CallerId*)(p_info->call_id_ext),
                                                                &(p_info->caller_id_ext_len), AT_CALLER_ID_STRING_MAX_LEN) )
                            {
                                /* check if the number valid */
                                if ( BT_FALSE == APP_PHONE_bIsValidPhoneNumber((Uint8Type *)&(p_info->call_id_ext), p_info->caller_id_ext_len))
                                {
                                    /* not supported signs in string -> only CallerId parameter */
                                    onlyCallerId = 1;
                                }
                                else
                                {
                                    /* valid number */
                                    p_info->ext_avail = AT_TRUE;
                                    p_tmp = p_tmp - 2;
                                    /* set pointer direct to PN type */
                                    if (SUCCESS == _ATParser_sFExtLastPNTypeValue(&p_tmp,
                                            &(p_info->call_type),AT_PNT_GENERAL,AT_PNT_OTHER))
                                    {
                                        p_info->type_avail = AT_TRUE;
                                        /* set pointer to the comma */
                                        p_tmp = p_tmp - 1;
                                        /* replace comma with zero */
                                        *p_tmp = '\0';
                                        onlyCallerId = 1;
    }
                                    else
                                    {
                                        /* no vailid PN type - only CallerId parameter */
                                        p_info->ext_avail = AT_FALSE;
                                        p_info->type_avail = AT_FALSE;
                                        onlyCallerId = 1;
                                    }

                                }
                            }
                            else
                            {
                                onlyCallerId = 1;
                            }
                        }
                        else if (AT_PNT_GENERAL <= *p_tmp && AT_PNT_OTHER >= *p_tmp)
                        {
                            /* valid PNType */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                            (void)DEBUG_Printf(UHV_TRACE,UHV_AT_PARSER,"ExtPlccInfo SpezPlccHandl valid %d", *p_tmp);
#endif
                            DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER,"ExtPlccInfo SpezPlccHandl valid %d", *p_tmp);


                            if (SUCCESS == _ATParser_sFExtLastPNTypeValue(&p_tmp,
                                    &(p_info->call_type),AT_PNT_GENERAL,AT_PNT_OTHER))
                            {
                                p_info->type_avail = AT_TRUE;
                                /* set pointer to the comma */
                                p_tmp = p_tmp - 1;
                                /* replace comma with zero */
                                *p_tmp = '\0';
                                onlyCallerId = 1;

                            }
                            else
                            {
                                /* invalid PNType -  only CallerId parameter */
                                onlyCallerId = 1;
                            }
                        }
                        else
                        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                            (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,"ExtPlccInfo SpezPlccHandl invalid %d", *p_tmp);
#endif
                            DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPlccInfo SpezPlccHandl invalid %d", *p_tmp);

                            /* invalid parameter */
                        }
                    }

                    if (onlyCallerId != 0)
                    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                        (void)DEBUG_Printf(UHV_TRACE,UHV_AT_PARSER,"ExtPlccInfo SpezPlccHandl cpy CallerId");
#endif
                        DEBUG_TEXT(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo SpezPlccHandl cpy CallerId");
                        DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo p_info->callerid_len = %d",p_info->caller_id_len); 
                        if (SUCCESS == _ATParser_ExtLastCallerID(&save_p_tmp,(AT_CallerId*)(p_info->caller_id),
                                        &(p_info->caller_id_len), AT_CALLER_ID_STRING_MAX_LEN) )
                        {
                            p_info->id_avail = AT_TRUE;
                            sF = SUCCESS;
                        }
                    }
                }
            }
        }
        else
        {   /* an error occured, nevertheless send data so far
               but stop processing further parameters */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,"ExtPlccInfo stop processing further parameters");
#endif
            DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "ExtPlccInfo stop processing further parameters");
        }
    }
    if((sF == SUCCESS)&&(p_info->caller_id_len == AT_CALLER_ID_STRING_MAX_LEN)&&((*(p_info->caller_id + AT_CALLER_ID_STRING_MAX_LEN-1) & 0xC0) == 0xC0))
    {
        p_info->caller_id_len= p_info->caller_id_len-1;
        //parrot submits 50 bytes whithout check for UTF8-coding (it can happen that a sign is cut in the middle of the coding for example german ü)
        DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo p_info->callerid_len shortened now lenght %d",p_info->caller_id_len); 
    }
    DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo p_info->callerid_len = %d",p_info->caller_id_len); 
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPwng                                        */
/**
    \brief      Extract handler for PWNG warning elements

    \brief      this function extracts from given string the elements of\n
                PWNG warning and also do the necessary range and value checks\n


    \param      pointer to message for data elements to be filled in
                ATRspWarningType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPwng(ATRspWarningType * p_info,
                                  Uint8Type         * p_msg,
                                  Uint32Type         string_len)
{
    SuccessFailType sF    = FAIL;
    Uint8Type *     p_tmp = p_msg;
    Uint8Type       avail_param = 0;
    /* first check amount of available parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_NUM_PWNG_PARAM,
                               &avail_param, string_len);
    if (AT_PARSER_MIN_NUM_PWNG_PARAM <= avail_param  &&
        AT_PARSER_MAX_NUM_PWNG_PARAM >= avail_param)
    {   /* first extract warning code which is mandatory */
        Uint8Type tmp_value = 0;
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(tmp_value),1,21))
        {
            p_info->code = (ATWarningCodeEnum)tmp_value;
            /* now check if device ID is also present, if so process */
            sF = SUCCESS; /* treat anyway as success as mandatory elements filled */
            if (AT_PARSER_MAX_NUM_PWNG_PARAM == avail_param)
            {   /* now read device id */
                if (SUCCESS == _ATParser_ExtDeviceId(&p_tmp,&(p_info->device_id)))
                {
                    p_info->device_id_avail = AT_TRUE;
                 }
                else
                {   /* an error occured, nevertheless send data so far */
                    p_info->device_id_avail = AT_FALSE;
                }
            }
        }
    }
    else
    {   /* invalid range !!! */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPwng ooR %d",avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtPwng ooR %d",avail_param);
    }
    return sF;
}
/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPindVal                                         */
/**
    \brief      Extract handler for PIND value elements

    \brief      this function extracts from given string the elements of\n
                PIND values and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPhoneStatusIndValueType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPindVal(ATRspPhoneStatusIndValueType * p_info,
                                     Uint8Type         * p_msg,
                                     Uint32Type         string_len)
{
    SuccessFailType sF          = FAIL;
    Uint16Type      count       = 0;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* check if amount of required parameters exists */
    if (SUCCESS == _ATParser_CheckNumberParam(p_msg,
                                              AT_PARSER_NUM_PIND_PARAM,
                                              &avail_param,
                                              string_len)  )
    {
        /* first search next comma in the string */
        count = _ATParser_OtherToken(p_msg);
        if (count > 0 && count < string_len) /* found -> extract device */
        {   /* */
            if ( SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->bt_conn),
                            (Uint8Type)AT_FALSE,(Uint8Type)AT_TRUE)           &&
                 SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->service),
                            (Uint8Type)AT_FALSE,(Uint8Type)AT_TRUE)           &&
                 SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->roaming),
                            (Uint8Type)AT_FALSE,(Uint8Type)AT_TRUE)           &&
                 SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                             &(p_info->signal_level),0,5)                     &&
                 SUCCESS == _ATParser_ExtName(&p_tmp,
                             (AT_DeviceNameString*)(p_info->operator_name),
                             &(p_info->operator_name_len),
                             AT_OPERATOR_NAME_PIND_MAX_LEN)                   &&
                 SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                             &(p_info->battery_level),0,5)                    &&
                 SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                             &(p_info->sms_receive),0,255)                    &&
                 SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->voice_mail),
                         (Uint8Type)AT_FALSE,(Uint8Type)AT_TRUE)
                )
            {   /* all params filled -> return success */
                sF = SUCCESS;
            }
            /* else return default failure */
        }
        /* else return default failure */
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPindSup                                         */
/**
    \brief      Extract handler for PIND support elements

    \brief      this function extracts from given string the elements of\n
                PIND support and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPhoneStatusIndSupportType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPindSup(ATRspPhoneStatusIndSupportType * p_info,
                                     Uint8Type         * p_msg,
                                     Uint32Type         string_len)
{
    SuccessFailType sF          = FAIL;
    Uint16Type      count       = 0;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* check if amount of required parameters exists */
    if (SUCCESS == _ATParser_CheckNumberParam(p_msg,
                                              AT_PARSER_NUM_PIND_PARAM,
                                              &avail_param,
                                              string_len)  )
    {
        /* first search next comma in the string */
        count = _ATParser_OtherToken(p_msg);
        if (count > 0 && count < string_len) /* found -> extract device */
        {   /* */
            if ( SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->bt_conn)) &&
                 SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->service)) &&
                 SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->roaming)) &&
                 SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->signal_level)) &&
                 SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->operator_name)) &&
                 SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->battery_level)) &&
                 SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->sms_receive)) &&
                 SUCCESS == _ATParser_ExtSupport(&p_tmp,&(p_info->voice_mail))
                )
            {   /* all params filled -> return success */
                sF = SUCCESS;
            }
            /* else return default failure */
        }
        /* else return default failure */
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPievInfo                                         */
/**
    \brief      Extract handler for PIEV elements

    \brief      this function extracts from given string the elements of\n
                PIEV and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPhoneStatusIndEventType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPievInfo(ATRspPhoneStatusIndEventType * p_info,
                                      Uint8Type                    * p_msg,
                                      Uint32Type                     string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_str       = 0;
    Uint8Type *     p_tmp       = p_msg;
    Uint8Type       index = 0;
    /* check if amount of required parameters exists */
    if (SUCCESS == _ATParser_CheckNumberParam(p_msg,
                                              AT_PARSER_NUM_PIEV_PARAM,
                                              &avail_param,
                                              string_len)  )
    {
        /* fetch the index from the string */
        if ( SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                &index, (Uint8Type)AT_INDI_BT_CONN_STAT, (Uint8Type)AT_INDI_VOICE_MAIL) )
        {   /* go on with processing */
            p_info->index = (ATIndicatorIndexEnum)index;
            /* go on with next parameter, first check if param map to enum */
            p_str = (Uint8Type*)strchr((const char *)p_tmp,AT_ASCII_QUOTE);
            if (NULL != p_str)
            {   /* this is response with value string, cross check that index is
                   equal to AT_INDI_OPERATOR_NAME, otherwise this is mal formated
                   string  */
                if (AT_INDI_OPERATOR_NAME == p_info->index)
                {
                    if (SUCCESS == _ATParser_ExtName(&p_tmp,
                            (AT_DeviceNameString*)(p_info->value_str),
                            &(p_info->len),AT_OPERATOR_NAME_STRING_MAX_LEN))
                    {
                        sF = SUCCESS;
                    }
                }
                else
                {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                    (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"PIEV mismatch %d",
                            p_info->index);
#endif
                    DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "PIEV mismatch %d",
                            p_info->index);

                }
            }
            else /* response with value only */
            {   /* checks are related to received index */
                switch (p_info->index)
                {
                    case AT_INDI_BT_CONN_STAT:
                    case AT_INDI_NW_SERV_AVAIL:
                    case AT_INDI_ROAMING:
                    case AT_INDI_VOICE_MAIL:
                    {
                        ATBooleanEnum indicator = AT_FALSE;
                        if (SUCCESS == _ATParser_ExtBool(&p_tmp,&indicator))
                        {
                            p_info->value = (Uint8Type)indicator;
							sF = SUCCESS;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
							(void)DEBUG_Printf(   UHV_DEBUG,UHV_AT_PARSER,
                                        "Got valid PIEV index %d with value %d",
                                        p_info->index,
                                        p_info->value);
#endif
                           DEBUG_VALUE2(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "Got valid PIEV index %d with value %d",
                                        p_info->index,
                                        p_info->value);

                        }
                        break;
                    }
                    case AT_INDI_NW_SIGSTRENGHT:
                    case AT_INDI_BATT_LEVEL:
                    {
                        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                                &(p_info->value),0,5))
                        {
                            sF = SUCCESS;
                        }
                        break;
                    }
                    case AT_INDI_NEW_SMS_REC:
                    {
                        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                                &(p_info->value),0,255))
                        {
                            sF = SUCCESS;
                        }
                        break;
                    }
                    case AT_INDI_OPERATOR_NAME:
                    {

                        break;
                    }
                    default:
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"PIEV inco index %d",
                                p_info->index);
#endif
                        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "PIEV inco index %d",
                                p_info->index);
                        break;
                }
            }
        }
         /* else return default failure */
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sfExtPsprInfo                                         */
/**
    \brief      Extract handler for PSPR elements

    \brief      this function extracts from given string the elements of\n
                PSSP and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspSSPRequestType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sfExtPsprInfo(ATRspSSPRequestType * pstPspr,
                                        Uint8Type           * pu8Msg,
                                        Uint32Type          u32StringLen)
{
    SuccessFailType sfReturnValue   = FAIL;
    Uint8Type       u8AvailParam     = 0;
    Uint8Type *     pu8Tmp          = pu8Msg;
    Uint8Type       u8Value         = 0;

    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(pu8Msg, AT_PARSER_MIN_NUM_PPBR_PARAM,
            &u8AvailParam, u32StringLen);

    if (AT_PARSER_MIN_NUM_PSPR_PARAM <= u8AvailParam &&
        AT_PARSER_MAX_NUM_PSPR_PARAM >= u8AvailParam )
    {
        /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType( &pu8Tmp,
                                                         &(pstPspr->deviceType),
                                                         &(pstPspr->deviceId)) )
        {
            if( SUCCESS == _ATParser_sFExtu8RangeValue( &pu8Tmp,
                                                        &u8Value,
                                                        (Uint8Type)AT_SSP_DISABLE,
                                                        (Uint8Type)AT_SSP_REQUEST))
            {
                pstPspr->mode = (ATSSPModeEnum)u8Value;
                if( SUCCESS == _ATParser_sFExtu8RangeValue( &pu8Tmp,
                                                            &u8Value,
                                                            (Uint8Type)AT_PARAM_SSP_IOCAP_DISPLAY,
                                                            (Uint8Type)AT_PARAM_SSP_IOCAP_INVALID))
                {
                    pstPspr->capability = (ATSSPIoCapParameterEnum)u8Value;
                    if(AT_PARSER_MIN_NUM_PSPR_PARAM == u8AvailParam )
                    {
                        pstPspr->passAvail = AT_FALSE;
                        sfReturnValue = SUCCESS;
                    }
                    else
                    {
                        if (SUCCESS == _ATParser_ExtSSPPin( &pu8Tmp, &(pstPspr->passphrase) ))
                        {
                            pstPspr->passAvail = AT_TRUE;
                            sfReturnValue = SUCCESS;
                        }
                        else
                        {
                            DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value SSP code from PSPR response!");
                        }
                    }
                }
                else
                {
                    DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value IOCap from PSPR response!");
                }
            }
            else
            {
                DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value SSPMode from PSPR response!");
            }
        }
        else
        {
            DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value RemoteId from PSPR response!");
        }
    }
    else
    {
        /* invalid number of params */
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPspr invalid number of params %d",
               u8AvailParam);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sfReturnValue;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParserExtPsspInfo                                         */
/**
    \brief      Extract handler for PSSP elements

    \brief      this function extracts from given string the elements of\n
                PSSP and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPhoneStatusIndEventType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sfExtPsspInfo(ATRspSSPSetupType * pstPssp,
                                      Uint8Type         * pu8Msg,
                                      Uint32Type        u32StringLen)
{
    SuccessFailType sfReturnValue   = FAIL;
    Uint8Type       u8AvailParam     = 0;
    Uint8Type *     pu8Tmp          = pu8Msg;
    Uint8Type       u8Value         = 0;

    /* check if amount of required parameters exists */
    if (SUCCESS == _ATParser_CheckNumberParam(pu8Msg,
            AT_PARSER_NUM_PSSP_PARAM,
            &u8AvailParam,
            u32StringLen)){
        if ( SUCCESS == _ATParser_sFExtu8RangeValue(&pu8Tmp,
                &u8Value, (Uint8Type)AT_SSP_DISABLE, (Uint8Type)AT_SSP_ENABLE) ){
            pstPssp->mode = (ATSSPModeEnum)u8Value;

            if (SUCCESS == _ATParser_sFExtu8RangeValue(&pu8Tmp,
                    &u8Value, (Uint8Type)AT_PARAM_SSP_IOCAP_DISPLAY, (Uint8Type)AT_PARAM_SSP_IOCAP_NOINNOOUT) ){
                pstPssp->IOCap = (ATSSPIoCapParameterEnum)u8Value;

                if (SUCCESS == _ATParser_sFExtu8RangeValue(&pu8Tmp,
                        &u8Value, (Uint8Type)AT_SSP_DISABLE, (Uint8Type)AT_SSP_ENABLE) )
                {
                    pstPssp->nextMode = (ATSSPModeEnum)u8Value;

                    if (SUCCESS == _ATParser_sFExtu8RangeValue(&pu8Tmp,
                            &u8Value, (Uint8Type)AT_PARAM_SSP_IOCAP_DISPLAY, (Uint8Type)AT_PARAM_SSP_IOCAP_NOINNOOUT) )
                    {
                        pstPssp->nextIOCap = (ATSSPIoCapParameterEnum)u8Value;

                        DEBUG_STATE2(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "Received psp with mode: $ATSSPModeEnum$ = %d, IOcap: $ATSSPIoCapParameterEnum$ = %d",pstPssp->mode,pstPssp->IOCap);
                        DEBUG_STATE2(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "Received psp with next mode: $ATSSPModeEnum$ = %d, next IOcap: $ATSSPIoCapParameterEnum$ = %d",pstPssp->nextMode,pstPssp->nextIOCap);
                        sfReturnValue = SUCCESS;
                    }
                    else {
                        DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value NextIOCap from PSSP response!");
                    }
                }
                else {
                    DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value NextMode from PSSP response!");
                }
            }
            else {
                DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value IOCap from PSSP response!");
            }
        }
        else {
            DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value Mode from PSSP response!");
        }
    }
    return sfReturnValue;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParserExtPbsfInfo                                         */
/**
    \brief      Extract handler for PSSP elements

    \brief      this function extracts from given string the elements of\n
                PSSP and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPhoneStatusIndEventType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sfExtPbsfInfo(ATResBtFeatureSupportType * pstPbsf,
                                      Uint8Type         * pu8Msg,
                                      Uint32Type        u32StringLen)
{
    SuccessFailType sfReturnValue   = FAIL;
    Uint8Type       u8AvailParam     = 0;
    Uint8Type *     pu8Tmp          = pu8Msg;
    Uint8Type       u8Value         = 0;

    /* check if amount of required parameters exists */
    if (SUCCESS == _ATParser_CheckNumberParam(pu8Tmp, AT_PARSER_MIN_PBSF_PARAM,
                        &u8AvailParam, u32StringLen))
    {
        if ( SUCCESS == _ATParser_ExtPairingDeviceIdType(&pu8Tmp,
                &(pstPbsf->deviceType),&(pstPbsf->device_id)) )
        {
            if (SUCCESS == _ATParser_sFExtu8RangeValue(&pu8Tmp,
                    &u8Value, (Uint8Type)AT_SSP_FEATURE,(Uint8Type)AT_SSP_FEATURE) ){
                pstPbsf->feature = (ATBTFeatureEnum)u8Value;

                if (SUCCESS == _ATParser_sFExtu8RangeValue(&pu8Tmp,
                        &u8Value, (Uint8Type)AT_BT_FEAT_UNSUPPORTED, (Uint8Type)AT_BT_FEAT_UNKNOWN) )
                {
                    pstPbsf->result = (ATBTFeatureSupportResultEnum)u8Value;
                    sfReturnValue = SUCCESS;
                }
                else {
                    DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value result from PBSF response!");
                }
            }
            else {
                DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed value feature from PBSF response!");
            }
        }
        else {
            DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "Not successfull parsed device from PBSF response!");
        }
    }
    return sfReturnValue;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPbcd                                            */
/**
    \brief      Extract handler for PBCD message elements

    \brief      this function extracts from given string the elements of\n
                PGMT message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResGetMetadataType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPbcd(ATRspClassOfDeviceRequestType   * p_info,
                                  Uint8Type                       * p_msg,
                                  Uint32Type                        string_len)
{
    SuccessFailType  sF                  = FAIL;
    Uint8Type        avail_param         = 0;
    Uint8Type *      p_tmp               = p_msg;
    ATDeviceTypeEnum device_type_tmp     = AT_DEVICE_BLUETOOTH;

    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam( p_msg,AT_PARSER_MIN_NUM_PBCD_PARAM,&avail_param,string_len);

    if (AT_PARSER_MAX_NUM_PBCD_PARAM >= avail_param &&
        AT_PARSER_PBCD_PARAM_NO_OPT  <= avail_param)
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtDeviceIdType(&p_tmp,&device_type_tmp,&(p_info->device_id)) &&
            AT_DEVICE_BLUETOOTH == device_type_tmp &&
            SUCCESS == _ATParser_ExtCoD(&p_tmp,&(p_info->bt_ClassOfDevice)) )
        {
            sF = SUCCESS;
            /* now check for optional parameters */
            if (AT_PARSER_MAX_NUM_PBCD_PARAM == avail_param)
            {
                if (SUCCESS == _ATParser_sFExtCodProfileValue(
                        &p_tmp,&(p_info->profile),
                        AT_CODPROF_HEADSET,
                        AT_CODPROF_HANDSFREE_1_50))
                {
                    p_info->profile_avail = AT_TRUE;
                }
                else
                {
                    p_info->profile_avail = AT_FALSE;
                }
            }
            else
            {
                p_info->profile_avail = AT_FALSE;
                p_info->profile = AT_CODPROF_NONE;
            }
        }
        else
        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPbcd extract failed!");
#endif
            DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPbcd extract failed!");
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPbcd inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPbcd inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/* Function    : _ATParser_ExtPmrdNum                                         */
/**
    \brief      Extract handler for PMRD number elements

    \brief      this function extracts from given string the elements of\n
                PMRD numbers and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspSmsReceiveNumType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPmrdNum(ATRspSmsReceiveNumType * p_info,
                                     Uint8Type              * p_msg,
                                     Uint32Type               string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* check if amount of required parameters exists */
    if (SUCCESS == _ATParser_CheckNumberParam(p_msg,
                                              AT_PARSER_NUM_PMRDNUM_PARAM,
                                              &avail_param,
                                              string_len)  )
    {
        if ( SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,&(p_info->unread),
                                              0,AT_MAX_NUM_SMS_FOLDER_SIZE) &&
             SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,&(p_info->read),
                                              0,AT_MAX_NUM_SMS_FOLDER_SIZE) &&
             SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,&(p_info->unsent),
                                              0,AT_MAX_NUM_SMS_FOLDER_SIZE) &&
             SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,&(p_info->sent),
                                              0,AT_MAX_NUM_SMS_FOLDER_SIZE)
            )
        {   /* all params filled -> return success */
            sF = SUCCESS;
        }
        /* else return default failure */
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPmrdMsg                                         */
/**
    \brief      Extract handler for PMRD message elements

    \brief      this function extracts from given string the elements of\n
                PMRD message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspSmsReceiveMsgType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPmrdMsg(ATRspSmsReceiveMsgType * p_info,
                                     Uint8Type              * p_msg,
                                     Uint32Type               string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_PMRDMSG_PARAM,
                               &avail_param,string_len);

    if (AT_PARSER_MIN_PMRDMSG_PARAM <= avail_param )
    {   /*extract mandatory first 2 params */
        if (SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                                        (Uint8Type*)(p_info->origin),
                                        &(p_info->origin_len),AT_SMS_ORIGIN_MAX_LEN) &&
            SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                                        (Uint8Type*)(p_info->date),
                                        &(p_info->date_len),AT_SMS_DATE_MAX_LEN)  )
        {
            p_info->message_len = (Uint16Type)(string_len - (p_tmp - p_msg));

            if(p_info->message_len > AT_MAX_SMS_LEN_TOTAL)
            {
                p_info->message_len = AT_MAX_SMS_LEN_TOTAL;
            }

            (void)memcpy(p_info->message, p_tmp, p_info->message_len);

            sF = SUCCESS;

//            /* now check if min or max available as this impact order of params */
//            if (AT_PARSER_MAX_PMRDMSG_PARAM == avail_param)
//            {   /* next param is total length followed by message itself */
//                if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
//                                      &(p_info->totallength),0,AT_MAX_SMS_LEN_TOTAL) &&
//                    SUCCESS == _ATParser_sFExtUnquotedString(&p_tmp,
//                            (Uint8Type*)&(p_info->message),
//                            &(p_info->message_len),AT_MAX_SMS_LEN_TOTAL) )
//                {
//                    p_info->length_avail = AT_TRUE;
//                    sF = SUCCESS;
//                }
//            }
//            else
//            {   /* next param is message itself */
//                if (SUCCESS == _ATParser_sFExtUnquotedString(&p_tmp,
//                        (Uint8Type*)&(p_info->message),
//                        &(p_info->message_len),AT_MAX_SMS_LEN_TOTAL)  )
//                {
//                    p_info->length_avail = AT_FALSE;
//                    sF = SUCCESS;
//                }
//            }
        }
        /* else not needed as failure is handled by default set for sF */
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPmrdMsg inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPmrdMsg inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPmsdexMsg                                         */
/**
    \brief      Extract handler for PMSDEX message response element

    \brief      this function extracts from given string the elements of\n
                PMSDEX message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspSmsConcReceiveType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPmsdexMsg(ATRspSmsConcReceiveType * p_info,
                                       Uint8Type               * p_msg,
                                       Uint32Type              string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_SMS_CONC_PARAM,
                               &avail_param,string_len);

    if (AT_PARSER_SMS_CONC_PARAM <= avail_param )
    {   /*extract mandatory first param */
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                                        (Uint8Type*)(p_info->NbOfSentPdu),
                                        0,255) )
        {
            (void)memcpy(p_info->NbOfSentPdu, p_tmp, sizeof(Uint8Type));

            sF = SUCCESS;
        }
        /* else not needed as failure is handled by default set for sF */
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPmsdexMsg inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPmsdexMsg inconum %d",
                avail_param);

    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPpbp                                         */
/**
    \brief      Extract handler for PPBP message elements

    \brief      this function extracts from given string the elements of\n
                PPBP message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPBReadNumberType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPpbp(ATRspPBReadNumberType * p_info,
                                  Uint8Type             * p_msg,
                                  Uint32Type              string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PPBP_PARAM,&avail_param,string_len))
    {   /* now edtract the parameters */
        //SUSANNE: 20.10.2009 Number of max pb entries is neither fixed to 1000 nor 2500
        // it depends on the structure of the contacts and the memory free in Ck5050
        // so we will not limit the valid size here
        if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                        &(p_info->pb_idx),1,/*1000*//*AT_MAX_PHONEBOOK_ENTRIES*/ 0xFFFF) &&
            SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                        &(p_info->pn_idx),1,/*1000*//*AT_MAX_PHONEBOOK_ENTRIES*/ 0xFFFF) &&
            SUCCESS == _ATParser_sFExtPNTypeValue(&p_tmp,
                        &(p_info->pn_type),AT_PNT_GENERAL,AT_PNT_OTHER) &&
            SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                    (Uint8Type*)(p_info->phone_number),
                    &(p_info->phone_number_len),AT_MAX_PHONE_NUM_LEN) )
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPpbp inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPpbp inconum %d",
                avail_param);

    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPpbr                                         */
/**
    \brief      Extract handler for PPBR message elements

    \brief      this function extracts from given string the elements of\n
                PPBR message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPBReadEntryType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPpbr(ATRspPBReadEntryType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    Uint8Type *     p_tmp_save  = p_msg;
    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_NUM_PPBR_PARAM,
            &avail_param,string_len);
    if (AT_PARSER_MIN_NUM_PPBR_PARAM == avail_param)
    {   /* is is response to size */
        p_info->is_size_only = AT_TRUE;
        /* extract size */
        //SUSANNE: 20.10.2009 Number of max pb entries is neither fixed to 1000 nor 2500
        // it depends on the structure of the contacts and the memory free in Ck5050
        // so we will not limit the valid size here
        if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                                   &(p_info->pb_size),0,/*1000*//*AT_MAX_PHONEBOOK_ENTRIES*/ 0xFFFF) )
        {
            sF = SUCCESS;
        }
        // SUSANNE: 20.10.2009 This should now never be needed again due to the complete u16 Range allowed above
#if 0
        else
        {
            // Susanne: 29.07.2009, CK5050 sometimes sends pb size greater than AT_MAX_PHONEBOOK_ENTRIES,
            // which results in a not available phonebook
            // maybe we can fix the error with setting the size to the max. suported value
            // as p_info->pb_size is not set in case of an error, we try to get the value again, with max set to
            // the max possible value for UINT16, then set it to AT_MAX_PHONEBOOK_ENTRIES
            if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                                   &(p_info->pb_size),0,0xFFFF) )
            {   // got the value, now set it to AT_MAX_PHONEBOOK_ENTRIES
                DEBUG_Printf(UHV_ERROR, UHV_AT_PARSER, "ExtPpbr: pbSize %d, set to %d",
                             p_info->pb_size, AT_MAX_PHONEBOOK_ENTRIES);
                p_info->pb_size = AT_MAX_PHONEBOOK_ENTRIES;
                sF = SUCCESS;
            }
        }
#endif
    }
    else if (AT_PARSER_MAX_NUM_PPBR_PARAM >= avail_param &&
             AT_PARSER_PPBR_PARAM_NO_OPT  <= avail_param)
    {   /* response with entry */
        p_info->is_size_only = AT_FALSE;
        /* extract mandatory elements */
        //SUSANNE: 20.10.2009 Number of max pb entries is neither fixed to 1000 nor 2500
        // it depends on the structure of the contacts and the memory free in Ck5050
        // so we will not limit the valid size here
        if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                                   &(p_info->pb_idx),1,/*1000*//*AT_MAX_PHONEBOOK_ENTRIES*/ 0xFFFF) &&
            SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                          (Uint8Type*)(p_info->name),
                          &(p_info->name_len),AT_CALLER_ID_STRING_MAX_LEN) &&
            SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                          &(p_info->pn_size),0,255) &&
            SUCCESS == _ATParser_ExtBoolSpecial(&p_tmp,&(p_info->voice_tag_recorded) ) )
        {
            sF = SUCCESS;
            /* now check for optional parameters */
            if (AT_PARSER_MAX_NUM_PPBR_PARAM == avail_param)
            {   /* NOTE: only if both optional params are avail values are set */
                /* extract both params */
                if (SUCCESS == _ATParser_sFExtPNTypeSpecialValue(&p_tmp,
                                     &(p_info->pn_type),
                                     AT_PNT_GENERAL,AT_PNT_OTHER) &&
                    SUCCESS ==  _ATParser_sFExtQuotedString(&p_tmp,
                                     (Uint8Type*)(p_info->phone_number),
                                     &(p_info->phone_number_len),
                                     AT_MAX_PHONE_NUM_LEN)              )
                {
                    p_info->pn_type_numer_available = AT_TRUE;
                }
                else
                {   /* something failed in extraction */
                    p_info->pn_type_numer_available = AT_FALSE;
                    p_info->phone_number_len = 0;
                    sF = FAIL;
                }
            }
            else
            {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                (void)DEBUG_Printf(UHV_TRACE,UHV_AT_PARSER,"ExtLastPpbp SpezHandl %d", avail_param);
#endif
                DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtLastPpbp SpezHandl %d", avail_param);

                sF = FAIL;
            }
        }
        else
        {
            /* invalid number of params */
            sF = FAIL;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,
                    "ExtPpbp: params: %d idx: %d name: %s",
                    avail_param, p_info->pb_idx, p_info->name);
#endif
            DEBUG_VALUE2(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "ExtPpbp: invalid number of params %d idx %d",
                    avail_param, p_info->pb_idx);
        }
    }

    if (SUCCESS != sF) /* if (AT_PARSER_MAX_NUM_PPBR_PARAM < avail_param) */
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_TRACE,UHV_AT_PARSER,"ExtLastPpbp SpezHandl");
#endif
        DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtLastPpbp SpezHandl");

        p_tmp = p_tmp_save;
        p_info->is_size_only = AT_FALSE;
        if((SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,&(p_info->pb_idx),1,0xFFFF) &&
           (SUCCESS == _ATParser_sFExtQuotedLeftPosition(&p_tmp,
                                     &p_info))))
        {
            sF = SUCCESS;
        }
    }
    
    if((sF == SUCCESS)&&(p_info->name_len == AT_CALLER_ID_STRING_MAX_LEN)&&((*(p_info->name + AT_CALLER_ID_STRING_MAX_LEN-1) & 0xC0) == 0xC0))
    {
        p_info->name_len= p_info->name_len-1;
        //parrot submits 50 bytes whithout check for UTF8-coding (it can happen that a sign is cut in the middle of the coding for example german ü)
        DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER, "ExtPlccInfo p_info->name_len shortened now lenght %d",p_info->name_len); 
    }
    
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPbsc                                            */
/**
    \brief      Extract handler for PBSC message elements

    \brief      this function extracts from given string the elements of\n
                PBSC message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResBTServiceConnType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPbsc(ATResBTServiceConnType * p_info,
                                  Uint8Type              * p_msg,
                                  Uint32Type             string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PBSC_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtDeviceId(&p_tmp,&(p_info->device_id)) &&
            SUCCESS == _ATParser_sFExtATSupportedValue(&p_tmp,
                        &(p_info->res1),AT_NOT_SUPPORTED,AT_SUPPORTED) &&
            SUCCESS == _ATParser_sFExtATSupportedValue(&p_tmp,
                    &(p_info->res2),AT_NOT_SUPPORTED,AT_SUPPORTED) &&
            SUCCESS == _ATParser_sFExtATSupportedValue(&p_tmp,
                    &(p_info->res3),AT_NOT_SUPPORTED,AT_SUPPORTED) &&
            SUCCESS == _ATParser_sFExtATSupportedValue(&p_tmp,
                    &(p_info->res4),AT_NOT_SUPPORTED,AT_SUPPORTED) &&
            SUCCESS == _ATParser_sFExtATSupportedValue(&p_tmp,
                    &(p_info->res5),AT_NOT_SUPPORTED,AT_SUPPORTED) &&
            SUCCESS == _ATParser_sFExtATSupportedValue(&p_tmp,
                    &(p_info->res6),AT_NOT_SUPPORTED,AT_SUPPORTED) )
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPbsc inconum %d",
                avail_param);
#endif
       DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPbsc inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPgmt                                            */
/**
    \brief      Extract handler for PGMT message elements

    \brief      this function extracts from given string the elements of\n
                PGMT message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResGetMetadataType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPgmt(ATResGetMetadataType   * p_info,
                                  Uint8Type              * p_msg,
                                  Uint32Type             string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PGMT_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtDeviceId(&p_tmp,&(p_info->device_id)) &&
            SUCCESS == _ATParser_sFExtMetadataTypeValue(&p_tmp,
                        &(p_info->type),AT_META_NOT_SUPPORTED,
                        AT_META_AVRCP_1_4)  )
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPgmt inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPgmt inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPgmt                                            */
/**
    \brief      Extract handler for PBSN message elements

    \brief      this function extracts from given string the elements of\n
                PBSN message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResBTServiceNotType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPbsn(ATResBTServiceNotType  * p_info,
                                  Uint8Type              * p_msg,
                                  Uint32Type             string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PBSN_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtDeviceId(&p_tmp,&(p_info->device_id)) &&
            SUCCESS == _ATParser_sFExtBTServiceResultValue(&p_tmp,
                    &(p_info->result),AT_SERVRES_START_SUCCEED,
                    AT_SERVRES_STOP_FAILED) &&
             SUCCESS == _ATParser_sFExtBTServiceValue(&p_tmp,
                      &(p_info->service),AT_SERVICE_PHONE,AT_SERVICE_SPP))
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPbsn inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPbsn inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPamu                                            */
/**
    \brief      Extract handler for PAMU message elements

    \brief      this function extracts from given string the elements of\n
                PBSN message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResBTServiceNotType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPamu(ATResMetaDataUpdateType * p_info,
                                  Uint8Type              * p_msg,
                                  Uint32Type             string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PAMU_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtDeviceId(&p_tmp,&(p_info->device_id)) &&
            SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->fieldmask),
                                                   AT_PARSER_MIN_PAMU_FIELDMASK, AT_PARSER_MAX_PAMU_FIELDMASK))
        {
            sF = SUCCESS;
        }
        else
        {
            DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPamu:parameter out of range");
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPamu inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPamu inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPsbd                                            */
/**
    \brief      Extract handler for PSBD message elements

    \brief      this function extracts from given string the elements of\n
                PSBD message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspSelectBTDeviceType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPsbd(ATRspSelectBTDeviceType * p_info,
                                  Uint8Type               * p_msg,
                                  Uint32Type              string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PSBD_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtDeviceId(&p_tmp,&(p_info->id)) &&
            SUCCESS == _ATParser_sFExtBTServiceResultValue(&p_tmp,
                    (ATBTServiceResultEnum *)&(p_info->status), AT_SERVRES_START_SUCCEED,
                    AT_SERVRES_STOP_FAILED) )
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPsbd inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPsbd inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtDgps                                            */
/**
    \brief      Extract handler for DGPS message elements

    \brief      this function extracts from given string the elements of\n
                DGPS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspGetPlayerStatusType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtDgps(ATRspGetPlayerStatusType * p_info,
                                  Uint8Type                * p_msg,
                                  Uint32Type               string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if ( SUCCESS == _ATParser_CheckNumberParam(p_msg,AT_PARSER_NUM_DGPS_PARAM,
                                               &avail_param,string_len))
    {
        /* extract mandatory parameters */
        if (SUCCESS == _ATParser_ExtDeviceIdType(&p_tmp,
                &(p_info->device_type),&(p_info->device_id)) &&
            SUCCESS == _ATParser_sFExtDeviceStatusValue(&p_tmp,
                    &(p_info->status),AT_DGPS_STATUS_STOP,
                    AT_DGPS_STATUS_TRACK_CHANGED) &&
            SUCCESS == _ATParser_sFExtu32RangeValue(&p_tmp,
                  &(p_info->pos),0,0xFFFFFFFF)
           )
        {
            p_info->ext_info_available = AT_FALSE;
            sF = SUCCESS;
        }
    }
    else if( SUCCESS == _ATParser_CheckNumberParam(p_msg,AT_PARSER_NUM_DGPS_EXT_PARAM,
                                               &avail_param,string_len))
    {
        if (SUCCESS == _ATParser_ExtDeviceIdType(&p_tmp,
                &(p_info->device_type),&(p_info->device_id)) &&
            SUCCESS == _ATParser_sFExtDeviceStatusValue(&p_tmp,
                    &(p_info->status),AT_DGPS_STATUS_STOP,
                    AT_DGPS_STATUS_TRACK_CHANGED) &&
            SUCCESS == _ATParser_sFExtu32RangeValue(&p_tmp,
                  &(p_info->pos),0,0xFFFFFFFF) &&
              /* extract also extended parameters */
            SUCCESS == _ATParser_sFExtu32RangeValue(&p_tmp,
                                &(p_info->total_len),0,0xFFFFFFFF) &&
            SUCCESS == _ATParser_sFExtDeviceplayerCodedValue(&p_tmp,
                            &(p_info->codec),AT_DEVCODED_SBC,AT_DEVCODED_NONE) &&
            SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,(Uint8Type*)&(p_info->repeat_mode),(Uint8Type)AT_DEVREPEAT_DISABLE, (Uint8Type)AT_DEVREPEAT_ENABLE_CURR_GROUP_SONG) &&
            SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,(Uint8Type*)&(p_info->random_mode),(Uint8Type)AT_DEVRANDOM_DISABLE, (Uint8Type)AT_DEVRANDOM_UNKNOWN))
                {
                    p_info->ext_info_available = AT_TRUE;
                    sF = SUCCESS;
                }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtDgps inconum %d",
                avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtDgps inconum %d",
                avail_param);
    }

    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtDged                                            */
/**
    \brief      Extract handler for DGED message elements

    \brief      this function extracts from given string the elements of\n
                DGED message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResPlayerGetEntDataType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtDged(ATResPlayerGetEntDataType * p_info,
                                  Uint8Type                * p_msg,
                                  Uint32Type               string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_DGED_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtDeviceIdType(&p_tmp,&(p_info->device_type),
                         &(p_info->device_id)) &&
            SUCCESS == _ATParser_sFExtEntryFieldMaskValue(&p_tmp,
                    &(p_info->field_id),AT_ENTRY_MASK_FILEPATH,
                    AT_ENTRY_MASK_GENRE) )
        {
            /* TODO  check if last param is string or number for correct extract function */
            if (NULL == (Uint8Type*)strchr((const char *)p_msg,AT_ASCII_QUOTE))
            {   /* last one is only number */
                if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                        &(p_info->data_value),0,0xFFFF))
                {
                    p_info->data_value_avail = AT_TRUE;
                    p_info->data_string_len = 0;
                    sF = SUCCESS;
                }
            }
            else
            {   /* last paramter is quoted string */
                if (SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                        (Uint8Type*)(p_info->data_string),&(p_info->data_string_len),
                        AT_MDI_ENTRY_DATA_LEN))      // bugfix for RT #6137
                {
                    p_info->data_value_avail = AT_FALSE;
                    sF = SUCCESS;
                }
            }
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtDged inconum %d",
                avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtDged inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPasa                                            */
/**
    \brief      Extract handler for PASA message elements

    \brief      this function extracts from given string the elements of\n
                PASA message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResAudioSrcActType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPasa(ATResAudioSrcActType * p_info,
                                  Uint8Type            * p_msg,
                                  Uint32Type           string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PASA_PARAM,&avail_param,string_len))
    {   /* now extract the parameter */
        if (SUCCESS == _ATParser_sFExtASourceActStateValue(&p_tmp,
                    &(p_info->state),AT_AUDIO_STATE_NO_ACTIVE,
                    AT_AUDIO_STATE_SIVR_PROMPTS) )
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPasa inconum %d",
                avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPasa inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPcms                                            */
/**
    \brief      Extract handler for PCMS message elements

    \brief      this function extracts from given string the elements of\n
                PCMS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResBTAutoConnType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPcms(ATResBTAutoConnType * p_info,
                                  Uint8Type           * p_msg,
                                  Uint32Type          string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    if (SUCCESS ==  _ATParser_CheckNumberParam(
                     p_msg,AT_PARSER_NUM_PCMS_PARAM,&avail_param,string_len))
    {   /* now extract the parameter */
        if (SUCCESS == _ATParser_sFExtAutoConnStatusValue(&p_tmp,
                    &(p_info->status),AT_AUTOCONN_STOPPED,
                    AT_AUTOCONN_IN_PROGRESS) )
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPcms inconum %d",
                avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPcms inconum %d",
                avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtRscl                                            */
/**
    \brief      Extract handler for RSRF message elements

    \brief      this function extracts from given string the elements of\n
                RVRS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResSIVRSetSupLangType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtRscl(ATResSIVRSetSupLangType   * p_info,
                                  Uint8Type                 * p_msg,
                                  Uint32Type                  string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */

    /* first check if it is end */
    if (0 == strncmp((char *)p_msg, "END", string_len))
    {
        p_info->is_end = AT_TRUE;
        sF = SUCCESS;
    }
    else
    {
        p_info->is_end = AT_FALSE;
        if (SUCCESS ==  _ATParser_CheckNumberParam(
                p_msg,AT_PARSER_NUM_RSCL_PARAM,&avail_param,string_len))
        {
            if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->lang_index),
                    (Uint8Type)AT_RGSL_MIN_START_INDEX, (Uint8Type)AT_RGSL_MAX_STOP_INDEX) &&
                SUCCESS == _ATParser_sFExtSIVRLanguageValue(&p_tmp,
                        &(p_info->lang_id),
                        AT_SIVR_LANG_US_ENG,
                        (ATSIVRLanguagesEnum) (AT_SIRV_LANG_MAXNUM -1 ))  )
            {
                sF = SUCCESS;
            }
        }
        else
        {
            /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtRscl inconum %d",
                   avail_param);
#endif

            DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtRscl inconum %d",
                    avail_param);
        }
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtRsrf                                            */
/**
    \brief      Extract handler for RSRF message elements

    \brief      this function extracts from given string the elements of\n
                RVRS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResSIVRSetResFormatType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtRsrf(ATResSIVRSetResFormatType   * p_info,
                                  Uint8Type                   * p_msg,
                                  Uint32Type                    string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */

    if (SUCCESS ==  _ATParser_CheckNumberParam(
            p_msg,AT_PARSER_NUM_RSRF_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->max_result),
                              AT_RSRF_MIN_RESULT,AT_RSRF_MAX_RESULT) &&
            SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->rejection_level),
                              0x00, AT_RSRF_MAX_REJECT_LEV)       )
        {
           sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtRsrf inconum %d",
               avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtRsrf inconum %d",
               avail_param);

    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtRgsl                                            */
/**
    \brief      Extract handler for RGSL message elements

    \brief      this function extracts from given string the elements of\n
                RVRS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResSIVRGetSupLangType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtRgsl(ATResSIVRGetSupLangType   * p_info,
                                  Uint8Type                 * p_msg,
                                  Uint32Type                  string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_RGSL_PARAM,&avail_param,string_len);
    if (AT_PARSER_MIN_RGSL_PARAM == avail_param)
    {
        p_info->is_lang_count = AT_TRUE;
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->lang_count),
                (Uint8Type)AT_RGSL_MIN_START_INDEX, (Uint8Type)AT_RGSL_MAX_STOP_INDEX) )
        {
            sF = SUCCESS;
        }
    }
    else if (AT_PARSER_MAX_RGSL_PARAM == avail_param)
    {
        p_info->is_lang_count = AT_FALSE;
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->lang_index),
                    (Uint8Type)AT_RGSL_MIN_START_INDEX, (Uint8Type)AT_RGSL_MAX_STOP_INDEX) &&
            SUCCESS == _ATParser_sFExtSIVRLanguageValue(&p_tmp,
                    &(p_info->lang_id),
                    AT_SIVR_LANG_US_ENG,
                    (ATSIVRLanguagesEnum) (AT_SIRV_LANG_MAXNUM -1 ))       )
        {
            sF = SUCCESS;
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtRgsl inconum %d",
               avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtRgsl inconum %d",
               avail_param);
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtRres                                            */
/**
    \brief      Extract handler for RRES message elements

    \brief      this function extracts from given string the elements of\n
                RVRS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResSIVRResultType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtRres(ATResSIVRResultType   * p_info,
                                  Uint8Type           * p_msg,
                                  Uint32Type          string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    Uint8Type *     p_quote     = NULL;
    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_RRES_PARAM,&avail_param,string_len);
    if (AT_PARSER_MIN_RRES_PARAM == avail_param)
    {
        p_info->is_nbr = AT_TRUE;
        /* extract number */
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->nb_of_result),
                0x00,0xFF)  )
        {
            sF = SUCCESS;
        }
    }
    else if (AT_PARSER_MAX_RRES_PARAM >= avail_param &&
             AT_PARSER_MID_RRES_PARAM <= avail_param)
    {
        p_info->is_nbr = AT_FALSE;
        /* extract AT_PARSER_MID_RRES_PARAM mandatory params */
        if (SUCCESS == _ATParser_sFExtSIVRPromptValue(&p_tmp,&(p_info->menu_id),
                AT_SIVR_MENU_MAIN, ( ATSIVRPromptEnum ) 0xFFFF ) &&
                /* workaround for RT-705
                       AT_SIVR_MENU_MAIN,AT_SIVR_PRMT_CK5050_MAXVALUE) &&
                 */
            SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->result_index),
                        0x00,0xFF) &&
            SUCCESS == _ATParser_sFExtSIVRResultValue(&p_tmp,
                        &(p_info->result_type),
                        AT_SIVR_RES_NO_SPEECH,AT_SIVR_RES_TRAIN_FINISH) &&
            SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,&(p_info->confidence_rate),
                    AT_RRES_MIN_CONF_LEV,AT_RRES_MAX_CONF_LEV)
             )
        {
            /* get value with respect to result_type */

            switch ( p_info->result_type )
            {
                case AT_SIVR_RES_VT_REJECTED:
                {
                    if ( SUCCESS == _ATParser_sFExtSIVRCmdValue(&p_tmp,
                                    &(p_info->result_value),
                                    (ATSIVRCmdIdEnum)AT_VTREC_RES_TOO_CLOSE,
                                    (ATSIVRCmdIdEnum)AT_VTREC_RES_TOO_BAD) )
                    {
                        sF = SUCCESS;
                    }
                    else
                    {
                        sF = FAIL;
                    }

                    break;
                }
                default:
                {
                    if ( SUCCESS == _ATParser_sFExtSIVRCmdValue(&p_tmp,
                                    &(p_info->result_value),
                                    AT_SIVR_CMD_UNKNOWN,
                                    AT_SIVR_CMD_STORE_NAME_REC) )
                    {
                        sF = SUCCESS;
                    }
                    else
                    {
                        sF = FAIL;
                    }

                    break;
                }
            }


            /* check for addition al parameters and their meaning */
            if (AT_PARSER_MAX_RRES_PARAM == avail_param)
            {   /* 2 additional params available only ids */
                if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                                 &(p_info->opt.id.id1),0x0000,0xFFFF) &&
                    SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                                 &(p_info->opt.id.id2),0x0000,0xFFFF))
                {
                    p_info->opt.id.type = AT_SIVR_RES_IDs;
                    p_info->opt.id.id1_available = AT_TRUE;
                    p_info->opt.id.id2_available = AT_TRUE;
                }
                else
                {
                    sF = FAIL;
                    /* param extraction failed */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                    (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,
                            "ExtRres opt ext failed %d",__LINE__);
#endif
                    DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtRres param extraction failed - Line %d",__LINE__);
                }
            }
            else if (AT_PARSER_MID_RRES_PARAM != avail_param)
            /* only one add - check if number or id */
            {
                /* check if there is a quote in string - if so it is number */
                p_quote = (Uint8Type*)strchr((const char *)p_tmp,AT_ASCII_QUOTE);
                if (NULL != p_quote)
                {   /* quote found - this is a phone number given as string */
                    if (SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                              (Uint8Type*)(p_info->opt.nb.number),
                              &(p_info->opt.nb.number_len),
                              AT_MAX_PHONE_NUM_LEN))
                    {
                        p_info->opt.nb.type = AT_SIVR_RES_NUMBER;
                    }
                    else
                    {
                       sF = FAIL;
                       /* param extraction failed */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                       (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,
                               "ExtRres opt ext failed %d",__LINE__);
#endif
                        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtRres param extraction failed - Line %d",__LINE__);
                    }
                }
                else
                {   /* this is location */
                    if (SUCCESS == _ATParser_sFExtu16RangeValue(&p_tmp,
                                    &(p_info->opt.id.id1),0x0000,0xFFFF))
                    {
                        p_info->opt.id.type = AT_SIVR_RES_IDs;
                        p_info->opt.id.id1_available = AT_TRUE;
                        p_info->opt.id.id2_available = AT_FALSE;
                    }
                    else
                    {
                       sF = FAIL;
                       /* param extraction failed */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                       (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,
                               "ExtRres opt ext failed %d",__LINE__);
#endif
                       DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtRres param extraction failed - Line %d",__LINE__);
                    }
                }
            }
            else
            {   /* else only the mandatory ones have been added */
                p_info->opt.id.type = AT_SIVR_RES_NONE;
            }
        }
    }
    else
    {   /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtRres inconum %d",
               avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtRres invalid number of params %d",
               avail_param);
    }

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPprs                                            */
/**
    \brief      Extract handler for PPRS message elements

    \brief      this function extracts from given string the elements of\n
                PPRS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResPairingResultType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPprs(ATResPairingResultType * p_pprs,
                                  Uint8Type              * p_msg,
                                  Uint32Type             string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;


    if ( SUCCESS == _ATParser_CheckNumberParam( p_msg,
                                AT_PARSER_MIN_PPRS_PARAM,
                                &avail_param,
                                string_len) )
    {
        /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType( &p_tmp,
                                                         &(p_pprs->ext_device.device_type),
                                                         &(p_pprs->ext_device.device_id)) &&
            SUCCESS == _ATParser_sFExtPairingResultValue( &p_tmp,
                                                     &(p_pprs->result),
                                                     AT_PAIRING_FAILED_UNKNOWN,
                                                     AT_MAX_PAIRED_DEVICES_REACHED))
        {
            p_pprs->device_id = AT_INVALID_DEVICE_ID;
            sF = SUCCESS;
        }
    }
    else if ( SUCCESS == _ATParser_CheckNumberParam( p_msg,
                                AT_PARSER_MAX_PPRS_PARAM,
                                &avail_param,
                                string_len) )
    {
        /* now extract the parameters */
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType( &p_tmp,
                                                         &(p_pprs->ext_device.device_type),
                                                         &(p_pprs->ext_device.device_id)) &&
            SUCCESS == _ATParser_sFExtPairingResultValue( &p_tmp,
                                                     &(p_pprs->result),
                                                     AT_PAIRING_FAILED_UNKNOWN,
                                                     AT_MAX_PAIRED_DEVICES_REACHED))
        {
            sF = _ATParser_ExtDeviceId( &p_tmp, &(p_pprs->device_id) );
        }
    }
    else
    {
        /* invalid number of params */
        DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "ExtPprs invalid number of params %d",
               avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPalgoselect                                     */
/**
    \brief      Extract handler for PALGOSELECT message elements

    \brief      this function extracts from given string the elements of\n
                PALGOSELECT message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResPalgoSelectType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPalgoselect(ATResPalgoSelectType * p_info,
                                         Uint8Type            * p_msg,
                                         Uint32Type           string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */

    if (SUCCESS ==  _ATParser_CheckNumberParam( p_msg,
            AT_PARSER_NUM_PALGOSELECT_PARAM,&avail_param,string_len))
    {   /* now extract the parameter */
        if (SUCCESS == _ATParser_sFExtMicAlgoValue(&p_tmp,
                &(p_info->algotype),
                AT_MIC_ALGO_NONE,AT_MIC_ALGO_DBL_AEC_NR3_AMS)     )
        {
           sF = SUCCESS;
        }
    }
    else
    {   /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPalgosel inconum %d",
               avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPalgosel invalid number of params %d",
               avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtMicGain                                     */
/**
    \brief      Extract handler for VGM message elements

    \brief      this function extracts from given string the elements of\n
                VGM message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResMicGainType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtMicGain(ATResMicGainType * p_info,
        Uint8Type            * p_msg,
        Uint32Type           string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */

    if (SUCCESS ==  _ATParser_CheckNumberParam( p_msg,
            AT_PARSER_NUM_MICGAIN_PARAM,&avail_param,string_len))
    {   /* now extract the parameter */
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->mic_gain),
                AT_MIC_GAIN_MIN,AT_MIC_GAIN_MAX)     )
        {
           sF = SUCCESS;
        }
    }
    else
    {   /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtMicGain inconum %d",
               avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtMicGain invalid number of params %d",
               avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPalgoAEC                                     */
/**
    \brief      Extract handler for PALGOAEC message elements

    \brief      this function extracts from given string the elements of\n
                PALGOAEC message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResPalgoAECType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPalgoAEC(ATResPalgoAECType * p_info,
                                      Uint8Type            * p_msg,
                                      Uint32Type           string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */

    if (SUCCESS ==  _ATParser_CheckNumberParam( p_msg,
            AT_PARSER_NUM_PALGOAEC_PARAM,&avail_param,string_len))
    {   /* now extract the parameters */
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->maxAttAec),
                          AT_RX_MAXATTAEC_MIN,AT_RX_MAXATTAEC_MAX) &&
            SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->switchTime),
                          AT_RX_SWITCHTIME_MIN,AT_RX_SWITCHTIME_MAX) )
        {
           sF = SUCCESS;
        }
    }
    else
    {   /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPalgoAEC inconum %d",
               avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "ExtPalgoAEC invalid number of params %d",
               avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtVolumeSet                                     */
/**
    \brief      Extract handler for PNVS message elements

    \brief      this function extracts from given string the elements of\n
                PNVS message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResVolumeSetType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtVolumeSet(ATResVolumeSetType * p_info,
                                       Uint8Type            * p_msg,
                                       Uint32Type           string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */

    if (SUCCESS ==  _ATParser_CheckNumberParam( p_msg,
            AT_PARSER_NUM_PNVS_PARAM,&avail_param,string_len))
    {   /* now check if response include paramter ranges or current values */
        /* NOTE: we do not support extract of ranges so far but answer with default
         * ranges which are hard coded */
        if (0 == strncmp((char *)p_msg, "(", string_len))
        {   /* ranges included, fill response message */
            p_info->is_current = AT_FALSE;
            p_info->min_comm   = AT_VOLUME_GEN_MIN;
            p_info->max_comm   = AT_VOLUME_GEN_MAX;
            p_info->min_ring   = AT_VOLUME_GEN_MIN;
            p_info->max_ring   = AT_VOLUME_GEN_MAX;
            p_info->min_prompt = AT_VOLUME_GEN_MIN;
            p_info->max_prompt = AT_VOLUME_GEN_MAX;
            p_info->min_stream = AT_VOLUME_STREAM_MIN;
            p_info->max_stream = AT_VOLUME_STREAM_MAX;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtVolSet range defaults");
#endif
            DEBUG_TEXT(AT_PARSER_SWI_WNG, MOD_AT_PARSER, "ExtVolSet range defaults");
        }
        else
        {   /* extract paramters */
            p_info->is_current = AT_TRUE;
            if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->min_comm),
                    AT_VOLUME_GEN_MIN,AT_VOLUME_GEN_MAX) &&
                SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->min_ring),
                        AT_VOLUME_GEN_MIN,AT_VOLUME_GEN_MAX) &&
                SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->min_prompt),
                        AT_VOLUME_GEN_MIN,AT_VOLUME_GEN_MAX)&&
                SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->min_stream),
                        AT_VOLUME_STREAM_MIN,AT_VOLUME_STREAM_MAX) )
            {   /* copy min into max */
                p_info->max_comm   = p_info->min_comm;
                p_info->max_ring   = p_info->min_ring;
                p_info->max_prompt = p_info->min_prompt;
                p_info->max_stream = p_info->min_stream;
                sF = SUCCESS;
            }
        }
    }
    else
    {   /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPalgoAEC inconum %d",
               avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtPalgoAEC invalid number of params %d",
               avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPalgoSetMic                                     */
/**
    \brief      Extract handler for PALGOSETMIC message elements

    \brief      this function extracts from given string the elements of\n
                PALGOSETMIC message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATResPalgoSetMicType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPalgoSetMic(ATResPalgoSetMicType * p_info,
                                         Uint8Type            * p_msg,
                                         Uint32Type           string_len)
{
    SuccessFailType sF          = FAIL;
    Uint8Type       avail_param = 0;
    Uint8Type *     p_tmp       = p_msg;
    /* get amount of parameters */

    if (SUCCESS ==  _ATParser_CheckNumberParam( p_msg,
            AT_PARSER_NUM_PALGOSETMIC_PARAM,&avail_param,string_len))
    {   /* now extract the parameter */
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,&(p_info->mic_id),
                            AT_MIC_ID_MIN,AT_MIC_ID_MAX)     )
        {
           sF = SUCCESS;
        }
    }
    else
    {   /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtSetMic inconum %d",
               avail_param);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtPalgoSetMic invalid number of params %d",
               avail_param);
    }
    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtDeviceId                                        */
/**
    \brief      Extract device ID from given string

    \brief      this function extracts from given string the device id\n
                of a BT device

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to device id to be filled
                Uint8Type *
                valid range for application internal device ID
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtDeviceId(Uint8Type         ** p_msg,
                                      Uint8Type         * p_device_id)
{
    SuccessFailType sF        = FAIL;
    Uint8Type       device_id = 0;

    if ('P' == **p_msg) /* correct identifier found */
    {
        device_id = atoi((const char *)*p_msg+1);
        /* check device id is in valid range and if so do alignment
         * for 0 based index inside application */
        /* valid range from parrot is 1..5, inside 0..4 */
        if (AT_PARROT_MIN_DEVICE_ID <= device_id &&
            AT_PARROT_MAX_DEVICE_ID >= device_id    )
        {   /* valid device id found - set pointer value */
            *p_device_id = device_id - 1;
            sF = SUCCESS;
            /* set to character after next comma */
            *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
            if (NULL != *p_msg)
            {
                *p_msg = *p_msg + 1;
            }
        }
        /* else return failure */
    }
    /* else return failure */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtDeviceIdType                                        */
/**
    \brief      Extract device ID from given string

    \brief      this function extracts from given string the device id\n
                of a BT device

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to device type to be filled
                ATDeviceTypeEnum *
                valid enum values
    \param      pointer to device id to be filled
                Uint8Type *
                valid range for application internal device ID
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtDeviceIdType(Uint8Type         ** p_msg,
                                          ATDeviceTypeEnum  * p_device_type,
                                          Uint8Type         * p_device_id)
{
    SuccessFailType sF        = SUCCESS;
    Uint8Type       device_id = 0;

    if ('P' == **p_msg)
    {
        *p_device_type = AT_DEVICE_BLUETOOTH;
    }
    else if ('D' == **p_msg)
    {
        *p_device_type = AT_DEVICE_PLAYER;
    }
    else
    {   /* unsupported character - stop here!!! */
        sF = FAIL;
    }

    if (SUCCESS == sF) /* correct identifier found - go on */
    {
        device_id = atoi((const char *)*p_msg+1);
        /* check device id is in valid range and if so do alignment
         * for 0 based index inside application */
        /* valid range from parrot is 1..5, inside 0..4 */
        if (AT_PARROT_MIN_DEVICE_ID <= device_id &&
            AT_PARROT_MAX_DEVICE_ID >= device_id    )
        {   /* valid device id found - set pointer value */
            *p_device_id = device_id - 1;
            /* set to character after next comma */
            *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
            if (NULL != *p_msg)
            {
                *p_msg = *p_msg + 1;
            }
        }
        else
        {
            sF = FAIL;
        }
    }
    /* else return failure */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPairingDeviceIdType                             */
/**
    \brief

    \brief

    \brief

    \param

    \param

    \param

    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPairingDeviceIdType( Uint8Type           ** p_msg,
                                                  ATPairingDeviceType * p_device_type,
                                                  Uint8Type           * p_device_id)
{
    SuccessFailType sF        = SUCCESS;
    Uint8Type       device_id = 0;

    if ('I' == **p_msg)
    {
        *p_device_type = AT_INQUIRY_DEVICE;
    }
    else if ('P' == **p_msg)
    {
        *p_device_type = AT_DELETED_DEVICE;
    }
    else if ('R' == **p_msg)
    {
        *p_device_type = AT_INITIATED_DEVICE;
    }
    else
    {   /* unsupported character - stop here!!! */
        sF = FAIL;
    }

    if (SUCCESS == sF) /* correct identifier found - go on */
    {
        device_id = atoi((const char *)*p_msg+1);
        /* our device id starts with 0 not 1 ! */
        /* parrot starts paired and init devices with 1 */
        /* request devices starts with 0 */
        /* ticket 12177: initiate device index starts with 0 */
        /* also do not check for max pairing index if initiated device is parsed */
        if (*p_device_type != AT_INITIATED_DEVICE)
         {
            device_id = device_id - 1;

#ifdef UHV_SHOW_UP_TO30_DEVICES
            if ( AT_MAX_DEVICES_AFTER_SEARCH < device_id )
#else
                if ( AT_MAX_PARIRING_DEVICE_ID < device_id )
#endif        
            {   /* no valid device id found */
                sF = FAIL;
            }
         }
        /* valid device id found - set pointer value */
        *p_device_id = device_id;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
    }
    /* else return failure */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtLastName                                        */
/**
    \brief      Extract name identifier

    \brief      this function extracts name identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \param      pointer to name len parameter to be filled
                Uint8Type *
                maximum AT_DEVICE_NAME_MAX_LEN
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtLastName(Uint8Type   ** p_msg,
                                  AT_DeviceNameString   * p_device_name,
                                  Uint8Type   * p_name_len,
                                  Uint32Type    max_len    )
{
    SuccessFailType sF        = FAIL;
    Uint8Type  * p_tmp = *p_msg;

    p_tmp = p_tmp + 1;
    *p_name_len = strlen((const char *)p_tmp) - 1;

    /* check against max length, if too long reduce len */
    if (max_len < *p_name_len)
    {
        *p_name_len = (Uint8Type)max_len;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtLastName inval len/msg %d",max_len);
#endif

        DEBUG_VALUE2(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastName inval length %d - reduce length to %d",
               *p_name_len, max_len);
    }

    if (0 == *p_name_len)
    {   /* ensure '\0' termination */
        (void)memset(p_device_name,'\0',1);
    }
    else
    {   /* copy the amount of characters to the value */
        (void)memcpy(p_device_name,p_tmp,*p_name_len);
        sF = SUCCESS;
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtLastCallerID                                        */
/**
    \brief      Extract name identifier

    \brief      this function extracts name identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \param      pointer to name len parameter to be filled
                Uint8Type *
                maximum AT_DEVICE_NAME_MAX_LEN
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
 extern SuccessFailType _ATParser_ExtLastCallerID(Uint8Type   ** p_msg,
                                         AT_CallerId   * p_device_name,
                                         Uint8Type   * p_name_len,
                                         Uint32Type    max_len    )
{
    SuccessFailType sF        = FAIL;
    Uint8Type  * p_tmp = *p_msg;

    p_tmp = p_tmp + 1;
    *p_name_len = strlen((const char *)p_tmp) - 1;

    /* check against max length, if too long reduce len */
    if (max_len < *p_name_len)
    {
        *p_name_len = (Uint8Type)max_len;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtLastName inval len/msg %d",max_len);
#endif

        DEBUG_VALUE2(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastName inval length %d - reduce length to %d",
               *p_name_len, max_len);
    }

    if (0 == *p_name_len)
    {   /* ensure '\0' termination */
        (void)memset(p_device_name,'\0',1);
    }
    else
    {   /* copy the amount of characters to the value */
        (void)memcpy(p_device_name,p_tmp,*p_name_len);
        sF = SUCCESS;
    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtName                                             */
/**
    \brief      Extract name identifier

    \brief      this function extracts name identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \param      pointer to name len parameter to be filled
                Uint8Type *
                maximum AT_DEVICE_NAME_MAX_LEN
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtName(Uint8Type   ** p_msg,
                                  AT_DeviceNameString   * p_device_name,
                                  Uint8Type   * p_name_len,
                                  Uint32Type    max_len    )
{
    SuccessFailType sF        = FAIL;
    Uint16Type count = 0;
    Uint8Type  * p_tmp = *p_msg;

    /* first check when next token follows */
    count = _ATParser_OtherToken(*p_msg);
    /* as the string must be marked with ' '*/
    if (count >= 3 && p_tmp[0] == AT_ASCII_QUOTE && p_tmp[count-2] == AT_ASCII_QUOTE )
    {
        *p_name_len = count-3;
        /* check against max length, if too long reduce len */
        if (max_len < *p_name_len)
        {
            *p_name_len = (Uint8Type)max_len;
        }
        if (0 == *p_name_len)
        {   /* ensure '\0' termination */
            (void)memset(p_device_name,'\0',1);
        }
        else
        {   /* copy the amount of characters to the value */
            (void)memcpy(p_device_name,&p_tmp[1],*p_name_len);
        }

        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
        sF = SUCCESS;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtName set len %d",*p_name_len);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtName set len %d",*p_name_len);
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtName inval len/msg %d",count);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtName invalid length %d",count)
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtCallerID                                             */
/**
    \brief      Extract name identifier

    \brief      this function extracts name identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \param      pointer to name len parameter to be filled
                Uint8Type *
                maximum AT_DEVICE_NAME_MAX_LEN
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
extern SuccessFailType _ATParser_ExtCallerID(Uint8Type   ** p_msg,
                                         AT_CallerId   * p_device_name,
                                         Uint8Type   * p_name_len,
                                         Uint32Type    max_len    )
{
    SuccessFailType sF        = FAIL;
    Uint16Type count = 0;
    Uint8Type  * p_tmp = *p_msg;

    /* first check when next token follows */
    count = _ATParser_OtherToken(*p_msg);
    /* as the string must be marked with ' '*/
    DEBUG_VALUE2(AT_PARSER_SWI_TRC, MOD_AT_PARSER,"ExtName length is %d, max length is %d",count,max_len);
    
    
    if (count >= 3 && p_tmp[0] == AT_ASCII_QUOTE && p_tmp[count-2] == AT_ASCII_QUOTE )
    {
        *p_name_len = count-3;
        /* check against max length, if too long reduce len */
        if (max_len < *p_name_len)
        {
            *p_name_len = (Uint8Type)max_len;
        }
        if (0 == *p_name_len)
        {   /* ensure '\0' termination */
            (void)memset(p_device_name,'\0',1);
        }
        else
        {   /* copy the amount of characters to the value */
            (void)memcpy(p_device_name,&p_tmp[1],*p_name_len);
        }

        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
        sF = SUCCESS;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtName set len %d",*p_name_len);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_TRC, MOD_AT_PARSER,"ExtName set len %d",*p_name_len);
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtName inval len/msg %d",count);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtName invalid length %d",count)
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtUnquotedString                                             */
/**
    \brief      Extract string not enclosed in quotes

    \brief      this function extracts string identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \param      pointer to name len parameter to be filled
                Uint8Type *
                maximum AT_DEVICE_NAME_MAX_LEN
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtUnquotedString(Uint8Type   ** p_msg,
                                              Uint8Type   * p_string,
                                              Uint16Type  * p_string_len,
                                              Uint32Type    max_len    )
{
    SuccessFailType sF        = FAIL;
    Uint16Type count = 0;
    Uint8Type  * p_tmp = *p_msg;

    /* first check when next token follows */
    count = _ATParser_OtherToken(*p_msg);
    /* as the string is NOT marked with quotes count must only be >= 1 as
     * we also support empty strings */
    if (count >= 1 )
    {
        *p_string_len = count-1;
        /* check against max length, if too long reduce len */
        if (max_len < *p_string_len)
        {
            *p_string_len = (Uint16Type)max_len;
        }
        if (0 == *p_string_len)
        {   /* ensure '\0' termination */
            (void)memset(p_string,'\0',1);
        }
        else
        {   /* copy the amount of characters to the value */
            (void)memcpy(p_string,&p_tmp[0],*p_string_len);
        }

        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
        sF = SUCCESS;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtUnquotedString set len %d",*p_string_len);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtUnquotedString set len %d",*p_string_len);
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtUnquotedString inval len/msg %d",count);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtUnquotedString invalid length %d",count)
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtQuotedLeftPosition                            */
/**
    \brief      Extract string enclosed in quotes from the back of the string

    \brief      this function extracts string identifier from given string

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtQuotedLeftPosition(Uint8Type    ** p_msg,
                                                    ATRspPBReadEntryType ** p_string)
{
    SuccessFailType sF        = FAIL;
    Uint8Type  * p_tmp = *p_msg;
    Uint8Type  * p_start_tmp = *p_msg;
    ATRspPBReadEntryType *pstring_temp = *p_string;

    /* search the last comma in string and overwrite the value at this address with zero */
    p_tmp = (Uint8Type*)strrchr((const char *)p_tmp, AT_ASCII_COMMA);
    *p_tmp = '\0';
    p_tmp = p_tmp + 1;

    /* if we have a long string with 4 or 6 elements */
    if (*p_tmp == AT_ASCII_QUOTE)
    {
        /* increment pionter at the first number */
        p_tmp = p_tmp + 1;
        pstring_temp->phone_number_len = strlen((const char *)&p_tmp[0]) - 1;
        if (pstring_temp->phone_number_len > AT_MAX_PHONE_NUM_LEN)
        {
            pstring_temp->phone_number_len = AT_MAX_PHONE_NUM_LEN;
        }
        (void)strncpy((char *)pstring_temp->phone_number, (char *)&p_tmp[0],pstring_temp->phone_number_len);

        /* search the last comma in string and overwrite the value at this address with zero */
        p_tmp = (Uint8Type*)strrchr((const char *)p_start_tmp, AT_ASCII_COMMA);
        if (NULL != p_tmp)
        {
            *p_tmp = '\0';
        }

        /* read string element <PNType> */
        /* increment pionter */
        p_tmp = p_tmp + 1;
        pstring_temp->pn_type = (ATPhoneNumberTypeEnum)atoi((const char *)&p_tmp[0]);
        /* range check for PNType */
        if (AT_PNT_GENERAL <= pstring_temp->pn_type && AT_PNT_OTHER >= pstring_temp->pn_type)
        {
        pstring_temp->pn_type_numer_available = AT_TRUE;
        }

        /* search the last comma in string and overwrite the value at this address with zero */
        p_tmp = (Uint8Type*)strrchr((const char *)p_start_tmp, AT_ASCII_COMMA);
        if (NULL != p_tmp)
        {
            *p_tmp = '\0';
        }
    }
    /* read string element <VoiceTag> */
    /* increment pionter */
    p_tmp = p_tmp + 1;

    if (((BT_TRUE ==(BooleanType)atoi((const char *)&p_tmp[0]))) ||
        (BT_FALSE == (BooleanType)atoi((const char *)&p_tmp[0])))
    {
        pstring_temp->voice_tag_recorded = (ATBooleanEnum)atoi((const char *)&p_tmp[0]);
    }
    /* search the last comma in string and overwrite the value at this address with zero */
    p_tmp = (Uint8Type*)strrchr((const char *)p_start_tmp, AT_ASCII_COMMA);
    if (NULL != p_tmp)
    {
        *p_tmp = '\0';
    }
    /* read string element <PNCount> */
    /* increment pionter */
    p_tmp = p_tmp + 1;
    pstring_temp->pn_size = (Uint8Type)atoi((const char *)&p_tmp[0]);

    /* read string element <PBName> */
    pstring_temp->name_len = strlen((const char *)&p_start_tmp[1]) - 1;
    if (AT_CALLER_ID_STRING_MAX_LEN < pstring_temp->name_len)
    {
        pstring_temp->name_len = AT_CALLER_ID_STRING_MAX_LEN;
    }
    (void)strncpy ((char *)pstring_temp->name,(char *)&p_start_tmp[1],pstring_temp->name_len);
    sF = SUCCESS;

    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtLastQuotedString                              */
/**
    \brief      Extract string  enclosed in quotes

    \brief      this function extracts identifier from the back of the string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \param      pointer to name len parameter to be filled
                Uint8Type *
                maximum AT_DEVICE_NAME_MAX_LEN
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtLastQuotedString(Uint8Type   ** p_msg,
                                            Uint8Type   * p_string,
                                            Uint16Type  * p_string_len,
                                            Uint32Type    max_len    )
{
    SuccessFailType sF        = FAIL;
    Uint8Type  * p_tmp = *p_msg;

    p_tmp = p_tmp + 1;
    *p_string_len = strlen((const char *)&p_tmp[0]) - 1;

    /* check against max length, if too long reduce len */
    if (max_len < *p_string_len)
    {
        *p_string_len = (Uint8Type)max_len;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtLastQuotedString inval len/msg %d",max_len);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastQuotedString invalid length %d",max_len)
    }

    if (0 == *p_string_len)
    {   /* ensure '\0' termination */
        (void)memset(p_string,'\0',1);
    }
    else
    {   /* copy the amount of characters to the value */
        (void)memcpy(p_string,p_tmp,*p_string_len);
        sF = SUCCESS;
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtQuotedString                                             */
/**
    \brief      Extract string  enclosed in quotes

    \brief      this function extracts string identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to name parameter to be filled
                AT_DeviceNameString *
                all value allowed
    \param      pointer to name len parameter to be filled
                Uint8Type *
                maximum AT_DEVICE_NAME_MAX_LEN
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtQuotedString(Uint8Type   ** p_msg,
                                            Uint8Type   * p_string,
                                            Uint16Type  * p_string_len,
                                            Uint32Type    max_len    )
{
    SuccessFailType sF        = FAIL;
    Uint16Type count = 0;
    Uint8Type  * p_tmp = *p_msg;

    /* first check when next token follows */
    count = _ATParser_OtherToken(*p_msg);
    /* as the string must be marked with ' '*/
    if (count >= 3 && p_tmp[0] == AT_ASCII_QUOTE && p_tmp[count-2] == AT_ASCII_QUOTE )
    {
        *p_string_len = count-3;
        /* check against max length, if too long reduce len */
        if (max_len < *p_string_len)
        {
            *p_string_len = (Uint16Type)max_len;
        }
        if (0 == *p_string_len)
        {   /* ensure '\0' termination */
            (void)memset(p_string,'\0',1);
        }
        else
        {   /* copy the amount of characters to the value */
            (void)memcpy(p_string,&p_tmp[1],*p_string_len);
        }
        /* set to end quote which is at p_tmp with index count-1 */
        *p_msg = &(p_tmp[count-2]);
        /* check if we have a comma that follow */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {   /* if so set to character after the comma */
            *p_msg = *p_msg + 1;
        }
        sF = SUCCESS;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtQuoted set len %d",*p_string_len);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtLastQuotedString set len %d",*p_string_len);
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtQuoted inval len/msg %d",count);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtQuotedString invalid length %d",count)
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtLastBTAddress                                   */
/**
    \brief      Extract BT address identifier

    \brief      this function extracts the BT-Address from the back of the string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to BT address parameter to be filled
                AT_BT_Address *
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtLastBTAddress(Uint8Type       ** p_msg,
                                       AT_BT_Address   * p_bt_addr    )
{
    SuccessFailType sF        = FAIL;
    Uint8Type      *p_tmp     = *p_msg;

    /* search last comma in the string, convert comma to zero
       and increment pointer */
    p_tmp = (Uint8Type*)strrchr((const char *)p_tmp,AT_ASCII_COMMA);
    if (NULL != p_tmp)
    {
        *p_tmp = '\0';
        p_tmp = p_tmp + 1;
    }
    else
    {
        return sF;
    }

    /* length check of BT address */
    if (AT_BT_ADDRESS_LEN == strlen((const char *)p_tmp))
    {
        /* copy the amount of characters to the value */
        (void)memcpy(p_bt_addr,p_tmp,AT_BT_ADDRESS_LEN);
        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtLastBTAddress inval len %d",strlen((const char *)p_tmp));
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastBTAddress invalid length %d",strlen((const char *)p_tmp));
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtBTAddress                                             */
/**
    \brief      Extract BT address identifier

    \brief      this function extracts BT address identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to BT address parameter to be filled
                AT_BT_Address *
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtBTAddress(Uint8Type       ** p_msg,
                                       AT_BT_Address   * p_bt_addr    )
{
    SuccessFailType sF        = FAIL;
    Uint16Type count = 0;

    /* first check when next token follows and if the count match the len */
    count = _ATParser_OtherToken(*p_msg);
    if (AT_BT_ADDRESS_LEN == (count-1))
    {
        /* copy the amount of characters to the value */
        (void)memcpy(p_bt_addr,*p_msg,AT_BT_ADDRESS_LEN);
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != p_msg)
        {
            *p_msg = *p_msg + 1;
        }
        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtBTAddress inval len %d",count);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtBTAddress invalid length %d",count);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtLastCoD                                             */
/**
    \brief      Extract Class of Device identifier

    \brief      this function extracts ClassofDevice identifier \n
                from the back of the string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to ClassofDevice parameter to be filled
                AT_BTClassOfDevice *
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtLastCoD(Uint8Type            ** p_msg,
                                 Uint32Type            * p_bt_cod    )
{
    SuccessFailType sF        = FAIL;
    Uint8Type      *p_tmp     = *p_msg;

    /* search last comma in the string, convert comma to zero
       and increment pointer */
    p_tmp = (Uint8Type*)strrchr((const char *)p_tmp,AT_ASCII_COMMA);
    if (NULL != p_tmp)
    {
        *p_tmp = '\0';
        p_tmp = p_tmp + 1;
    }
    else
    {
        return sF;
    }

    /* length check of class of device */
    if (AT_BT_COD_LEN == strlen((const char *)p_tmp))
    {
        /* convert a string to an unsigned long integer and copy to variable */
        *p_bt_cod = strtoul((const char *)p_tmp, NULL, 16);
        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtLastCoD inval len %d",strlen((const char *)p_tmp));
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastCoD invalid length %d",strlen((const char *)p_tmp));
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtCoD                                             */
/**
    \brief      Extract Class of Device identifier

    \brief      this function extracts ClassofDevice identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to ClassofDevice parameter to be filled
                AT_BTClassOfDevice *
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtCoD(Uint8Type            ** p_msg,
                                 Uint32Type            * p_bt_cod    )
{
    SuccessFailType sF        = FAIL;
    Uint16Type count = 0;

    /* first check when next token follows and if the count match the len */
    count = _ATParser_OtherToken(*p_msg);
    if (AT_BT_COD_LEN == (count-1))
    {
        /* copy the amount of characters to the value */
        //(void)memcpy(p_bt_cod,*p_msg,AT_BT_COD_LEN);
        *p_bt_cod = strtoul((const char *)*p_msg, NULL, 16);
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtCoD inval len %d",count);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtCoD invalid length %d", count);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtSSPPin                                             */
/**
    \brief      Extract SSP pin code

    \brief      this function extracts SSP pin code identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to ClassofDevice parameter to be filled
                ATSSPPinCodeBT *
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtSSPPin(Uint8Type            ** p_msg,
                                    ATSSPPinCodeBT        * p_pin    )
{
    SuccessFailType sF  = FAIL;
    Uint16Type count = 0;

    /* first check when next token follows and if the count match the len */
    count = _ATParser_OtherToken(*p_msg);
    if (AT_MAX_SSP_PIN_CODE_LEN == (count-1))
    {
        /* ensure '\0' termination */
        memset(p_pin, 0, sizeof(*p_pin));

        /* copy the amount of characters to the value */
        (void)memcpy(p_pin, *p_msg, AT_MAX_SSP_PIN_CODE_LEN);

        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
        sF = SUCCESS;
    }
    else
    {
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtSSPPin invalid length %d", count);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtLastSupport                                     */
/**
    \brief      Extract support identifier

    \brief      this function extracts support identifier from the back of the string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to support parameter to be filled
                ATSupportedEnum *
                allowed values: AT_NOT_SUPPORTED or AT_SUPPORTED
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtLastSupport(Uint8Type         ** p_msg,
                                     ATSupportedEnum   * p_support   )
{
    SuccessFailType sF        = FAIL;
    ATSupportedEnum tmp       = AT_NOT_SUPPORTED;
    Uint8Type      *p_tmp     = *p_msg;

    p_tmp = (Uint8Type*)strrchr((const char *)p_tmp,AT_ASCII_COMMA);
    if (NULL != p_tmp)
    {
        *p_tmp = '\0';
        p_tmp = p_tmp + 1;
    }
    else
    {
        return sF;
    }

    tmp = (ATSupportedEnum)atoi((const char *)p_tmp);
    if (AT_NOT_SUPPORTED == tmp || AT_SUPPORTED == tmp)
    {   /* correct value - set and modify string pointer */
        *p_support = tmp;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtLastSupport set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtLastSupport set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtLastSupport ooR %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastSupport out of Range %d",tmp);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtSupport                                         */
/**
    \brief      Extract support identifier

    \brief      this function extracts support identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to support parameter to be filled
                ATSupportedEnum *
                allowed values: AT_NOT_SUPPORTED or AT_SUPPORTED
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtSupport(Uint8Type         ** p_msg,
                                     ATSupportedEnum   * p_support   )
{
    SuccessFailType sF        = FAIL;
    ATSupportedEnum tmp       = AT_NOT_SUPPORTED;

    tmp = (ATSupportedEnum)atoi((const char *)*p_msg);
    if (AT_NOT_SUPPORTED == tmp || AT_SUPPORTED == tmp)
    {   /* correct value - set and modify string pointer */
        *p_support = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtSupport set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtSupport set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtSupport ooR %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtSupport out of Range %d",tmp);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtLastBool                                            */
/**
    \brief      Extract boolean identifier from the end

    \brief      this function extracts boolean identifier from the back of the string

    \brief      this function set the given string pointer to value to the comma\n
                and overwrite the comma with a zero

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to boolean parameter to be filled
                ATBooleanEnum *
                allowed values: AT_FALSE or AT_TRUE
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtLastBool(Uint8Type       ** p_msg,
                                  ATBooleanEnum   * p_bool   )
{
    SuccessFailType sF        = FAIL;
    ATBooleanEnum   tmp       = AT_FALSE;
    Uint8Type      *p_tmp     = *p_msg;

    p_tmp = (Uint8Type*)strrchr((const char *)p_tmp,AT_ASCII_COMMA);
    if (NULL != p_tmp)
    {
        *p_tmp = '\0';
        p_tmp = p_tmp + 1;
    }
    else
    {
        return sF;
    }

    tmp = (ATBooleanEnum)atoi((const char *)p_tmp);
    if (AT_FALSE == tmp || AT_TRUE == tmp)
    {   /* correct value - set and modify string pointer */
        *p_bool = tmp;
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtLastBool set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtLastBool set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtLastBool ooR %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastBool out of Range %d",tmp);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtBool                                            */
/**
    \brief      Extract boolean identifier

    \brief      this function extracts boolean identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to boolean parameter to be filled
                ATBooleanEnum *
                allowed values: AT_FALSE or AT_TRUE
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtBoolSpecial(Uint8Type       ** p_msg,
                                  ATBooleanEnum   * p_bool   )
{
    SuccessFailType sF        = FAIL;

    /* min - integer and ** p_msg - string e.g.(0x00 <= 0x30)
       the atoi function can only convert a number to an integer */
    if (((AT_FALSE | 0x30) == ** p_msg) || ((AT_TRUE | 0x30) == ** p_msg))
    {   /* correct value - set and modify string pointer */
        *p_bool = (ATBooleanEnum)atoi((const char *)*p_msg);
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtBool set %d",** p_msg);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtBoolSpecial set %d",** p_msg);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtBool ooR %d",** p_msg);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtBoolSpecial out of Range %d",** p_msg);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtBool                                            */
/**
    \brief      Extract boolean identifier

    \brief      this function extracts boolean identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to boolean parameter to be filled
                ATBooleanEnum *
                allowed values: AT_FALSE or AT_TRUE
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtBool(Uint8Type       ** p_msg,
                                  ATBooleanEnum   * p_bool   )
{
    SuccessFailType sF        = FAIL;
    ATBooleanEnum   tmp       = AT_FALSE;

    tmp = (ATBooleanEnum)atoi((const char *)*p_msg);
    if (AT_FALSE == tmp || AT_TRUE == tmp)
    {   /* correct value - set and modify string pointer */
        *p_bool = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtBool set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtBool set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtBool ooR %d",tmp);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtBool out of Range %d",tmp);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtCallState                                       */
/**
    \brief      Extract call state identifier

    \brief      this function extracts call state identifier from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to call state parameter to be filled
                ATCallStateEnum *
                allowed values:     AT_CS_ACTIVE_CALL
                                    AT_CS_HELD_CALL
                                    AT_CS_OUTCALL_DIAL_IPR
                                    AT_CS_OUTCALL_RING_IPR
                                    AT_CS_INCOMING
                                    AT_CS_WAITING_CALL
                                    AT_CS_CALL_TERMINATED
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtCallState(Uint8Type        ** p_msg,
                                       ATCallStateEnum   * p_call_state   )
{
    SuccessFailType sF        = FAIL;
    ATCallStateEnum tmp       = AT_CS_ACTIVE_CALL;

    tmp = (ATCallStateEnum)atoi((const char *)*p_msg);
    if (AT_CS_ACTIVE_CALL <= tmp && AT_CS_CALL_TERMINATED >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_call_state = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtCallState set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtCallState set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtCallState ooR %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtCallState out of Range %d",tmp);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtPNTypeSpecialValue                            */
/**
    \brief      Extract phone number type identifier

    \brief      this function extracts phone number type identifier\n
                from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to phone number type parameter to be filled
                ATPhoneNumberTypeEnum *
                allowed values:         AT_PNT_GENERAL
                                        AT_PNT_HOME
                                        AT_PNT_OFFICE
                                        AT_PNT_MOBILE
                                        AT_PNT_OTHER
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtPNTypeSpecialValue(Uint8Type           ** p_msg,
                                 ATPhoneNumberTypeEnum   * p_value,
                                 ATPhoneNumberTypeEnum   min,
                                 ATPhoneNumberTypeEnum   max   )
{
    SuccessFailType sF        = FAIL;

    /* min - integer and ** p_msg - string e.g.(0x00 <= 0x30)
       the atoi function can only convert a number to an integer */
    if (((min | 0x30) <= ** p_msg) && ((max | 0x30) >= ** p_msg))
    {   /* correct value - set and modify string pointer */
        *p_value = (ATPhoneNumberTypeEnum)atoi((const char *)*p_msg);
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPnSpecType set %d",** p_msg);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtPNTypeSpecialValue set %d",** p_msg);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtPnSpecType ooR %d (%d%d)",
                ** p_msg,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtPNTypeSpecialValue out of Range %d min %d max %d",
                ** p_msg,min,max, 0);

    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtLastPNTypeValue                               */
/**
    \brief      Extract phone number type identifier

    \brief      this function extracts phone number type identifier\n
                from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to phone number type parameter to be filled
                ATPhoneNumberTypeEnum *
                allowed values:         AT_PNT_GENERAL
                                        AT_PNT_HOME
                                        AT_PNT_OFFICE
                                        AT_PNT_MOBILE
                                        AT_PNT_OTHER
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtLastPNTypeValue(Uint8Type           ** p_msg,
                                 ATPhoneNumberTypeEnum   * p_value,
                                 ATPhoneNumberTypeEnum   min,
                                 ATPhoneNumberTypeEnum   max   )
{

    SuccessFailType sF        = FAIL;

    /* min - integer and ** p_msg - string e.g.(0x00 <= 0x30)
       the atoi function can only convert a number to an integer */
    if (((min | 0x30) <= ** p_msg) && ((max | 0x30) >= ** p_msg))
    {   /* correct value - set and modify string pointer */
        *p_value = (ATPhoneNumberTypeEnum)atoi((const char *)*p_msg);

#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtLastPnType set %d",** p_msg);
#endif
        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtLastPnType set %d",** p_msg);
#endif
    }

    DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtLastPNTypeValue set %d",** p_msg);

    return sF;
}




/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtPNTypeValue                                       */
/**
    \brief      Extract phone number type identifier

    \brief      this function extracts phone number type identifier\n
                from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to phone number type parameter to be filled
                ATPhoneNumberTypeEnum *
                allowed values:         AT_PNT_GENERAL
                                        AT_PNT_HOME
                                        AT_PNT_OFFICE
                                        AT_PNT_MOBILE
                                        AT_PNT_OTHER
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtPNTypeValue(Uint8Type           ** p_msg,
                                 ATPhoneNumberTypeEnum   * p_value,
                                 ATPhoneNumberTypeEnum   min,
                                 ATPhoneNumberTypeEnum   max   )
{
    SuccessFailType sF        = FAIL;
    ATPhoneNumberTypeEnum       tmp       = min;

    tmp = (ATPhoneNumberTypeEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtPnType set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtPNTypeValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtPnType ooR %d (%d%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtPNTypeValue out of Range %d min %d max %d",
                ** p_msg,min,max, 0);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtu8RangeValue                                  */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts uint8 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtu8RangeValue(Uint8Type   ** p_msg,
                                        Uint8Type   * p_value,
                                        Uint8Type   min,
                                        Uint8Type   max)
{
    SuccessFailType sF        = FAIL;
    Uint8Type       tmp       = 0;

    tmp = (Uint8Type)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtRangeValue set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"Extu8RangeValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtRangeValue ooR %d (%d%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"Extu8RangeValue out of Range %d min %d max %d",
                tmp,min,max, 0);

    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtu16RangeValue                                  */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts uint8 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtu16RangeValue(Uint8Type   ** p_msg,
                                             Uint16Type   * p_value,
                                             Uint16Type   min,
                                             Uint16Type   max)
{
    SuccessFailType sF        = FAIL;
    Uint16Type      tmp       = 0;

    tmp = (Uint16Type)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"u16ExtRangeValue set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"Extu16RangeValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"u16ExtRangeValue ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE2(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"Extu16RangeValue out of Range %d max %d",
                tmp,max);

    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtLastu32RangeValue                             */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from the back of the string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtLastu32RangeValue(Uint8Type   ** p_msg,
                                             Uint32Type   * p_value,
                                             Uint32Type   min,
                                             Uint32Type   max)
{
    SuccessFailType sF        = FAIL;
    Uint32Type      tmp       = 0;
    Uint8Type      *p_tmp     = *p_msg;

    p_tmp = (Uint8Type*)strrchr((const char *)p_tmp,AT_ASCII_COMMA);
    if (NULL != p_tmp)
    {
        *p_tmp = '\0';
        p_tmp = p_tmp + 1;
    }
    tmp = (Uint32Type)atoi((const char *)p_tmp);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"u32ExtRangeValue ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtLastu32RangeValue out of Range %d",tmp);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtu32RangeValue                                  */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtu32RangeValue(Uint8Type   ** p_msg,
                                             Uint32Type   * p_value,
                                             Uint32Type   min,
                                             Uint32Type   max)
{
    SuccessFailType sF        = FAIL;
    Uint32Type      tmp       = 0;

    tmp = (Uint32Type)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"u32ExtRangeValue set %d",tmp);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"Extu32RangeValue set %d",tmp);
        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"u32ExtRangeValue ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"Extu32RangeValue out of Range %d",tmp);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtMicAlgoValue                                  */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtMicAlgoValue(Uint8Type   ** p_msg,
        ATMicAlgoEnum   * p_value,
        ATMicAlgoEnum   min,
        ATMicAlgoEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATMicAlgoEnum      tmp       = min;

    tmp = (ATMicAlgoEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtMicAlgo set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtMicAlgoValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtMicAlgo ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtMicAlgoValue out of Range %d min %d max %d",
                tmp,min,max, 0);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtPairingResultValue                            */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtPairingResultValue(Uint8Type   ** p_msg,
        ATPairingResultEnum   * p_value,
        ATPairingResultEnum   min,
        ATPairingResultEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATPairingResultEnum      tmp       = min;

    tmp = (ATPairingResultEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtPairingResult set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtPairingResultValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtPairingResult ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtPairingResultValue out of Range %d min %d max %d",
                tmp,min,max, 0);
    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtSIVRCmdValue                                  */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtSIVRCmdValue(Uint8Type   ** p_msg,
        ATSIVRCmdIdEnum   * p_value,
        ATSIVRCmdIdEnum   min,
        ATSIVRCmdIdEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATSIVRCmdIdEnum      tmp       = min;

    tmp = (ATSIVRCmdIdEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtSIVRCmd set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtSIVRCmdValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtSIVRCmd ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtSIVRCmdValue out of Range %d min %d max %d",
                tmp,min,max, 0);

    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtSIVRResultValue                               */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtSIVRResultValue(Uint8Type   ** p_msg,
        ATSIVRResultEnum   * p_value,
        ATSIVRResultEnum   min,
        ATSIVRResultEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATSIVRResultEnum      tmp       = min;

    tmp = (ATSIVRResultEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtSIVRResult set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtSIVRResultValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtSIVRResult ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtSIVRResultValue out of Range %d min %d max %d",
                tmp,min,max, 0);
    }
    return sF;
}




/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtSIVRPromptValue                               */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtSIVRPromptValue(Uint8Type   ** p_msg,
        ATSIVRPromptEnum   * p_value,
        ATSIVRPromptEnum   min,
        ATSIVRPromptEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATSIVRPromptEnum      tmp       = min;

    tmp = (ATSIVRPromptEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtSIVRPrompt set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtSIVRPromptValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtSIVRPrompt ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE2(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtSIVRPromptValue out of Range %d max %d",tmp,max);

    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtSIVRLanguageValue                             */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtSIVRLanguageValue(Uint8Type   ** p_msg,
        ATSIVRLanguagesEnum   * p_value,
        ATSIVRLanguagesEnum   min,
        ATSIVRLanguagesEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATSIVRLanguagesEnum      tmp       = min;

    tmp = (ATSIVRLanguagesEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtSIVRLanguage set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtSIVRLanguageValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtSIVRLanguage ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtSIVRLanguageValue out of Range %d min %d max %d",
                tmp,min,max, 0);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtAutoConnStatusValue                           */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtAutoConnStatusValue(Uint8Type   ** p_msg,
        ATAutoConnStatusEnum   * p_value,
        ATAutoConnStatusEnum   min,
        ATAutoConnStatusEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATAutoConnStatusEnum      tmp       = min;

    tmp = (ATAutoConnStatusEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtAutoConnStatus set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtAutoConnStatusValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtAutoConnStatus ooR %d(%d:%d)",
                tmp,min,max);
#endif

        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtAutoConnStatusValue out of Range %d min %d max %d",
                tmp,min,max, 0);
    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtASourceActStateValue                          */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtASourceActStateValue(Uint8Type   ** p_msg,
        ATAudioSourceActStateEnum   * p_value,
        ATAudioSourceActStateEnum   min,
        ATAudioSourceActStateEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATAudioSourceActStateEnum      tmp       = min;

    tmp = (ATAudioSourceActStateEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtASourceActState set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtASourceActStateValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtASourceActState ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtASourceActStateValue out of Range %d min %d max %d",
                tmp,min,max, 0);

    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtEntryFieldMaskValue                           */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtEntryFieldMaskValue(Uint8Type   ** p_msg,
        ATEntryFieldMaskEnum   * p_value,
        ATEntryFieldMaskEnum   min,
        ATEntryFieldMaskEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATEntryFieldMaskEnum      tmp       = min;

    tmp = (ATEntryFieldMaskEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtEntryFieldMask set %d",tmp);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"FExtEntryFieldMask set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtEntryFieldMask ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"FExtEntryFieldMask out of Range %d min %d max %d",
                tmp,min,max, 0);

    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtDeviceplayerCodedValue                        */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtDeviceplayerCodedValue(Uint8Type   ** p_msg,
        ATDevicePlayerCodedEnum   * p_value,
        ATDevicePlayerCodedEnum   min,
        ATDevicePlayerCodedEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATDevicePlayerCodedEnum      tmp       = min;

    tmp = (ATDevicePlayerCodedEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtDeviceplayerCoded set %d",tmp);
#endif

        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtDeviceplayerCodedValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtDeviceplayerCodedValue ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtDeviceplayerCodedValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtDeviceStatusValue                             */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtDeviceStatusValue(Uint8Type   ** p_msg,
        ATDGPSStatusEnum   * p_value,
        ATDGPSStatusEnum   min,
        ATDGPSStatusEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATDGPSStatusEnum tmp      = min;

    tmp = (ATDGPSStatusEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtDeviceStatus set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtDeviceStatusValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtDeviceStatus ooR %d(%d:%d)",
                tmp,min,max);
#endif

        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtDeviceStatusValue out of Range %d min %d max %d",
                tmp,min,max,0);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtBTServiceValue                                */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtBTServiceValue(Uint8Type   ** p_msg,
        ATBTServiceEnum   * p_value,
        ATBTServiceEnum   min,
        ATBTServiceEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATBTServiceEnum      tmp       = min;

    tmp = (ATBTServiceEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"ExtBTServiceValue set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtBTServiceValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtBTServiceValue ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtBTServiceValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtBTServiceResultValue                          */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtBTServiceResultValue(Uint8Type   ** p_msg,
        ATBTServiceResultEnum   * p_value,
        ATBTServiceResultEnum   min,
        ATBTServiceResultEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATBTServiceResultEnum      tmp       = min;

    tmp = (ATBTServiceResultEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtBTServiceResult set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtBTServiceResultValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtBTServiceResult ooR %d(%d:%d)",
                tmp,min,max);
#endif

        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtBTServiceResultValue out of Range %d min %d max %d",
                tmp,min,max,0);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtMetadataTypeValue                             */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtMetadataTypeValue(Uint8Type   ** p_msg,
        ATMetadataTypeEnum   * p_value,
        ATMetadataTypeEnum   min,
        ATMetadataTypeEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATMetadataTypeEnum      tmp       = min;

    tmp = (ATMetadataTypeEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtKMetadataType set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtMetadataTypeValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtKMetadataType ooR %d(%d:%d)",
                tmp,min,max);
#endif

        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtMetadataTypeValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtATSupportedValue                              */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtATSupportedValue(Uint8Type   ** p_msg,
        ATSupportedEnum   * p_value,
        ATSupportedEnum   min,
        ATSupportedEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATSupportedEnum      tmp       = min;

    tmp = (ATSupportedEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtATSupportedValue set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtATSupportedValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"ExtATSupportedValue ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtATSupportedValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtKeyRangeValue                                 */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtKeyRangeValue(Uint8Type   ** p_msg,
        ATRspKeyboardKeysEnum   * p_value,
        ATRspKeyboardKeysEnum   min,
        ATRspKeyboardKeysEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATRspKeyboardKeysEnum      tmp       = min;

    tmp = (ATRspKeyboardKeysEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtKeyRangeValue set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtKeyRangeValue set %d",tmp);


        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,"sFExtKeyRangeValue ooR %d(%d:%d)",
                tmp,min,max);
#endif

        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtKeyRangeValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtDiscoverModeValue                             */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtDiscoverModeValue(Uint8Type   ** p_msg,
        ATBTDiscoverModeEnum   * p_value,
        ATBTDiscoverModeEnum   min,
        ATBTDiscoverModeEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATBTDiscoverModeEnum      tmp       = min;

    tmp = (ATBTDiscoverModeEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtDiscoverModeValue set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtDiscoverModeValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,
                "sFExtDiscoverModeValue ooR %d(%d:%d)",
                tmp,min,max);
#endif

        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtDiscoverModeValue out of Range %d min %d max %d",
                tmp,min,max,0);
    }
    return sF;
}



/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtSIVREngStatusValue                            */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtSIVREngStatusValue(Uint8Type   ** p_msg,
        ATSIVREngStatusEnum   * p_value,
        ATSIVREngStatusEnum   min,
        ATSIVREngStatusEnum   max)
{
    SuccessFailType sF        = FAIL;
    ATSIVREngStatusEnum      tmp       = min;

    tmp = (ATSIVREngStatusEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtSIVREngStatusValue set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtSIVREngStatusValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,
                "sFExtSIVREngStatusValue ooR %d(%d:%d)",
                tmp,min,max);
#endif

        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtSIVREngStatusValue out of Range %d min %d max %d",
                tmp,min,max,0);
    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtRintoneModeValue                              */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtRintoneModeValue(Uint8Type   ** p_msg,
        ATRintoneModeEnum   * p_value,
        ATRintoneModeEnum   min,
        ATRintoneModeEnum   max)
{
    SuccessFailType   sF        = FAIL;
    ATRintoneModeEnum tmp       = min;

    tmp = (ATRintoneModeEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtRintoneMode set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtRintoneModeValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,
                "sFExtRintoneMode ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtRintoneModeValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtCodProfileValue                               */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts ucod profile value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type **
                all value allowed
    \param      pointer to ATCoDProfileEnum type parameter to be filled
                ATCoDProfileEnum *
                all value allowed
    \param      min ATCoDProfileEnum value for range check
                ATCoDProfileEnum
                all value allowed
    \param      max ATCoDProfileEnum value for range check
                ATCoDProfileEnum
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtCodProfileValue(Uint8Type   ** p_msg,
        ATCoDProfileEnum   * p_value,
        ATCoDProfileEnum   min,
        ATCoDProfileEnum   max)
{
    SuccessFailType   sF        = FAIL;
    ATCoDProfileEnum tmp       = min;

    tmp = (ATCoDProfileEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtCodProfile set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtCodProfileValue set %d",tmp);


        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,
                "sFExtCodProfile ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtCodProfileValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_sFExtVTResultValue                                 */
/**
    \brief      Extract value with given range to check

    \brief      this function extracts u32 value from given string

    \brief      this function set the given string pointer to value after comma\n
                in case of successful read

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      pointer to Uint8Type type parameter to be filled
                Uint8Type *
                all value allowed
    \param      min Uint8Type value for range check
                Uint8Type
                all value allowed
    \param      max Uint8Type value for range check
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sFExtVTResultValue(Uint8Type   ** p_msg,
        ATVTRecResultEnum   * p_value,
        ATVTRecResultEnum   min,
        ATVTRecResultEnum   max)
{
    SuccessFailType   sF        = FAIL;
    ATVTRecResultEnum tmp       = min;

    tmp = (ATVTRecResultEnum)atoi((const char *)*p_msg);
    if (min <= tmp && max >= tmp)
    {   /* correct value - set and modify string pointer */
        *p_value = tmp;
        /* set to character after next comma */
        *p_msg = (Uint8Type*)strchr((const char *)*p_msg,AT_ASCII_COMMA);
        if (NULL != *p_msg)
        {
            *p_msg = *p_msg + 1;
        }
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"sFExtVTResult set %d",tmp);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"ExtVTResultValue set %d",tmp);

        sF = SUCCESS;
    }
    else
    {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_WARNING,UHV_AT_PARSER,
                "sFExtVTResult ooR %d(%d:%d)",
                tmp,min,max);
#endif
        DEBUG_VALUE4(AT_PARSER_SWI_WNG, MOD_AT_PARSER,"ExtVTResultValue out of Range %d min %d max %d",
                tmp,min,max,0);

    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_CheckNumberParam                                   */
/**
    \brief      Check number of parameters in string

    \brief      this function check that the required number of parameters\n
                indicated by req_num_param is included in the given string\n
                the check search for comma seperator in the string and must\n
                find req_num_param -1 commas

    \brief      Note: this function does not modify the string pointer!!!

    \brief      Note: the function will also return failure if more commas\n
                then required are found!!!

    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      number of required parameters
                Uint8Type
                all value allowed
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_CheckNumberParam(Uint8Type   * p_msg,
                                           Uint8Type     req_num_param,
                                           Uint8Type    *avail_param,
                                           Uint32Type    string_len    )
{
    SuccessFailType         sF              = FAIL;
    Uint32Type              i               = 0;
    Uint8Type *             p_tmp           = p_msg;
    Uint8Type               cnt             = 0;
    ATParseQuotedParamsEnum enQuoteState    = AT_PARSER_enNoQuotes;


    for (i = 0; i < string_len; i++)
    {
        if( AT_PARSER_enEndQuote == enQuoteState )
        {
            if(',' != *p_tmp)
            {
                // go back to Begin mode cause the last quote is not directly
                // followed by comma, so quote must be within the string
                enQuoteState = AT_PARSER_enBeginQuote;
            }
        }

        if( AT_ASCII_QUOTE == *p_tmp)
        {
            if( AT_PARSER_enNoQuotes == enQuoteState )
            {
                // first quote detected
                enQuoteState = AT_PARSER_enBeginQuote;
            }
            else if( AT_PARSER_enBeginQuote == enQuoteState )
            {
                // sometimes there might be an additional quote within a quoted
                // string e.g. 'Archie's Holzmaden'
                // so if we have an ending quote we need to wait for the following
                // comma
                enQuoteState = AT_PARSER_enEndQuote;
            }
        }
        else if (',' == *p_tmp)
        {   /* match found */
            if( AT_PARSER_enEndQuote == enQuoteState ||
                AT_PARSER_enNoQuotes == enQuoteState )
            {
                // we found the trailing comma of a string
                enQuoteState = AT_PARSER_enNoQuotes;
                cnt++;
            }
            else // AT_PARSER_enBeginQuote == enQuoteState
            {
               // just ignore comma within quotes
            }
        }
        p_tmp++;
    }
    *avail_param = cnt+1;
    if (*avail_param == req_num_param )
    {
        sF = SUCCESS;

    }
    else
    {   /* write trace here */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,"CheckNumParam inco av %d != req %d\n",
                *avail_param,req_num_param);
#endif
        DEBUG_VALUE2(AT_PARSER_SWI_DBG, MOD_AT_PARSER,"CheckNumParam incorrect available %d != req %d",
                *avail_param,req_num_param);
    }
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPbdi                                            */
/**
    \brief      Extract handler for PBDI message elements

    \brief      this function extracts from given string the elements of\n
                PBDI message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspInquiryType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPbdi(ATRspInquiryType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len)
{
    SuccessFailType     sF              = FAIL;
    Uint8Type           avail_param     = 0;
    Uint8Type *         p_tmp           = p_msg;
    Uint8Type *         p_tmp_helper    = p_msg;
    ATPairingDeviceType device_type_tmp = AT_INQUIRY_DEVICE;

    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_PBDI_PARAM,
            &avail_param,string_len);

    if (AT_PARSER_MAX_PBDI_PARAM >= avail_param &&
        AT_PARSER_MIN_PBDI_PARAM <= avail_param)
    {   /* extract mandatory elements */
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType(
                          &p_tmp,
                          &device_type_tmp,
                          &(p_info->device_id)) &&
            AT_INQUIRY_DEVICE == device_type_tmp  &&
            SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                          (Uint8Type*)(p_info->name),
                          &(p_info->name_len),AT_BT_DEVICE_NAME_MAX_LEN) &&
            SUCCESS == _ATParser_ExtBTAddress(&p_tmp,&(p_info->bt_addr))&&
            SUCCESS == _ATParser_ExtCoD(&p_tmp,&(p_info->bt_ClassOfDevice)) )
        {
            sF = SUCCESS;
            p_info->device_type = device_type_tmp;
            /* now check for optional parameters */
            if (AT_PARSER_MAX_PBDI_PARAM == avail_param)
            {
                /* NOTE : The definition for HkProfileStatus within the
                          Parrot specification is missing.
                          Therefore the value could not be analyzed and
                          will be set to NOT AVAILABLE */

                p_info->hk_avail          = AT_FALSE;
                p_info->hk_profile_status = AT_HK_NONE;

#ifndef USE_DEBUGGING_WITH_TRACE_ID
                (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,
                    "ExtPbdi: mis spec for : hk_profile_status ");
#endif
                DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPbdi: mis spec for : hk_profile_status ");

            }
            else
            {
                p_info->hk_avail          = AT_FALSE;
                p_info->hk_profile_status = AT_HK_NONE;
            }
        }
        else
        {
            p_tmp = p_tmp_helper;
            /* error found on param checks */
        }
    }
    if (SUCCESS != sF)
    {
        if((SUCCESS == _ATParser_ExtPairingDeviceIdType(&p_tmp,&device_type_tmp,&(p_info->device_id))) &&
           (AT_INQUIRY_DEVICE == device_type_tmp) &&
           (SUCCESS == _ATParser_ExtLastCoD(&p_tmp,&(p_info->bt_ClassOfDevice))) &&
           (SUCCESS == _ATParser_ExtLastBTAddress(&p_tmp,&(p_info->bt_addr))) &&
           (SUCCESS == _ATParser_sFExtLastQuotedString(&p_tmp, (Uint8Type*)(p_info->name),
                                             &(p_info->name_len),AT_DEVICE_NAME_MAX_LEN)))
        {
            sF = SUCCESS;
            p_info->device_type = device_type_tmp;
            p_info->hk_avail          = AT_FALSE;
            p_info->hk_profile_status = AT_HK_NONE;
        }
        else
        {
            /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,"ExtPbdi inconum %d",
                    avail_param);
#endif
            DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPbdi inconum %d",
                    avail_param);
        }
    }

    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPgir                                            */
/**
    \brief      Extract handler for PGIR message elements

    \brief      this function extracts from given string the elements of\n
                PBDI message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspGetInquiryResType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPgir(ATRspGetInquiryResType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len)
{
    SuccessFailType     sF              = FAIL;
    Uint8Type           avail_param     = 0;
    Uint8Type *         p_tmp           = p_msg;
    Uint8Type *         p_tmp_helper    = p_msg;
    ATPairingDeviceType device_type_tmp = AT_INQUIRY_DEVICE;
    Uint8Type           length          = 0;

    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_PBDI_PARAM,
            &avail_param,string_len);

    if (AT_PARSER_MIN_PGIR_PARAM == avail_param)
    {   /* this is count only */
        p_info->is_count_only = AT_TRUE;
        if (SUCCESS == _ATParser_sFExtu8RangeValue(&p_tmp,
                &(p_info->count),
                0x00, 0xFF))
        {
            sF = SUCCESS;
        }
    }
    else if (AT_PARSER_MAX_PGIR_PARAM >= avail_param &&
             AT_PARSER_MID_PGIR_PARAM <= avail_param)
    {   /* extract mandatory elements */
        p_info->is_count_only = AT_FALSE;
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType(
                          &p_tmp,
                          &device_type_tmp,
                          &(p_info->device_id)) &&
            AT_INQUIRY_DEVICE == device_type_tmp  &&
            SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                          (Uint8Type*)(p_info->name),
                          &(p_info->name_len),AT_BT_DEVICE_NAME_MAX_LEN) &&
            SUCCESS == _ATParser_ExtBTAddress(&p_tmp,&(p_info->bt_addr))&&
            SUCCESS == _ATParser_ExtCoD(&p_tmp,&(p_info->bt_ClassOfDevice)) )
        {
            sF = SUCCESS;
            p_info->device_type = device_type_tmp;
            /* now check for optional parameters */
            if (AT_PARSER_MAX_PGIR_PARAM == avail_param)
            {   /* optional bitfields are available */
                if (SUCCESS == _ATParser_sFExtu32RangeValue(&p_tmp,
                        &(p_info->statusBitfield),
                        0x00000000, 0xFFFFFFFF) &&
                    SUCCESS == _ATParser_sFExtu32RangeValue(&p_tmp,
                        &(p_info->resultBitfield),
                        0x00000000, 0xFFFFFFFF) )
                {
                    p_info->bitfileds_avail = AT_TRUE;
                }
                else
                {   /* optional extract failed, only set avail to false */
                    p_info->bitfileds_avail = AT_FALSE;
                }
            }
            else
            {
                p_info->bitfileds_avail = AT_FALSE;
            }
        }
        else
        {
            p_tmp = p_tmp_helper;
        }
    }
    if (SUCCESS != sF)
    {
        p_info->is_count_only = AT_FALSE;
        if((SUCCESS == _ATParser_ExtPairingDeviceIdType(&p_tmp,
                                                                &device_type_tmp,
                                                                &(p_info->device_id))) &&
           (AT_INQUIRY_DEVICE == device_type_tmp))
        {
            p_tmp_helper = (Uint8Type*)strrchr((const char *)p_tmp,AT_ASCII_COMMA);
            length = strlen((const char *)&p_tmp_helper[1]);
            if ( AT_BT_COD_LEN == length)
            {   /* now check without optional parameters */
                if ((SUCCESS == _ATParser_ExtLastCoD(&p_tmp,&(p_info->bt_ClassOfDevice))) &&
                    (SUCCESS == _ATParser_ExtLastBTAddress(&p_tmp,&(p_info->bt_addr))) &&
                    (SUCCESS == _ATParser_sFExtLastQuotedString(&p_tmp,
                                                                (Uint8Type *)(p_info->name),
                                                                &(p_info->name_len),
                                                                AT_DEVICE_NAME_MAX_LEN)))
                {
                    sF = SUCCESS;
                    p_info->device_type = device_type_tmp;
                    /* optional extract failed, only set avail to false */
                    p_info->bitfileds_avail = AT_FALSE;
                }
            }
            else if (AT_BITFIELD_LEN == length)
            {   /* now check with optional parameters */
                if ((SUCCESS == _ATParser_sFExtLastu32RangeValue(&p_tmp,
                                                                &(p_info->statusBitfield),
                                                                0x00000000,
                                                                0xFFFFFFFF)) &&
                    (SUCCESS == _ATParser_sFExtLastu32RangeValue(&p_tmp,
                                                                &(p_info->resultBitfield),
                                                                0x00000000,
                                                                0xFFFFFFFF)) &&
                    (SUCCESS == _ATParser_ExtLastCoD(&p_tmp,&(p_info->bt_ClassOfDevice))) &&
                    (SUCCESS == _ATParser_ExtLastBTAddress(&p_tmp,&(p_info->bt_addr))) &&
                    (SUCCESS == _ATParser_sFExtLastQuotedString(&p_tmp,
                                                                (Uint8Type *)(p_info->name),
                                                                &(p_info->name_len),
                                                                AT_DEVICE_NAME_MAX_LEN)))
                {
                    sF = SUCCESS;
                    p_info->device_type = device_type_tmp;
                    p_info->bitfileds_avail = AT_TRUE;
                }
            }
            else
            {
                /* error found on param checks */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
                (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,
                        "ExtPgir: id: %d name: %s",
                        p_info->device_id, p_info->name);
#endif
                DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPbdi error found on param checks - device_id %d",
                      p_info->device_id);

            }

        }
        else
        {
            /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,"ExtPgir inconum %d",
                    avail_param);
#endif
            DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPbdi invalid number of params - device_id %d",
                     p_info->device_id);
        }
    }

    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtPprq                                            */
/**
    \brief      Extract handler for PPRQ message elements

    \brief      this function extracts from given string the elements of\n
                PBDI message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspInPairRequestType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtPprq(ATRspInPairRequestType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len)
{
    SuccessFailType     sF              = FAIL;
    Uint8Type           avail_param     = 0;
    Uint8Type *         p_tmp           = p_msg;
    ATPairingDeviceType device_type_tmp = AT_INQUIRY_DEVICE;

    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_MIN_PPRQ_PARAM,
            &avail_param,string_len);

    if (AT_PARSER_MIN_PPRQ_PARAM == avail_param)
    {   /* this is remote id and BT name only */
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType(
                &p_tmp, &device_type_tmp, &(p_info->remote_id)) &&
            SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                          (Uint8Type*)(p_info->name),
                          &(p_info->name_len),AT_BT_DEVICE_NAME_MAX_LEN) )
        {
            if  (AT_DELETED_DEVICE == device_type_tmp ||
                 AT_INITIATED_DEVICE == device_type_tmp  )
            {
                p_info->device_type = device_type_tmp;
                p_info->addr_avail = AT_FALSE;
                p_info->cod_avail  = AT_FALSE;
                sF = SUCCESS;
            }
            else
            {
                DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPprq: ignored device type %d", device_type_tmp);
            }
        }
    }
    else if (AT_PARSER_MID_PPRQ_PARAM == avail_param )
    {   /* extract mandatory elements */
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType(
                &p_tmp, &device_type_tmp, &(p_info->remote_id)) &&
          SUCCESS == _ATParser_ExtCoD(&p_tmp,&(p_info->bt_ClassOfDevice)) &&
          SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                        (Uint8Type*)(p_info->name),
                        &(p_info->name_len),AT_BT_DEVICE_NAME_MAX_LEN) )
        {
            if  (AT_DELETED_DEVICE == device_type_tmp ||
                 AT_INITIATED_DEVICE == device_type_tmp  )
            {
                p_info->device_type = device_type_tmp;
                p_info->addr_avail = AT_FALSE;
                p_info->cod_avail  = AT_TRUE;
                sF = SUCCESS;
            }
            else
            {
                DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPprq: ignored device type %d", device_type_tmp);
            }
        }
    }
    else if (AT_PARSER_MAX_PPRQ_PARAM == avail_param )
    {
        if (SUCCESS == _ATParser_ExtPairingDeviceIdType(
                &p_tmp, &device_type_tmp, &(p_info->remote_id)) &&
            SUCCESS == _ATParser_ExtCoD(&p_tmp,&(p_info->bt_ClassOfDevice))  &&
            SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                        (Uint8Type*)(p_info->name),
                        &(p_info->name_len),AT_BT_DEVICE_NAME_MAX_LEN)&&
            SUCCESS == _ATParser_ExtBTAddress(&p_tmp,&(p_info->bt_addr)))
        {
            if  (AT_DELETED_DEVICE == device_type_tmp ||
                 AT_INITIATED_DEVICE == device_type_tmp  )
            {
                p_info->device_type = device_type_tmp;
                p_info->addr_avail = AT_TRUE;
                p_info->cod_avail  = AT_TRUE;
                sF = SUCCESS;
            }
            else
            {
                DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPprq: ignored device type %d", device_type_tmp);
            }
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,"ExtPprq inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"ExtPprq invalid number of params %d",
                avail_param);
    }

    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_ExtRstu                                            */
/**
    \brief      Extract handler for RSTUEX message elements

    \brief      this function extracts from given string the elements of\n
                RSTUEX message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspRstuexType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_ExtRstu(ATRspRstuexType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len)
{
    SuccessFailType     sF              = FAIL;
    Uint8Type           avail_param     = 0;
    Uint8Type *         p_tmp           = p_msg;

    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_NUM_RSTUEX_PARAM,
            &avail_param,string_len);

    if ( AT_PARSER_NUM_RSTUEX_PARAM == avail_param )
    {   /* extract  elements */

        if (    SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->customer),
                               &(p_info->customer_len),
                               AT_CUSTOMER_MAX_LEN)
             && SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->project),
                               &(p_info->project_len),
                               AT_PROJECT_MAX_LEN)
             && SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->versionNum),
                               &(p_info->versionNum_len),
                               AT_VERSION_NUM_MAX_LEN)
             && SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->language),
                               &(p_info->language_len),
                               AT_LANGUAGE_MAX_LEN)
             && SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->languageVersion),
                               &(p_info->languageVersion_len),
                               AT_LANGUAGE_VERSION_MAX_LEN)
             )
        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,
                    "RSTUEX: params %d OK : %s %s %s %s %s",
                    avail_param,
                    p_info->customer,
                    p_info->project,
                    p_info->versionNum,
                    p_info->language,
                    p_info->languageVersion);
#endif
            DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "*RSTUEX Parameters OKAY");

            sF = SUCCESS;
        }
        else
        {
            /* error found on param checks */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,
                    "RSTUEX: params %d Error : %s %s %s %s %s",
                    avail_param,
                    p_info->customer,
                    p_info->project,
                    p_info->versionNum,
                    p_info->language,
                    p_info->languageVersion);
#endif
            DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "*RSTUEX Parameters ERROR");
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,"RSTUEX: inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"*RSTUEX invalid number of params %d",
                avail_param);
    }

    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}

/*----------------------------------------------------------------------------*/
/* Function    : _ATParser_Pbcv                                               */
/**
    \brief      Extract handler for PBCV message elements

    \brief      this function extracts from given string the elements of\n
                PBCV message and also do the necessary range and value checks\n

    \param      pointer to message for data elements to be filled in
                ATRspPbcvType
                all value allowed
    \param      Pointer to received string from Parrot module via UART
                Uint8Type *
                all value allowed
    \param      length indicator to allow length check to avoid search out of\n
                string range
                Uint32Type
                maximum value is maximum buffer size
    \return     SuccessFailType sF
                SUCCESS or FAIL
   */
/*----------------------------------------------------------------------------*/
SuccessFailType _ATParser_sfExtPbcv(ATRspPbcvType * p_info,
        Uint8Type            * p_msg,
        Uint32Type             string_len)
{
    SuccessFailType     sF              = FAIL;
    Uint8Type           avail_param     = 0;
    Uint8Type *         p_tmp           = p_msg;

    /* get amount of parameters */
    (void)_ATParser_CheckNumberParam(p_msg,AT_PARSER_NUM_PBCV_PARAM,
            &avail_param,string_len);

    if ( AT_PARSER_NUM_PBCV_PARAM == avail_param )
    {   /* extract  elements */

        if (    SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->hw_version),
                               &(p_info->hw_version_len),
                               AT_PBCV_HW_VERSION_LEN)
             && SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->sw_version),
                               &(p_info->sw_version_len),
                               AT_PBCV_SW_VERSION_LEN)
             && SUCCESS == _ATParser_sFExtQuotedString(&p_tmp,
                               (Uint8Type*)(p_info->manufacturer),
                               &(p_info->manufacturer_len),
                               AT_PBCV_MANUFACTURER_LEN)
             )
        {
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_DEBUG,UHV_AT_PARSER,
                    "PBCV: params %d OK : %s %s %s",
                    avail_param,
                    p_info->hw_version,
                    p_info->sw_version,
                    p_info->manufacturer);
#endif

            DEBUG_TEXT(AT_PARSER_SWI_DBG, MOD_AT_PARSER, "PBCV Parameters OKAY");
            sF = SUCCESS;
        }
        else
        {
            /* error found on param checks */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
            (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,
                    "PBCV: params %d Error : %s %s %s",
                    avail_param,
                    p_info->hw_version,
                    p_info->sw_version,
                    p_info->manufacturer);
#endif
            DEBUG_TEXT(AT_PARSER_SWI_ERR, MOD_AT_PARSER, "PBCV Parameters ERROR");
        }
    }
    else
    {
        /* invalid number of params */
#ifndef USE_DEBUGGING_WITH_TRACE_ID
        (void)DEBUG_Printf(UHV_ERROR,UHV_AT_PARSER,"PBCV: inconum %d",
                avail_param);
#endif
        DEBUG_VALUE1(AT_PARSER_SWI_ERR, MOD_AT_PARSER,"PBCV invalid number of params %d",
                avail_param);

    }

    /* else return default failure - trace is written in CheckNumberParam */
    return sF;
}


/* End Of File ATParser_Utils.c */
