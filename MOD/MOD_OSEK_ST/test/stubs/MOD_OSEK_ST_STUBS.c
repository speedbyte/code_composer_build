 #include "osek.h"

osqFunc1 StatusType osqFunc2 osGetMessageStatus(SymbolicName pMsg)
{
	return E_OK;
}

osqFunc1 StatusType osqFunc2 osReceiveMessage(SymbolicName pMsg, AccessNameRef pData)
{
	return E_OK;
}

osqFunc1 StatusType osqFunc2 osClearEvent (EventMaskType Mask )
{	
	return E_OK;
}

osqFunc1 StatusType osqFunc2 osCancelAlarm(AlarmType alarmID)
{
	return E_OK;
}
osqFunc1 StatusType osqFunc2 osSetRelAlarm(AlarmType alarmID, TickType deltaTicks, TickType cycle)
{
	return E_OK;
}

osqFunc1 StatusType osqFunc2 osSetEvent(TaskType taskIndex, EventMaskType setMask)
{
	return E_OK;
}