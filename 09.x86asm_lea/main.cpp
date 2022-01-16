#include <cstdio>
#include <cstdint>

void myFunction(int* param1, int* param2) {
	*param1 = 5;
	*param2 = 23;
}



int main() {

	printf("x86 asm.....LEA\n");
	getchar();

	__asm {

		push ebp // save previos ebp
		mov ebp, esp // move value of esp to ebp

		sub esp, 8 // create space for 2 int

		// prepare pointer params for calling myFunction
		lea eax, [ebp - 8] // load address of [ebp - 8] to eax
		push eax // push it to stack for param2
		lea eax, [ebp - 4] // load address of [ebp - 4] to eax
		push eax // push it to stack for param1

		call myFunction

		add esp, 8 // clear function params for myFunction (param1 , param2)

		mov eax, [ebp - 4] // move value of 1st local variable to eax
		mov eax, [ebp - 8] // move value of 2nd local variable to eax

		add esp, 8 // clear local variable ([ebp - 4]) and ([ebp - 8])

		mov esp, ebp
		pop ebp

	}


	
	return 0;
}