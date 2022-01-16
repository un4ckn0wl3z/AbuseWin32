#include <Windows.h>
#include <stdio.h>
#include <SubAuth.h>
#include <cstdint>

void SetupJumpInstructionBytes(uint8_t* jmpInstruction, uintptr_t jumpStartAddress, uintptr_t addressToJumpTo)
{
	const uintptr_t x86FixedJumpSize = 5;
	uintptr_t relativeAddress = addressToJumpTo - jumpStartAddress - x86FixedJumpSize;
	jmpInstruction[0] = 0xE9; // jump opcode
	*(uintptr_t*)&jmpInstruction[1] = relativeAddress;
}

void PlaceJumpToAddress(uintptr_t installAddress, uintptr_t addressToJumpTo)
{
	uint8_t jmpInstruction[5];
	SetupJumpInstructionBytes(jmpInstruction, installAddress, addressToJumpTo);
	DWORD dwProtect[2];
	VirtualProtect((void*)installAddress, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	memcpy((void*)installAddress, jmpInstruction, 5);
	VirtualProtect((void*)installAddress, 5, dwProtect[0], &dwProtect[1]);
}

uintptr_t HookFunction(uintptr_t targetAddress, uintptr_t HookFunctionAddress, uint32_t hookSize)
{
	// setup the trampoline
	uintptr_t trampolineAddress = (uintptr_t)VirtualAlloc(0, hookSize + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy((void*)trampolineAddress, (void*)targetAddress, hookSize);

	// place a jump at the bottom of the bytes, so we can go back to the original function
	uintptr_t jumpAddress = trampolineAddress + hookSize;
	uint8_t jmpInstruction[5];
	SetupJumpInstructionBytes(jmpInstruction, jumpAddress, targetAddress + hookSize);
	memcpy((void*)jumpAddress, jmpInstruction, 5);

	// The trampoline is done. Now we place a jump in the target function to redirect the 
	// call to our custom function
	PlaceJumpToAddress(targetAddress, HookFunctionAddress);
	return trampolineAddress;
}

typedef NTSTATUS(NTAPI* PFLDRLOADDLL)
(
	IN PWCHAR               PathToFile OPTIONAL,
	IN ULONG                Flags OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             ModuleHandle);

PFLDRLOADDLL OriginalLdrLoadDll = nullptr;

NTSTATUS NTAPI NewLdrLoadDll(
	IN PWCHAR               PathToFile OPTIONAL,
	IN ULONG                Flags OPTIONAL,
	IN PUNICODE_STRING      ModuleFileName,
	OUT PHANDLE             ModuleHandle)
{
	TCHAR path[MAX_PATH];
	if (ModuleFileName && lstrcpynW(path, ModuleFileName->Buffer, ModuleFileName->Length) != NULL)
		printf("LdrLoadDll: module = %ws\n", path);
	return OriginalLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);
}

int main()
{
	HMODULE ntdllBaseAddress = GetModuleHandleA("ntdll.dll");
	if (!ntdllBaseAddress) {
		printf("GetModuleHandleA failed\n");
		return 0;
	}
	uintptr_t ldrLoadDllAddress = (uintptr_t)GetProcAddress(ntdllBaseAddress, "LdrLoadDll");
	if (!ldrLoadDllAddress) {
		printf("GetProcAddress failed\n");
		return 0;
	}
	OriginalLdrLoadDll = (PFLDRLOADDLL)HookFunction(ldrLoadDllAddress, (uintptr_t)NewLdrLoadDll, 5);
	printf("ldrLoadDllAddress = %p | OriginalLdrLoadDll = %p\n", ldrLoadDllAddress, OriginalLdrLoadDll);
	getchar();
	return 0;
}