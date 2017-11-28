#include "global.h"












NTSTATUS avk_CopyRegistryPath(PUNICODE_STRING src , PUNICODE_STRING dst) {
	dst->Buffer = ExAllocatePoolWithTag(PagedPool , src->MaximumLength , 0x44504664);
	if (!dst->Buffer) {
		return 0x0C0000017;
	}
	RtlCopyUnicodeString(dst , src);
	return 0; 
}


NTSTATUS avk_GetSimulateUSBValue(PUNICODE_STRING RegistryPath) {
	typedef struct _KEYVALUEINFORMATION {
		__int64 field_0;
		int field_8;
		int field_C;
		int field_10;
	}KEYVALUEINFORMATION;
	HANDLE KeyHandle = 0;
	ULONG ResultLength = 0;
	UNICODE_STRING ValueName ={ 0 };
	OBJECT_ATTRIBUTES ObjectAttributes ={ 0 };
	KEYVALUEINFORMATION KeyValueInformation ={ 0 };

	InitializeObjectAttributes(&ObjectAttributes , RegistryPath , OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE , 0 , 0);
	g_Avk_Mutant.field_40 = 0;
	if (ZwOpenKey(&KeyHandle , 0x20019 , &ObjectAttributes) >= 0) {
		return 0;
	}
	RtlInitUnicodeString(&ValueName , L"SimulateUSBDevice");
	if (ZwQueryValueKey(KeyHandle ,
						&ValueName ,
						KeyValuePartialInformation ,
						&KeyValueInformation ,
						sizeof(KEYVALUEINFORMATION) ,
						&ResultLength) < 0) {
		g_Avk_Mutant.field_40 = KeyValueInformation.field_C;
	}
	ZwClose(KeyHandle);
	return 0;
}

NTSTATUS sub_1400019EC(void *a1) {
	UNREFERENCED_PARAMETER(a1);
	return 0;
}

void sub_14000172C() {

}

void sub_1400027F4() {

}

void sub_140002978(int itrue) {
	UNREFERENCED_PARAMETER(itrue);
}

void sub_140002E74(HANDLE ParentId , HANDLE ProcessId , BOOLEAN Create) {
	UNREFERENCED_PARAMETER(ParentId);
	UNREFERENCED_PARAMETER(ProcessId);
	UNREFERENCED_PARAMETER(Create);
}



NTSTATUS avk_DispatchCreateClose(_Inout_ struct _DEVICE_OBJECT *DeviceObject , _Inout_ struct _IRP *Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	return 0;
}

NTSTATUS avk_DispatchDeviceControl(_Inout_ struct _DEVICE_OBJECT *DeviceObject , _Inout_ struct _IRP *Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);
	return 0;
}


