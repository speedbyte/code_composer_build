/*****************************************************************************/
/* INTTYPES.H v4.1.4                                                         */
/* Copyright (c) 2002-2006 Texas Instruments Incorporated                    */
/*****************************************************************************/
#ifndef _INTTYPES_H_
#define _INTTYPES_H_

#include <stdint.h>

/* 7.8 Format conversion of integer types */

typedef struct { intmax_t quot, rem; } imaxdiv_t;

/* 
   According to footnotes in the 1999 C standard, "C++ implementations
   should define these macros only when __STDC_FORMAT_MACROS is defined
   before <inttypes.h> is included." 
*/
#if !defined(__cplusplus) || defined(__STDC_FORMAT_MACROS)

/* 7.8.1 Macros for format specifiers */

#define PRId8 		"d"
#define PRIi8 		"i"
#define PRIo8 		"o"
#define PRIu8 		"u"
#define PRIx8 		"x"
#define PRIX8 		"X"
#define SCNd8 		"hhd"
#define SCNi8 		"hhi"
#define SCNo8 		"hho"
#define SCNu8 		"hhu"
#define SCNx8 		"hhx"

#define PRIdLEAST8     	"d"
#define PRIiLEAST8     	"i"
#define PRIoLEAST8     	"o"
#define PRIuLEAST8     	"u"
#define PRIxLEAST8     	"x"
#define PRIXLEAST8     	"X"
#define PRIdFAST8     	"d"
#define PRIiFAST8     	"i"
#define PRIoFAST8     	"o"
#define PRIuFAST8     	"u"
#define PRIxFAST8     	"x"
#define PRIXFAST8     	"X"
#define PRId16		"d"
#define PRIi16		"i"
#define PRIo16		"o"
#define PRIu16		"u"
#define PRIx16		"x"
#define PRIX16		"X"
#define PRIdLEAST16    	"d"
#define PRIiLEAST16    	"i"
#define PRIoLEAST16    	"o"
#define PRIuLEAST16    	"u"
#define PRIxLEAST16    	"x"
#define PRIXLEAST16    	"X"
#define PRIdFAST16    	"d"
#define PRIiFAST16    	"i"
#define PRIoFAST16    	"o"
#define PRIuFAST16    	"u"
#define PRIxFAST16    	"x"
#define PRIXFAST16    	"X"

#define SCNdLEAST8 	"hhd"
#define SCNiLEAST8 	"hhi"
#define SCNoLEAST8 	"hho"
#define SCNuLEAST8 	"hhu"
#define SCNxLEAST8 	"hhx"
#define SCNdFAST8 	"d"
#define SCNiFAST8 	"i"
#define SCNoFAST8 	"o"
#define SCNuFAST8 	"u"
#define SCNxFAST8 	"x"
#define SCNd16		"hd"
#define SCNi16		"hi"
#define SCNo16		"ho"
#define SCNu16		"hu"
#define SCNx16		"hx"
#define SCNdLEAST16 	"hd"
#define SCNiLEAST16 	"hi"
#define SCNoLEAST16 	"ho"
#define SCNuLEAST16 	"hu"
#define SCNxLEAST16 	"hx"
#define SCNdFAST16 	"d"
#define SCNiFAST16 	"i"
#define SCNoFAST16 	"o"
#define SCNuFAST16 	"u"
#define SCNxFAST16 	"x"

#define PRId32		"d"
#define PRIi32		"i"
#define PRIo32		"o"
#define PRIu32		"u"
#define PRIx32		"x"
#define PRIX32		"X"
#define PRIdLEAST32    	"d"
#define PRIiLEAST32    	"i"
#define PRIoLEAST32    	"o"
#define PRIuLEAST32    	"u"
#define PRIxLEAST32    	"x"
#define PRIXLEAST32    	"X"
#define PRIdFAST32    	"d"
#define PRIiFAST32    	"i"
#define PRIoFAST32    	"o"
#define PRIuFAST32    	"u"
#define PRIxFAST32    	"x"
#define PRIXFAST32    	"X"
#define SCNd32		"d"
#define SCNi32		"i"
#define SCNo32		"o"
#define SCNu32		"u"
#define SCNx32		"x"
#define SCNdLEAST32    	"d"
#define SCNiLEAST32    	"i"
#define SCNoLEAST32    	"o"
#define SCNuLEAST32    	"u"
#define SCNxLEAST32    	"x"
#define SCNdFAST32    	"d"
#define SCNiFAST32    	"i"
#define SCNoFAST32    	"o"
#define SCNuFAST32    	"u"
#define SCNxFAST32    	"x"


#define PRId64		"lld"
#define PRIi64		"lli"
#define PRIo64		"llo"
#define PRIu64		"llu"
#define PRIx64		"llx"
#define PRIX64		"llX"
#define PRIdLEAST64    	"lld"
#define PRIiLEAST64    	"lli"
#define PRIoLEAST64    	"llo"
#define PRIuLEAST64    	"llu"
#define PRIxLEAST64    	"llx"
#define PRIXLEAST64    	"llX"
#define PRIdFAST64    	"lld"
#define PRIiFAST64    	"lli"
#define PRIoFAST64    	"llo"
#define PRIuFAST64    	"llu"
#define PRIxFAST64    	"llx"
#define PRIXFAST64    	"llX"
#define SCNd64		"lld"
#define SCNi64		"lli"
#define SCNo64		"llo"
#define SCNu64		"llu"
#define SCNx64		"llx"
#define SCNdLEAST64    	"lld"
#define SCNiLEAST64    	"lli"
#define SCNoLEAST64    	"llo"
#define SCNuLEAST64    	"llu"
#define SCNxLEAST64    	"llx"
#define SCNdFAST64    	"lld"
#define SCNiFAST64    	"lli"
#define SCNoFAST64    	"llo"
#define SCNuFAST64    	"llu"
#define SCNxFAST64    	"llx"

#define PRIdPTR		"d"
#define PRIiPTR		"i"
#define PRIoPTR		"o"
#define PRIuPTR		"u"
#define PRIxPTR		"x"
#define PRIXPTR		"X"
#define SCNdPTR		"d"
#define SCNiPTR		"i"
#define SCNoPTR		"o"
#define SCNuPTR		"u"
#define SCNxPTR		"x"

#define PRIdMAX		"lld"
#define PRIiMAX		"lli"
#define PRIoMAX		"llo"
#define PRIuMAX		"llu"
#define PRIxMAX		"llx"
#define PRIXMAX		"llX"
#define SCNdMAX		"lld"
#define SCNiMAX		"lli"
#define SCNoMAX		"llo"
#define SCNuMAX		"llu"
#define SCNxMAX		"llx"

#endif /* !defined(__cplusplus) || defined(__STDC_FORMAT_MACROS) */

/* 7.8.2 Functions for greatest-width integer types */
#include <linkage.h>

_CODE_ACCESS intmax_t  imaxabs(intmax_t j);
_CODE_ACCESS imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);
_CODE_ACCESS intmax_t  strtoimax(const char * restrict nptr, 
		    char ** restrict endptr, int base);
_CODE_ACCESS uintmax_t strtoumax(const char * restrict nptr, 
		    char ** restrict endptr, int base);

#endif /* _INTTYPES_H_ */
