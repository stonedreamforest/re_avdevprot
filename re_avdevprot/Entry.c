#include "global.h"



































//************************************************************************************************************
//	创建作者  :	Tennn
//  修改时间  :  2017/11/27 22:09
//	函数功能  :	DriverEntry
//************************************************************************************************************
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  DriverObject , _In_ PUNICODE_STRING RegistryPath) {
	NTSTATUS nStatus = 0;
	UNICODE_STRING DeviceName ={ 0 };
	UNICODE_STRING SymbolicLinkName ={ 0 };
	g_AvkInit.ListHead_40.Next = 0;
	KeInitializeSpinLock(&g_AvkInit.SpinLock);
	if (RegistryPath && RegistryPath->Length && RegistryPath->MaximumLength && RegistryPath->Buffer) {
		nStatus = avk_CopyRegistryPath(RegistryPath , &g_AvkInit.RegistryPath);
		if (nStatus) {
			goto LEAVE;
		}
	}
	g_AvkInit.field_12 = 1;
	avk_GetSimulateUSBValue(&g_AvkInit.RegistryPath);
	nStatus = avk_GetSystemInfo(&g_SysInfo);
	if (nStatus) {
		goto LEAVE;
	}
	RtlInitUnicodeString(&DeviceName , L"\\FileSystem\\Filters\\avdevprot");
	nStatus = WdmlibIoCreateDeviceSecure(DriverObject , 0 , &DeviceName , 8 , 0x100 , 0 , &DefaultSDDLString , 0 , &g_AvkInit.DeviceObject);
	if (nStatus > 0) {
		goto LEAVE;
	}
	g_AvkInit.field_14 = 1;
	RtlInitUnicodeString(&SymbolicLinkName , L"\\DosDevices\\avdevprot");
	nStatus = IoCreateSymbolicLink(&SymbolicLinkName , &DeviceName);
	if (nStatus < 0) {
		goto END;
	}
	g_AvkInit.field_11 = 1;
	g_AvkInit.DeviceObject->Flags |= 0x10;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = avk_DispatchCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = avk_DispatchCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = avk_DispatchDeviceControl;
	DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = avk_DispatchDeviceControl;
	DriverObject->DriverUnload = 0;
	sub_1400027F4();
	PsSetCreateProcessNotifyRoutine(sub_140002E74 , 0);
	sub_140002978(1);
LEAVE:
	if (nStatus < 0) {
	END:
		sub_14000172C();
	}
	return nStatus;
}


