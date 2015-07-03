#ifndef STDBOOL_H
#define STDBOOL_H

/***************************************************************************
 *                                                                         
 *  NAME:         $RCSfile: stdbool.h,v $, $Revision: 1$                                    
 *                                                                         
 *  FUNCTION:     Boolean type declarations (ANSI C99)                                       
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
 Revision 1.8  2003/04/22 13:32:34  duda
 modification history removed

*/
 
/*-----typedef-------------------------------------------------------------*/
typedef  unsigned char _Bool;        /* ANSIC 99 like                     */ 


/*-----defines-------------------------------------------------------------*/
#ifndef  true
#define  true          ((_Bool) 1)
#endif 
#ifndef  false 
#define  false         ((_Bool) 0)
#endif

#ifndef  TRUE
#define  TRUE          ((_Bool) 1)
#endif 
#ifndef  FALSE 
#define  FALSE         ((_Bool) 0)
#endif


#endif   /* STDBOOL_H */
