#include <cstdio>
#include <cstdint>
#include <iostream>


class Player 
{
public:
	char name[20];  // instance address + 0
	int skinID;     // instance address + sizeof(name)
	float health;	// instance address + sizeof(name) + izeof(skinID)
	float armour;	// instance address + sizeof(name) + izeof(skinID) + izeof(health)

	/*
	assuming instance address is 0x500ACE65
	char name[20];  // 0x500ACE65 + 0
	int skinID;     // 0x500ACE65 + 20
	float health;	// 0x500ACE65 + 24
	float armour;	// 0x500ACE65 + 28
	*/

	Player(char* theName, int theSkinID, float theHealth, float theArmour) {
		std::memcpy(name, theName, sizeof(name));
		skinID = theSkinID;
		health = theHealth;
		armour = theArmour;
	}

};

int main() {

	// player instance
	Player myPlayer((char*)"Anuwat", 100, 75.0f, 8.8f);
	std::printf("\nsize of `myPlayer`:%u | address of `myPlayer` :%p\n\n", sizeof(myPlayer), &myPlayer);
	getchar();

	Player* pPlayer = &myPlayer;
	/* You can do this */
	/* Player* pPlayer = (Player*)0x002DFBE4; */ 
	/* Player* pPlayer = reinterpret_cast<Player*>(0x002DFBE4); */

	std::printf("\nName: %s\nSkin ID: %d\nHealth: %.2f\nArmour: %.2f\n\n",
		pPlayer->name, pPlayer->skinID, pPlayer->health, pPlayer->armour
		);

	getchar();
	/* If you have only address, do this */
	std::uint8_t* playerAdress = reinterpret_cast<std::uint8_t*>(&myPlayer);
	char* pName = (char*)(playerAdress);
	int* pSkinID = (int*)(playerAdress + 0x14); // 20 in dec
	float* pHealth = (float*)(playerAdress + 0x18); // 24 in dec
	float* pArmour = (float*)(playerAdress + 0x1C); // 28 in dec
	 
	std::printf("\npName: %s\npSkin ID: %d\npHealth: %.2f\npArmour: %.2f\n\n",
		pName, (*pSkinID), (*pHealth), (*pArmour)
	);

	getchar();

	



	return 0;
}