#include <iostream>
#include <Windows.h>

int hp = 100;

int main() {

	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
			hp -= 7;
			std::cout << "Health: " << hp << std::endl;
		}

		Sleep(50);
	}


	return 0;
}