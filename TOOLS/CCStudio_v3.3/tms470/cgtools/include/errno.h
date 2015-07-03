/*****************************************************************************/
/*  ERRNO.H  v4.1.4                                                          */
/*  Copyright (c) 1995-2006 Texas Instruments Incorporated                   */
/*****************************************************************************/

#ifndef _ERRNO
#define _ERRNO

#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cerrno> IS RECOMMENDED OVER <errno.h>.  <errno.h> IS PROVIDED FOR
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------
extern "C" namespace std
{
#endif /* __cplusplus */

#include <linkage.h>

#define errno errno
extern _DATA_ACCESS int errno;

#define EDOM   1
#define ERANGE 2
#define ENOENT 3
#define EFPOS  5
#define EILSEQ 6

#ifdef __cplusplus
} /* extern "C" namespace std */

#ifndef _CPP_STYLE_HEADER
using std::errno;
#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#else

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::errno;
#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#endif  /* _ERRNO */
