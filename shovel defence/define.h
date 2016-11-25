#ifndef define_H
#define define_H
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <direct.h>
/**************Define_SizeCMD******************/
//�ܼ�â ũ��
#define cmdrow  40
#define cmdcol 140
//�迭 ũ��(==�� ũ��)
#define MAX_UD 30 //���Ʒ� ����
#define MAX_LR 30 //���� ������ ����
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
#define CASTLE_INV 5 //�� �ֺ��� ��ȿ�� �κ�.
/*****************Define_function*****************/
#define ColorInit() GetStdHandle(STD_OUTPUT_HANDLE)
#define setColor(x) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x)
#define cmdsize(x, y) {char string[30];sprintf(string,"%s%d%s%d","mode con: cols=",x," lines=",y);system(string);}
/*****************function_graphic***************/
void gotoxy(int x, int y);
void setcursortype(CURSOR_TYPE);
void init();//�ʱ�ȭ�� �ʿ��� �Լ����� �ʱ�ȭ.
void intro();//��Ʈ�� ������ ����մϴ�.
void printmain();//�ʱ� ���� ȭ���� ����մϴ�.
int printlevel();//�ʱ� ���� ���� ȭ���� ����մϴ�.
void selectlevel();//���� ȭ�鿡�� ������ ���.
void game(); //�� ����
void levelmaker();//���� ����Ŀ
void option();
int getselect();
void initSquare(pos p, char* str);
/******************function_game*****************/
void printmap(FILE* fp);
void printgame(); //���� ȭ�� �ʱ�ȭ.
void readfile(FILE* fp); //������ �����鼭 ���� ������ �����ϰ� �����Ѵ�.
void talk(pos position , char* str);
/******************function_levelmaker************/
int mapEdit(FILE* fp);
void initMapEdit();
void waveEdit(FILE* fp);
void fileEdit(FILE* fp);//���� ������ �����ϴ� ������ ����.
#endif