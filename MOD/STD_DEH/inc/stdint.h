#ifndef STDINT_H
  #define STDINT_H


/***************************************************************************
 *                                                                         
 *  NAME:         $RCSfile: stdint.h,v $, $Revision: 1$                                    
 *                                                                         
 *  FUNCTION:     Int type declarations (ANSI C99)                                       
 *                                                                        
 *                                                                         
 *                                                                         
 *                                                                         
 *  PROJECT:      VW Standard Software Platform            
 *                                                                         
 *  TOOLCHAIN:    TASKING C166/ST10 v7.5r2 compiler                        
 *                                                                         
 *  Copyright (c) 2002 by 3SOFT GmbH                                       
 *  - all rights reserved -                                                
 *                                                                         
 ***************************************************************************
 modification history:
 ---------------------
 $Log:
  1     EFF2-06023 1.0          30.01.2008 10:29:26     Ingo Gensert    
 $
---------------------------------------------------------------------------*/

/*-----typedef-------------------------------------------------------------*/
typedef signed char     int8_t;
typedef signed int      int16_t;
typedef signed long     int32_t;
#ifndef BAP_USER_STDTYPES_H
    typedef unsigned char   uint8_t;
    typedef unsigned short int    uint16_t;
    typedef unsigned long   uint32_t;
#endif
typedef signed int      int_fast8_t;
typedef signed int      int_fast16_t;
typedef signed long     int_fast32_t;
typedef unsigned int    uint_fast8_t;
typedef unsigned int    uint_fast16_t;
typedef unsigned long   uint_fast32_t;
typedef signed long     intmax_t;
typedef unsigned long   uintmax_t;


#endif   /* STDINT_H */
