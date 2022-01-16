#include <Windows.h>
#include <iostream>


DWORD moduleBase;
HMODULE myhModule;


DWORD __stdcall EjectThread(LPVOID lpParameter) {
	Sleep(100);
	FreeLibraryAndExitThread(myhModule, 0);
}

bool Hook(void* toHook, void* ourFunct, int len) {
	if (len < 5) {
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);

	return true;
}

void UnHook(void* toUnHook,void* backup, int len) {

	DWORD curProtection;
	VirtualProtect(toUnHook, len, PAGE_EXECUTE_READWRITE, &curProtection);
	memset(toUnHook, 0x90, len);
	std::memcpy(toUnHook, (void*)backup, len);
	DWORD temp;
	VirtualProtect(toUnHook, len, curProtection, &temp);

}


// mov [99_01.FunctionDetouringTargetProgram.exe+1C000],eax
DWORD jmpBackAddy;
void __declspec(naked) ourFunct() {

	__asm {
		pushad
		mov ecx, moduleBase
	}

	__asm {
		add eax, 1
		mov [ecx + 0x1C000], eax
	}

	_asm {
		popad
		jmp [jmpBackAddy]
	}
}

DWORD WINAPI MainThread(LPVOID param) {

	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout); // output only
	std::cout << "[+] Entrypoint reached" << std::endl;
	moduleBase = (uintptr_t)GetModuleHandle(NULL);

	int hookLength = 8;
	DWORD hookAddress = ((DWORD)moduleBase + 0x1261d);
	jmpBackAddy = hookAddress + hookLength;

	unsigned char backup[8];
	std::memcpy(backup, (void*)hookAddress, hookLength);


	std::printf("\n[+] ModuleBase address: %#.8x\n", moduleBase);
	std::printf("\[+] hookAddress address: %#.8x\n", hookAddress);
	std::printf("\[+] jmpBackAddy address: %#.8x\n", jmpBackAddy);
	std::printf("\[+] hookLength value: %d\n", hookLength);
	std::printf("\[+] backup address: %#.8x\n", backup);
	std::printf("\[+] backup value: ");
	for (int i = 0; i < hookLength; i++) printf("%.2x", backup[i]);
	std::printf("\n");
	if (Hook((void*)hookAddress, ourFunct, hookLength)) {
		std::cout << "[+] Hooked!!!" << std::endl;
		while (true) {
			if (GetAsyncKeyState(VK_ESCAPE)) break;
			Sleep(50);
		}
	}
	else {
		std::cout << "[-] Cannot Hooked" << std::endl;
	}

	std::cout << "[+] Self-Unloaded DLL" << std::endl;

	UnHook((void*)hookAddress,(void*)backup, hookLength);

	/*
	fclose(fp);
	FreeConsole();
	*/

	CreateThread(0, 0, EjectThread, 0, 0, 0);
	return 0;
}

extern "C" __declspec(dllexport)
BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		myhModule = hModule;
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;
	}

	return TRUE;
}