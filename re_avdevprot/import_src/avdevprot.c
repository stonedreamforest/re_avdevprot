#include "global.h"












NTSTATUS avk_CopyRegistryPath(PUNICODE_STRING src , PUNICODE_STRING dst) {
	PAGED_CODE();
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

	PAGED_CODE();
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

NTSTATUS avk_GetSystemInfo(SYSINFO *SysInfo) {
	PEPROCESS peprocess;
	OSVERSIONINFOEXW os ={ 0 };

	memset(&SysInfo->field_14 , 0xff , 0x28);
	SysInfo->NumberProcessors = KeNumberProcessors;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);	//	0x11c
	if (RtlGetVersion((POSVERSIONINFOW)&os) >= 0) {
		SysInfo->dwBuildNumber = os.dwBuildNumber;
		SysInfo->dwMajorVersion = os.dwMajorVersion;
		SysInfo->dwMinorVersion = os.dwMinorVersion;
		SysInfo->wServicePackMajor = os.wServicePackMajor;
	}
	int flag = avk_GetOffsetFlag(SysInfo->dwBuildNumber , SysInfo->dwMinorVersion);
	peprocess = IoGetCurrentProcess();
	SysInfo->field_14 = avk_GetPeprocessValue((char *)peprocess , 0x1000 , "System" , 6);
	if (SysInfo->dwMajorVersion == 5) {
		if (SysInfo->dwMinorVersion == 1 || SysInfo->dwMinorVersion == 2) {
			SysInfo->field_18 = 0x158;
			SysInfo->field_1C = 0x2B0;
			SysInfo->field_20 = 0x318;
			SysInfo->field_24 = 0x308;
			SysInfo->field_28 = 0x218;
			SysInfo->field_2C = 0x10;
			SysInfo->field_34 = 0x3C8;
		}
	}
	else if (SysInfo->dwMajorVersion == 6) {
		if (SysInfo->dwMinorVersion) {
			switch (SysInfo->dwMinorVersion) {
				case  1:
					SysInfo->field_18 = 0x200;
					SysInfo->field_1C = 0x328;
					SysInfo->field_20 = 0x390;
					SysInfo->field_24 = 0x168;
					SysInfo->field_28 = 0x290;
					SysInfo->field_2C = 0x180;
					SysInfo->field_34 = flag == 0x440 ? 0x410 : flag;
					break;
				case 2:
					SysInfo->field_18 = 0x408;
					SysInfo->field_1C = 0x480;
					SysInfo->field_20 = 0x450;
					SysInfo->field_24 = 0x2D0;
					SysInfo->field_28 = 0x3D0;
					SysInfo->field_2C = 0x2E0;
					SysInfo->field_34 = 0x3F0;
					break;
				case 3:
					SysInfo->field_18 = 0x408;
					SysInfo->field_1C = 0x480;
					SysInfo->field_20 = 0x450;
					SysInfo->field_24 = 0x2D0;
					SysInfo->field_28 = 0x3D0;
					SysInfo->field_2C = 0x2E0;
					SysInfo->field_34 = 0x678;
					break;
			}
		}
		else {
			SysInfo->field_18 = 352;
			SysInfo->field_1C = 640;
			SysInfo->field_20 = 744;
			SysInfo->field_24 = 200;
			SysInfo->field_28 = 496;
			SysInfo->field_2C = 224;
			SysInfo->field_34 = 992;
		}
	}
	if (SysInfo->field_20 != 0xFFFFFFFF) {
		SysInfo->field_1C = SysInfo->field_20 + 8;
	}
	if (SysInfo->dwMajorVersion < 5) {
		return 0x0C0000002;
	}
	else if (SysInfo->dwMajorVersion != 5) {
		return 0;
	}
	else if (SysInfo->dwMinorVersion) {
		return 0;
	}
	return 0x0C0000002;
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
	Irp->IoStatus.Status = 0;
	Irp->IoStatus.Information = 0;
	IofCompleteRequest(Irp , 0);
	return 0;
}

NTSTATUS avk_DispatchDeviceControl(_Inout_ struct _DEVICE_OBJECT *DeviceObject , _Inout_ struct _IRP *Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS Status = 0;
	ULONG  flag = 0;
	Irp->IoStatus.Status = 0;		//	0x30
	Irp->IoStatus.Information = 0;	//	0x38

	PAGED_CODE();
	PAM_OFF28 pAm_off28 = sub_1400019E4();
	if (!pAm_off28) {
		Status = 0x0C0000002;
		goto loc_140009354;
	}
	if (!Irp->AssociatedIrp.MasterIrp) {
		Status = 0xC0000206;
		goto loc_140009354;
	}
	//	Irp->Tail.Overlay.CurrentStackLocation->MajorFunction //	0xb8
	//	Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.EaLength //	0x18
	//	Irp->AssociatedIrp.MasterIrp->Type //	p->18->0
	flag = Irp->Tail.Overlay.CurrentStackLocation->Parameters.Read.ByteOffset.LowPart - 0x22001C;
	if (!flag) {
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.Options != 4) {
			Status = 0x0C000000D;
			goto loc_140009354;
		}
		Irp->AssociatedIrp.MasterIrp->Type = pAm_off28->Type;
		Irp->AssociatedIrp.MasterIrp->Size = pAm_off28->Size;
		Irp->IoStatus.Information = 4;
		goto loc_140009354;
	}
	flag -= 4;
	if (!flag) {
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.Options != 4) {
			Status = 0x0C000000D;
		}
		else if (*(int *)&Irp->AssociatedIrp.MasterIrp->Type == 0x47555741) {
			sub_1400032F8();
		}
		goto loc_140009354;
	}
	flag -= 0x3FE4;
	if (!flag) {
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.Options != 0x10) {
			Status = 0x0C000000D;
		}
		else {
			pAm_off28->field_0 = *(__int64 *)&Irp->AssociatedIrp.MasterIrp->Type;
		}
		goto loc_140009354;
	}
	flag -= 4;
	if (!flag) {
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.Options != 0x10) {
			Status = 0x0C000000D;
			goto loc_140009354;
		}
		if (Irp->AssociatedIrp.MasterIrp->MdlAddress) {
			Status = sub_140003290(Irp->AssociatedIrp.MasterIrp);
			if (Status < 0) {
				goto loc_140009354;
			}
		}
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Read.Length < 0x28 || pAm_off28->field_8) {
			goto loc_140009354;
		}
		memset(Irp->AssociatedIrp.MasterIrp->MdlAddress , 0 , Irp->Tail.Overlay.CurrentStackLocation->Parameters.Read.Length);
		*(__int32 *)&Irp->AssociatedIrp.MasterIrp->Type = Irp->Tail.Overlay.CurrentStackLocation->Parameters.Read.Length;
		sub_140002A40(pAm_off28 , Irp);
		return 0x103;
	}
	flag -= 4;
	if (!flag) {
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.Options != 8) {
			Status = 0x0C000000D;
		}
		else {
			*(__int64 *)&pAm_off28->field_C[12] = *(__int64 *)&Irp->AssociatedIrp.MasterIrp->Type;
		}
		goto loc_140009354;
	}
	flag -= 4;
	if (!flag) {
		avk_GetSimulateUSBValue(&g_AvkInit.RegistryPath);
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.Options == 4 && Irp->AssociatedIrp.MasterIrp->MdlAddress) {
			Irp->AssociatedIrp.MasterIrp->Type = pAm_off28->Type;
			Irp->AssociatedIrp.MasterIrp->Size = pAm_off28->Size;
			sub_1400032EC();
			sub_140003150();
		}
		goto loc_140009354;
	}
	flag -= 4;
	if (!flag) {
		if (Irp->Tail.Overlay.CurrentStackLocation->Parameters.Create.Options != 8) {
			Status = 0x0C000000D;
		}
		else if (Irp->AssociatedIrp.MasterIrp->MdlAddress) {
			Status = sub_140003290(Irp->AssociatedIrp.MasterIrp);
		}
		goto loc_140009354;
	}
	flag -= 4;
	if (!flag) {
		Status = 0x0C00000BB;
	}
loc_140009354:
	Irp->IoStatus.Status = Status;
	_InterlockedExchange((volatile LONG *)&Irp->CancelRoutine , 0);
	IofCompleteRequest(Irp , 0);
	return Status;
}

int avk_GetOffsetFlag(int dwBuildNumber , int dwMinorVersion) {
	if (dwBuildNumber != 6 || dwMinorVersion != 1) {
		return 0x0FFFFFFFF;
	}
	UNICODE_STRING US_PsIsThreadTerminating ={ 42u, 44u, L"PsIsThreadTerminating" };
	char *OffsetFlag =  MmGetSystemRoutineAddress(&US_PsIsThreadTerminating);
	if (!OffsetFlag || ((short *)OffsetFlag)[0] != 0x818b) {
		return 0x0FFFFFFFF;
	}
	if (OffsetFlag[6] != 0x24 || OffsetFlag[7] != 1 || OffsetFlag[8] != 0xc3) {
		return 0x0FFFFFFFF;
	}
	return *(int *)(OffsetFlag + 2);
}



int avk_GetPeprocessValue(char *peprocess , int length , char *aSystem , int LimitValue) {
	for (int i = 0 , j = 0; i < length; i++) {
		for (j = 0; j < length; j++) {
			if (j > LimitValue) {
				break;
			}
			else if (j == LimitValue) {
				return i;
			}
			int x_flag = aSystem[j];
			int v_flag = peprocess[i + j];
			if ((v_flag - 0x41) <= 0x19) {
				v_flag |= 0x20;
			}
			if ((x_flag - 0x41) <= 0x19) {
				x_flag |= 0x20;
			}
			if (x_flag == v_flag) {
				break;
			}
		}
		if (j == LimitValue) {
			return i;
		}
	}
	return 0;
}

PAM_OFF28 sub_1400019E4() {
	return g_AvkInit.am_off28;
}

NTSTATUS sub_140003290(void *a1) {
	UNREFERENCED_PARAMETER(a1);
	return 0;
}

void sub_1400032EC() {
	g_AvkInit.field_68 = 1;
}

NTSTATUS sub_140003150() {
	return 0;
}

void sub_140002A40(void *a1 , void *a2) {
	UNREFERENCED_PARAMETER(a1);
	UNREFERENCED_PARAMETER(a2);
}

void sub_1400032F8() {

}


