/*****************************************************************************/
/* stdlib.h   v4.1.4                                                         */
/* Copyright (c) 1993-2006 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _STDLIB
#define _STDLIB

#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cstdlib> IS RECOMMENDED OVER <stdlib.h>.  <stdlib.h> IS PROVIDED FOR 
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------
extern "C" namespace std {
#endif /* !__cplusplus */

typedef struct { int quot, rem; } div_t;

typedef struct { int quot, rem; } ldiv_t;

#define _LLONG_AVAILABLE 1
typedef struct { long long quot, rem; } lldiv_t;

#define MB_CUR_MAX    1

#ifndef NULL
#define NULL          0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_T_TYPE__ size_t;
#endif

#ifndef __cplusplus
#ifndef _WCHAR_T
#define _WCHAR_T

typedef unsigned short wchar_t;

#endif /* _WCHAR_T */
#endif /* ! __cplusplus */

#define EXIT_FAILURE  1
#define EXIT_SUCCESS  0

#define RAND_MAX      32767

#include <linkage.h>

/*---------------------------------------------------------------*/
/* NOTE - Normally, abs, labs, and fabs are expanded inline, so  */
/*        no formal definition is really required. However, ANSI */
/*        requires that they exist as separate functions, so     */
/*        they are supplied in the library.  The prototype is    */
/*        here mainly for documentation.                         */
/*---------------------------------------------------------------*/
    _CODE_ACCESS  int       abs(int _val); 
    _CODE_ACCESS  long      labs(long _val);
#if defined(_LLONG_AVAILABLE)
    _CODE_ACCESS  long long llabs(long long _val);
#endif
        
    _CODE_ACCESS int       atoi(const char *_st);
    _CODE_ACCESS long      atol(const char *_st);
#if defined(_LLONG_AVAILABLE)
    _CODE_ACCESS long long atoll(const char *_st);
#endif
    _CODE_ACCESS int       ltoa(long val, char *buffer);
          _IDECL double    atof(const char *_st);

    _CODE_ACCESS long      strtol(const char *_st, char **_endptr, int _base);
    _CODE_ACCESS unsigned long strtoul(const char *_st, char **_endptr,
    					  int _base);
#if defined(_LLONG_AVAILABLE)
    _CODE_ACCESS long long strtoll(const char *_st, char **_endptr, int _base);
    _CODE_ACCESS unsigned long long strtoull(const char *_st, char **_endptr,
					     int _base);
#endif
    _CODE_ACCESS double    strtod(const char *_st, char **_endptr);
    
    _CODE_ACCESS int    rand(void);
    _CODE_ACCESS void   srand(unsigned _seed);
    
    _CODE_ACCESS void  *calloc(size_t _num, size_t _size);
    _CODE_ACCESS void  *malloc(size_t _size);
    _CODE_ACCESS void  *realloc(void *_ptr, size_t _size);
    _CODE_ACCESS void   free(void *_ptr);
    _CODE_ACCESS void  *memalign(size_t _aln, size_t _size);
    
    _CODE_ACCESS void   abort(void); 
    _CODE_ACCESS int    atexit(void (*_func)(void));
    _CODE_ACCESS void  *bsearch(const void *_key, const void *_base,
    				   size_t _nmemb, size_t _size, 
    			           int (*compar)(const void *,const void *));
    _CODE_ACCESS void   qsort(void *_base, size_t _nmemb, size_t _size, 
    			         int (*_compar)(const void *, const void *));
    _CODE_ACCESS void   exit(int _status);
    
    _CODE_ACCESS div_t  div(int _numer, int _denom);
    _CODE_ACCESS ldiv_t ldiv(long _numer, long _denom);
#if defined(_LLONG_AVAILABLE)
    _CODE_ACCESS lldiv_t lldiv(long long _numer, long long _denom);
#endif

    _CODE_ACCESS char  *getenv(const char *_string);
    _CODE_ACCESS int    system(const char *_name);

#ifdef __cplusplus
} /* extern "C" namespace std */

#ifndef _CPP_STYLE_HEADER
using std::div_t;
using std::ldiv_t;
#if defined(_LLONG_AVAILABLE)
using std::lldiv_t;
#endif
using std::size_t;
using std::abs;
using std::labs;
using std::atoi;
using std::atol;
#if defined(_LLONG_AVAILABLE)
using std::llabs;
using std::atoll;
#endif
using std::atof;
using std::strtol;
using std::strtoul;
#if defined(_LLONG_AVAILABLE)
using std::strtoll;
using std::strtoull;
#endif
using std::strtod;
using std::rand;
using std::srand;
using std::calloc;
using std::malloc;
using std::realloc;
using std::free;
using std::memalign;
using std::abort;
using std::atexit;
using std::bsearch;
using std::qsort;
using std::exit;
using std::div;
using std::ldiv;
#if defined(_LLONG_AVAILABLE)
using std::lldiv;
#endif
using std::getenv;
using std::system;
#endif /* ! _CPP_STYLE_HEADER */

#endif /* __cplusplus */


#ifdef _INLINE

#ifdef __cplusplus
namespace std {
#endif

static __inline double atof(const char *_st) 
{
  return strtod(_st, (char **)0); 
}

#ifdef __cplusplus
} /* namespace std */
#endif

#endif  /* _INLINE */

#else

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::div_t;
using std::ldiv_t;
#if defined(_LLONG_AVAILABLE)
using std::lldiv_t;
#endif
using std::size_t;
using std::abs;
using std::labs;
using std::atoi;
using std::atol;
#if defined(_LLONG_AVAILABLE)
using std::llabs;
using std::atoll;
#endif
using std::atof;
using std::strtol;
using std::strtoul;
#if defined(_LLONG_AVAILABLE)
using std::strtoll;
using std::strtoull;
#endif
using std::strtod;
using std::rand;
using std::srand;
using std::calloc;
using std::malloc;
using std::realloc;
using std::free;
using std::memalign;
using std::abort;
using std::atexit;
using std::bsearch;
using std::qsort;
using std::exit;
using std::div;
using std::ldiv;
#if defined(_LLONG_AVAILABLE)
using std::lldiv;
#endif
using std::getenv;
using std::system;
#endif /* ! _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#endif  /* ! _STDLIB */
