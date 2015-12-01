/*****************************************************************************/
/* linkage.h   v4.1.4                                                        */
/* Copyright (c) 1998-2006 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _LINKAGE
#define _LINKAGE

/* No modifiers are needed to access code or data */

#define _CODE_ACCESS
#define _DATA_ACCESS

/*--------------------------------------------------------------------------*/
/* Define _IDECL ==> how inline functions are declared                      */
/*--------------------------------------------------------------------------*/
#ifdef _INLINE
#define _IDECL static __inline
#define _IDEFN static __inline
#define __INLINE static __inline
#else
#define _IDECL extern _CODE_ACCESS
#define _IDEFN _CODE_ACCESS
#define __INLINE __EXTERN
#endif

#endif /* ifndef _LINKAGE */
