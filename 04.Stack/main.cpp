#include <cstdio>
#include <cstdint>

void printBalanceSum(int balance1, int balance2, int balance3) {
	int balanceSum = balance1 + balance2 + balance3;
	printf("Balance sum = %d\n", balanceSum);
}

void printBalance(int a, int b, int c) {
	int user1Balance = a;
	int user2Balance = b;
	int user3Balance = c;
	printBalanceSum(a,b,c);
}


int main() {

	getchar();
	printBalance(48, 33, 85);
	return 0;
}