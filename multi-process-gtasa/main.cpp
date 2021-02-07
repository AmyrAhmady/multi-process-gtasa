#include <Windows.h>

void EnableMultiProcess()
{
	if (GetModuleHandleA(0) == (HMODULE)0x400000
		&& !IsBadReadPtr((const void *)0x4028A2, 4)
		&& *reinterpret_cast<DWORD *>(0x4028A2) == 0xE1FB058D)
	{
		DWORD OldProtect, OldProtect2;

		VirtualProtect((LPVOID)0x74872D, 9, 0x40, &OldProtect);
		*reinterpret_cast<DWORD *>(0x74872D) = 0x90909090;
		*reinterpret_cast<DWORD *>(0x748731) = 0x90909090;
		*reinterpret_cast<BYTE *>(0x748735) = 0x90;
		VirtualProtect((LPVOID)0x74872D, 9, OldProtect, &OldProtect);

		VirtualProtect((LPVOID)0x406946, 4, 0x40, &OldProtect2);
		*reinterpret_cast<DWORD *>(0x406946) = 0x90909090;
		VirtualProtect((LPVOID)0x406946, 4, OldProtect2, &OldProtect2);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);
		EnableMultiProcess();
	}
	break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

