#include "header.h"

int main() {
	
	int key, step, check = 0;
	HANDLE hThread = CreateThread(NULL, 0, StarTimer, NULL, 0, NULL);
	TRAIN T;

	//Ŀ�� ���߱�
	CONSOLE_CURSOR_INFO CurInfo;

	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo( GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo );

	//�ܼ��� ũ�� ����
	system("mode con: cols=40 lines=24");
	
	init(&T);

	while(1) {	

		display();
		key = getKey();
		step = checkMove(key, &T);

		if(step) {

			moveUser(key, step, &T);
			check = checkFinish(&T);
		}

		if(check) {
			finish();
			break;
		}
	}


	WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

	return 0;
}
