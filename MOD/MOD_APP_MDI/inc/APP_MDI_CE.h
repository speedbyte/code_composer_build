/******************************************************************************/
/*****************  Copyright (c) 2008 by S1nn GmbH & Co. KG  *****************/
/***************************  All Rights Reserved  ****************************/
/******************************************************************************/

/******************************************************************************/
/* File Name:   APP_MDI_CE.h
*-----------------------------------------------------------------------------
* Module Name:  APP_MDI
*-----------------------------------------------------------------------------
* Description:  
*-----------------------------------------------------------------------------
* $Date: 2011-06-15 14:52:02 +0200 (Mi, 15 Jun 2011) $
* $Rev: 20164 $
* $Author: nikolov $
* $URL: http://subversion-01/svn/SW_HW_System/Projects/VW_UHV-NAR/SW/MOD/MOD_APP_MDI/trunk/inc/APP_MDI_CE.h $
*-----------------------------------------------------------------------------
*/
/******************************************************************************/

#ifndef _APP_MDI_CE_H
#define _APP_MDI_CE_H

/*----------------------------------------------------------------------------*/
/* Public Manifest Constant Definitions                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Import configuration                                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Public Code Macros                                                         */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Public Constants                                                           */
/*----------------------------------------------------------------------------*/

#define APP_MDI_TOTAL_TRACK_TIME_INVALID    0xFFFFFFFF

typedef enum
{
/* 0 */    APP_MDI_STOP = 0,
/* 1 */    APP_MDI_PLAY,
/* 2 */    APP_MDI_PAUSE,
/* 3 */    APP_MDI_RESUME,
/* 4 */    APP_MDI_NEXT_SONG,
/* 5 */    APP_MDI_PREVIOUS_SONG,
/* 6 */    APP_MDI_FAST_FORWARD,
/* 7 */    APP_MDI_FAST_BACKWARD
} APP_MDI_enDiscoStatusValue;

typedef enum
{
/* 0 */    APP_MDI_ACT_STATE_PAUSE = 0,
/* 1 */    APP_MDI_ACT_STATE_PLAY,
/* 2 */    APP_MDI_ACT_STATE_FAST_FORWARD,
/* 3 */    APP_MDI_ACT_STATE_FAST_REWIND,
/* 4 */    APP_MDI_ACT_STATE_STOP,
/* 5 */    APP_MDI_ACT_STATE_INIT,
/* 5 */    APP_MDI_ACT_STATE_INVALID
} APP_MDI_enActiveStatusValue;


typedef enum
{
    MDI_RANDOM_DISABLE = 0,
    MDI_RANDOM_ENABLE  = 1,
    MDI_RANDOM_FOLDER_PLAYLISTS = 2
} APP_MDI_enRandomMode;

typedef enum
{
    MDI_AUDIO_LOCK_STOP = 0,
    MDI_AUDIO_LOCK_START
} APP_MDI_enAudioLock;

typedef enum
{
/* 0 */    APP_MDI_INIT  = 0,        // default init value
/* 1 */    APP_MDI_ONLY_A2DP,        // player does not support avrcp and metadata
/* 2 */    APP_MDI_AVRCP_BASE,       // player supports avrcp but no metadata
/* 3 */    APP_MDI_SONY_ERICCSON,    // supports avrcp and metadata specific
/* 4 */    APP_MDI_AVRCP_1_3,        // supports avrcp 1.3 and the associated metadata stuff
/* 5 */    APP_MDI_AVRCP_1_4         // supports avrcp 1.4 and the associated metadata stuff
} APP_MDI_enMetaDataType;

typedef enum
{
    MDI_REPEAT_DISABLE = 0,
    MDI_REPEAT_ONE_TRACK,
    MDI_REPEAT_ALL_TRACK,
    MDI_REPEAT_GROUP
} APP_MDI_enRepeatMode;

typedef enum
{
    MDI_AUTO_NOT_DISABLE = 0,
    MDI_AUTO_NOT_ENABLE
} APP_MDI_enAutoNotMode;

typedef enum
{
    APP_MDI_DEFER_INIT = 0,
    APP_MDI_DEFER_RESUME,
    APP_MDI_DEFER_PAUSE,
    APP_MDI_DEFER_FAST_FORWARD,
    APP_MDI_DEFER_FAST_BACKWARD
} APP_MDI_enDeferStatusValue;


typedef struct
{
    Uint8Type       length;
    ATMdiFileInfo   data;
}AppMdiFileInfoDataType;

typedef struct
{
    Uint8Type       length;
    ATMdiFileName   data;
}AppMdiFileNameDataType;


#endif /* _APP_MDI_CE_H */

/* End Of File APP_MDI_CE.h */
