/*****************************************************************************/
/* ctype.h    v4.1.4                                                         */
/* Copyright (c) 1993-2006 Texas Instruments Incorporated                    */
/*****************************************************************************/

/************************************************************************/
/*                                                                      */
/*  CHARACTER TYPING FUNCTIONS AND MACROS                               */
/*                                                                      */
/*  Note that in this implementation, either macros or functions may    */
/*  be used.  Macros are prefixed with an underscore.                   */
/*                                                                      */
/************************************************************************/
#ifndef _CTYPE
#define _CTYPE

#include <linkage.h>

#ifdef __cplusplus
//----------------------------------------------------------------------------
// <cctype> IS RECOMMENDED OVER <ctype.h>.  <ctype.h> IS PROVIDED FOR
// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
//----------------------------------------------------------------------------
extern "C" namespace std
{
#endif /* __cplusplus */

extern _DATA_ACCESS const unsigned char _ctypes_[];

/************************************************************************/
/*   FUNCTION DECLARATIONS                                              */
/************************************************************************/
_IDECL int isalnum(int _c);
_IDECL int isalpha(int _c);
_IDECL int iscntrl(int _c);
_IDECL int isdigit(int _c);
_IDECL int isgraph(int _c);
_IDECL int islower(int _c);
_IDECL int isprint(int _c);
_IDECL int ispunct(int _c);
_IDECL int isspace(int _c);
_IDECL int isupper(int _c);
_IDECL int isxdigit(int _c);
_IDECL int isascii(int _c);

_IDECL int toupper(int _c);
_IDECL int tolower(int _c);
_IDECL int toascii(int _c);

#ifdef __cplusplus
} /* extern "C" namespace std */

#ifndef _CPP_STYLE_HEADER
using std::_ctypes_;
using std::isalnum;
using std::isalpha;
using std::iscntrl;
using std::isdigit;
using std::isgraph;
using std::islower;
using std::isprint;
using std::ispunct;
using std::isspace;
using std::isupper;
using std::isxdigit;
using std::isascii;
using std::toupper;
using std::tolower;
using std::toascii;
#endif /* ! _CPP_STYLE_HEADER */

#endif /* __cplusplus */

/************************************************************************/
/*  MACRO DEFINITIONS                                                   */
/************************************************************************/
#define _U_   0x01
#define _L_   0x02
#define _N_   0x04
#define _S_   0x08
#define _P_   0x10
#define _C_   0x20
#define _H_   0x40
#define _B_   0x80

#define _isalnum(a)  (_ctypes_[(a)+1] & (_U_ | _L_ | _N_))
#define _isalpha(a)  (_ctypes_[(a)+1] & (_U_ | _L_))
#define _iscntrl(a)  (_ctypes_[(a)+1] & _C_)
#define _isdigit(a)  (_ctypes_[(a)+1] & _N_)
#define _isgraph(a)  (_ctypes_[(a)+1] & (_U_ | _L_ | _N_ | _P_))
#define _islower(a)  (_ctypes_[(a)+1] & _L_)
#define _isprint(a)  (_ctypes_[(a)+1] & (_B_ | _U_ | _L_ | _N_ | _P_))
#define _ispunct(a)  (_ctypes_[(a)+1] & _P_)
#define _isspace(a)  (_ctypes_[(a)+1] & _S_)
#define _isupper(a)  (_ctypes_[(a)+1] & _U_)
#define _isxdigit(a) (_ctypes_[(a)+1] & _H_)
#define _isascii(a)  (((a) & ~0x7F) == 0)

#define _toupper(b)  ((_islower(b)) ? (b) - ('a' - 'A') : (b))
#define _tolower(b)  ((_isupper(b)) ? (b) + ('a' - 'A') : (b))
#define _toascii(a)  ((a) & 0x7F)

#ifdef _INLINE

#ifdef __cplusplus
using std::_ctypes_;
namespace std {
#endif /* __cplusplus */

static __inline int isalnum(int c)  { return(_isalnum(c)); }
static __inline int isalpha(int c)  { return(_isalpha(c)); }
static __inline int isascii(int c)  { return(_isascii(c)); }
static __inline int iscntrl(int c)  { return(_iscntrl(c)); }
static __inline int isdigit(int c)  { return(_isdigit(c)); }
static __inline int isgraph(int c)  { return(_isgraph(c)); }
static __inline int islower(int c)  { return(_islower(c)); }
static __inline int isprint(int c)  { return(_isprint(c)); }
static __inline int ispunct(int c)  { return(_ispunct(c)); }
static __inline int isspace(int c)  { return(_isspace(c)); }
static __inline int isupper(int c)  { return(_isupper(c)); }
static __inline int isxdigit(int c) { return(_isxdigit(c));}
static __inline int toascii(int c)  { return(_toascii(c)); }

/****************************************************************************/
/*  tolower                                                                 */
/****************************************************************************/
static __inline int tolower(int ch)
{
   /*
    This code depends on two assumptions: (1) all of the letters of the
    alphabet of a given case are contiguous, and (2) the lower and upper
    case forms of each letter are displaced from each other by the same
    constant value.
   */

   if ( (unsigned int)(ch - 'A') <= (unsigned int)('Z' - 'A')) ch += 'a' - 'A';
   return ch;
}

/****************************************************************************/
/*  toupper                                                                 */
/****************************************************************************/
static __inline int toupper(int ch)
{
   /*
    This code depends on two assumptions: (1) all of the letters of the
    alphabet of a given case are contiguous, and (2) the lower and upper
    case forms of each letter are displaced from each other by the same
    constant value.
   */

   if ( (unsigned int)(ch - 'a') <= (unsigned int)('z' - 'a')) ch -= 'a' - 'A';
   return ch;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _INLINE */

#else

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::_ctypes_;
using std::isalnum;
using std::isalpha;
using std::iscntrl;
using std::isdigit;
using std::isgraph;
using std::islower;
using std::isprint;
using std::ispunct;
using std::isspace;
using std::isupper;
using std::isxdigit;
using std::isascii;
using std::toupper;
using std::tolower;
using std::toascii;
#endif /* ! _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#endif /* ! _CTYPE */
