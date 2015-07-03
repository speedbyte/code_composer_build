/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL_ARRAY.c
 *-----------------------------------------------------------------------------
 * Module Name: Pool handling
 *-----------------------------------------------------------------------------
 * Description: Contains the generated data areas for the pool handling
 *-----------------------------------------------------------------------------
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 *-----------------------------------------------------------------------------
*/
#include "POOL.h"

!!loop
POOL_st<NAME>Element POOL_st<NAME>[POOL_en<NAME>Elements];
!!endloop

POOL_stGenControl poolGenControl[POOL_enLAST] = {
!!loop
{POOL_en<NAME>Elements,POOL_<NAME>Size,(Uint8Type*)POOL_st<NAME>},
!!endloop
};
// ======================================= end of POOL_ARRAY.c ===
