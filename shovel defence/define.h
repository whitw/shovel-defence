#pragma once
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
typedef enum keyT {
	normalKey,
	shortenKey
} keyT;
typedef enum dr {
	UP = 72,
	LEFT = 75,
	RIGHT = 77,
	DOWN = 80,
	NO = 0
} dir;
typedef enum color {
	black, blue, green, sky, red, violet, yellow, ivory, gray, dblue, rime, aqua, bloody, \
	lviolet, lyellow, white
} color;
typedef enum unitT {
	uVillager,
	uArchery,
	uCannon,
	uSniper,
	uIce,
	uLaser,
	uFarm,
	uWarrior,
	uTank
}unitT;
typedef enum enemyT {
	eEmpty,
	eChicken,
	eThief,
	eRunner,
	ePirates,
	eBomb,
	eShield,
	eAirship,
	eTank,
	eKnight,
	eKing,
	eEnd = -1
}enemyT;
typedef struct pos
{
	int x;
	int y;
}pos;
typedef struct enemy
{
	enemyT type;
	int hp;
	int speed;
	int damage; //���ػ��. hp * damage�� ���� �������� ����.
	char pic[3];
	color color;
	int isFrozen; //0�� �ƴϸ� ����� ����. 0�̸� ����� �̵�. frozen������ 2i�Ͽ� �ѹ�, i�� ���̸� �ӵ��� ������ �ȴ�. 
						 //!!!!�̹� �� ���� �����ϸ� ���� �ȵȴ�.
						 //��ĩ�ϸ� �����ϰ� ������ ���� ä �������� ���� �� ����. �Ǵ� �������ݿ� �������� �ִ� ����� �ᵵ �ǰ����� �̰� ĭ�� 10�� ���� �ѱ� �� ����. 
	int money;
}enemy;
typedef struct rd
{
	pos here; // ���� ��ġ
	dir direct; //����, ���� ���� �� ���ٸ� ���� �ʿ䰡 ������ �����.
	struct rd* before;//���� �� ��.
	struct rd* next; //���� �� ��.
	struct enemy* enemyStart[10]; //���Ⱑ �� �迭�� ��������. ���� ��ĭ�� 10�� �̻� ���ھ�<<<������ �����̳� �޸��� �� ������ �� �� �� �ִ�.<<<<���� ����� ã�ƾ� �Ѵ�.
	int enemyIndex; //�� ĭ�� ���� �� ������ �ִ����� �����Ѵ�. ���� �װų� ���������� ��� 0���������� ����, ���� ������ ��쿡�� enemyStart[enemyIndex]�� �ְ� enemyIndex++�ϸ� �ȴ�.
}_road;
typedef struct unit
{
	unitT type;
	int upgrade;//1~3, ó�� ���� �� 1~ �����ܰ� 3. upgradeUnit()�� �־ ���׷��̵�����.
	int str;
	int delay;
	int lastTick;//���������� ������ ƽ
	int rng;
	int cost;
	char pic[3];
	color color;
	pos pos;
	struct rd* road[100];//���� ������ �ִ� �� ���. �̰͵� ���߿� ��ũ�� ����Ʈ�� ���ľ� �Ѵ�.
}unit;

/****************Define_Ascii********************/

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
#define INVALID 6 //�� �ٱ�.
#define ROAD 10 //��.
#define UNIT 29 //����. ���� 29�� �׳� ���ڱ� ���ܼ� ����
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