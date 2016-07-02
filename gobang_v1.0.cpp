//玩家持黑棋1先手mn，电脑持白棋2后手xy,返回值表示胜利方
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define size 15
#define halfsize 7
short chess[size][size];
void init();
bool check(int x, int y, bool player);
bool setInput(int m, int n, bool &player);
int value(int x, int y, bool player);
void computer(int &x, int &y);
void printChess();
int main() {
	init();
	int x, y;
	bool player = true;	//true user,false computer
	do {
		system("cls");
		printChess();
		if (player) {
			printf("Enter x and y:");
			while (!(scanf("%d%d", &x, &y) == 2 && setInput(x, y, player))) {
				printf("Enter Error, Please try again\nEnter x and y:");
			}
		} else {
			computer(x, y);
			setInput(x, y, player);
		}
	} while (!check(x, y, player));
	system("cls");
	printChess();
	if (!player) {
		printf("Well done! You win!!\n");
	} else {
		printf("Sorry, you failed\n");
	}
	system("pause");
	return 0;
}
void init() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			chess[i][j] = 0;
		}
	}
}
bool check(int x, int y, bool player) {
	if (value(x, y, !player)>30000) {
		return true;
	}
	return false;
}
bool setInput(int x, int y, bool &player) {
	if (x < 0 || x > size - 1 || y < 0 || y > size - 1) {
		return false;
	}
	if (chess[x][y] == 0) {
		if (player)
			chess[x][y] = 1;
		else
			chess[x][y] = 2;
		player = !player;
		return true;
	} else {
		return false;
	}
}
int value(int x, int y, bool play) {
	short player = play ? 1 : 2;
	int v = 0, k;
	int up, down, left, right, num = 0, x1, x2, y1, y2;
	up = x - 4 > 0 ? x - 4 : 0;
	down = x + 4 < size - 1 ? x + 4 : size - 1;
	left = y - 4 > 0 ? y - 4 : 0;
	right = y + 4 < size - 1 ? y + 4 : size - 1;
	//横向
	x1 = x2 = x;
	while (x1 >= up && chess[x1][y] == player)	x1--;
	while (x2 <= down && chess[x2][y] == player)	x2++;
	if (x2 - x1 < 6) {
		k = 64 << (x2 - x1);
	} else {
		return 32365 + player*100;
	}
	if (x1 == -1 || chess[x1][y] == (player ^ 3))	k /= 2;
	if (x2 == size || chess[x2][y] == (player ^ 3))	k /= 2;
	v += k;
	//纵向
	y1 = y2 = y;
	while (y1 >= left && chess[x][y1] == player)	y1--;
	while (y2 <= right && chess[x][y2] == player)	y2++;
	if (y2 - y1 < 6) {
		k = 64 << (y2 - y1);
	} else {
		return 32365 + player*100;
	}
	if (y1 == -1 || chess[x][y1] == (player ^ 3))	k /= 2;
	if (y2 == size || chess[x][y2] == (player ^ 3))	k /= 2;
	v += k;
	//右下
	x1 = x2 = x;
	y1 = y2 = y;
	while (x1 >= up && y1 >= left && chess[x1][y1] == player) {
		x1--;
		y1--;
	}
	while (x2 <= down && y2 <= right && chess[x2][y2] == player) {
		x2++;
		y2++;
	}
	if (x2 - x1 < 6) {
		k = 64 << (x2 - x1);
	} else {
		return 32365 + player*100;
	}
	if (x1 == -1 || y1 == -1 || chess[x1][y1] == (player ^ 3))	k /= 2;
	if (x2 == size || y2 == size || chess[x2][y2] == (player ^ 3))	k /= 2;
	v += k;
	//左下
	x1 = x2 = x;
	y1 = y2 = y;
	while (x1 >= up && y2 <= right && chess[x1][y2] == player) {
		x1--;
		y2++;
	}
	while (x2 <= down && y1 >= left && chess[x2][y1] == player) {
		x2++;
		y1--;
	}
	if (x2 - x1 < 6) {
		k = 64 << (y2 - y1);
	} else {
		return 32365 + player*100;
	}
	if (x1 == -1 || y2 == size || chess[x1][y2] == (player ^ 3))	k /= 2;
	if (x2 == size || y1 == -1 || chess[x2][y1] == (player ^ 3))	k /= 2;
	v += k;
	return v;
}
void computer(int &x, int &y) {
	int value1[size][size], value2[size][size];
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (chess[i][j] == 0) {
				chess[i][j] = 1;
				value1[i][j] = value(i, j, true) + 128 - (i - halfsize)*(i - halfsize) - (j - halfsize)*(j - halfsize);
				chess[i][j] = 2;
				value2[i][j] = value(i, j, false) + 128 - (i - halfsize)*(i - halfsize) - (j - halfsize)*(j - halfsize);
				chess[i][j] = 0;
			} else {
				value1[i][j] = value2[i][j] = 0;
			}
		}
	}
	int max1x, max1y, max2x, max2y, max1 = 0, max2 = 0;
	srand(time(NULL));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (max1 < value1[i][j]||(max1==value1[i][j]&&rand()%2)) {
				max1x = i;
				max1y = j;
				max1 = value1[i][j];
			}
			if (max2 < value2[i][j]||(max2==value2[i][j]&&rand()%2)) {
				max2x = i;
				max2y = j;
				max2 = value2[i][j];
			}
		}
	}
	if (max1 < max2) {
		x = max2x;
		y = max2y;
	} else {
		x = max1x;
		y = max1y;
	}
}
void printChess() {
	printf("x\\y0");
	for (int i = 1; i < size; i++) {
		printf("%2d", i);
	}
	if (chess[0][0] == 0) {
		printf("\n 0┌");
	} else if (chess[0][0] == 1) {
		printf("\n 0●");
	} else if (chess[0][0] == 2) {
		printf("\n 0○");
	} else {
		chess[0][0] = 0;
		printf("\n 0┌");
	}
	for (int j = 1; j < size - 1; j++) {
		if (chess[0][j] == 0) {
			printf("┬");
		} else if (chess[0][j] == 1) {
			printf("●");
		} else if (chess[0][j] == 2) {
			printf("○");
		} else {
			chess[0][j] = 0;
			printf("┬");
		}
	}
	if (chess[0][size - 1] == 0) {
		printf("┐\n");
	} else if (chess[0][size - 1] == 1) {
		printf("●\n");
	} else if (chess[0][size - 1] == 2) {
		printf("○\n");
	} else {
		chess[0][size - 1] = 0;
		printf("┐\n");
	}
	for (int i = 1; i < size - 1; i++) {
		printf("%2d", i);
		if (chess[i][0] == 0) {
			printf("├");
		} else if (chess[i][0] == 1) {
			printf("●");
		} else if (chess[i][0] == 2) {
			printf("○");
		} else {
			chess[i][0] = 0;
			printf("├");
		}
		for (int j = 1; j < size - 1; j++) {
			if (chess[i][j] == 0) {
				printf("┼");
			} else if (chess[i][j] == 1) {
				printf("●");
			} else if (chess[i][j] == 2) {
				printf("○");
			} else {
				chess[i][j] = 0;
				printf("┼");
			}
		}
		if (chess[i][size - 1] == 0) {
			printf("┤\n");
		} else if (chess[i][size - 1] == 1) {
			printf("●\n");
		} else if (chess[i][size - 1] == 2) {
			printf("○\n");
		} else {
			chess[i][size - 1] = 0;
			printf("┤\n");
		}
	}
	if (chess[size - 1][0] == 0) {
		printf("%2d└", size - 1);
	} else if (chess[size - 1][0] == 1) {
		printf("%2d●", size - 1);
	} else if (chess[size - 1][0] == 2) {
		printf("%2d○", size - 1);
	} else {
		chess[size - 1][0] = 0;
		printf("%2d└", size - 1);
	}
	for (int j = 1; j < size - 1; j++) {
		if (chess[size - 1][j] == 0) {
			printf("┴");
		} else if (chess[size - 1][j] == 1) {
			printf("●");
		} else if (chess[size - 1][j] == 2) {
			printf("○");
		} else {
			chess[size - 1][j] = 0;
			printf("┴");
		}
	}
	if (chess[size - 1][size - 1] == 0) {
		printf("┘\n");
	} else if (chess[size - 1][size - 1] == 1) {
		printf("●\n");
	} else if (chess[size - 1][size - 1] == 2) {
		printf("○\n");
	} else {
		chess[size - 1][size - 1] = 0;
		printf("┘\n");
	}
}

