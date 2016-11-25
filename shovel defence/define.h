#ifndef define_H
#define define_H
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <direct.h>
/**************Define_SizeCMD******************/
//콘솔창 크기
#define cmdrow  40
#define cmdcol 140
//배열 크기(==맵 크기)
#define MAX_UD 30 //위아래 길이
#define MAX_LR 30 //왼쪽 오른쪽 길이
/****************typedef*************************/
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE;

typedef enum color {
	black, blue, green, sky, red, violet, yellow, ivory, gray, dblue, rime, aqua, bloody, \
	lviolet, lyellow, white
} color;
typedef enum unitT {
	uKid, uArchery, uCannon, uLaser,uWarrior,uSniper,uFarm,uTank
}unitT;
typedef enum enemyT {
	eEmpty,eChicken,eThief,eRunner,ePirates,eBomb,eShield,eAirship,eTank,eKnight,eKing,eEnd = -1
}enemyT;
typedef struct pos
{
	int x;
	int y;
}pos;
typedef struct unit
{
	unitT type;
	int str;
	int delay;
	int rng;
	int cost;
	char* pic;
	color color;
	pos pos;
}unit;
typedef struct enemy
{
	enemyT type;
	int hp;
	int speed;
	char* pic;
	color color;
}enemy;
/****************Define_Ascii********************/
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define ENTER 13
#define SPACE 32
#define ESC 27
#define BKSP 8
/***************Define_MAP_UNIT***************/
#define EMPTY 0
#define STONE 1
#define CASTLE 2
#define ENEMYPOS 3
#define CASTLE_DOOR 4
#define CASTLE_INV 5 //성 주변의 무효한 부분.
/*****************Define_function*****************/
#define ColorInit() GetStdHandle(STD_OUTPUT_HANDLE)
#define setColor(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x)
#define cmdsize(x, y) {char string[30];sprintf(string,"%s%d%s%d","mode con: cols=",x," lines=",y);system(string);}
/*****************function_graphic***************/
void gotoxy(int x, int y);
void setcursortype(CURSOR_TYPE);
void init();//초기화가 필요한 함수들의 초기화.
void intro();//인트로 영상을 출력합니다.
void printmain();//초기 메인 화면을 출력합니다.
int printlevel();//초기 레벨 선택 화면을 출력합니다.
void selectlevel();//레벨 화면에서 선택을 담당.
void game(); //본 게임
void levelmaker();//레벨 메이커
void option();
int getselect();
void initSquare(pos p, char* str);
/******************function_game*****************/
void printmap(FILE* fp);
void printgame(); //게임 화면 초기화.
void readfile(FILE* fp); //파일을 읽으면서 게임 순서를 지정하고 실행한다.
void talk(pos position , char* str);
/******************function_levelmaker************/
int mapEdit(FILE* fp);
void initMapEdit();
void waveEdit(FILE* fp);
void fileEdit(FILE* fp);//게임 순서를 지정하는 파일을 만듬.
#endif