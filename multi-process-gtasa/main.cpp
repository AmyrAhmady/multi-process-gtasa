#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "injector/injector.hpp"

struct RsGlobalType
{
	char * AppName;
	int MaximumWidth;
	int MaximumHeight;
	int frameLimit;
	int quit;
	int ps;
};

DWORD _EAX;
void __declspec(naked) AllowMouseMovement()
{
	_asm
	{
		mov _EAX, eax
		mov eax, dword ptr ds : [0x8D621C]
		cmp eax, 0
		jne label1
		mov eax, _EAX
		ret

		label1 :
		mov eax, _EAX
		mov _EAX, 0x7453F0
		jmp _EAX
	}
}

void EnableMultiProcess()
{
	if (GetModuleHandleA(0) == (HMODULE)0x400000
		&& !IsBadReadPtr((const void *)0x4028A2, 4)
		&& *reinterpret_cast<DWORD *>(0x4028A2) == 0xE1FB058D)
	{
		DWORD OldProtect;
		RsGlobalType * RsGlobal = reinterpret_cast<RsGlobalType *>(0xC17040);

		// Disable more than one gtasa instance check so we can run more than one
		injector::MakeNOP(0x74872D, 9, true);

		// Enable windowed mode that can be activated using Enter+Alt (or Alt+Enter for some people)
		injector::WriteMemory<DWORD>(0x406946, 0x00, true);

		// Disable menu after Alt+Tab
		injector::WriteMemory<uint8_t>(0x53BC78, 0x00, true);

		// Allow Alt+Tab'ing without pausing
		injector::MakeNOP(0x748A8D, 6, true);
		injector::MakeJMP(0x6194A0, AllowMouseMovement, true);
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

