#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "CUnit.h"
#include "Basic.h"
#include "Automated.h"
#include "TestRun.h"

#include "APP_MDI.h"
#include "APP_MDI_CI.h"
#include "APP_MDI_ERM.h"
#include "APP_MDI_AT.h"

extern AppMdiContext gAppMdiContext;
extern AT_DeviceId STUB_ERM_ActivePlayerdeviceId;
extern ERMBTDeviceTableType    STUB_ERM_ERM_stBTDeviceTable[AT_MAX_DEVICE_ID+1];

typedef enum
{
	UHV_MODULE_TEST_SUCCESS = 0,
	UHV_MODULE_TEST_FAILED,
	UHV_MODULE_TEST_FRAMEWORK_ERROR
}enReturnCode;

int suite_init_APP_MDI_BUSY_RESPONSE(void) 
{ 
	gAppMdiContext.fct.common.fctType = APP_MDI_KEY_EVENT;
	return 0; 
}
int suite_clean_APP_MDI_BUSY_RESPONSE(void) 
{ 
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	return 0; 
}


void test_APP_MDI_BUSY_RESPONSE_ActivateMdiDevice(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_sfActivateMdiDevice(NULL, 0xFF));  
}

void test_APP_MDI_BUSY_RESPONSE_DeactivateMdiDevice(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_sfDeactivateMdiDevice(NULL));  
}

void test_APP_MDI_BUSY_RESPONSE_SetPlayMode(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_sfSetPlayMode(NULL, APP_MDI_DISCO_RESUME)); 
}

void test_APP_MDI_BUSY_RESPONSE_SetRepeatMode(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_sfSetRepeatMode(NULL, 0xFF));  
}

void test_APP_MDI_BUSY_RESPONSE_SetRandomMode(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_sfSetRandomMode(NULL, 0xFF));  
}

void test_APP_MDI_BUSY_RESPONSE_SkipForward(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_sfSkipForward(NULL, 0xFF, 0));  
}

void test_APP_MDI_BUSY_RESPONSE_SkipBackward(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_sfSkipBackward(NULL, 0xFF, 0));  
}

void test_APP_MDI_BUSY_RESPONSE_SetPlayerEnabled(void) 
{ 
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == APP_MDI_rcSetPlayerEnabled(NULL, BT_TRUE));  
}

void test_APP_MDI_BUSY_RESPONSE_RequestDgps(void)
{
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == _APP_MDI_sfRequestDgps(NULL, 0xFF));  
}

void test_APP_MDI_BUSY_RESPONSE_RequestDged(void)
{
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == _APP_MDI_sfRequestDged(NULL, 0xFF));  
}

void test_APP_MDI_BUSY_RESPONSE_SetAutoNotMode(void)
{
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY == _APP_MDI_vSetAutomaticNotificationMode(NULL, 0xFF));  
}

CU_TestInfo tests_APP_MDI_BUSY_RESPONSE[] = {
  { "test_APP_MDI_ActivateMdiDevice",	test_APP_MDI_BUSY_RESPONSE_ActivateMdiDevice },
  { "test_APP_MDI_DeactivateMdiDevice",	test_APP_MDI_BUSY_RESPONSE_DeactivateMdiDevice },
  { "test_APP_MDI_SetPlayMode",			test_APP_MDI_BUSY_RESPONSE_SetPlayMode },
  { "test_APP_MDI_SetRepeatMode",		test_APP_MDI_BUSY_RESPONSE_SetRepeatMode },
  { "test_APP_MDI_SetRandomMode",		test_APP_MDI_BUSY_RESPONSE_SetRandomMode },
  { "test_APP_MDI_SkipForward",			test_APP_MDI_BUSY_RESPONSE_SkipForward },
  { "test_APP_MDI_SetPlayerEnabled",	test_APP_MDI_BUSY_RESPONSE_SetPlayerEnabled },
  { "test_APP_MDI_RequestDgps",			test_APP_MDI_BUSY_RESPONSE_RequestDgps },
  { "test_APP_MDI_RequestDged",			test_APP_MDI_BUSY_RESPONSE_RequestDged },
  { "test_APP_MDI_SetAutoNotMode",		test_APP_MDI_BUSY_RESPONSE_SetAutoNotMode },
	CU_TEST_INFO_NULL,
};

int suite_init_APP_MDI_GET_VALUES(void) 
{ 
	APP_MDI_sfInitApp();
	return 0; 
}
int suite_clean_APP_MDI_GET_VALUES(void) 
{ 
	suite_init_APP_MDI_GET_VALUES();
	return 0; 
}

void test_APP_MDI_GET_VALUES_bIsInitialized(void)
{
	// after init this should be not initialized	
	CU_ASSERT(BT_FALSE == APP_MDI_bIsInitialized());

	// set it to TRUE
	gAppMdiContext.btModuleInitialized = BT_TRUE;
	CU_ASSERT(BT_TRUE == APP_MDI_bIsInitialized());
}

void test_APP_MDI_GET_VALUES_bPlayerAvailable(void)
{
	// after init this should be not available	
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerAvailable());	
}

void test_APP_MDI_GET_VALUES_bRemoteControlAvailable(void)
{
	// after init this should be not available	
	CU_ASSERT(BT_FALSE == APP_MDI_bRemoteControlAvailable());	
}

void test_APP_MDI_GET_VALUES_bPlayerActive(void)
{
	
	// after init player state should be not active	
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());	

	// now set play state to different values and check return value
	// should be true for play and resume, else false
	gAppMdiContext.playStatus = APP_MDI_PLAY;
	CU_ASSERT(BT_TRUE == APP_MDI_bPlayerActive());	

	gAppMdiContext.playStatus = APP_MDI_RESUME;
	CU_ASSERT(BT_TRUE == APP_MDI_bPlayerActive());	

	gAppMdiContext.playStatus = APP_MDI_STOP;  
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());	

    gAppMdiContext.playStatus = APP_MDI_PAUSE;    
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());	

    gAppMdiContext.playStatus = APP_MDI_NEXT_SONG;
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());	

    gAppMdiContext.playStatus = APP_MDI_PREVIOUS_SONG;
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());	

    gAppMdiContext.playStatus = APP_MDI_FAST_FORWARD;
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());	

    gAppMdiContext.playStatus = APP_MDI_FAST_BACKWARD;
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());	

	APP_MDI_sfInitApp();

}

void test_APP_MDI_GET_VALUES_GetActiveServiceDeviceId(void)
{
	// after init this should be an invalid device id	
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK));	
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_AVRCP));	
}

void test_APP_MDI_GET_VALUES_GetPlayStatus(void)
{
	APP_MDI_sfInitApp();

	// after init this should be se4t to PAUSE	
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());	

	// now set different values and check if function returns the correct values	
	gAppMdiContext.playStatus = APP_MDI_PLAY;
	CU_ASSERT(APP_MDI_PLAY == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_RESUME;
	CU_ASSERT(APP_MDI_RESUME == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_STOP;  
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

    gAppMdiContext.playStatus = APP_MDI_PAUSE;    
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());	

    gAppMdiContext.playStatus = APP_MDI_NEXT_SONG;
	CU_ASSERT(APP_MDI_NEXT_SONG == APP_MDI_GetPlayStatus());	

    gAppMdiContext.playStatus = APP_MDI_PREVIOUS_SONG;
	CU_ASSERT(APP_MDI_PREVIOUS_SONG == APP_MDI_GetPlayStatus());	

    gAppMdiContext.playStatus = APP_MDI_FAST_FORWARD;
	CU_ASSERT(APP_MDI_FAST_FORWARD == APP_MDI_GetPlayStatus());	

    gAppMdiContext.playStatus = APP_MDI_FAST_BACKWARD;
	CU_ASSERT(APP_MDI_FAST_BACKWARD == APP_MDI_GetPlayStatus());

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetRandomMode(void)
{
	// after init this should be disbaled	
	CU_ASSERT(MDI_RANDOM_DISABLE == APP_MDI_GetRandomMode());	

	// now set other values and check whether function returns correct values
	gAppMdiContext.random = MDI_RANDOM_ENABLE;
	CU_ASSERT(MDI_RANDOM_ENABLE == APP_MDI_GetRandomMode());	

	APP_MDI_sfInitApp();
}
	
void test_APP_MDI_GET_VALUES_GetRepeatMode(void)
{
	// after init this should be disbaled	
	CU_ASSERT(MDI_REPEAT_DISABLE == APP_MDI_GetRepeatMode());	

	// now set other values and check whether function returns correct values
	gAppMdiContext.repeat = MDI_REPEAT_ONE_TRACK;
	CU_ASSERT(MDI_REPEAT_ONE_TRACK == APP_MDI_GetRepeatMode());	

    gAppMdiContext.repeat = MDI_REPEAT_ALL_TRACK;
	CU_ASSERT(MDI_REPEAT_ALL_TRACK == APP_MDI_GetRepeatMode());	

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetTrackTime(void)
{
	// after init this should be disbaled	
	CU_ASSERT(0x00 == APP_MDI_GetTrackTime());	

	// now set other values and check whether function returns correct values
	gAppMdiContext.trackTime = 0x12345678;
	CU_ASSERT(0x12345678 == APP_MDI_GetTrackTime());	

    gAppMdiContext.trackTime = 0xFFFFFFFE;
	CU_ASSERT(0xFFFFFFFE == APP_MDI_GetTrackTime());	

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetTrackTimePosition(void)
{
	// after init this should be disbaled	
	CU_ASSERT(0x00 == APP_MDI_GetTrackTimePosition());	

	// now set other values and check whether function returns correct values
	gAppMdiContext.timePosition = 0x12345678;
	CU_ASSERT(0x12345678 == APP_MDI_GetTrackTimePosition());	

    gAppMdiContext.timePosition = 0xFFFFFFFE;
	CU_ASSERT(0xFFFFFFFE == APP_MDI_GetTrackTimePosition());	

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetFileName(void)
{
	// after init this should be disbaled	
	const AppMdiFileNameDataType * name = APP_MDI_GetFileName(); 
	CU_ASSERT(NULL != name);	
	CU_ASSERT(0 == name->length);

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetArtist(void)
{
	// after init this should be disbaled	
	const AppMdiFileInfoDataType * info = APP_MDI_GetArtist(); 
	CU_ASSERT_FATAL(NULL != info);	
	CU_ASSERT(0 == info->length);

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetTitle(void)
{
	// after init this should be disbaled	
	const AppMdiFileInfoDataType* title = APP_MDI_GetTitle();
	CU_ASSERT_FATAL(NULL != title);	
	CU_ASSERT(0 == title->length);

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetAlbum(void)
{
	// after init this should be disbaled	
	const AppMdiFileInfoDataType* album = APP_MDI_GetAlbum(  );
	CU_ASSERT_FATAL(NULL != album);	
	CU_ASSERT(0 == album->length);

	APP_MDI_sfInitApp();
}

void test_APP_MDI_GET_VALUES_GetMetadataType(void)
{
	// after init this should be disabled	
	APP_MDI_sfInitApp();

	CU_ASSERT(APP_MDI_INIT == APP_MDI_GetMetadataType());

    gAppMdiContext.MetadataTypeUsed = APP_MDI_ONLY_A2DP;
	CU_ASSERT(APP_MDI_ONLY_A2DP == APP_MDI_GetMetadataType());

	gAppMdiContext.MetadataTypeUsed = APP_MDI_NOT_SUPPORTED;
	CU_ASSERT(APP_MDI_NOT_SUPPORTED == APP_MDI_GetMetadataType());

	gAppMdiContext.MetadataTypeUsed = APP_MDI_SONY_ERICCSON;
	CU_ASSERT(APP_MDI_SONY_ERICCSON == APP_MDI_GetMetadataType());

	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_3;
	CU_ASSERT(APP_MDI_AVRCP_1_3 == APP_MDI_GetMetadataType());

	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_4;
	CU_ASSERT(APP_MDI_AVRCP_1_4 == APP_MDI_GetMetadataType());

   	APP_MDI_sfInitApp();
}


CU_TestInfo tests_APP_MDI_GET_VALUES[] = {
  { "test_APP_MDI_bIsInitialized",			test_APP_MDI_GET_VALUES_bIsInitialized },
  { "test_APP_MDI_bPlayerAvailable",		test_APP_MDI_GET_VALUES_bPlayerAvailable },
  { "test_APP_MDI_bRemoteControlAvailable",	test_APP_MDI_GET_VALUES_bRemoteControlAvailable },
  { "test_APP_MDI_bPlayerActive",			test_APP_MDI_GET_VALUES_bPlayerActive },
  { "test_APP_MDI_GetActiveServiceDeviceId",test_APP_MDI_GET_VALUES_GetActiveServiceDeviceId },
  { "test_APP_MDI_GetPlayStatus",			test_APP_MDI_GET_VALUES_GetPlayStatus },
  { "test_APP_MDI_GetRandomMode",			test_APP_MDI_GET_VALUES_GetRandomMode }, 
  { "test_APP_MDI_GetRepeatMode",			test_APP_MDI_GET_VALUES_GetRepeatMode },
  { "test_APP_MDI_GetTrackTime",			test_APP_MDI_GET_VALUES_GetTrackTime },
  { "test_APP_MDI_GetTrackTimePosition",	test_APP_MDI_GET_VALUES_GetTrackTimePosition },
  { "test_APP_MDI_GetFileName",				test_APP_MDI_GET_VALUES_GetFileName },
  { "test_APP_MDI_GetArtist",				test_APP_MDI_GET_VALUES_GetArtist },
  { "test_APP_MDI_GetTitle",				test_APP_MDI_GET_VALUES_GetTitle },
  { "test_APP_MDI_GetAlbum",				test_APP_MDI_GET_VALUES_GetAlbum },
  { "test_APP_MDI_GetMetadataType",			test_APP_MDI_GET_VALUES_GetMetadataType },

	CU_TEST_INFO_NULL,
};




void test_APP_MDI_ERM_NOTIFY_PLAYER_ACTIVE_STATE_CHANGED(void)
{
	/* set all context values to a arbitrary choosed value */
	memset(&gAppMdiContext, 0xAA, sizeof(gAppMdiContext));

	/* set to INVALID_DEVICE_ID, to signalize that the player was disconnected */
	STUB_ERM_ActivePlayerdeviceId = AT_INVALID_DEVICE_ID;

	APP_MDI_ERM_vHandleErmNotification(ERM_NOTIFY_PLAYER_ACTIVE_STATE_CHANGED);

	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerAvailable());
	CU_ASSERT(BT_FALSE == APP_MDI_bRemoteControlAvailable());
	CU_ASSERT(BT_FALSE == APP_MDI_bPlayerActive());
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_AVRCP));
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());
	CU_ASSERT(MDI_RANDOM_DISABLE == APP_MDI_GetRandomMode());
	CU_ASSERT(MDI_REPEAT_DISABLE == APP_MDI_GetRepeatMode());
	CU_ASSERT(0 == APP_MDI_GetTrackTime());
	CU_ASSERT(0 == APP_MDI_GetTrackTimePosition());
	CU_ASSERT(APP_MDI_GetFileName()->length == 0);
	CU_ASSERT(APP_MDI_GetArtist()->length == 0);
	CU_ASSERT(APP_MDI_GetTitle()->length == 0);
	CU_ASSERT(APP_MDI_GetAlbum()->length == 0);
	CU_ASSERT(APP_MDI_GetMetadataType() == APP_MDI_INIT);
}

int suite_clean_APP_MDI_ERM_NOTIFICATION(void)
{
	memset(&gAppMdiContext, 0, sizeof(gAppMdiContext));
	return 0;
}

CU_TestInfo tests_APP_MDI_ERM_NOTIFICATION[] = {
  { "test_APP_MDI_Player_Active_State_Changed",	test_APP_MDI_ERM_NOTIFY_PLAYER_ACTIVE_STATE_CHANGED },
	CU_TEST_INFO_NULL,

};

void test_APP_MDI_HANDLE_AT_RSP_PKEV(void)
{	
	ATRspMessageType atRsp;
	ATRspAllType rspAll;	
	// play status should be changed via PKEV commnd for avrcp versions less than 1.3
	atRsp.common.rsp_type = AT_RSP_AS_KEY_EVENT_INDICATOR;
	atRsp.data = &rspAll;

	// ****************************************************
	// set avrcp version so that PKEV needs to be evaluated
	gAppMdiContext.playStatus = APP_MDI_STOP;
	gAppMdiContext.MetadataTypeUsed = APP_MDI_NOT_SUPPORTED;
	rspAll.pkev.key = AT_RSP_KEY_AUDIO_RESUMED;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PLAY == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_PLAY;
	rspAll.pkev.key = AT_RSP_KEY_AUDIO_SUSPENDED;
	_APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());	

	// set avrcp version so that PKEV needs to be evaluated
	gAppMdiContext.playStatus = APP_MDI_STOP;
	gAppMdiContext.MetadataTypeUsed = APP_MDI_SONY_ERICCSON;
	rspAll.pkev.key = AT_RSP_KEY_AUDIO_RESUMED;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PLAY == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_PLAY;
	rspAll.pkev.key = AT_RSP_KEY_AUDIO_SUSPENDED;
	_APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());	

	// *******************************************************
	// set avrcp version so that PKEV MUST NOT to be evaluated
	gAppMdiContext.playStatus = APP_MDI_STOP;
	gAppMdiContext.MetadataTypeUsed = APP_MDI_ONLY_A2DP;
	rspAll.pkev.key = AT_RSP_KEY_AUDIO_RESUMED;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

	rspAll.pkev.key = AT_RSP_KEY_AUDIO_SUSPENDED;
	_APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

	// set avrcp version so that PKEV MUST NOT to be evaluated
	gAppMdiContext.playStatus = APP_MDI_STOP;
	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_3;
	rspAll.pkev.key = AT_RSP_KEY_AUDIO_RESUMED;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

	rspAll.pkev.key = AT_RSP_KEY_AUDIO_SUSPENDED;
	_APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

	// set avrcp version so that PKEV MUST NOT to be evaluated
	gAppMdiContext.playStatus = APP_MDI_STOP;
	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_4;
	rspAll.pkev.key = AT_RSP_KEY_AUDIO_RESUMED;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

	rspAll.pkev.key = AT_RSP_KEY_AUDIO_SUSPENDED;
	_APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

	// set received PKEV type to invalid values and meta data type to AVRCP ( less than v1.3 ) supported 
	// -> invalid values must be ignored
	gAppMdiContext.playStatus		= APP_MDI_STOP;
	gAppMdiContext.MetadataTypeUsed = APP_MDI_SONY_ERICCSON;

	rspAll.pkev.key = AT_RSP_KEY_AUDIO_RESUMED - 2;			// create invalid value
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	

	rspAll.pkev.key = AT_RSP_KEY_AUDIO_SUSPENDED + 0x11;	// create invalid value
	_APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_STOP == APP_MDI_GetPlayStatus());	


}

void test_APP_MDI_HANDLE_AT_RSP_PASA(void)
{
	ATRspMessageType atRsp;
	atRsp.common.rsp_type = AT_RSP_AMGMT_SRD_ACTIVE_IND;

    _APP_MDI_vHandleATResponse(&atRsp);
}

void test_APP_MDI_HANDLE_AT_RSP_DGPS(void)
{
	ATRspMessageType atRsp;
	ATRspAllType rspAll;	

	// play status should be changed via DGPS commnd for avrcp versions less than 1.3
	atRsp.common.rsp_type = AT_RSP_PLAYER_GET_STATUS;
	atRsp.data = &rspAll;

	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_START_PLAYING;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PLAY == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_STOP_PLAYING;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_PAUSE_PLAYING;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_RESUME_PLAYING;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_RESUME == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_NEXT_SONG;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_NEXT_SONG == APP_MDI_GetPlayStatus());	
    
    gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_PREVIOUS_SONG;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_PREVIOUS_SONG == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_F_FORWARD_PLAYING;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_FAST_FORWARD == APP_MDI_GetPlayStatus());	

	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = AT_MDB_F_REWIND_PLAYING;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_FAST_BACKWARD == APP_MDI_GetPlayStatus());	

	// try invalid value
	gAppMdiContext.playStatus = APP_MDI_STOP;
	rspAll.dgps.status = 0xFF;
    _APP_MDI_vHandleATResponse(&atRsp);
	// play status shall be set to PAUSE due to the invalid received value
	CU_ASSERT(APP_MDI_PAUSE == APP_MDI_GetPlayStatus());	

}

void test_APP_MDI_HANDLE_AT_RSP_PGMT(void)
{
	ATRspMessageType atRsp;
	ATRspAllType rspAll;	

	// 
	atRsp.common.rsp_type = AT_RSP_AS_GET_META_TYPE;
	atRsp.data = &rspAll;

	gAppMdiContext.MetadataTypeUsed			= APP_MDI_INIT;
	rspAll.pgmt.type						= AT_META_NOT_SUPPORTED;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_NOT_SUPPORTED == APP_MDI_GetMetadataType());	

    gAppMdiContext.MetadataTypeUsed			= APP_MDI_INIT;
	rspAll.pgmt.type						= AT_META_SONY_ERICCSON;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_SONY_ERICCSON == APP_MDI_GetMetadataType());	

	gAppMdiContext.MetadataTypeUsed			= APP_MDI_INIT;
	rspAll.pgmt.type						= AT_META_AVRCP_1_3;
    _APP_MDI_vHandleATResponse(&atRsp);
	CU_ASSERT(APP_MDI_AVRCP_1_3 == APP_MDI_GetMetadataType());	

	gAppMdiContext.MetadataTypeUsed			= APP_MDI_INIT;
	rspAll.pgmt.type						= AT_META_AVRCP_1_4;
    _APP_MDI_vHandleATResponse(&atRsp);
	// shall be mapped to APP_MDI_AVRCP_1_3 due to 1.4 not supported
	CU_ASSERT(APP_MDI_AVRCP_1_3 == APP_MDI_GetMetadataType());	

	// try invalid value
	gAppMdiContext.MetadataTypeUsed			= APP_MDI_INIT;
	rspAll.pgmt.type						= 0xFF;
    _APP_MDI_vHandleATResponse(&atRsp);
	// shall be mapped to APP_MDI_NOT_SUPPORTED due to invalid value
	CU_ASSERT(APP_MDI_NOT_SUPPORTED == APP_MDI_GetMetadataType());	
}

void test_APP_MDI_HANDLE_AT_RSP_DGED_Filename(void)
{
	const char * str = "Sample filename for module test";
	const AppMdiFileNameDataType * filename;
	ATRspMessageType atRsp;
	ATRspAllType rspAll;	
	
	atRsp.common.rsp_type = AT_RSP_DISCO_GET_ENTRY_DATA;
	atRsp.data = &rspAll;

	// 1. Test -> check file name is reset to zeros
	// reset variables
	gAppMdiContext.fileName.length = 0xFF;
	(void)memset(&gAppMdiContext.fileName.data, 0xFF, AT_MDI_FILE_NAME_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_FILEPATH;
	rspAll.dged.data_string_len = 0;
	(void)memset(&rspAll.dged.data_string,0x00,sizeof(rspAll.dged.data_string));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	filename = APP_MDI_GetFileName();
	CU_ASSERT_FATAL(NULL != filename);
	CU_ASSERT(0 == filename->length);
	CU_ASSERT(0 == memcmp(filename->data,&rspAll.dged.data_string,AT_MDI_FILE_NAME_LEN));

	// 2. Test -> check received filename is copied and available for applications
	// reset variables	
	gAppMdiContext.fileName.length = 0;
	(void)memset(&gAppMdiContext.fileName.data, 0, AT_MDI_FILE_NAME_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_FILEPATH;
	rspAll.dged.data_string_len = strlen(str);
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	filename = APP_MDI_GetFileName();
	CU_ASSERT_FATAL(NULL != filename);
	CU_ASSERT(strlen(str) == filename->length);
	CU_ASSERT(0 == memcmp(filename->data,&rspAll.dged.data_string, filename->length));

	// 3. Test -> check if size is limited to AT_MDI_FILE_INFO_LEN
	// reset variables	
	gAppMdiContext.fileName.length = 0;
	(void)memset(&gAppMdiContext.fileName.data, 0, AT_MDI_FILE_NAME_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_FILEPATH;
	rspAll.dged.data_string_len = 0xFF;
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	filename = APP_MDI_GetFileName();
	CU_ASSERT_FATAL(NULL != filename);
	CU_ASSERT(AT_MDI_FILE_NAME_LEN == filename->length);
}

void test_APP_MDI_HANDLE_AT_RSP_DGED_Album(void)
{
	const char * str = "Sample album name for module test";
	const AppMdiFileInfoDataType * fileinfo;
	ATRspMessageType atRsp;
	ATRspAllType rspAll;	
	
	atRsp.common.rsp_type = AT_RSP_DISCO_GET_ENTRY_DATA;
	atRsp.data = &rspAll;
	
	// 1. Test -> check album name is reset to zeros
	// reset variables 
	gAppMdiContext.album.length = 0xFF;
	(void)memset(&gAppMdiContext.album.data, 0xFF, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_ALBUM;
	rspAll.dged.data_string_len = 0;
	(void)memset(&rspAll.dged.data_string,0x00,sizeof(rspAll.dged.data_string));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetAlbum();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(0 == fileinfo->length);
	CU_ASSERT(0 == memcmp(fileinfo->data,&rspAll.dged.data_string,AT_MDI_FILE_INFO_LEN));

	// 2. Test -> check received album name is copied and available for applications
	// reset variables	
	gAppMdiContext.album.length = 0;
	(void)memset(&gAppMdiContext.album.data, 0, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_ALBUM;
	rspAll.dged.data_string_len = strlen(str);
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetAlbum();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(strlen(str) == fileinfo->length);
	CU_ASSERT(0 == memcmp(fileinfo->data,&rspAll.dged.data_string, fileinfo->length));

	// 3. Test -> check if size is limited to AT_MDI_FILE_INFO_LEN
	gAppMdiContext.album.length = 0;
	(void)memset(&gAppMdiContext.album.data, 0, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_ALBUM;
	rspAll.dged.data_string_len = 0xFF;
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetAlbum();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(AT_MDI_FILE_INFO_LEN == fileinfo->length);
}

void test_APP_MDI_HANDLE_AT_RSP_DGED_Artist(void)
{
	const char * str = "Sample artist name for module test";
	const AppMdiFileInfoDataType * fileinfo;
	ATRspMessageType atRsp;
	ATRspAllType rspAll;	
	
	atRsp.common.rsp_type = AT_RSP_DISCO_GET_ENTRY_DATA;
	atRsp.data = &rspAll;
	
	// 1. Test 
	// reset variables
	gAppMdiContext.artist.length = 0xFF;
	(void)memset(&gAppMdiContext.artist.data, 0xFF, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_ARTIST;
	rspAll.dged.data_string_len = 0;
	(void)memset(&rspAll.dged.data_string,0x00,sizeof(rspAll.dged.data_string));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetArtist();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(0 == fileinfo->length);
	CU_ASSERT(0 == memcmp(fileinfo->data,&rspAll.dged.data_string,AT_MDI_FILE_INFO_LEN));

	// 2. Test
	// reset variables	
	gAppMdiContext.artist.length = 0;
	(void)memset(&gAppMdiContext.artist.data, 0, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_ARTIST;
	rspAll.dged.data_string_len = strlen(str);
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetArtist();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(strlen(str) == fileinfo->length);
	CU_ASSERT(0 == memcmp(fileinfo->data,&rspAll.dged.data_string, fileinfo->length));

	// 3. Test -> check if size is limited to AT_MDI_FILE_INFO_LEN
	// reset variables	
	gAppMdiContext.artist.length = 0;
	(void)memset(&gAppMdiContext.artist.data, 0, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_ARTIST;
	rspAll.dged.data_string_len = 0xFF;
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetArtist();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(AT_MDI_FILE_INFO_LEN == fileinfo->length);
}

void test_APP_MDI_HANDLE_AT_RSP_DGED_Title(void)
{
	const char * str = "Sample artist name for module test";
	const AppMdiFileInfoDataType * fileinfo;
	ATRspMessageType atRsp;
	ATRspAllType rspAll;	
	
	atRsp.common.rsp_type = AT_RSP_DISCO_GET_ENTRY_DATA;
	atRsp.data = &rspAll;

	// 1. Test 
	// reset variables
	gAppMdiContext.title.length = 0xFF;
	(void)memset(&gAppMdiContext.title.data, 0xFF, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_TITLE;
	rspAll.dged.data_string_len = 0;
	(void)memset(&rspAll.dged.data_string,0x00,sizeof(rspAll.dged.data_string));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetTitle();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(0 == fileinfo->length);
	CU_ASSERT(0 == memcmp(fileinfo->data,&rspAll.dged.data_string,AT_MDI_FILE_INFO_LEN));

	// 2. Test
	// reset variables	
	gAppMdiContext.title.length = 0;
	(void)memset(&gAppMdiContext.title.data, 0, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_TITLE;
	rspAll.dged.data_string_len = strlen(str);
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetTitle();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(strlen(str) == fileinfo->length);
	CU_ASSERT(0 == memcmp(fileinfo->data,&rspAll.dged.data_string, fileinfo->length));

	// 3. Test
	// reset variables -> check if size is limited to AT_MDI_FILE_INFO_LEN
	// reset variables		
	gAppMdiContext.title.length = 0;
	(void)memset(&gAppMdiContext.title.data, 0, AT_MDI_FILE_INFO_LEN);
	// build up at response
	rspAll.dged.field_id		= AT_ENTRY_MASK_TITLE;
	rspAll.dged.data_string_len = 0xFF;
	memcpy(&rspAll.dged.data_string, str, strlen(str));
    _APP_MDI_vHandleATResponse(&atRsp);
	// check if at response has been processed
	fileinfo = APP_MDI_GetTitle();
	CU_ASSERT_FATAL(NULL != fileinfo);
	CU_ASSERT(AT_MDI_FILE_INFO_LEN == fileinfo->length);
}

int suite_init_APP_MDI_HANDLE_AT_RSP(void)
{
	memset(&gAppMdiContext, 0, sizeof(gAppMdiContext));
	return 0;
}

int suite_clean_APP_MDI_HANDLE_AT_RSP(void)
{
	memset(&gAppMdiContext, 0, sizeof(gAppMdiContext));
	return 0;
}

CU_TestInfo tests_APP_MDI_HANDLE_AT_RSP[] = {
  { "test_APP_MDI_Pkev",			test_APP_MDI_HANDLE_AT_RSP_PKEV },
  //{ "test_APP_MDI_Pasa",			test_APP_MDI_HANDLE_AT_RSP_PASA },
  { "test_APP_MDI_Dgps",			test_APP_MDI_HANDLE_AT_RSP_DGPS },
  { "test_APP_MDI_Pgmt",			test_APP_MDI_HANDLE_AT_RSP_PGMT },
  { "test_APP_MDI_Dged_Filename",	test_APP_MDI_HANDLE_AT_RSP_DGED_Filename },
  { "test_APP_MDI_Dged_Album",		test_APP_MDI_HANDLE_AT_RSP_DGED_Album },
  { "test_APP_MDI_Dged_Artist",		test_APP_MDI_HANDLE_AT_RSP_DGED_Artist },
  { "test_APP_MDI_Dged_Title",		test_APP_MDI_HANDLE_AT_RSP_DGED_Title },
	CU_TEST_INFO_NULL,
};



void test_APP_MDI_InvalidDevId_ActMdiDev(void)
{	
	APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
	AT_DeviceId devId;

	// Test large value
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfActivateMdiDevice(NULL, 0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID + 1 
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfActivateMdiDevice(NULL, AT_MAX_DEVICE_ID + 1);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID without device id being in table
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfActivateMdiDevice(NULL, AT_MAX_DEVICE_ID );
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID with device id being in table
	devId = AT_MAX_DEVICE_ID;
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_FALSE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_FALSE;

	rsp = APP_MDI_sfActivateMdiDevice(NULL, devId );
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_SERVICE_ACTIVATION == gAppMdiContext.fct.common.fctType);

	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));

	// test device id = 0 with device id being in table
	devId = 0;
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_FALSE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_FALSE;
	rsp = APP_MDI_sfActivateMdiDevice(NULL, devId );
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_SERVICE_ACTIVATION == gAppMdiContext.fct.common.fctType);

	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));
}

void test_APP_MDI_InvalidDevId_RequestDgps(void)
{
	APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
	AT_DeviceId devId;

	// Test large value
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = _APP_MDI_sfRequestDgps(NULL, 0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID + 1 
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = _APP_MDI_sfRequestDgps(NULL, AT_MAX_DEVICE_ID + 1);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID without device id being in table
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = _APP_MDI_sfRequestDgps(NULL, AT_MAX_DEVICE_ID );
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID with device id being in table
	devId = AT_MAX_DEVICE_ID;
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	// player must be connected and support avrcp 1.3
	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_3;
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_TRUE;

	rsp = _APP_MDI_sfRequestDgps(NULL, devId );
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_REQUEST_DGPS == gAppMdiContext.fct.common.fctType);

	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));
	APP_MDI_sfInitApp();

	// test device id = 0 with device id being in table
	devId = 0;
	// player must be connected and support avrcp 1.3
	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_3;
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_TRUE;

	rsp = _APP_MDI_sfRequestDgps(NULL, devId );
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_REQUEST_DGPS == gAppMdiContext.fct.common.fctType);

	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));
	APP_MDI_sfInitApp();
}

void test_APP_MDI_InvalidDevId_RequestDged(void)
{
	APP_COMMON_enResponseCode rsp = APP_COMMON_RSP_OK;
	AT_DeviceId devId;

	// Test large value
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = _APP_MDI_sfRequestDged(NULL, 0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID + 1 
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = _APP_MDI_sfRequestDged(NULL, AT_MAX_DEVICE_ID + 1);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID without device id being in table
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = _APP_MDI_sfRequestDged(NULL, AT_MAX_DEVICE_ID );
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// test AT_MAX_DEVICE_ID with device id being in table
	devId = AT_MAX_DEVICE_ID;
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	// player must be connected and support avrcp 1.3
	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_3;
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_TRUE;

	rsp = _APP_MDI_sfRequestDged(NULL, devId );
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_REQUEST_DGED == gAppMdiContext.fct.common.fctType);

	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));
	APP_MDI_sfInitApp();

	// test device id = 0 with device id being in table 
	devId = 0;
	// player must be connected and support avrcp 1.3
	gAppMdiContext.MetadataTypeUsed = APP_MDI_AVRCP_1_3;
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_TRUE;

	rsp = _APP_MDI_sfRequestDged(NULL, devId );
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_REQUEST_DGED == gAppMdiContext.fct.common.fctType);

	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));
	APP_MDI_sfInitApp();
}

void test_APP_MDI_InvalidService_GetActServiceDev(void)
{
	AT_DeviceId devId = 0;

	// no (active) device at all
	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));

	// valid services ( connected, supported by device )
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_AVRCP));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK));

	// no a2dp sink service active, avrcp active
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_FALSE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_FALSE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_TRUE;

	// check for services
	CU_ASSERT(devId == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_AVRCP));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK));

	// no avrcp service active, a2dp sink active
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_FALSE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_FALSE;

	// check for services
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_AVRCP));
	CU_ASSERT(devId == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK));
	
	// player must be connected and support the requested service
	STUB_ERM_ERM_stBTDeviceTable[devId].device_state		= ERM_DEVICE_STATE_USED;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_supported	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_supported		= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].a2dp_sink_active	= BT_TRUE;
	STUB_ERM_ERM_stBTDeviceTable[devId].avrcp_active		= BT_TRUE;

	// valid services ( connected, supported by device )
	CU_ASSERT(devId == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_AVRCP));
	CU_ASSERT(devId == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SINK));

	// invalid services ( no (supported ) player services or out of range )
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_PHONE));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_DUN));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_A2DP_SOURCE));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_AUDIO_GATEWAY));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(AT_SERVICE_SPP));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(0x00));
	CU_ASSERT(AT_INVALID_DEVICE_ID == APP_MDI_GetActiveServiceDeviceId(0xFF));

	memset(STUB_ERM_ERM_stBTDeviceTable, 0x00, sizeof(STUB_ERM_ERM_stBTDeviceTable));
	APP_MDI_sfInitApp();
}

void test_APP_MDI_InvalidDiscoStatus_SetPlayMode(void)
{
	
}

void test_APP_MDI_InvalidRepeatMode_SetRepeatMode(void)
{
	APP_COMMON_enResponseCode rsp;
	APP_MDI_sfInitApp();

	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfSetRepeatMode(NULL,MDI_REPEAT_DISABLE);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_REPEAT_MODE_DISABLED == gAppMdiContext.fct.common.fctType);

	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;	
	rsp = APP_MDI_sfSetRepeatMode(NULL,MDI_REPEAT_ONE_TRACK);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_REPEAT_MODE_ONE_TRACK_ENABLED == gAppMdiContext.fct.common.fctType);

	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfSetRepeatMode(NULL,MDI_REPEAT_ALL_TRACK);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_REPEAT_MODE_ALL_TRACKS_ENABLED == gAppMdiContext.fct.common.fctType);
	
	// invalid repeat mode
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfSetRepeatMode(NULL,0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	APP_MDI_sfInitApp();
}

void test_APP_MDI_InvalidRandomMode_SetRandomMode(void)
{
	APP_COMMON_enResponseCode rsp;
	APP_MDI_sfInitApp();

	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;	
	rsp = APP_MDI_sfSetRandomMode(NULL,MDI_RANDOM_DISABLE);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_RANDOM_MODE_DISABLED == gAppMdiContext.fct.common.fctType);

	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfSetRandomMode(NULL,MDI_RANDOM_ENABLE);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);
	CU_ASSERT(APP_MDI_RANDOM_MODE_ENABLED == gAppMdiContext.fct.common.fctType);
	
	// invalid repeat mode
	gAppMdiContext.fct.common.fctType = APP_MDI_INVALID_FCT_ID;
	rsp = APP_MDI_sfSetRandomMode(NULL,0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	APP_MDI_sfInitApp();
}

void test_APP_MDI_InvalidERMState_HdlErmStateChang(void)
{
	//tbd.
}

void test_APP_MDI_InvalidNotMode_SetAutoNotMode(void)
{
	APP_COMMON_enResponseCode rsp;
	APP_MDI_sfInitApp();

	// automatic notification should be enabled / disabled only for metadata type AVRCP 1.3 and 1.4

	// *******
	// 1. Test
	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	gAppMdiContext.MetadataTypeUsed		= APP_MDI_INIT;
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_ENABLE);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does not allow
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT_FATAL(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_DISABLE);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does not allow
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT_FATAL(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, 0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does not allow and invalid parameter
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT_FATAL(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// *******
	// 2. Test
	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	gAppMdiContext.MetadataTypeUsed		= APP_MDI_SONY_ERICCSON;
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_ENABLE);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does not allow but valid parameter
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT_FATAL(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_DISABLE);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does not allow but valid parameter
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT_FATAL(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, 0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does not allow and invalid parameter
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT_FATAL(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// now check again but set metadata type so that enable / disable is allowed ( in principle )
	// *******
	// 3. Test
	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	gAppMdiContext.MetadataTypeUsed		= APP_MDI_AVRCP_1_3;
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_ENABLE);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);			// metadata type does allow and valid parameter
	CU_ASSERT(APP_MDI_AUTOMTIC_NOTIFICTION_MODE_ENABLED == gAppMdiContext.fct.common.fctType);

	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_DISABLE);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);			// metadata type does allow and valid parameter
	CU_ASSERT(APP_MDI_AUTOMTIC_NOTIFICTION_MODE_DISABLED == gAppMdiContext.fct.common.fctType);

	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, 0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does allow but invalid parameter
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);

	// *******
	// 4. Test
	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	gAppMdiContext.MetadataTypeUsed		= APP_MDI_AVRCP_1_4;
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_ENABLE);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);			// metadata type does allow and valid parameter
	CU_ASSERT(APP_MDI_AUTOMTIC_NOTIFICTION_MODE_ENABLED == gAppMdiContext.fct.common.fctType);

	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, MDI_AUTO_NOT_DISABLE);
	CU_ASSERT(APP_COMMON_RSP_OK == rsp);			// metadata type does allow and valid parameter
	CU_ASSERT(APP_MDI_AUTOMTIC_NOTIFICTION_MODE_DISABLED == gAppMdiContext.fct.common.fctType);

	gAppMdiContext.fct.common.fctType	= APP_MDI_INVALID_FCT_ID;	
	rsp = _APP_MDI_vSetAutomaticNotificationMode(NULL, 0xFF);
	CU_ASSERT(APP_COMMON_RSP_OK != rsp);			// metadata type does allow but invalid parameter
	CU_ASSERT(APP_COMMON_RSP_ERROR_BUSY != rsp);
	CU_ASSERT(APP_MDI_INVALID_FCT_ID == gAppMdiContext.fct.common.fctType);
}

int suite_init_APP_MDI_INVALID_PARAMS(void)
{
	memset(&gAppMdiContext, 0, sizeof(gAppMdiContext));
	return 0;
}

int suite_clean_APP_MDI_INVALID_PARAMS(void)
{
	memset(&gAppMdiContext, 0, sizeof(gAppMdiContext));
	return 0;
}

CU_TestInfo tests_APP_MDI_INVALID_PARAMS[] = {  
  { "test_APP_MDI_InvalidDevId_ActMdiDev",			test_APP_MDI_InvalidDevId_ActMdiDev }, 
  { "test_APP_MDI_InvalidDevId_RequestDgps",		test_APP_MDI_InvalidDevId_RequestDgps },	
  { "test_APP_MDI_InvalidDevId_RequestDged",		test_APP_MDI_InvalidDevId_RequestDged },	 
  { "test_APP_MDI_InvalidService_GetActServiceDev",	test_APP_MDI_InvalidService_GetActServiceDev }, 
  { "test_APP_MDI_InvalidDiscoStatus_SetPlayMode",	test_APP_MDI_InvalidDiscoStatus_SetPlayMode }, 
  { "test_APP_MDI_InvalidRepeatMode_SetRepeatMode",	test_APP_MDI_InvalidRepeatMode_SetRepeatMode }, 
  { "test_APP_MDI_InvalidRandomMode_SetRandomMode",	test_APP_MDI_InvalidRandomMode_SetRandomMode }, 
  { "test_APP_MDI_InvalidERMState_HdlErmStateChang",test_APP_MDI_InvalidERMState_HdlErmStateChang },
  { "test_APP_MDI_InvalidNotMode_SetAutoNotMode",	test_APP_MDI_InvalidNotMode_SetAutoNotMode },  
	CU_TEST_INFO_NULL,
};



CU_SuiteInfo suite_APP_MDI[] = {
  { "tests_APP_MDI_BUSY_RESPONSE",		suite_init_APP_MDI_BUSY_RESPONSE,	suite_clean_APP_MDI_BUSY_RESPONSE,		tests_APP_MDI_BUSY_RESPONSE },
  { "tests_APP_MDI_GET_VALUES",			suite_init_APP_MDI_GET_VALUES,		suite_clean_APP_MDI_GET_VALUES,			tests_APP_MDI_GET_VALUES },
  { "tests_APP_MDI_ERM_NOTIFICATION",	NULL,								suite_clean_APP_MDI_ERM_NOTIFICATION,	tests_APP_MDI_ERM_NOTIFICATION },
  { "tests_APP_MDI_HANDLE_AT_RSP",		suite_init_APP_MDI_HANDLE_AT_RSP,	suite_clean_APP_MDI_HANDLE_AT_RSP,		tests_APP_MDI_HANDLE_AT_RSP },  
  { "tests_APP_MDI_INVALID_PARAMS",		suite_init_APP_MDI_INVALID_PARAMS,	suite_clean_APP_MDI_INVALID_PARAMS,		tests_APP_MDI_INVALID_PARAMS },  
	CU_SUITE_INFO_NULL,
};

void AddModuleTests()
{
	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());

	/* Register suites. */
	if (CU_register_nsuites(2, &suite_APP_MDI, NULL /*.... */ ) != CUE_SUCCESS) {
		fprintf(stderr, "suite registration failed - %s\n",
			CU_get_error_msg());
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char* argv[])
{
	enReturnCode returnCode = UHV_MODULE_TEST_SUCCESS;
	CU_ErrorCode code		= CUE_SUCCESS;
	CU_BasicRunMode mode	= CU_BRM_VERBOSE;
	

/*	if(0 != Usage(argc, argv))
		return 1;*/		

	CU_set_output_filename("APP_MDI_ModuleTest");


	// setup successful, now setup framework and run tests

	if (CU_initialize_registry()) 
	{
		printf("\nInitialization of Test Registry failed.");
	}
	else 
	{
		CU_pRunSummary pSummary;	
		const char* msg; 

		AddModuleTests();		
		CU_basic_set_mode(mode);
		//CU_set_error_action(error_action);

		//code = CU_basic_run_tests();
		CU_automated_run_tests();

		pSummary = CU_get_run_summary();

		printf("  Test Overview:\n");
		printf("  Number of Testsuites: %d\n", pSummary->nSuitesRun);
		printf("  Failed:               %d\n", pSummary->nSuitesFailed);
		printf("  Number of Tests:      %d\n", pSummary->nTestsRun);
		printf("  Failed:               %d\n", pSummary->nTestsFailed);
		printf("  Number of Asserts:    %d\n", pSummary->nAsserts);
		printf("  Failed:               %d\n", pSummary->nAssertsFailed);
          
		printf("\n\nOverview of Failures:\n");
		CU_basic_show_failures(CU_get_failure_list());


		// 
		code = CU_get_error();
		msg = CU_get_error_msg(); 

		printf("\n\nFramework status: %d, error message: %s", code, msg);

		// decide if we have to return an error	
		// first check for framework errors 
		if( code != CUE_SUCCESS )
		{
			returnCode = UHV_MODULE_TEST_FRAMEWORK_ERROR;	
			printf("\nReturnCode is %d due to FrameworkError", returnCode);
		}
		// check if there where failed tests
		else if(pSummary->nTestsFailed > 0)
		{
			returnCode = UHV_MODULE_TEST_FAILED;
			printf("\nReturnCode is %d due to TestsFailed", returnCode);
		}

		//printf("\nTests completed with return value %d.\n", code);
		CU_cleanup_registry();
  }

	//getchar();
	return returnCode;
}