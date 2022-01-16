#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

struct Player
{
	int health;
	int attackCount;

	Player(int hp) {
		health = hp;
		attackCount = 0;
	}
};

void Attack(Player& player);

int main() {
	std::cout << "[+] Player initialization" << std::endl;
	Player mPlayer(300);
	std::cout << "[+] Player health: " << mPlayer.health << std::endl;

	while (true)
	{
		if (GetAsyncKeyState(VK_NUMPAD0) & 1) {
			Attack(mPlayer);
			if (mPlayer.health > 0) {
				std::cout << "[+] Current health: " << mPlayer.health << std::endl;
				std::cout << "[+] Attack count: " << mPlayer.attackCount << std::endl;
			}
			else {
				std::cout << "[-] YOU DIED! " <<  std::endl;
				system("pause");
				break;
			}

		}

		Sleep(50);
	}

	return 0;
}


void Attack(Player& player) {
	std::srand(std::time(NULL));
	int decreaseHp = 1 + rand() % 4;
	std::cout << "[+] Attack player by: "<< decreaseHp << " damage!" << std::endl;
	player.health -=  decreaseHp;
	player.attackCount++;
}