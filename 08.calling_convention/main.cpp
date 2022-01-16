#include <cstdio>
#include <cstdint>


int __cdecl Sum(int a, int b) {
	return a + b;
}

int __stdcall SumEx(int a, int b) {
	return a + b;
}

int __fastcall SumFast(int a, int b, int c) {
	printf("[+]::debug:: Extended 3rd param is %d\n", c);
	return a + b;
}

class MyClass 
{
public:

	static int Sum(int a, int b) { // static method in class also __cdecl
		return a + b;
	}

};


class MyScoreClass {

public:

	int scoreA;
	int scoreB;

	MyScoreClass() { // __thiscall (hidden parameter is `this` it set to ECX)
		scoreA = 0;
		scoreB = 0;
	}

	int SetNewScore(int a, int b)  // __thiscall (hidden parameter is `this` it set to ECX)
	{
		scoreA = a;
		scoreB = b;

		printf("The score of `a` and `b` is set!\n");

		return  a + b;
	}

};




int main() {

	getchar();

	int theSum = 0;

	/* call the classic Sum (__cdecl convention) */
	__asm {


		push 15		// param `b`
		push 10		// param `a`

		call Sum

		add esp, 8
		mov theSum, eax // move result from Sum() to theSum variable

	}

	printf("__cdecl Sum = %d\n", theSum);
	getchar();


	/* call the __stdcall Sum */

	__asm {


		push 20		// param `b`
		push 30		// param `a`

		call SumEx

		// calle clear stack

		mov theSum, eax // move result from Sum() to theSum variable

	}



	printf("__stdcall Sum = %d\n", theSum);
	getchar();


	/* call the __fastcall Sum */

	__asm {


		push 7			// param `c`
		mov edx, 2		// param `b`
		mov ecx, 4		// param `a`

		call SumFast

		// calle clear stack

		mov theSum, eax // move result from Sum() to theSum variable

	}

	printf("__fastcall Sum = %d\n", theSum);
	getchar();




	/* call the __thiscall Class METHOD */

	// define custom type for SetNewScore method pointer
	typedef int (MyScoreClass::* fpSetNewScoreFn)(int a, int b);

	MyScoreClass myScore;
	MyScoreClass* pThis = &myScore; // keep the position of `myScore` into `pThis` pointer

	// defind function pointer
	fpSetNewScoreFn SetNewScoreFn = &MyScoreClass::SetNewScore;

	__asm {

		mov ecx, pThis // mov address of `myScore` to ECX
		push 3
		push 5
		call SetNewScoreFn

		// calle clear stack

		mov theSum, eax

	}

	printf("__thiscall Sum = %d\n", theSum);
	getchar();


	return 0;
}