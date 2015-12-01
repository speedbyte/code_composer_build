/*****************************************************************************/
/* float.h    v4.1.4                                                         */
/* Copyright (c) 1996-2006 Texas Instruments Incorporated                    */
/*****************************************************************************/

/********************************************************************/
/*    KEY:  FLT_     - APPLIES TO TYPE FLOAT                        */
/*          DBL_     - APPLIES TO TYPE DOUBLE                       */
/*          LDBL_    - APPLIES TO TYPE LONG DOUBLE                  */
/********************************************************************/
#ifndef _FLOAT
#define _FLOAT

#define FLT_RADIX                         2   /* RADIX OF EXPONENT         */
#define FLT_ROUNDS                        1   /* TRUE IF ADDITION ROUNDS   */

#define FLT_DIG                           6   /* DECIMAL PRECISION         */
#define FLT_MANT_DIG                     24   /* BITS IN MANTISSA          */
#define FLT_MIN_EXP                    -125   /* SMALLEST EXPONENT         */
#define FLT_MAX_EXP                     128   /* LARGEST EXPONENT          */
#define FLT_MIN_10_EXP                  -37   /* MIN POWER OF 10           */
#define FLT_MAX_10_EXP                   38   /* MAX POWER OF 10           */
#define FLT_EPSILON        1.192092896E-07F   /* SMALLEST X WHERE 1+X != 1 */
#define FLT_MIN            1.175494351E-38F   /* SMALLEST POSITIVE VALUE   */
#define FLT_MAX            3.402823466E+38F   /* LARGEST POSITIVE VALUE    */

#define DBL_DIG                          15   /* DECIMAL PRECISION         */
#define DBL_MANT_DIG                     53   /* BITS IN MANTISSA          */
#define DBL_MIN_EXP                   -1021   /* SMALLEST EXPONENT         */
#define DBL_MAX_EXP                    1024   /* LARGEST EXPONENT          */
#define DBL_MIN_10_EXP                 -307   /* MIN POWER OF 10           */
#define DBL_MAX_10_EXP                  308   /* MAX POWER OF 10           */
#define DBL_EPSILON  2.2204460492503131E-16   /* SMALLEST X WHERE 1+X != 1 */
#define DBL_MIN     2.2250738585072014E-308   /* SMALLEST POSITIVE VALUE   */
#define DBL_MAX     1.7976931348623157E+308   /* LARGEST POSITIVE VALUE    */
 
#define LDBL_DIG                         15   /* DECIMAL PRECISION         */
#define LDBL_MANT_DIG                    53   /* BITS IN MANTISSA          */
#define LDBL_MIN_EXP                  -1021   /* SMALLEST EXPONENT         */
#define LDBL_MAX_EXP                   1024   /* LARGEST EXPONENT          */
#define LDBL_MIN_10_EXP                -307   /* MIN POWER OF 10           */
#define LDBL_MAX_10_EXP                 308   /* MAX POWER OF 10           */
#define LDBL_EPSILON 2.2204460492503131E-16   /* SMALLEST X WHERE 1+X != 1 */
#define LDBL_MIN    2.2250738585072014E-308   /* SMALLEST POSITIVE VALUE   */
#define LDBL_MAX    1.7976931348623157E+308   /* LARGEST POSITIVE VALUE    */

#endif
