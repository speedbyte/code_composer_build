/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_COMMON_POOL.h
*-----------------------------------------------------------------------------
* Module Name:  APP_COMMON    
*-----------------------------------------------------------------------------
* Description:  pool type definitions for all applications 
*-----------------------------------------------------------------------------
* $Date: 2009-03-16 10:43:03 +0100 (Mo, 16 Mrz 2009) $
* $Rev: 5248 $
* $Author: schmidt $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_COMMON/trunk/inc/APP_COMMON_Pool.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_COMMON_POOL_H
#define _APP_COMMON_POOL_H

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Libraries from external Vendors                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Own Header Files                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

#define MAX_BAP_REQUEST_SIZE                    1548 //1545
#define MAX_BAP_INDICATION_SIZE_APP_PHONE       100  
#define MAX_BAP_INDICATION_SIZE_APP_PB          60   //payload max 51  
#define MAX_BAP_INDICATION_SIZE_APP_PB_LARGE    850  //payload max 841  
#define MAX_BAP_INDICATION_SIZE_APP_MDI         40  
#define MAX_BAP_INDICATION_SIZE_KPB             10  
#define MAX_BAP_ARRAY_HEADER_SIZE               5  
#define MAX_BAP_MD_ACTIVE_SIZE                  32 

/*----------------------------------------------------------------------------*/
/* Public Types                                                               */
/*----------------------------------------------------------------------------*/

typedef struct
{
    Uint8Type data[MAX_BAP_REQUEST_SIZE];
}BapReqDataType;

typedef struct
{
    Uint8Type data[MAX_BAP_INDICATION_SIZE_APP_PHONE];
}   BapIndDataTypeAppPhone;

typedef struct
{
    Uint8Type data[MAX_BAP_INDICATION_SIZE_APP_PB];
}BapIndDataTypeAppPB;

typedef struct
{
    Uint8Type data[MAX_BAP_INDICATION_SIZE_APP_PB_LARGE];
}BapIndDataTypeAppPBLarge;

typedef struct
{
    Uint8Type data[MAX_BAP_INDICATION_SIZE_APP_MDI];
}BapIndDataTypeAppMdi;

typedef struct
{
    Uint8Type data[MAX_BAP_INDICATION_SIZE_KPB];
}BapIndDataTypeKpb;

typedef struct
{
    Uint8Type data[MAX_BAP_ARRAY_HEADER_SIZE];
}BapArrayHeaderDataType;

/* types are used for the pool handler */
//typedef Uint8Type    BapReqDataType[MAX_BAP_REQUEST_SIZE];
//typedef Uint8Type    BapIndDataTypeAppPhone[MAX_BAP_INDICATION_SIZE_APP_PHONE];
//typedef Uint8Type    BapIndDataTypeAppPB[MAX_BAP_INDICATION_SIZE_APP_PB];
//typedef Uint8Type    BapIndDataTypeAppMdi[MAX_BAP_INDICATION_SIZE_APP_MDI];
//typedef Uint8Type    BapArrayHeaderDataType[MAX_BAP_ARRAY_HEADER_SIZE];

/*----------------------------------------------------------------------------*/
/* Global Variables Declarations (avoid as much as possible!)                 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Methods                                                             */
/*----------------------------------------------------------------------------*/


#endif /* _APP_COMMON_POOL_H */

/* End Of File APP_COMMON_POOL.h */

