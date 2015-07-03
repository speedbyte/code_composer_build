/**************************************************************************
 *  COPYRIGHT (C) In2Soft GmbH, 2005-2007                                 *
 *  COPYRIGHT (C) Volkswagen AG, 2006-2007                                *
 *  ALLE RECHTE VORBEHALTEN. ALL RIGHTS RESERVED.                         *
 *                                                                        *
 **************************************************************************
 *  FILE: UDS_CFG.H
 **************************************************************************
 *  PURPOSE:
 *  Configuration H-File for UDS Layer
 *
 *************************************************************************
 *  CHANGE HISTORY: In2Soft GmbH
 *
 *  Version    Name     Date   Description
 **************************************************************************
 *  V_00_01_00 Ki 12.05.2005 - VW/Audi conformance, first version 
 *  V_00_01_01 Ki 30.05.2005 - Adjustments due to the comments by Audi 
 *  V_00_01_02 Ki 03.06.2005 - Further adjustments 
 *  V_00_01_03 Ki 07.06.2005 - C++ comment eliminated (MISRA conformance)  
 *  V_00_01_04 Ki 22.07.2005 - Configuration matching demo application  
 *  V_00_01_05 Ki 04.08.2005 - Bug-Fix: suppress response, not supported
 *                             in active mode handling
 *  V_00_01_06 Ki 09.08.2005 - Timing P2 conform to specification
 *  V_00_01_07 Ki 19.09.2005 - UDS_XX_SUPPRESS_RESP = 1
 *  V_00_01_08 Ki 05.12.2005 - revision and suppress bit configuration
 *  V_01_00_00 Ki 18.01.2006 - no change
 *  V_01_01_00 Ki 09.03.2006 - no change
 *  V_01_02_00 Ki 02.06.2006 - Multi client capability
 *  V_01_02_01 Ki 04.09.2006 - General revision based on review (by Audi) 
 *  V_01_02_02 Ki 10.10.2006 - UDS_CLIENTS moved into uds.h
 *  V_01_02_03 Ki 02.11.2006 - Services $19,27,28,85 supported for client #1
 *  V_01_02_04 Ki 19.12.2006 - i2s_types.h included
 *                           - use of i2s type instead of C types
 *  V_01_02_05 Ki 12.01.2007 - UDS_STANDARD_SERVICES_USED new
 *  V_01_02_06 Ki 22.01.2007 - no change
 *  V_01_02_07 Ki 02.03.2007 - general revision, review by Audi
 *  V_01_02_08 Ki 30.03.2007 - comment for t_DiagServiceItem
 *                           - session bits corrected
 *  V_01_02_09 Ki 02.07.2007 - UDS_ubDiagService11 not supported in default
 *                             session: session config 0x1F --> 0x1E
 *                           - new: UDS_SEND_POS_RESP_AFTER_78H
 *                           - all prototypes UDS_ubDiagServiceXX as comment
 *  V_01_02_10 Ki 11.07.2007 - UDS_ubDiagService11 supported in all sessions
 *  V_01_02_11 Ki 07.08.2007 - initialization of ubDiagSession[] modified 
 *  V_01_02_12 Ki 02.11.2007 - no change
 *  V_01_02_13 Ki 18.06.2008 - no change 
 *  V_01_02_14 Ki 23.07.2008 - no change
 *************************************************************************/

#ifndef  _I2S_UDS_CFG_H_
#define  _I2S_UDS_CFG_H_

/* ----------------------------------------------------------------------- */
/*  - C o n s t a n t s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */

/* suppress or don´t pos. response after 78h was sent */
#define UDS_SEND_POS_RESP_AFTER_78H                  1   /* 0=suppress(LAH V1.4), 1=send(LAH V1.6) */

/* set here the target env. of UDS modulue  */
#define UDS_USAGE                                    0   /* 0 = ISO-TP, else = AALI */

/* be sure that TP_CHANNELS >= UDS_CLIENTS: see tp_cfg.h */
#define UDS_CLIENTS                                  1   /* number of UDS clients */

/* define here if SDS module used or not  */
#define UDS_STANDARD_SERVICES_USED                   1   /* 0 = SDS unused, else = SDS used */

/* Definiton of the call cycle of the UDS tasks in ms  */
#define cuw_UDS_TASK_CYCLE                    (word)10   /* Task cycle time [ms] */

/* Definition of timouts */
#define cuw_P2_MAX_DEF                        (word)50   /* ECU response time [ms] */
#define cuw_P2_MAX                          (word)5000   /* ECU response time extended [ms] */
#define cuw_S3                              (word)5000   /* diagnosis time out [ms] */             

#ifdef  _I2S_UDS_C_
/* NOTICE: please substitute not supported sessions by UDScub_INVALID_SESSION */
static const byte ubDiagSession[UDS_CLIENTS][UDS_MAX_NO_OF_SESSIONS] = {
/* example for full session support */
{ UDScub_DEFAULT_SESSION ,UDScub_PROGRAMMING_SESSION ,UDScub_EXTENTED_DIAG_SESSION ,UDScub_EOL_SESSION, 
  UDScub_IO_TEST_SESSION, UDScub_DEVELOPMENT_SESSION },
#if( UDS_CLIENTS == 2 )
/* example for: UDScub_IO_TEST_SESSION not supported */
{ UDScub_DEFAULT_SESSION ,UDScub_PROGRAMMING_SESSION ,UDScub_EXTENTED_DIAG_SESSION ,UDScub_EOL_SESSION, 
  UDScub_INVALID_SESSION, UDScub_DEVELOPMENT_SESSION },
#elif( UDS_CLIENTS > 2 )
#error "array size does not match with UDS_CLIENTS value"
#endif
};
#endif

/* ----------------------------------------------------------------------- */
/*  - E x t e r n a l s -------------------------------------------------- */
/* ----------------------------------------------------------------------- */
/* expected call back functions */
extern void UDS_vDiagTimeout( void );
/*extern byte UDS_ubDiagService01( byte*, word*, byte );
extern byte UDS_ubDiagService02( byte*, word*, byte );
extern byte UDS_ubDiagService03( byte*, word*, byte );
extern byte UDS_ubDiagService04( byte*, word*, byte );
extern byte UDS_ubDiagService06( byte*, word*, byte );
extern byte UDS_ubDiagService07( byte*, word*, byte );
extern byte UDS_ubDiagService08( byte*, word*, byte );
extern byte UDS_ubDiagService09( byte*, word*, byte );*/
extern byte UDS_ubDiagService10( byte*, word*, byte );
extern byte UDS_ubDiagService11( byte*, word*, byte );
extern byte UDS_ubDiagService14( byte*, word*, byte ); 
extern byte UDS_ubDiagService19( byte*, word*, byte );
extern byte UDS_ubDiagService22( byte*, word*, byte ); 
extern byte UDS_ubDiagService23( byte*, word*, byte );
extern byte UDS_ubDiagService27( byte*, word*, byte ); 
extern byte UDS_ubDiagService28( byte*, word*, byte );
/*extern byte UDS_ubDiagService2F( byte*, word*, byte ); 
extern byte UDS_ubDiagService31( byte*, word*, byte );
extern byte UDS_ubDiagService34( byte*, word*, byte ); 
extern byte UDS_ubDiagService35( byte*, word*, byte );
extern byte UDS_ubDiagService36( byte*, word*, byte ); 
extern byte UDS_ubDiagService37( byte*, word*, byte );
extern byte UDS_ubDiagService2E( byte*, word*, byte ); 
extern byte UDS_ubDiagService3D( byte*, word*, byte );*/
extern byte UDS_ubDiagService3E( byte*, word*, byte ); 
extern byte UDS_ubDiagService85( byte*, word*, byte );
/*extern byte UDS_ubDiagService86( byte*, word*, byte );
extern byte UDS_ubDiagService87( byte*, word*, byte );*/

#if( UDS_CLIENTS == 2 )
extern void UDS_vDiagTimeout_ECM( void );
extern byte UDS_ubDiagService10_ECM( byte*, word*, byte );
extern byte UDS_ubDiagService11_ECM( byte*, word*, byte );
extern byte UDS_ubDiagService14_ECM( byte*, word*, byte ); 
extern byte UDS_ubDiagService22_ECM( byte*, word*, byte ); 
extern byte UDS_ubDiagService3E_ECM( byte*, word*, byte ); 
#elif( UDS_CLIENTS > 2 )
#error "missing call back function prototypes for clients given by UDS_CLIENTS value"
#endif

/* ----------------------------------------------------------------------- */
/*  - T i m e o u t  Callbacks ------------------------------------------- */
/* ----------------------------------------------------------------------- */
#ifdef  _I2S_UDS_C_
static const t_DiagTimeoutFkt cs_TimeoutFkt[UDS_CLIENTS] = {
{ UDS_vDiagTimeout },
#if( UDS_CLIENTS == 2 )
{ UDS_vDiagTimeout_ECM },
#elif( UDS_CLIENTS > 2 )
#error "array size does not match with UDS_CLIENTS value"
#endif
};
#endif

/* ----------------------------------------------------------------------- */
/*  - S e r v i c e  Callbacks ------------------------------------------- */
/* ----------------------------------------------------------------------- */

/* mapping of ModeAvailability bits to diagnostic sessions */ 
/*00111111   ModeAvailability byte mapping  */
/*||||bit0-- UDScub_DEFAULT_SESSION         */
/*||||||1--- UDScub_PROGRAMMING_SESSION     */
/*|||||2---- UDScub_EXTENTED_DIAG_SESSION   */
/*||||3----- UDScub_EOL_SESSION             */
/*|||4------ UDScub_IO_TEST_SESSION         */
/*||5------- UDScub_DEVELOPMENT_SESSION     */
/*|6-------- unused                         */
/*7--------- unused                         */

#ifdef  _I2S_UDS_C_
static const t_DiagServiceItem cs_ServicesModes[UDS_CLIENTS][UDS_MAX_NO_OF_SERVICES] = {
/*  |-------------------- service id
    |    |--------------- address of the callback function
    |    |  |------------ suppress pos. response 1=suppress, 0=don´t
    |    |  |   |-------- valid in session(s) */
{{ 0x01, 0, 0, 0x2D },
 { 0x02, 0, 0, 0x2D }, /* Service 01-09 are OBD modes! */
 { 0x03, 0, 0, 0x2D },
 { 0x04, 0, 0, 0x2D },
 { 0x06, 0, 0, 0x2D },
 { 0x07, 0, 0, 0x2D },
 { 0x08, 0, 0, 0x2D },
 { 0x09, 0, 0, 0x2D },
 { 0x10, UDS_ubDiagService10, 1, 0x3F },
 { 0x11, UDS_ubDiagService11, 1, 0x3F },      /* 0x3E in case of VW80124 V1.4 */
 { 0x14, UDS_ubDiagService14, 0, 0x2D },
 { 0x19, UDS_ubDiagService19, 0, 0x3D },
 { 0x22, UDS_ubDiagService22, 0, 0x3F },
 { 0x23, UDS_ubDiagService23, 0, 0x20 },
 { 0x27, UDS_ubDiagService27, 0, 0x2E }, 
 { 0x28, UDS_ubDiagService28, 1, 0x2C },
 { 0x2F, 0, 0, 0x3D },
 { 0x31, 0, 0, 0x2F },
 { 0x34, 0, 0, 0x2A },
 { 0x35, 0, 0, 0x28 },
 { 0x36, 0, 0, 0x2A },
 { 0x37, 0, 0, 0x2A },
 { 0x2E, 0, 0, 0x2F },
 { 0x3D, 0, 0, 0x20 },
 { 0x3E, UDS_ubDiagService3E, 1, 0x3F },
 { 0x85, UDS_ubDiagService85, 1, 0x3C },
 { 0x86, 0, 1, 0x2C },
 { 0x87, 0, 1, 0x2E }
},
#if( UDS_CLIENTS == 2 )
{{ 0x01, 0, 0, 0x2D },
 { 0x02, 0, 0, 0x2D },
 { 0x03, 0, 0, 0x2D },
 { 0x04, 0, 0, 0x2D },
 { 0x06, 0, 0, 0x2D },
 { 0x07, 0, 0, 0x2D },
 { 0x08, 0, 0, 0x2D },
 { 0x09, 0, 0, 0x2D },
 { 0x10, UDS_ubDiagService10_ECM, 1, 0x3F },
 { 0x11, UDS_ubDiagService11_ECM, 1, 0x3F },  /* 0x3E in case of VW80124 V1.4 */
 { 0x14, UDS_ubDiagService14_ECM, 0, 0x2D },
 { 0x19, 0, 0, 0x3D },
 { 0x22, UDS_ubDiagService22_ECM, 0, 0x3F },
 { 0x23, 0, 0, 0x20 },
 { 0x27, 0, 0, 0x2E },
 { 0x28, 0, 1, 0x2C },
 { 0x2F, 0, 0, 0x3D },
 { 0x31, 0, 0, 0x2F },
 { 0x34, 0, 0, 0x2A },
 { 0x35, 0, 0, 0x28 },
 { 0x36, 0, 0, 0x2A },
 { 0x37, 0, 0, 0x2A },
 { 0x2E, 0, 0, 0x2F },
 { 0x3D, 0, 0, 0x20 },
 { 0x3E, UDS_ubDiagService3E_ECM, 1, 0x3F },
 { 0x85, 0, 1, 0x3C },
 { 0x86, 0, 1, 0x2C },
 { 0x87, 0, 1, 0x2E }
}
#elif( UDS_CLIENTS > 2 )
#error "array size does not match with UDS_CLIENTS value"
#endif
};
#endif

#endif
