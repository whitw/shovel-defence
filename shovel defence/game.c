#include "define.h"
#include "game.h"

#pragma warning(disable:4996)
color colorRoad[5] = { green, bloody, aqua , sky, lviolet };
//extern : Key
extern char keyNormal[8][2];
extern char keyShort[2][2];
extern char keyCommon[6][2];
extern char keySpeed[3][2];

//Road

pos start = { 8, 6 };
_road road[5];
_road* roadEnd[5]; //현재 작업중인 길의 끝지점을 저장함.
int currentRoad = 0; //지금 선택하고 있는 길

					 //MAP & config

int map[MAX_UD][MAX_LR];//받아온 파일은 여기에 복사하고, 여기에다 플레이시 변하는 것들을 담는다. 파일 내용은 건드리지 않는다.
pos enemystart[5];
pos pt = { 0,0 };
enemy enemyarr[5][1000]; //
int money, healthMax, currentHealth;
int tick[3] = { 500,250,125 };
int gameSpeed = 0;
int openedUnit[10] = {1,0};

void game()
{
	FILE* fp;
	char filename[25], string[15];
	int select;
	//level select
	select = printlevel();
	if (select == -1)
		return;
	fp = fopen("config.txt", "rt"); //키 입력 가져오기 시작
	if (fp == NULL)
	{
		fp = fopen("config.txt", "wt");
		fprintf(fp, "0 113\n0 119\n0 101\n0 114\n0 97\n0 115\n0 100\n0 102\n0 97\n0 100\n0 32\n0 27\n-32 72\n-32 75\n-32 77\n-32 80\n0 49\n0 50\n0 51\n");//초기 상태로 복구.
		fclose(fp);
		fp = fopen("config.txt", "rt");
	}
		for (int i = 0; i < 8; i++)
			fscanf(fp, "%c %c\n", &keyNormal[i][0], &keyNormal[i][1]);
		for (int i = 0; i < 2;i++)
			fscanf(fp, "%c %c\n", &keyShort[i][0], &keyShort[i][1]);
		for (int i = 0; i < 6;i++)
			fscanf(fp, "%c %c\n", &keyCommon[i][0], &keyCommon[i][1]);
		for (int i = 0; i < 3; i++)
			fscanf(fp, "%c %c\n", &keySpeed[i][0], &keySpeed[i][1]);
		fclose(fp);
	//키 설정 가져오기 끝
	fp = fopen("levels.txt", "rt");//파일 이름 가져오기 시작
	if (fp == NULL)
	{
		fp = fopen("levels.txt", "wt");
		fclose(fp);
		fp = fopen("levels.txt", "rt");
	}
	for (int i = 0; i < select; i++)
	{
		fscanf(fp, "%*s");
		fscanf(fp, "%*d");//읽지 않을 부분은 버린다.
	}
	strcpy(filename, "MAP\\");
	fscanf(fp, "%s", string);
	strcat(string, ".level");
	strcat(filename, string);
	fclose(fp);//파일 이름 가져오기 끝
	//copy MAP file to array
	fp = fopen(filename, "rt");//여기서부터 fp는 맵 파일을 저장한다.
	printmap(fp);//파일을 읽어서 맵에 복사한다. 이 함수가 끝난 뒤에 fp는 이벤트 위치를 가리키고 있어야 한다.
	printgame();
	readfile(fp);//여기서 파일을 읽어나가면서 한 줄씩 실행. 항상 road(길파기) -> start(실제 게임) -> if(ifclear) -> end순서를 사용할 것.
	fclose(fp);
}
void printmap(FILE* fp)
{
	pos castleMid = { -1,-1 };
	int temp;
	rewind(fp);
	for (int i = 0; i < MAX_UD; i++) //파일에서 받아오기 시작
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			fscanf(fp, "%d", &map[i][j]);
		}
		fscanf(fp, "\n");
	}
	system("cls");
	setColor(yellow);
	initSquare(start, "●");
	gotoxy(start.x, start.y);
	printArrMap();
	//파일의 남은 부분들을 읽어서 저장한다. 적의 시작 위치, 적군 배열, 주어진 돈, 초기 성의 체력.
	for (int i = 0; i < 5; i++)//적의 시작 위치
		fscanf(fp,"%d %d", &enemystart[i].x,&enemystart[i].y);

	for (int i = 0; i < 5; i++)//적군 배열
	{
		for (int j = 0; j < 1000; j++)
		{
			fscanf(fp,"%d", &temp);
			 initEnemy(&enemyarr[i][j],temp);
			if(temp == -1)
			{
				break;
			}
		}
	}
	fscanf(fp, "%d", &money);
	fscanf(fp, "%d", &healthMax);
	currentHealth = healthMax;
}
void readfile(FILE* fp) //파일을 읽으면서 게임 순서를 지정하고 실행한다.
{
	FILE* config;
	FILE* level;
	FILE* ftest;
	char ch[256];
	char temp[128];
	pos talkp = { start.x + 2 * MAX_LR + 10, start.y - 4 };
	//파일 읽고 돌리기 시작.
	setColor(yellow);
	gotoxy(start.x + MAX_LR, start.y - 3); printf("●게임 속도:");
	gotoxy(start.x + 2 * MAX_LR - 1, start.y - 3); printf("%1d●",gameSpeed);
	gotoxy(start.x + MAX_LR, start.y - 2); printf("●돈:");
	gotoxy(start.x + 2 * MAX_LR - 14, start.y - 2); printf("%10u코인●",money);
	config = fopen("config.txt", "rt");
																																															talk(talkp, "갸아악", 1000);
																																															if (makeroad()){
																																																talk(talkp, "안녕하시오?", 500);
																																																startGame();
																																															}
	level = fopen("levels.txt", "at");//이미 game에서 만들었으므로 없을 리가 없다.
	if (config == NULL)
	{
		config = fopen("config.txt", "wt");
		fclose(config);
		config = fopen("config.txt", "rt");
	}
	while (feof(fp) != 0)//파일을 다 읽을 때 까지
	{
		fgets(ch, 256, fp);//한 줄 씩 읽은 뒤 맨 앞글자를 읽어서
		switch (ch[0])
		{
		case ';'://주석처리 ;
			break;
		case '\"'://비서가 하는 말. "
			talk(talkp, ch + 1, 500);
			break;
		case 'O'://새로운 유닛을 연다. 이미 열려있으면 열지 않음.
			break;
		case 'L'://새로운 레벨을 연다. 단순히 levels.txt에 추가하면 되는 식.
			strcmp(temp, "MAP\\");
			strcat(temp, ch + 2);
			strcat(temp, ".level");
			ftest = fopen(temp, "rt");
			if (ftest == NULL)
			{
				fprintf(level, "%s 0\n", ch + 2);
				fclose(ftest);
			}
		case 'r'://길 파기
			makeroad();
			break;
		case 's'://게임 시작
			startGame();
			break;
		case 'e'://게임 끝. 별 갯수를 계산하고 레벨에 저장해야 한다.
			
			break;
		}
	}
	fclose(config);
	fclose(level);
}
int startGame() //본 게임. 배열만 이용하고 파일은 건들지 않는다.반환값은 성의 남은 체력. 0이면 게임오버고 체력이 낮으면 별도 적은 방식. 반환값은 뭐지...? 뭐였더라
{
	pos castleMid = { -1,-1 };
	char ch[2] = { 0 };
	time_t t1, t2; //게임 속도 조절에 사용한다.
	int currentTick = 0; //현재까지 지난 틱. 0부터 시작해서 999에서 1000으로 바뀔 때 끝.
	int enemyIndex[5] = { 0 };//지금 출력해야 하는 적 배열. 귀찮게 5개를 쓰는 이유는 이미 -1이 나온 배열에서는 읽어서는 안되기 때문.
	t1 = clock();
	setColor(yellow);
	gotoxy(cmdcol / 3, cmdrow / 3 + 0); printf("▩▩▩▩▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 1); printf("        ▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 2); printf("▩▩▩▩▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 3); printf("        ▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 4); printf("▩▩▩▩▩");
	Sleep(1000);

	gotoxy(cmdcol / 3, cmdrow / 3 + 0); printf("▩▩▩▩▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 1); printf("        ▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 2); printf("▩▩▩▩▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 3); printf("▩        ");
	gotoxy(cmdcol / 3, cmdrow / 3 + 4); printf("▩▩▩▩▩");
	Sleep(1000);

	gotoxy(cmdcol / 3, cmdrow / 3 + 0); printf("        ▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 1); printf("        ▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 2); printf("        ▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 3); printf("        ▩");
	gotoxy(cmdcol / 3, cmdrow / 3 + 4); printf("        ▩");
	Sleep(1000);
	system("cls");
	initSquare(start, "●");
	printgame();
	gotoxy(start.x, start.y);
	printArrMap();
	setColor(sky);
	gotoxy(start.x + 2 * MAX_LR + 10, start.y + 4); printf("■■■■■■■");
	for (int i = 0; i < 5; i++){
		gotoxy(start.x + 2 * MAX_LR + 10, start.y + 5 + i); printf("■          ■");
	}
	gotoxy(start.x + 2 * MAX_LR + 10, start.y + 10); printf("■■■■■■■");
	gotoxy(cmdcol / 4 * 3 - 9, start.y + 4); printf(">>              <<");
	while (1)
	{
		/***************이쪽에서는 한 틱당 한번만 하면 되는 것들을 담당합니다********************/
		//적 유닛이 있을 경우 이동
												//적군 그래픽 지우기, 이 과정에서 적군 그래픽도 지운다.
		for (int i = 0; i < 5; i++)	//roadEnd를 맨 뒤로 보낸 뒤 앞으로 한 칸씩 옮기면서 적들을 뒤로 한칸씩 옮긴다.
		{
			roadEnd[i] = &(road[i]);
			while (roadEnd[i]->next != NULL)
			{
				roadEnd[i] = roadEnd[i]->next;
				clearCur(roadEnd[i]->here, ROAD);
			}
		}
		//맨 마지막에 있는 적들은 지우고 성의 체력을 깎는다.
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < roadEnd[i]->enemyIndex; j++)
			{
				currentHealth -= roadEnd[i]->enemyStart[j]->hp * roadEnd[i]->enemyStart[j]->damage;
				roadEnd[i]->enemyStart[j] = NULL;
				roadEnd[i]->enemyIndex = 0;
			}
		for (int i = 0; i < 5;i++)
			roadEnd[i] = roadEnd[i]->before;
		for (int i = 0; i < 5; i++)
		{
			while (roadEnd[i]->next != NULL)
			{
				for (int j = 0; j < roadEnd[i]->before->enemyIndex;j++)
					if (roadEnd[i]->enemyStart[j]->isFrozen <= 0)
					{
						roadEnd[i]->enemyStart[j] = roadEnd[i]->before->enemyStart[j];
						roadEnd[i]->enemyIndex = roadEnd[i]->before->enemyIndex;
						roadEnd[i] = roadEnd[i]->before;
					}
					else roadEnd[i]->enemyStart[j]->isFrozen--;
			}
		}
		//적 유닛 출현. enemyIndex 수정
		//for (int i = 0; i < 5;i++)
		

		//적 유닛 그리기
		for (int i = 0; i < 5; i++)
		{
			roadEnd[i] = &(road[i]);
			while (roadEnd[i]->next != NULL)
			{
				roadEnd[i] = roadEnd[i]->next;
				if (roadEnd[i]->enemyIndex)
				{
					gotoxy(start.x + 2 * pt.x, start.y + pt.y);
					printf("%s", roadEnd[i]->enemyStart[0]->pic);
				}
			}
		}
		//아군 유닛의 공격


		do {
			/***************여기서는 상시로 해야 하는 것들을 담당합니다***********************/
			if (_kbhit())
			{
				ch[0] = _getch();
				ch[1] = 0;
				if (ch[0] == 0x80 || ch[0] == 0)
					ch[1] = _getch();
			}
			//포인터 지우기
			//포인터 이동
			//포인터 표시
			//파란 사각형에 블럭 종류 표시
			t2 = clock();
		} while (t2 - t1 < tick[gameSpeed]);
	}
	return 1;
}
void showSelectedRoad()
{
	gotoxy(start.x + 2 * pt.x, start.y + pt.y);
	setColor(colorRoad[currentRoad]);
	printf("▤");
}
int makeroad() //길파기. 파일에서 읽어온 배열을 사용하되 파일은 건들면 안된다. 반환값은 성공적으로 끝내지 못했으면 0
{//pos enemystart[5]을 이용한다. 초기화는 되어 있으니 그대로 쓰면 됨.
	int ret = 0;
	char ch[2] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		if (enemystart[i].x != -1)
		{
			pt.x = enemystart[0].x;
			pt.y = enemystart[0].y;
			break;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		road[i].here.x = enemystart[i].x;
		road[i].here.y = enemystart[i].y;
		road[i].next = NULL;
		road[i].before = NULL;
		roadEnd[i] = &road[i];
	}
	//길 파기
	showSelectedRoad();
	while (1)
	{
		ch[0] = _getch();
		ch[1] = 0;
		if (_kbhit())
		{
			ch[0] = 0;
			ch[1] = _getch();
		}
		if (ch[0] == ESC && ch[1] == 0)
		{
			gotoxy((int)(cmdcol * 0.6 - 10), cmdrow / 2 - 10);
			printf("시작 화면으로 돌아갑니다. 정말로 끝낼까요?(ENTER를 눌러서 확인)");
			ch[0] = _getch();
			ch[1] = 0;
			if (_kbhit())
			{
				ch[0] = 0;
				ch[1] = _getch();
			}
			if (ch[0] == ENTER && ch[1] == 0)
				return 0;
		}
		if (ch[0] == SPACE && ch[1] == 0)
		{
			do {
				currentRoad = (currentRoad + 1) % 5;
			} while (roadEnd[currentRoad]->here.x == -1 || map[roadEnd[currentRoad]->here.y][roadEnd[currentRoad]->here.x] == CASTLE_DOOR);
			clearCur(pt, map[pt.y][pt.x]);
			pt = roadEnd[currentRoad]->here; 
			showSelectedRoad();
		}
		if (pt.y > 0 && ch[1] == UP || ch[0] == 'w' || ch[0] == 'W')//커서가 위로 움직일 수 있는 상황에서 UP키가 눌림.
		{
			clearCur(pt, map[pt.y][pt.x]); //전에 있던 커서를 지운다.
			ret = updateRoad(UP);
		}
		else if (pt.x > 0 && (ch[1] == LEFT || ch[0] == 'a' || ch[0] == 'a'))
		{
			clearCur(pt, map[pt.y][pt.x]);
			ret = updateRoad(LEFT);
		}
		else if (pt.x < MAX_LR && (ch[1] == RIGHT || ch[0] == 'd' || ch[0] == 'D'))
		{
			clearCur(pt, map[pt.y][pt.x]);
			ret = updateRoad(RIGHT);
		}
		else if (pt.y < MAX_UD && (ch[1] == DOWN || ch[0] == 's' || ch[0] == 's'))
		{
			clearCur(pt, map[pt.y][pt.x]);
			ret = updateRoad(DOWN);
		}
		if (ret == 1)
			break;
	}
	return 1;
}
void clearCur(pos pt, int block)
{
	gotoxy(start.x + 2 * pt.x, start.y + pt.y);
	switch (block)
	{
	case EMPTY:
		setColor(gray);
		printf(". ");
		break;
	case ENEMYPOS:
		setColor(bloody);
		printf("◆");
		break;
	case ROAD:
		setColor(yellow);
		printf("▤");
		break;
	case INVALID:
		setColor(bloody);
		printf("IV");
		break;
	default:
		setColor(bloody);
		printf("??");
		break;
	}
}
int updateRoad(dir direction)
{
	if ((direction == UP && map[pt.y - 1][pt.x] == EMPTY)\
		|| (direction == LEFT &&  map[pt.y][pt.x - 1] == EMPTY)\
		||(direction == RIGHT &&  map[pt.y][pt.x + 1] == EMPTY)\
		||(direction == DOWN &&  map[pt.y + 1][pt.x] == EMPTY))//새로 길을 만들 수 있다면
	{
		//새로 길을 만든다.
		roadEnd[currentRoad]->next = (_road*)malloc(sizeof(road));
		roadEnd[currentRoad]->next->before = roadEnd[currentRoad];
		roadEnd[currentRoad] = roadEnd[currentRoad]->next;
		if (direction == UP){
			roadEnd[currentRoad]->before->direct = UP;
			pt.y--;
		}
		else if (direction == LEFT){
			roadEnd[currentRoad]->before->direct = LEFT;
			pt.x--;
		}
		else if (direction == RIGHT){
			roadEnd[currentRoad]->before->direct = RIGHT;
			pt.x++;
		}
		else if (direction == DOWN) {
			roadEnd[currentRoad]->before->direct = DOWN;
			pt.y++;
		}
		showSelectedRoad();
		map[pt.y][pt.x] = ROAD;
		roadEnd[currentRoad]->here.x = pt.x;
		roadEnd[currentRoad]->here.y = pt.y;
	}
	else if ((direction == UP && map[pt.y - 1][pt.x] == ROAD && roadEnd[currentRoad]->before->direct == DOWN) || \
		(direction == LEFT && map[pt.y][pt.x - 1] == ROAD&& roadEnd[currentRoad]->before->direct == RIGHT) || \
		(direction == RIGHT && map[pt.y][pt.x + 1] == ROAD&& roadEnd[currentRoad]->before->direct == LEFT) || \
		(direction == DOWN && map[pt.y + 1][pt.x] == ROAD&& roadEnd[currentRoad]->before->direct == UP))//돌아가려고 하는 경우에는
		{//이전 블럭으로 간 경우에는 길을 지운다.
			map[pt.y][pt.x] = EMPTY;
			clearCur(pt, map[pt.y][pt.x]);
			if(direction == UP)pt.y--;
			if (direction == LEFT)pt.x--;
			if (direction == RIGHT)pt.x++;
			if (direction == DOWN)pt.y++;
			showSelectedRoad();
			roadEnd[currentRoad] = roadEnd[currentRoad]->before;
			free(roadEnd[currentRoad]->next);
			roadEnd[currentRoad]->next = NULL;
			roadEnd[currentRoad]->direct = NO;
		//그 외의 경우에는 서로 다른 길이 겹치거나 하나의 길이 꼬이는 경우밖에 없으므로 아무것도 하지 않는다. 
	}
	else if ((direction == UP && map[pt.y - 1][pt.x] == ROAD)||
		(direction == LEFT && map[pt.y][pt.x - 1] == ROAD) || \
		(direction == RIGHT && map[pt.y][pt.x + 1] == ROAD) || \
		(direction == DOWN && map[pt.y + 1][pt.x] == ROAD))//길이 겹치거나 꼬이는 경우
	{
		showSelectedRoad();
	}
	else if ((direction == UP && map[pt.y - 1][pt.x] == CASTLE_DOOR)\
		|| (direction == LEFT &&  map[pt.y][pt.x - 1] == CASTLE_DOOR)\
		|| (direction == RIGHT &&  map[pt.y][pt.x + 1] == CASTLE_DOOR)\
		|| (direction == DOWN &&  map[pt.y + 1][pt.x] == CASTLE_DOOR))
	{
		if (direction == UP)pt.y--;
		if (direction == LEFT)pt.x--;
		if (direction == RIGHT)pt.x++;
		if (direction == DOWN)pt.y++;
		roadEnd[currentRoad]->next = (_road*)malloc(sizeof(road));
		roadEnd[currentRoad]->next->before = roadEnd[currentRoad];
		roadEnd[currentRoad] = roadEnd[currentRoad]->next;
		roadEnd[currentRoad]->next = NULL;
		roadEnd[currentRoad]->here = pt;
		if (direction == UP) {
			roadEnd[currentRoad]->before->direct = UP;
			pt.y--;
		}
		else if (direction == LEFT) {
			roadEnd[currentRoad]->before->direct = LEFT;
			pt.x--;
		}
		else if (direction == RIGHT) {
			roadEnd[currentRoad]->before->direct = RIGHT;
			pt.x++;
		}
		else if (direction == DOWN) {
			roadEnd[currentRoad]->before->direct = DOWN;
			pt.y++;
		}
		for (int i = 0;; i++)
		{
			if (i >= 5 || roadEnd[i]->here.x == -1)//모든 길을 이음.
			{
				Sleep(1000);
				system("cls");
				return 1;//완성 신호
			}
			else if (map[roadEnd[i]->here.y][roadEnd[i]->here.x] != CASTLE_DOOR) //완성되지 않은 길을 찾았다
			{
				currentRoad = i;
				pt.x = roadEnd[currentRoad]->here.x;
				pt.y = roadEnd[currentRoad]->here.y;
				showSelectedRoad();
				break;
			}

		}
	}
	else
	{
		showSelectedRoad();
		return 0;
	}
	return 0;
}
void printArrMap()
{
	pos castleMid;
	for (int i = 0; i < MAX_UD; i++) //출력하기 시작
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			switch (map[i][j])
			{
			case INVALID:
				printf("  ");
				break;
			case ENEMYPOS:
				setColor(bloody);
				printf("◆");
				break;
			case EMPTY:
				setColor(gray);
				printf(". ");
				break;
			case STONE:
				setColor(gray);
				printf("■");
				break;
			case CASTLE_DOOR:
				setColor(violet);
				printf("∩");
				break;
			case CASTLE_INV:
				setColor(gray);
				printf("  ");
				break;
			case CASTLE:
				printf("  ");
				castleMid.y = i;
				castleMid.x = j;
				break;
			case ROAD:
				setColor(yellow);
				printf("▤");
				break;
			default:
				setColor(bloody);
				printf("??");
				break;
			}
		}
		gotoxy(start.x, start.y + i + 1);
	}
	setColor(sky);
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y - 2); printf("/|  /|  /|");
	setColor(ivory);
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y - 1); printf("□  □  □");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y); printf("□□□□□");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 1); printf("□++〓++□");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 2); printf("□□∩□□");
}
void initEnemy(enemy* ep, enemyT type)
{
	ep->type = type;
	ep->isFrozen = 0;
	ep->speed = 1; //한 턴마다 한 번 이동
	switch (type)
	{
	case eChicken:
		ep->damage = 1;//피해상수
		ep->hp = 10;
		ep->money = 1;
		strcpy(ep->pic, "O>");
		ep->color = ivory;
		break;
	case eThief:
		ep->damage = 1;
		ep->hp = 40;
		ep->money = 2;
		strcpy(ep->pic, "人");
		ep->color = gray;
		break;
	case eRunner:
		ep->speed = 2;
		ep->damage = 2;
		ep->hp = 30;
		ep->money = 5;
		strcpy(ep->pic, "&>");
		ep->color = yellow;
		break; 
	case ePirates:
		ep->damage = 2;
		ep->hp = 50;
		ep->money = 5;
		strcpy(ep->pic, "% ");
		ep->color = blue;
		break;
	case eBomb:
		ep->damage = 20;
		ep->hp = 50;
		ep->money = 10;
		strcpy(ep->pic, "●");
		ep->color = gray;
			break;
	case eShield:
		ep->damage = 1;
		ep->hp = 200;
		ep->money = 2;
		strcpy(ep->pic, "[]");
		ep->color = ivory;
		break;
	case eAirship:
		ep->damage = 1;
		ep->hp = 300;
		ep->money = 10;
		strcpy(ep->pic, "▶");
		ep->color = sky;
		break; 
	case eTank:
		ep->damage = 2;
		ep->hp = 100;
		ep->money = 5;
		strcpy(ep->pic, "±");
		ep->color = green;
		break;
	case	eKnight:
		ep->damage = 10;
		ep->hp = 50;
		ep->money = 20;
		strcpy(ep->pic, "Ω");
		ep->color = bloody;
		break;
	case eKing:
		ep->damage = 1;
		ep->hp = 1000;
		ep->money = 100;
		strcpy(ep->pic, "Ψ");
		ep->color = lyellow;
		break;
	}
}
void initUnit(unit* u, unitT type, pos position, int currentTick)
{
	u->type = type;
	u->upgrade = 1;
	u->pos = position;
	u->lastTick = currentTick;
	switch (type)
	{
	case uVillager:
		u->str = 2;
		u->delay = 1;
		u->rng = 2;
		u->cost = 5;
		strcpy(u->pic, "人");
		u->color = ivory;
		break;
	case uArchery:
		u->str = 5;
		u->delay = 2;
		u->rng = 5;
		u->cost = 25;
		strcpy(u->pic, "))");
		u->color = yellow;
		break;
	case uCannon:
		u->str = 7;
		u->delay = 2;
		u->rng = 3;
		u->cost = 25;
		strcpy(u->pic, "ㄷ");
		u->color = gray;
		break;
	case uSniper:
		u->str = 20;
		u->delay = 6;
		u->rng = 15;
		u->cost = 100;
		strcpy(u->pic, "⊙");
		u->color = violet;
		break;
	case uIce:
		u->str = 0;
		u->delay = 1;
		u->rng = 2;
		u->cost = 50;
		strcpy(u->pic, "※");
		u->color = sky;
		break;
	case uLaser:
		u->str = 2;
		u->delay = 2;
		u->rng = 10;
		u->cost = 100;
		strcpy(u->pic, "=");
		u->color = bloody;
		break;
	case uFarm:
		u->str = 0;
		u->delay = 1;
		u->rng = 0;
		u->cost = 100;
		strcpy(u->pic, "▤");
		u->color = green;
		break;
	case uWarrior:
		u->str = 10;
		u->delay = 2;
		u->rng = 1;
		u->cost = 50;
		strcpy(u->pic, "+ ");
		u->color = white;
		break;
	case uTank:
		u->str = 10;
		u->delay = 10;
		u->rng = 10;
		u->cost = 200;
		strcpy(u->pic, "人");
		u->color = ivory;
		break;
	}
	//여기서 _road* 목록을 편집해야 한다. 죄다 배열/링크드 리스트로 박아넣고 앞에서부터 읽던 어쩌던 해야 함.
}
void upgradeUnit(unit *u)
{
	if (u->upgrade < 3)
		u->upgrade++;
}