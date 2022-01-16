#include <cstdio>
#include <cstdint>


int GetAverage(int a, int b, int c) {
	int sum = a + b + c;
	int average = sum / 3;

	return average;
}

int main() {

	printf("x86 asm.....PUSH/POP\n");
	getchar();

	/* The original C code*/
	/*
	int theAverage = GetAverage(10, 5 , 15);
	printf("theAverage = %d\n", theAverage);

	*/


	/* ASM Code */

	__asm {

		/* it push righ to left params*/

		push 15		// param `c`
		push 5		// param `b`
		push 10		// param `a`

		call GET_AVERAGE_FUNCTION // cpu will also push the address of next instruction (in this case it push addres of `add esp, 10` instructions)

		add esp, 10 // clear above 3 params from stack: 4 + 4 + 4 = 12 bytes

		jmp DONE_EXECUTE

		GET_AVERAGE_FUNCTION:
/**/	push ebp // save previous EBP on stack
/**/	mov ebp, esp // move the value of esp to ebp
/* this is prolog */
/**/	sub esp, 8 // allocate memory space to local variable for `sum` and `average` 4 + 4 = 8 bytes

		// calculate the sum and store it in ecx
		mov ecx, [ebp + 8]		// [ebp + 8] = address of parameter `a` on stack
		add ecx, [ebp + 12]		// [ebp + 12] = address of parameter `b` on stack
		add ecx, [ebp + 16]		// [ebp + 16] = address of parameter `c` on stack

		mov [ebp - 4], ecx		// [ebp - 4] = address of `sum` variable on stack

		xor edx, edx			// set edx register to zero for the next instruction (div)
		mov eax, [ebp - 4]		// move `sum` value to eax
		mov ecx, 3				// move divisor 3 into ecx
		div ecx					// divides eax by ecx. Quotient is store in eax, remainder in edx

		mov [ebp - 8], eax		// [ebp - 8] = address of `average` on stack (cuz the original C code is `int average = sum / 3` program will move eax to `average` location)

		mov eax, [ebp - 8]		// for return value (cuz `return average` in original C code, result is normally in EAX)

		add esp, 8				// clear local variable on stack

/**/	mov esp, ebp			// restore old esp value
/**/	pop ebp					// restore old ebp value
/**/	ret						// return (will pop ESP to EIP for the next instruction for execute)
/* this is epilogue */

		DONE_EXECUTE:			// DONE section

	}


	return 0;
}