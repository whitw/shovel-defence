#pragma once
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
	int damage; //피해상수. hp * damage가 성에 데미지로 들어간다.
	char pic[3];
	color color;
	int isFrozen; //0이 아니면 얼려진 상태. 0이면 제대로 이동. frozen공격을 2i턴에 한번, i씩 먹이면 속도가 절반이 된다. 
						 //!!!!이미 언 놈을 공격하면 절대 안된다.
						 //자칫하면 무료하게 적들이 굳은 채 움직이지 않을 수 있음. 또는 얼음공격에 데미지를 주는 방식을 써도 되겠지만 이건 칸당 10명 룰을 넘길 것 같다. 
	int money;
}enemy;
typedef struct rd
{
	pos here; // 블럭의 위치
	dir direct; //방향, 길을 만들 때 쓴다만 있을 필요가 있을지 고민중.
	struct rd* before;//이전 길 블럭.
	struct rd* next; //다음 길 블럭.
	struct enemy* enemyStart[10]; //여기가 적 배열의 시작지점. 설마 한칸에 10기 이상 들어가겠어<<<하지만 얼음이나 달리는 놈 때문에 들어갈 수 도 있다.<<<<수정 방법을 찾아야 한다.
	int enemyIndex; //그 칸에 적이 몇 마리나 있는지를 저장한다. 적이 죽거나 빠져나가는 경우 0번에서부터 제거, 적이 들어오는 경우에는 enemyStart[enemyIndex]에 넣고 enemyIndex++하면 된다.
}_road;
typedef struct unit
{
	unitT type;
	int upgrade;//1~3, 처음 만들 떄 1~ 최종단계 3. upgradeUnit()에 넣어서 업그레이드하자.
	int str;
	int delay;
	int lastTick;//마지막으로 공격한 틱
	int rng;
	int cost;
	char pic[3];
	color color;
	pos pos;
	struct rd* road[100];//공격 범위에 있는 길 목록. 이것도 나중에 링크드 리스트로 고쳐야 한다.
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
#define CASTLE_INV 5 //성 주변의 무효한 부분.
#define INVALID 6 //맵 바깥.
#define ROAD 10 //길.
#define UNIT 29 //유닛. 숫자 29는 그냥 갑자기 땡겨서 넣음
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