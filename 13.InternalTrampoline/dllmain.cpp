#include <iostream>
#include "mem.h"

uintptr_t moduleBase;
BYTE* originAddress;

void __declspec(naked) ourFunct() {



	__asm {
		 mov ecx, 999999
		 jmp originAddress
	}



}


DWORD WINAPI HackThread(HMODULE hModule)
{

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	DWORD hookAddress = ((DWORD)moduleBase + 0x1249e);

	// Hook
	originAddress = mem::TrampHook32((BYTE*)hookAddress, (BYTE*)ourFunct, 5);
	//

	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}