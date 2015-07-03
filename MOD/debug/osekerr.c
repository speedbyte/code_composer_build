/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osekerr.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.27
|
|  Description: functions for handling of errors
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

/* next define is set in the makefile
 * switches off the include of (generated) tcb.h, includes instead
 * the library config file
 */

#ifdef osdGenerateLib
#define osdIncludeLibConfig 1
#endif

#if defined USE_QUOTE_INCLUDES
 #include "osek.h"
#else
 #include <osek.h>
#endif
#if defined USE_QUOTE_INCLUDES
 #include "osekext.h"
#else
 #include <osekext.h>
#endif

/* Vector release management */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif
/* KB begin vrmReleaseNumber */
/* Source release number */
#define osdVrmMajRelNum 3
#define osdVrmMinRelNum 10
/* KB end vrmReleaseNumber */
#if defined USE_QUOTE_INCLUDES
 #include "vrm.h"
#else
 #include <vrm.h>
#endif

/* CovComment 0:  file: osekerr.c */

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

/*-+--------------------------------------------------------------------------
 / osAbortSystem
 / Funktion:
 /              - called from system assertion osSysErrFail
 / Parameter:   uiError  error code
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
osqFunc1 void osqFunc2 osAbortSystem(uint16 uiError)
{
   /* CovComment 4:  Function: osAbortSystem */
   COVER(0x0218)
   osDisableGlobal();

   #if osdErrorHook
   COVER(0x0219)
   osLastError = uiError; 
   #if (STATUS_LEVEL == EXTENDED_STATUS) && !defined(osdSuppressFilenames)
   COVER(0x021A)
   
   /* 
      The following line will violate MISRA rule 6.3 but the char data type is necessary
      because the assigned __FILE__ will otherwise cause errors on some compilers
   */
   osActiveTaskModule = (osqPROM1 const osqPROM2 char osqPROM3 * osqPRAM3) osdNULL;   /* PRQA S 5013 */
   osActiveTaskLineNumber = 0;
   #endif
   osErrorHook(E_OS_SYS_ABORT);
   #else
   COVER(0x021B)
   /* KB begin dummyAssignAbortSystem */
   osdDummyRead(uiError)   /* PRQA S 3199 */ /* suppress QAC warning about never used variable */
   /* KB end dummyAssignAbortSystem */
   #endif

   COVER(0x021C)
   ShutdownOS(E_OS_SYS_ABORT);
} /* END OF osAbortSystem */

/*-+--------------------------------------------------------------------------
 / osAssertFailed
 / Funktion:
 /              - called if assertion failed, interrupts are disabled in 
 /                assertion
 / Parameter:    
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
#if (STATUS_LEVEL == EXTENDED_STATUS) && (osdEnableAssertions==1)
osqFunc1 void osqFunc2 osAssertFailed(void)
{
   /* CovComment 4:  Function: osAssertFailed */
   COVER(0x021D)
   #if osdErrorHook
   COVER(0x021E)
   osErrorHook(E_OS_SYS_ASSERTION);
   #endif
   COVER(0x021F)
   /* system is now shut down */
   ShutdownOS(E_OS_SYS_ASSERTION);
} /* END OF osAssertFailed */
#endif

/*-+--------------------------------------------------------------------------
 / osErrorHook
 / Funktion:
 /   - wrap Errorhook: disable interrupts and prevent recursive call
 / Parameter:    StatusType ErrorCode
 / Returnvalue:
 /--------------------------------------------------------------------------*/

#if osdErrorHook

osqFunc1 void osqFunc2 osErrorHook(StatusType ErrorCode)
{
   /* CovComment 4:  Function: osErrorHook  */
#if STATUS_LEVEL == EXTENDED_STATUS
#if osdEnableAssertions
   uint8 osSavedAPIStatus;
#endif
#endif
   
   COVER(0x0220)
   OS_ERHOOK_ENTRY()

#if STATUS_LEVEL == EXTENDED_STATUS
   COVER(0x0221)
#if osdEnableAssertions
   COVER(0x0222)
   /* save and reset API status to prevent IntAPI errors inside errorhook */
   osSavedAPIStatus = osIntAPIStatus;
   osSetIntAPIStatus(0);
#endif
#endif

   /* KB begin osekAssignGlobalError */
   #if (osdORTIDebug || defined(osdGHSDebugSupport))
   osLastErrorCode=ErrorCode;
   #ifndef osdOSEK2_2
   osLastError=uiError;
   #endif
   #endif
   /* KB end osekAssignGlobalError */

   if (osErrorFlag == 0)
   {
      COVER(0x0223)
      osErrorFlag=TRUE; /* first entry in ErrorHook, lock ErrorHook */

#if (STATUS_LEVEL == EXTENDED_STATUS)
      COVER(0x0224)
      osSysErrAssert((osCheckInterruptsDisabled() != 0), osdErrEHInterruptsEnabled)
#endif
      osInterErrorHook(ErrorCode);

      osErrorFlag=FALSE; /* unlock ErrorHook */
   }
   ELSEWITHCOVER(0x0225)

#if STATUS_LEVEL == EXTENDED_STATUS
#if osdEnableAssertions
   /* restore old IntAPI status */
   COVER(0x0226)
   osSetIntAPIStatus(osSavedAPIStatus);
#endif
#endif

   COVER(0x0227)
   OS_ERHOOK_EXIT()
} /* END OF osErrorHook */
#endif
                  

/*-+--------------------------------------------------------------------------
 / osMissingTerminateError
 / Funktion:
 /              - called on return from task function without TerminateTask
 /              - shutdown system
 / Parameter:
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
#if (STATUS_LEVEL == EXTENDED_STATUS) 
osqFunc1 void osqFunc2 osMissingTerminateError(void)   /* PRQA S 1503 */ /* function might be called from assembler code only */
{
   /* CovComment 4:  Function: osMissingTerminateError */
   COVER(0x0228)
   osAbortSystem(osdErrMTMissingTerminateTask);
}
#endif

/*-+--------------------------------------------------------------------------
 / osUnhandledException
 / Funktion:
 /              - called if a unhandled exception occures
 / Parameter:
 / Returnvalue:
 / Context:     any, this function is meant for OS internal use
 /--------------------------------------------------------------------------*/
osqFunc1 void osqFunc2 osUnhandledException(void)   /* PRQA S 1503 */ /* function might be called from assembler code only */
{
   /* CovComment 4:  Function: osUnhandledException */
   COVER(0x0229)
   osAbortSystem(osdErrUEUnhandledException);

} /* END OF osUnhandledException */


/* KB begin osekHWosISRUnhandledExeptionSrc */
/* KB end osekHWosISRUnhandledExeptionSrc */


/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */

/* END OF MODULE osekerr.c */
