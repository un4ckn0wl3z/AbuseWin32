#include <cstdio>
#include <cstdint>



int main() {

	printf("x86 asm.....PUSH/POP\n");
	getchar();


	// LOOP IN ASM
	__asm {

		mov eax, 50 // set eax to 50
		push eax // push eax to top of stack

		mov eax, 0 // set eax to 0 for initial loop couter

		REPEAT_CODE:

		inc eax // increse eax value by 1

		cmp eax, 10 // compare if eax lower than 10 the jump to REPEAT_CODE section
		jl REPEAT_CODE

		pop eax // restore eax value by pop the value on the top of stack to eax


	}



	return 0;
}