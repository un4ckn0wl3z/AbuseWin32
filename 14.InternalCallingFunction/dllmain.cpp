#include <iostream>
#include <Windows.h>

HMODULE myhModule;


DWORD __stdcall EjectThread(LPVOID lpParameter) {
	Sleep(100);
	FreeLibraryAndExitThread(myhModule, 0);
}

typedef void(__cdecl* fpFuncA)(void);
typedef void(__cdecl* fpFuncB)(char const*);

fpFuncA FuncA;
fpFuncB FuncB;

DWORD WINAPI HackThread(HMODULE hModule)
{
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);
	FuncA = (fpFuncA)(moduleBase + 0x12340);
	FuncB = (fpFuncB)(moduleBase + 0x123C0);


	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
			FuncA();
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
			FuncB("I'm function B!");
		}

		if (GetAsyncKeyState(VK_END) & 1) {
			break;
		}

		Sleep(100);

	}

	CreateThread(0, 0, EjectThread, 0, 0, 0);
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
		myhModule = hModule;
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}