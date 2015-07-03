/* Kernbauer Version: 1.14 Konfiguration: TMS470 Erzeugungsgangnummer: 372 */

/*****************************************************************************
| Project Name: OSEK 2.2
|    File Name: osektrace.c
|
|  General code: @Version: 3.54b@
|  Module version: $vv$=1.11
|
|  Description: trace functions
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2000-2008 Vector Informatik GmbH          All rights reserved.
|****************************************************************************/

#if defined USE_QUOTE_INCLUDES
 #include "osek.h"         /* includes testmac2.h which includes osektrace.h */
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

/* CovComment 0:  file: osektrac.c */

/* KB begin osekStartOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=".osdata"
#pragma ghs section text=".oscode"
#pragma ghs section rodata=".osconst"
#endif
/* KB end osekStartOfCModule */

#ifdef osdUseTrace

/* global variables for trace buffer */
tOsTraceBuffer osTraceBuffer[osdTraceDepth];
uint16 osTraceBufferIndex;

/* initialise trace - call this function at the beginning of StartOS */
osqFunc1 void osqFunc2 osInitTrace(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osInitTrace */
   uint16 i;

   COVER(0x0403)
   /* KB begin osekHWosSaveDisableLevelNested */
   osSaveDisableLevelNested();
   /* KB end osekHWosSaveDisableLevelNested */

   for(i=0;i<osdTraceDepth;i++)
   {
      COVER(0x0404)
      osTraceBuffer[i].stateNo=0xff;
      osTraceBuffer[i].taskNo=0xff;
#ifdef osdTraceUseTimestamp
      COVER(0x0405)
      osTraceBuffer[i].timeStamp=0;
#endif
   }
   COVER(0x0406)
   osTraceBufferIndex=0;

   /* KB begin osekHWosRestoreEnableLevelNested */
   osRestoreEnableLevelNested();
   /* KB end osekHWosRestoreEnableLevelNested */
}

/* print trace buffer to file */
/* needs to be customized for each target system */
#ifdef osdTraceUsePrintout
osqFunc1 void osqFunc2 osPrintTrace(void)  /* PRQA S 1503 */ /* API functions might be never called */
{
   /* CovComment 4:  Function: osPrintTrace */

/* KB begin osekPrintTraceBody */
   uint16 i;

   COVER(0x0407)
   for(i=osTraceBufferIndex;i<osdTraceDepth;i++)
   {
      COVER(0x0408)
#ifdef osdTraceUseTimestamp   
      COVER(0x0409)
      printf("%3d %3d %6d\n",osTraceBuffer[i].taskNo,osTraceBuffer[i].stateNo,osTraceBuffer[i].timeStamp);
#else
      COVER(0x040A)
      printf("%3d %3d\n",osTraceBuffer[i].taskNo,osTraceBuffer[i].stateNo);
#endif
   }

   for(i=0;i<osTraceBufferIndex;i++)
   {
      COVER(0x040B)
#ifdef osdTraceUseTimestamp   
      COVER(0x040C)
      printf("%3d %3d %6d\n",osTraceBuffer[i].taskNo,osTraceBuffer[i].stateNo,osTraceBuffer[i].timeStamp);
#else
      COVER(0x040D)
      printf("%3d %3d\n",osTraceBuffer[i].taskNo,osTraceBuffer[i].stateNo);
#endif
   }

/* KB end osekPrintTraceBody */
}
#endif

#endif /* osdUseTrace */

/* KB begin osekEndOfCModule */
#if (osdCompiler==osdCompilerGHSARM7)
#pragma ghs section bss=default
#pragma ghs section text=default
#pragma ghs section rodata=default
#endif
/* KB end osekEndOfCModule */
