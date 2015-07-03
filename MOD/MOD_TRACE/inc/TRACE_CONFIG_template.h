/******************************************************************************/
/*****************  Copyright (c) 2010 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

#ifndef _TRACE_CONFIG_H
#define _TRACE_CONFIG_H


/* DEFINE MAXIMUM 64 TRACE MODULES, VALID ID'S ARE 0-63 */


typedef enum {
MOD_TRACE                =  0,     // nothing to do           -> done
MOD_UNUSED63             = 63       /* 63 = THIS IS THE LAST ONE !!!  */
} TRACE_MODULES;


/*------Start for MOD_TRACE ---------------*/
/* use a name without MOD_  */
#define TRACE_SWI_ERR  1
#define TRACE_SWI_WNG  1
#define TRACE_SWI_TRC  1
#define TRACE_SWI_DBG  1

/*------ End for MOD_TRACE   ---------------*/


#include "TRACE_CONFIG_GENERATED.h"

#endif /* _TRACE_CONFIG_H */

/* End Of File _TRACE_CONFIG_H */
