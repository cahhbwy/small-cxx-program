#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#if defined(_WIN32)
#define bool unsigned char
#define false 0x00
#define true  0x01
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#include<conio.h>
#else
#include<curses.h>
#endif

int size = 4;
float probability = 0.9f;
unsigned int *grid;
void create() {
	int *blank = (int*)calloc(size*size, sizeof(int));
	int nums = 0, i;
	for (i = 0; i < size*size; ++i) {
		if (grid[i] == 0) {
			blank[nums] = i;
			++nums;
		}
	}
	i = blank[(int)(((float)rand() / RAND_MAX)*nums)];
	grid[i] = rand() < RAND_MAX*probability ? 2 : 4;
	free(blank);
}
bool moveOnce(int *index) {
	int blank = 0, i;
	bool changed = false;
	for (i = 1; i < size; ++i) {
		if (grid[index[i]] != 0) {
			while (blank < i) {
				if (grid[index[blank]] == 0 || grid[index[blank]] == grid[index[i]]) {
					changed = true;
					grid[index[blank]] += grid[index[i]];
					if (grid[index[blank]] != grid[index[i]]) {
						++blank;
					}
					grid[index[i]] = 0;
					break;
				} else {
					++blank;
				}
			}
		}
	}
	return changed;
}
bool moveWhole(char operation) {
	bool changed = false;
	int i, j;
	int *index = (int*)calloc(size, sizeof(int));
	switch (operation) {
		case 'u':
			for (i = 0; i < size; ++i) {
				for (j = 0; j < size; ++j) {
					index[j] = j*size + i;
				}
				changed |= moveOnce(index);
			}
			break;
		case 'd':
			for (i = 0; i < size; ++i) {
				for (j = 0; j < size; ++j) {
					index[j] = (size - 1 - j)*size + i;
				}
				changed |= moveOnce(index);
			}
			break;
		case 'l':
			for (i = 0; i < size; ++i) {
				for (j = 0; j < size; ++j) {
					index[j] = i*size + j;
				}
				changed |= moveOnce(index);
			}
			break;
		case 'r':
			for (i = 0; i < size; ++i) {
				for (j = 0; j < size; ++j) {
					index[j] = i*size + (size - 1 - j);
				}
				changed |= moveOnce(index);
			}
			break;
		default:break;
	}
	free(index);
	return changed;
}
void show() {
	int i, j;
#if defined(_WIN32)
	system("cls");
#else
	system("clear");
#endif
	for (i = 0; i < size; ++i) {
		for (j = 0; j < size; ++j) {
			if (grid[i*size + j] == 0) {
				printf("  ___");
			} else {
				printf("%5d", grid[i*size + j]);
			}
		}
		printf("\r\n");
	}
	printf("\r\n");
}
bool control() {
#if defined(_WIN32)
	int command = _getch();
#else
	int command = getch();
#endif
	switch (command) {
		case KEY_UP:
		case '8':return moveWhole('u');
		case KEY_LEFT:
		case '4':return moveWhole('l');
		case KEY_DOWN:
		case '5':return moveWhole('d');
		case KEY_RIGHT:
		case '6':return moveWhole('r');
		default:return false;
	}
}
char isFinished(bool keep) {
	int i, j;
	if (!keep) {
		unsigned int max = 0;
		for (i = 0; i < size*size; ++i) {
			max = max >= grid[i] ? max : grid[i];
		}
		if (max == 2048) {
			return 1;
		}
	}
	if (grid[0] == 0) {
		return 0;
	}
	for (i = 1; i < size; ++i) {
		if (grid[i*size] == 0) {
			return 0;
		}
		for (j = 0; j < size; ++j) {
			if (grid[j*size + i] == 0 || grid[i + j*size] == grid[i - 1 + j*size] || grid[i*size + j] == grid[(i - 1)*size + j]) {
				return 0;
			}
		}
	}
	return 2;
}
int main(int argc, char** argv) {
	if (argc >= 2) {
		size = atoi(argv[1]);
		if (argc >= 3) {
			probability = atof(argv[2]);
		}
	}
	grid = (unsigned int*)calloc(size*size, sizeof(unsigned int));
	if (grid == NULL) {
		printf("calloc error.\n");
		return 1;
	}
	srand((unsigned)time(NULL));

#if !defined(_WIN32)
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	refresh();
#endif
	bool keep = false;
	bool changed = true;
	char state;
	while (true) {
		if (changed) {
			create();
		}
		state = isFinished(keep);
		show();
		if (state == 0) {
			changed = control();
		} else if (state == 1) {
			printf("����y/n(other key)?\n");
			char x = getchar();
			fflush(stdin);
			if (x == 'y') {
				keep = true;
			} else {
				break;
			}
		} else {
			break;
		}
	}
#if !defined(_WIN32)
	endwin();
#endif
	free(grid);
	return 0;
}
