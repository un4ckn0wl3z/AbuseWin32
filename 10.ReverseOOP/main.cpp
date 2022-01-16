#include <cstdio>
#include <cstdint>


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


	int theSum = 0;

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