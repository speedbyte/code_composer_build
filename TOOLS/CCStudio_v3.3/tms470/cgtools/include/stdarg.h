/*****************************************************************************/
/* stdarg.h   v4.1.4                                                         */
/* Copyright (c) 1996-2006 Texas Instruments Incorporated                    */
/*****************************************************************************/

#ifndef _STDARG
#define _STDARG

#ifdef __cplusplus
#define _NS_PREFIX std::
namespace std {
#else
#define _NS_PREFIX
#endif /* __cplusplus */

#ifdef __eabi__
typedef struct __va_list {
    void * __ap;
} va_list;
#else
typedef char *va_list;
#endif

#ifdef __cplusplus
} /* namespace std */
#endif

#define _VA_RNDUP(sz) 	(((sz) + 3) & ~3)
#define _VA_MASK(tp)     ((__ALIGNOF__(tp) == 8) ? 7 : 3)
#define _VA_ALN(ap,tp)  (((int)(ap) + _VA_MASK(tp)) & ~(_VA_MASK(tp)))

#ifdef __eabi__
#define va_start(ap, parmN)						      \
   ((ap).__ap = ((void *)((((int)__va_parmadr(parmN))&~3) +                   \
		                              _VA_RNDUP(sizeof(parmN)))))
#else
#define va_start(ap, parmN)						      \
   ((ap) = (__va_argref(parmN) 						      \
	    ? (_NS_PREFIX va_list)((int)__va_parmadr(parmN) + 4 )	      \
	    : (sizeof(parmN) < sizeof(int)) 				      \
	      ? (_NS_PREFIX va_list)((int)__va_parmadr(parmN) + 4 & ~3)	      \
	      : (_NS_PREFIX va_list)((int)__va_parmadr(parmN) + sizeof(parmN))))
#endif

#ifdef __big_endian__
#ifdef __eabi__
#define va_arg(_ap, _type)					 	      \
    (((_ap).__ap = (void*)(_VA_ALN((_ap).__ap, _type) +                       \
			                         _VA_RNDUP(sizeof(_type)))),  \
     (*(_type *)((int)(_ap).__ap -                                            \
		   (__va_argcsu(_type) ? _VA_RNDUP(sizeof(_type))	      \
		                       : (sizeof(_type))))))
#else
#define va_arg(_ap, _type)                                       	      \
        (__va_argref(_type)                                		      \
	 ? ((_ap += sizeof(_type*)),(**(_type**)(_ap-(sizeof(_type*)))))      \
	 : ((_ap += _VA_RNDUP(sizeof(_type))),(*(_type*)(_ap-(sizeof(_type))))))
#endif
#else
#ifdef __eabi__
#define va_arg(_ap, _type) 						      \
   (((_ap).__ap = (void *)(_VA_ALN((_ap).__ap,_type) +                        \
			   _VA_RNDUP(sizeof(_type)))),                        \
	     (*(_type *) ((int)(_ap).__ap - _VA_RNDUP(sizeof(_type)))))
#else
#define va_arg(_ap, _type)                                       	      \
        (__va_argref(_type)                         		              \
	 ? ((_ap += sizeof(_type*)), (**(_type**)(_ap- sizeof(_type*))))      \
	 : ((_ap += _VA_RNDUP(sizeof(_type))),				      \
	     (*(_type*)(_ap- _VA_RNDUP(sizeof(_type))))))
#endif
#endif

#define va_end(ap)

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::va_list;
#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#else

#ifdef __cplusplus

#ifndef _CPP_STYLE_HEADER
using std::va_list;
#endif /* _CPP_STYLE_HEADER */

#endif /* __cplusplus */

#endif /* _STDARG */
