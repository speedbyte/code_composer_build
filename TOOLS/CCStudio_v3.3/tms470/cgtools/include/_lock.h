/*****************************************************************************/
/*  _lock.h v4.1.4                                                           */
/*  Copyright (c) 2000-2006 Texas Instruments Incorporated                   */
/*****************************************************************************/

#ifndef __LOCK_H
#define __LOCK_H

#include <linkage.h>

#ifdef __cplusplus
extern "C" namespace std {
#endif

_CODE_ACCESS void _nop();

extern _DATA_ACCESS void (  *_lock)();
extern _DATA_ACCESS void (*_unlock)();

_CODE_ACCESS void _register_lock  (void (  *lock)());
_CODE_ACCESS void _register_unlock(void (*unlock)());

#ifdef __cplusplus
} /* extern "C" namespace std */
#endif

#endif /* __LOCK_H */
