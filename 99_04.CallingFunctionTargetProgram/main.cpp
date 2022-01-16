#include <Windows.h>
#include <iostream>

void FuncA() {
	std::cout << "I'm function A!" << std::endl;
}

void FuncB(const char* string) {
	std::cout << string << std::endl;

}

int main() {
	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
			FuncA();
			FuncB("I'm function B!");
		}
		Sleep(100);
	}

	return 0;
}