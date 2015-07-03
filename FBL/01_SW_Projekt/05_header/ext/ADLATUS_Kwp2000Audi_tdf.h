/*-----------------------------------------------------------------------------+
|            ____    ___            ____    _____           ____               |
|           |    |  |   \   |      |    |     |    |    |  |                   |
|           |____|  |    |  |      |____|     |    |    |  |____               |
|           |    |  |    |  |      |    |     |    |    |       |              |
|           |    |  |___/   |____  |    |     |    |____|   ____|              |
|                                                                              |
|           T H E   S M A R T   E M B E D D E D   S O L U T I O N              |
|                                                                              |
+------------------------------------------------------------------------------+
| F I L E   I N F O R M A T I O N                                              |
+------------------------------------------------------------------------------+
|                                                                              |
|  Filename:  ADLATUS_Kwp2000audi_tdf.h                                        |
|                                                                              |
|  Comment:   This file includes all type defines and extern declarations for  |
|             the SMART ADLATUS KWP2000audi protocol handling.                 |
|                                                                              |
+------------------------------------------------------------------------------+
| R E V I S I O N   H I S T O R Y                                              |
+------------------------------------------------------------------------------+
|
| Subversion Info:
| $Id: ADLATUS_Kwp2000Audi_tdf.h 4396 2008-11-04 09:26:32Z maennel $
| $HeadURL: http://smartsvn/svn/adlatusrepository/auslieferungen/S1NN/086350-S1NN-ADLATUS-TMS470R-MucSuc/20081201_S1NN_TMS470R_BlfIso-Source_V1.00/01_SW_Projekt/05_header/ext/ADLATUS_Kwp2000Audi_tdf.h $
|
+------------------------------------------------------------------------------+
|      (c) SMART ELECTRONIC DEVELOPMENT GMBH - all rights reserved             |
+-----------------------------------------------------------------------------*/

/* ========================================================================== */
/*      O P E N   M U L T I P L E   I N C L U D E   P R O T E C T I O N       */
/* ========================================================================== */
#ifndef __TDF_KWP2000audi_
#define __TDF_KWP2000audi_
/* ========================================================================== */


/* ========================================================================== */
/*                                                                            */
/*      E X T E R N   D E C L A R A T I O N S                                 */
/*                                                                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* Description:   ReturnCodeTables for each Service of the KWP2000Base modul. */
/* used in Modul: ADLATUS_KWP2000audi.c                                       */
/*                ADLATUS_SequenceX.c                                         */
/*----------------------------------------------------------------------------*/
/* General error services                                                     */
extern const UBYTE c_CCkwpaudi_RCTRequestLengthError_AUB[];
extern const UBYTE c_CCkwpaudi_RCTSequenceError_AUB[];
extern const UBYTE c_CCkwpaudi_RCTServiceNotSupported_AUB[];
extern const UBYTE c_CCkwpaudi_RCTServiceNotAllowed_AUB[];
extern const UBYTE c_CCkwpaudi_RCTAddressingNotAllowed_AUB[];
extern const UBYTE c_CCkwpaudi_RCTSecurityAccessDenied_AUB[];

extern const UBYTE c_CCkwpaudi_RCTSubfunctionNotSupported_AUB[];
extern const UBYTE c_CCkwpaudi_RCTRcRoutineIdNotSupported_AUB[];

/*----------------------------------------------------------------------------*/
/* Description:   ReturnCodeTables for each Service of the KWP2000Base modul. */
/* used in Modul: ADLATUS_KWP2000audi.c                                      */
/*                ADLATUS_SequenceX.c                                         */
/*----------------------------------------------------------------------------*/
extern const UBYTE c_CCkwpaudi_RCTDSCDefaultSession_AUB[];
extern const UBYTE c_CCkwpaudi_RCTDSCProgSession_AUB[];
extern const UBYTE c_CCkwpaudi_RCTDSCExtendedSession_AUB[];
extern const UBYTE c_CCkwpaudi_RCTControlDTCSetting_AUB[];
extern const UBYTE c_CCkwpaudi_RCTCommunicationControl_AUB[];
extern const UBYTE c_CCkwpaudi_RCTWriteDataByID_AUB[];
extern const UBYTE c_CCkwpaudi_RCTTesterPresent_AUB[];
extern const UBYTE c_CCkwpaudi_RCTECUReset_AUB[];

extern const UBYTE c_CCkwpaudi_RCTSecurityAccessGetSeed_AUB[];
extern const UBYTE c_CCkwpaudi_RCTSecurityAccessSendKey_AUB[];

extern const UBYTE c_CCkwpaudi_RCTRequestDownload_AUB[];
extern const UBYTE c_CCkwpaudi_RCTTransferData_AUB[];
extern const UBYTE c_CCkwpaudi_RCTRequestTransferExit_AUB[];
extern const UBYTE c_CCkwpaudi_RCTRcCheckProgramPreCond_AUB[];
extern const UBYTE c_CCkwpaudi_RCTRcEraseMemory_AUB[];
extern const UBYTE c_CCkwpaudi_RCTRcCheckMemory_AUB[];
extern const UBYTE c_CCkwpaudi_RCTRcChkProgDependencies_AUB[];
extern const UBYTE c_CCkwpaudi_RCTReadDataById_AUB[];


/* ========================================================================== */
/*                                                                            */
/*      G L O B A L   F U N C T I O N   P R O T O T Y P E S                   */
/*                                                                            */
/* ========================================================================== */

/*----------------------------------------------------------------------------*/
/* Description:   InitialisationRoutine for the KWP2000audi.c modul.         */
/* used in Modul: ADLATUS_KWP2000audi.c                                      */
/*----------------------------------------------------------------------------*/
void FUN_CCkwpaudi_InitKwp2000_V (const KWP2000INTERFACE_ST* KWP2000Interface_PST);
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void FUN_CCkwpaudi_InitKwp2000internal_V(void);

/*----------------------------------------------------------------------------*/
/* Description:   KWP2000ServiceRoutines of the KWP2000audi.c modul.          */
/* used in Modul: ADLATUS_KWP2000audi.c                                       */
/*                ADLATUS_SequenceX.c                                         */
/*----------------------------------------------------------------------------*/
/* error services */
UBYTE FUN_CCkwpaudi_RequestLengthError_UB         ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_SubfunctionNotSupported_UB    ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );

UBYTE FUN_CCkwpaudi_SequenceError_UB              ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_ServiceNotSupported_UB        ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_ServiceNotAllowed_UB          ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_AddressingNotAllowed_UB       ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_SecurityAccessDenied_UB       ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );

/* Service functions */
UBYTE FUN_CCkwpaudi_DSCDefaultSession_UB          ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_DSCProgSession_UB             ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_DSCExtendedSession_UB         ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_ControlDTCSetting_UB          ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_CommunicationControl_UB       ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_WriteDataByID_UB              ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_TesterPresent_UB              ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_ECUReset_UB                   ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_SaGetSeed_UB                  ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_SaSendKey_UB                  ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_RequestDownload_UB            ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_TransferData_UB               ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_RequestTransferExit_UB        ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_RcCheckProgramPreCond_UB      ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_RcEraseMemory_UB              ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_RcCheckMemory_UB              ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_RcChkProgDependencies_UB      ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_RcRoutineIdNotSupported_UB    ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );
UBYTE FUN_CCkwpaudi_ReadDataById_UB               ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                                    PTR_BUFFER_UB t_OutBuffer_PBUF );

/* internal used */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
UWORD FUN_CCkwpaudi_LogicalAddressCheck_UW        ( ULONG t_StartAdress_UL, \
                                                    ULONG t_EndAddress_UL,  \
                                                    UBYTE t_Handle_UB       );
UBYTE FUN_CCkwpaudi_ResolveMemoryMapIndex_UB      ( ULONG *t_StartAdress_UL, \
                                                    ULONG *t_EndAddress_UL,  \
                                                    UWORD t_MemoryIndex_UW  );
UBYTE FUN_CCkwpaudi_ResolveValidationMapIndex_UB  ( UWORD t_MemoryIndex_UW );

/* CallBack functions used */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
UBYTE FUN_CCkwpaudi_GetSecurityLevel_UB();

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

UBYTE FUN_CCkwpaudi_CheckAddrMode_UB      ( PTR_BUFFER_UB t_InBuffer_PBUF, \
                                            UBYTE t_RequAddrMode_UB);
                                                  

UBYTE FUN_CCkwpaudi_ReadTimelock_UB         ( PTR_BUFFER_UB t_Destination_PUB );
UBYTE FUN_CCkwpaudi_WriteTimelock_UB        ( PTR_BUFFER_UB t_Source_PUB );
                                                    
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void FUN_CCkwpaudi_ClearFlashFunctions_V(void);

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void FUN_CCkwpaudi_InhibitReset_UB(void);
void FUN_CCkwpaudi_GrantReset_UB(void);


/* ========================================================================== */
/*      C L O S E   M U L T I P L E   I N C L U D E   P R O T E C T I O N     */
/* ========================================================================== */
#endif
/* ========================================================================== */

/* ========================================================================== */
/*      E N D   O F   F I L E                                                 */
/* ========================================================================== */
