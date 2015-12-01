/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:     POOL_GENERATED.h
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

/* Control Structure for Pools */
typedef struct POOL_stGenControl {
   Uint32Type noOfElements;
   Uint32Type elementSize;
   Uint8Type *addr;
} POOL_stGenControl;

/* Enumerations for Pool Names */
typedef enum {
!!loop
   POOL_en<NAME>,  /* Pool name */
!!endloop
   POOL_enLAST    /* Last entry,must only be used internally in pool handling.*/
} POOL_enPoolNames;

/* Enumerations for Pools Elements */
typedef enum {
!!loop
   POOL_en<NAME>Elements  = <COUNT>, /** Num of elem for pool <NAME>*/
!!endloop
   POOL_enLASTSIZE    /** Last entry.*/
} POOL_enPoolElements;


/* Hash defines for Pools Element Size */
!!loop
#  include "<INCLUDE>"
#define  POOL_<NAME>Size   sizeof(<STRUCTURE>) // sizeof pool element
!!endloop

!!loop

/* Structure for Pool */
typedef struct POOL_st<NAME>Element {
//#ifdef POOL_START_PATTERN
	 Uint32Type  startMarker;
//#endif
	 Uint8Type    content[POOL_<NAME>Size];
#ifdef POOL_END_PATTERN
	 Uint32Type  endMarker;
#endif
} POOL_st<NAME>Element;
!!endloop


/* Extern Definition for Pools */
!!loop
extern POOL_st<NAME>Element  POOL_st<NAME>[POOL_en<NAME>Elements];
!!endloop

extern POOL_stGenControl poolGenControl[POOL_enLAST];
// =================================== end of POOL_GENERATED.h ===
