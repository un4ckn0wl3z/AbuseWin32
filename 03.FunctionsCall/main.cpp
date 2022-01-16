#include <cstdio>
#include <cstdint>
#include <iostream>


int Sum(int score1, int score2) {
	return score1 + score2;
}

int main() {

	int playerScore1 = 10;
	int playerScore2 = 15;

	int sumScore = Sum(playerScore1, playerScore2);
	printf("Player sumScore = %d\n", sumScore);

	getchar();

	return 0;
}