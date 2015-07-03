/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     ATParser_CI.h
*-----------------------------------------------------------------------------
* Module Name:    AT Parser
*-----------------------------------------------------------------------------
* Description:    The AT Parser search for carrage returns und line feeds.
                  Analysis the incomming data from the UART minterface.
*-----------------------------------------------------------------------------
* $Date: 2011-10-04 15:35:52 +0200 (Di, 04 Okt 2011) $
* $Rev: 22027 $
* $Author: maennel $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_AT_Parser/trunk/inc/ATParser_CI.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _ATPARSER_CI_H
#define _ATPARSER_CI_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include "ERM_CE.h"
/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Code Macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Private Constants                                                          */
/*----------------------------------------------------------------------------*/

/* defines for buffer sizes */
/* Buffersize must be 400 for SMS messages */
#define SIZEOF_PAT_RX_MSG_BUFFER            400
#define PAT_MAX_TOKEN_LENGTH                SIZEOF_PAT_RX_MSG_BUFFER    


/* defines for parameter number checks */    
#define AT_PARSER_NUM_PLPD_INFO_PARAM  8
#define AT_PARSER_NUM_PIND_PARAM       8
#define AT_PARSER_MIN_NUM_PLCC_PARAM   5
#define AT_PARSER_MAX_NUM_PLCC_PARAM   8
#define AT_PARSER_NUM_PIEV_PARAM       2
#define AT_PARSER_NUM_PPBP_PARAM       4
#define AT_PARSER_MIN_NUM_PPBR_PARAM   1
#define AT_PARSER_MAX_NUM_PPBR_PARAM   6
#define AT_PARSER_PPBR_PARAM_NO_OPT    4
#define AT_PARSER_NUM_PMRDNUM_PARAM    4
#define AT_PARSER_MIN_PMRDMSG_PARAM    3
#define AT_PARSER_MAX_PMRDMSG_PARAM    4
#define AT_PARSER_NUM_PBSC_PARAM       7
#define AT_PARSER_NUM_PGMT_PARAM       2
#define AT_PARSER_NUM_PBSN_PARAM       3
#define AT_PARSER_NUM_PAMU_PARAM       2
#define AT_PARSER_NUM_PSBD_PARAM       2
#define AT_PARSER_NUM_DGPS_PARAM       3
#define AT_PARSER_NUM_DGPS_EXT_PARAM   AT_PARSER_NUM_DGPS_PARAM + 4
#define AT_PARSER_NUM_DGED_PARAM       3
#define AT_PARSER_NUM_PASA_PARAM       1
#define AT_PARSER_NUM_PCMS_PARAM       1
#define AT_PARSER_NUM_RSTUEX_PARAM     5
#define AT_PARSER_SMS_CONC_PARAM       1
#define AT_PARSER_NUM_PBCV_PARAM       3

#define AT_PARSER_NUM_PSSP_PARAM       4
#define AT_PARSER_MIN_NUM_PSPR_PARAM   3
#define AT_PARSER_MAX_NUM_PSPR_PARAM   4

#define AT_PARSER_MIN_NUM_SMS_CONC_PARAM    1
#define AT_PARSER_MAX_NUM_SMS_CONC_PARAM    1

#define AT_PARSER_MIN_NUM_PWNG_PARAM   1
#define AT_PARSER_MAX_NUM_PWNG_PARAM   2
    
#define AT_PARSER_MIN_NUM_RVRS_PARAM   2
#define AT_PARSER_MAX_NUM_RVRS_PARAM   4

#define AT_PARSER_MIN_PPBU_VALUE   0
#define AT_PARSER_MAX_PPBU_VALUE   2
    
#define AT_PARSER_MIN_PPBC_VALUE   0
#define AT_PARSER_MAX_PPBC_VALUE   3

#define AT_PARSER_MIN_PPRS_PARAM   2
#define AT_PARSER_MAX_PPRS_PARAM   3
    
#define AT_PARSER_NUM_RSRF_PARAM   2
#define AT_PARSER_NUM_RSCL_PARAM   2
    
#define AT_PARSER_MIN_RGSL_PARAM   1
#define AT_PARSER_MAX_RGSL_PARAM   2

#define AT_PARSER_MIN_PLCC_PARAM   0
#define AT_PARSER_MAX_PLCC_PARAM   ERM_MAX_NUM_PLCC

#define AT_PARSER_MIN_RRES_PARAM   1
#define AT_PARSER_MID_RRES_PARAM   5    
#define AT_PARSER_MAX_RRES_PARAM   7   
    
#define AT_PARSER_NUM_PALGOSELECT_PARAM   1
#define AT_PARSER_NUM_PALGOSETMIC_PARAM   1
#define AT_PARSER_NUM_MICGAIN_PARAM   1
    
#define AT_PARSER_NUM_PALGOAEC_PARAM  2
    
#define AT_PARSER_NUM_PNVS_PARAM  4
    
#define AT_PARSER_MIN_PALGOALCRX_PARAM   1
#define AT_PARSER_MAX_PALGOALCRX_PARAM   2

#define AT_PARSER_MIN_PALGOSLTX_PARAM   1
#define AT_PARSER_MAX_PALGOSLTX_PARAM   2

#define AT_PARSER_MIN_PBDI_PARAM         4
#define AT_PARSER_MAX_PBDI_PARAM         4 /* no options with 3.10.9 */
    
#define AT_PARSER_MIN_PGIR_PARAM         1 /* this is for count*/
#define AT_PARSER_MID_PGIR_PARAM         4 /* info without options */
#define AT_PARSER_MAX_PGIR_PARAM         6 /* info with options */
    
#define AT_PARSER_MIN_PPRQ_PARAM         2 
#define AT_PARSER_MID_PPRQ_PARAM         3 
#define AT_PARSER_MAX_PPRQ_PARAM         4 

#define AT_PARSER_MIN_PBSF_PARAM         3
#define AT_PARSER_MAX_PBSF_PARAM         AT_PARSER_MIN_PBSF_PARAM
  
// RS: duplicated  
//#define AT_PARSER_MIN_PPRS_PARAM         2
//#define AT_PARSER_MAX_PPRS_PARAM         3

#define AT_PARSER_MIN_NUM_PBCD_PARAM   2
#define AT_PARSER_MAX_NUM_PBCD_PARAM   3
#define AT_PARSER_PBCD_PARAM_NO_OPT    2

#define AT_PARSER_MIN_PAMU_FIELDMASK   0x2   // only title info changed
#define AT_PARSER_MAX_PAMU_FIELDMASK   0x3E  // whole meta data information changed
/*----------------------------------------------------------------------------*/
/* Private Enumerations and structures                                        */
/*----------------------------------------------------------------------------*/
typedef enum
{
    PAT_RX_MSG_STATE_WAIT_FOR_PRE_CR    = 0,
    PAT_RX_MSG_STATE_WAIT_FOR_PRE_LF,
    PAT_RX_MSG_STATE_WAIT_FOR_MSG,
    PAT_RX_MSG_STATE_WAIT_FOR_POST_LF
} ParserRxMsgStateType;    
    
    
#ifdef __cplusplus
}
#endif  /*__cplusplus */

#endif /* _ATPARSER_CI_H */

/* End Of File ATParser_CI.h */
