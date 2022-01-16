#include <cstdio>
#include <cstdint>

int main() {

	int amount = 90000;


	std::printf("Address of `amount` variable: %p\n Value of `amount` in decimal : %d\n Value of `amount` in hex: %#.8x\n\n",
		&amount, amount, amount);

	std::uint8_t* adressOfamount = reinterpret_cast<std::uint8_t*> (&amount);
	for (int i = 0; i < sizeof(int); i++) {
		std::printf("\nAdress of byte %d: 0x%p | value %#.2x\n", i + 1, adressOfamount + i, *(adressOfamount+ i));
	}

	getchar();
	return 0;
}