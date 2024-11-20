#ifndef _HEADER_H
#define _HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<Windows.h>

#define TRAIN_LEN		200
#define	GAME_SIZE		20

#define WALL			0
#define BLANK			1
#define USER			2
#define TARGET			3
#define TAIL			4

#define KEY_UP			72
#define KEY_DOWN		80
#define KEY_RIGHT		77
#define KEY_LEFT		75

#define CLOSING			"GAME FINISH!!"

typedef struct _POSOTION {

	int row;
	int col;

} POSITION;

typedef struct _TRAIN {

	int head;
	POSITION tail;
	POSITION train[TRAIN_LEN];

} TRAIN;

DWORD WINAPI StarTimer(LPVOID lpParam);
int getKey();
void gotoxy(int ,int );
int init(TRAIN *);
int display();
int random_target();
int checkMove(int , TRAIN *);
int moveUser(int , int , TRAIN *);
int checkFinish(TRAIN *);
void drawingFin();
void finish();

#endif