#******************************************************************************/
#*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
#***************************  All Rights Reserved  ****************************/
#******************************************************************************/
#
#******************************************************************************/
#* File Name:     POOL_FLAGS.mk
# -----------------------------------------------------------------------------
#  Module Name: Pool handling
# -----------------------------------------------------------------------------
#  Description: Contains the generated data areas for the pool handling
# -----------------------------------------------------------------------------
#  $Date: 2009-03-16 10:34:10 +0100 (Mo, 16 Mrz 2009) $
#  $Rev: 5237 $
#  $Author: schmidt $
#  $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_POOL/trunk/inc/POOL_FLAGS.mk $
# -----------------------------------------------------------------------------
#
   # CFLAGS += -DPOOL_ADDRESS_RANGE
   # CFLAGS += -DPOOL_END_PATTERN
   # CFLAGS += -DPOOL_FREE
     CFLAGS += -DPOOL_INITIALIZATION_AT_ALLOCATE
     CFLAGS += -DPOOL_START_PATTERN
// ======================================= end of POOL_FLAGS.mk ===
