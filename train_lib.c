#include "header.h"

extern unsigned char map[GAME_SIZE][GAME_SIZE];
unsigned char pre_map[GAME_SIZE][GAME_SIZE] = {0};

int stopStar = 1;

int getKey() {

	int key;

	key = getch();

	if(key == 224) key = getch();

	return key;
}

DWORD WINAPI StarTimer(LPVOID lpParam) {
    
	int i = 0;
	DWORD start = GetTickCount64();

    while(stopStar) {

		Sleep(2000 - i);
		if(stopStar == 1) random_target();

		if(2000 - i > 1000) i += 10;
	}

	return 0;
}

int init(TRAIN *T) {

	int i, k;

	T->head = 0;

	memset( pre_map, 10, GAME_SIZE * GAME_SIZE );

	for(i = 0 ; i < TRAIN_LEN ; i++) {

		T->train[i].row = 0;
		T->train[i].col = 0;
	}

	for(i = 0 ; i < GAME_SIZE ; i++) {
		for(k = 0 ; k < GAME_SIZE ; k++) {

			if(map[i][k] == USER) {
				
				T->train[0].row = i;
				T->train[0].col = k;

				return 0;
			}
		}
	}

	return 0;
}


void gotoxy(int x, int y) {

	static HANDLE	h = 0;
	COORD			c = {x, y};

	if(!h) {
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	SetConsoleCursorPosition(h, c);
}

int display() {

	int i, k;

	srand( (unsigned)time(NULL) );

	for(i = 0 ; i < GAME_SIZE ; i++) {
		for(k = 0 ; k < GAME_SIZE ; k++) {

			if( pre_map[i][k] != map[i][k] ) gotoxy(k*2, i);

			else continue;

			switch( map[i][k] ) {

			case WALL:
				printf("□");
				break;
				
			case BLANK:
				printf("  ");
				break;

			case USER:
				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 10 );
				printf("◎");
				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 15 );
				break;
				
			case TARGET:	

				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 6 );
				printf("★");
				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 15 );
				break;

			case TAIL:
				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 10 );
				printf("★");
				SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 15 );
				break;

			default:
				break;
			}
			pre_map[i][k] = map[i][k];
		}
	}

	return 0;
}


int random_target() {

	int i;
	int targetRow, targetColumn;

	srand( (unsigned)time(NULL) );

	for(i = 0 ; i < 20 ; i++) {

		targetRow = (rand() % 18) + 1;
		targetColumn = (rand() % 18) + 1;

		if(map[targetRow][targetColumn] == BLANK) {
			
			map[targetRow][targetColumn] = TARGET;

			gotoxy(targetColumn*2, targetRow);
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 6 );
			printf("★");
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 15 );
			break;
		}
	}
	
	return 0;
}

int checkMove(int key, TRAIN *T) {

	int temp = 0;
	int tempRow, tempColumn;
	static int check = 1;

	tempRow = T->train[T->head].row;
	tempColumn = T->train[T->head].col;

	if( (key == KEY_RIGHT) || (key == KEY_DOWN) ) temp = 1;
	else if( (key == KEY_LEFT) || (key == KEY_UP) ) temp = -1;

	if( (key == KEY_UP) || (key == KEY_DOWN) )  tempRow += temp;
	else if( (key == KEY_RIGHT) || (key == KEY_LEFT) ) tempColumn += temp;

	if(map[tempRow][tempColumn] == TARGET) {
		check = 2;
		T->head++;
	}

	if( (map[tempRow][tempColumn] == BLANK) || 
		(map[tempRow][tempColumn] == TARGET) ) return check; //1 -> 뒤에 별 x / 2 -> 뒤에 별 o 

	else return 0; // 0을 return -> 움직일 칸이 벽이거나 꼬리여서 move 불가!

}

int moveUser(int key, int step, TRAIN *T) {

	int temp, i;
	int tempRow, tempColumn;

	tempRow = T->train[T->head].row;
	tempColumn = T->train[T->head].col;
	

	if(step == 2 && T->train[T->head].row == 0) { //별을 먹었을 때
		tempRow = T->train[T->head-1].row;
		tempColumn = T->train[T->head-1].col;
	}

	if( (key == KEY_RIGHT) || (key == KEY_DOWN) ) temp = 1;
	else if( (key == KEY_LEFT) || (key == KEY_UP) ) temp = -1;

	if( (key == KEY_UP) || (key == KEY_DOWN) )  tempRow += temp;
	else if( (key == KEY_RIGHT) || (key == KEY_LEFT) ) tempColumn += temp;

	if(step == 1) {

		map[tempRow][tempColumn] = USER;
		map[T->train[T->head].row][T->train[T->head].col] = BLANK;

		T->train[T->head].row = tempRow;
		T->train[T->head].col = tempColumn;

		gotoxy(0, 20);
		printf("Head	: (%d, %d) \n", T->train[T->head].row, T->train[T->head].col);
	}

	else if(step == 2) {

		//다음칸이 BLANK
		if(map[tempRow][tempColumn] == BLANK) { 
			
			map[T->tail.row][T->tail.col] = BLANK;

			for(i = 0 ; i < T->head ; i++) {

				T->train[i].row = T->train[i+1].row;
				T->train[i].col = T->train[i+1].col;
			}
		}

		// 공통 (다음칸이 TARGET)
		T->train[T->head].row = tempRow;
		T->train[T->head].col = tempColumn;
		
		map[tempRow][tempColumn] = USER;
		map[T->train[T->head-1].row][T->train[T->head-1].col] = TAIL;

		T->tail.row = T->train[0].row;
		T->tail.col = T->train[0].col;

		gotoxy(0, 20);
		printf("Head		: (%d, %d) \n", T->train[T->head].row, T->train[T->head].col);
		gotoxy(0, 21);
		printf("Head - 1	: (%d, %d) \n", T->train[T->head-1].row, T->train[T->head-1].col);
	}

	return 0;
}

int checkFinish(TRAIN *T) {

	int row, col;

	row = T->train[T->head].row;
	col = T->train[T->head].col; 

	if( (map[row + 1][col] == TAIL) ||
		(map[row + 1][col] == WALL) ) {

		if( (map[row - 1][col] == TAIL) ||
		    (map[row - 1][col] == WALL) ) {

			if( (map[row][col + 1] == TAIL) ||
			    (map[row][col + 1] == WALL) ) {

				if( (map[row][col - 1] == TAIL) ||
				    (map[row][col - 1] == WALL) ) {
					
						display();

						return 1;
				}
			}
		}
	}

	return 0; // 안 끝남
}

void drawingFin() {

	gotoxy(8, 4);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 1 );
	printf("*");

	gotoxy(3, 13);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 10 );
	printf("*");

	gotoxy(22, 3);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 14 );
	printf("*");

	gotoxy(36, 2);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 2 );
	printf("*");

	gotoxy(10, 20);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 12 );
	printf("*");

	gotoxy(28, 7);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 4 );
	printf("*");

	gotoxy(16, 15);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 5 );
	printf("*");

	gotoxy(34, 17);
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 6 );
	printf("*");

	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), 15 );
}

void finish() {

	int i;

	stopStar = 0;

	system("cls");

	drawingFin();
	
	gotoxy(GAME_SIZE - 7, GAME_SIZE / 2);
	
	for(i = 0 ; CLOSING[i] != '\0' ; i++) {

		printf("%c", CLOSING[i]);
		
		if(i != 4) Sleep(500);
	}

	for(i = 0 ; i < 3 ; i++) {

		system("cls");
		Sleep(300);

		drawingFin();

		gotoxy(GAME_SIZE - 7, GAME_SIZE / 2);
		printf("%s", CLOSING);

		Sleep(300);
	}

	gotoxy(0, 23);
}