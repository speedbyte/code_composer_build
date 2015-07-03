
#include "ERM.h"
#include "ERM_CE.h"
#include "ERM_AudioSets.h"
#include "ERM_Utils.h"
#include "ERM_MessageHandler.h"
#include "ERM_DeviceHandler.h"

AT_DeviceId STUB_ERM_ActivePlayerdeviceId = AT_INVALID_DEVICE_ID;

ERMBTDeviceTableType    STUB_ERM_ERM_stBTDeviceTable[AT_MAX_DEVICE_ID+1] = {0};
ERMPhysicalBTDeviceType STUB_ERM_ERM_stPhyDevTable[ERM_MAX_NUM_PHYSICAL_DEVICES] = {0}; 
ERMUserProfileType      STUB_ERM_ERM_stUserTable[ERM_MAX_NUM_USER_PROFILES] = {0};

const ERMBTDeviceTableType *ERM_GetBTDevice(AT_DeviceId deviceId)
{
	if( deviceId > AT_MAX_DEVICE_ID )
    {
        return NULL;
    }

    if( STUB_ERM_ERM_stBTDeviceTable[deviceId].device_state != ERM_DEVICE_STATE_USED )
    {
        return NULL;
    }

    return &STUB_ERM_ERM_stBTDeviceTable[deviceId];
}

const ERMPlayerHeadsetProfileType *ERM_pGetPlayerHeadsetProfile(Uint8Type index)
{
	return NULL;
}


AT_DeviceId ERM_u8GetActivePlayer(void)
{

	return STUB_ERM_ActivePlayerdeviceId;
}


const SCI_ST*  ERM_GetUartForMDI(void)
{
	return NULL;
}

SuccessFailType ERM_ReleaseUartForMDI()
{
	return SUCCESS;
}

void ERM_vOnAppMdiNotification(APP_MDI_enNotificationId event)
{
	;
}

SuccessFailType EEPROM_LAYOUT_vGetPlayerEnabled(BooleanType * Value)
{
    return SUCCESS;
}