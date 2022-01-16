#include <cstdio>
#include <cstdint>



int main() {

	/* original C Code */
	/*
	int balance = 130;
	int paymentAmount = 50;

	if (balance > paymentAmount) {
		printf("processing payment\n");
	}
	else {
		printf("cancelling payment\n");
	}
	*/

	printf("x86 asm.....MOV\n");
	getchar();


	/* in ASM Code*/

	__asm {


		mov eax, 130 // balacne
		mov ecx, 50 // payment

		cmp eax, ecx // compare value balacne with payment
		jl CANCEL_PAYMENT_CODE // if eax lower than ecx, will jump....

		jmp DONE_EXECUTING // else jump to DONE_EXECUTING

		CANCEL_PAYMENT_CODE:

		DONE_EXECUTING:

	}




	return 0;
}