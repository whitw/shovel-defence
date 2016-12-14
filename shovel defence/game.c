#include "define.h"
#include "game.h"

#pragma warning(disable:4996)
#pragma warning(disable:4477)
color colorRoad[5] = { green, bloody, aqua , sky, lviolet };
//extern : Key
extern int keyNormal[9][2]; // qwertasdf
extern int keyShort[2][2]; // a d
extern int keyCommon[6][2]; // w a s d select escape
extern int keySpeed[5][2];// speedUp speedDown
extern keyT keyType; 
//Road
unit unitArr[900];//ㅜㅜ
int currentUnit; //현재 유닛의 갯수.
pos start = { 8, 6 };
pos castleMid = { -1,-1 };
_road road[5];
_road* roadEnd[5]; //현재 작업중인 길의 끝지점을 저장함.
int currentRoad = 0; //지금 선택하고 있는 길
int openedUnit[9] = {1 , 0}; //열린 유닛들을 받아온다.
					 //MAP & config

int map[MAX_UD][MAX_LR];//받아온 파일은 여기에 복사하고, 여기에다 플레이시 변하는 것들을 담는다. 파일 내용은 건드리지 않는다.
pos enemystart[5];
pos pt = { 0,0 };
enemyT enemyarr[5][1000]; //
int money, healthMax, currentHealth;
int tick[5] = { 2000, 1000, 500, 250,125 };
int gameSpeed = 2;

void game()
{
	FILE* fp;
	char filename[25], string[20];
	int select;
	//level select
	select = printlevel();
	if (select == -1)
		return;
	//키 설정 가져오기 끝
	fp = fopen("units.txt", "rt"); //열려있는 유닛 종류 가져오기 시작
	if (fp == NULL)
	{
		fp = fopen("units.txt", "wt");
		fprintf(fp, "1 0 0 0 0 0 0 0 0 ");
		fclose(fp);
		fp = fopen("units.txt", "rt");
	}
	for (int i = 0; i < 9; i++)
	{
		fscanf(fp, "%d", &openedUnit[i]);
	}
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
			enemyarr[i][j] = temp;
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
	FILE* units = fopen("units.txt", "rt");
	FILE* config = fopen("config.txt", "rt");;
	FILE* level = fopen("levels.txt", "at");//이미 game에서 만들었으므로 없을 리가 없다.;
	FILE* ftest;
	char ch[256];
	char temp[128];
	int willBegin = 0;
	int star = 0;
	int tempd = 0;
	pos talkp = { start.x + 2 * MAX_LR + 10, start.y - 4 };
	//파일 읽고 돌리기 시작.
	if (units == NULL)
	{
		units = fopen("units.txt", "wt");
		fprintf(units, "1 0 0 0 0 0 0 0 0");
		fclose(units);
		units = fopen("units.txt", "rt");
	}
	for (int i = 0; i < 9; i++)
		fscanf(units, "%d", &openedUnit[i]);
	if (config == NULL)
	{
		config = fopen("config.txt", "wt");
		fclose(config);
		config = fopen("config.txt", "rt");
	}
	if (makeroad())
		startGame();
	while (feof(fp) != 0)//파일을 다 읽을 때 까지
	{
		fscanf(fp, "%d", &ch[0]);
		switch (ch[0])
		{
		case ';'://주석처리 ;
			fscanf(fp, "%*[^\n]");
			break;
		case '\"'://비서가 하는 말. "
			fscanf(fp, "%[^\n]", ch);
			talk(talkp, ch, 500);
			break;
		case 'O'://새로운 유닛을 연다. 이미 열려있으면 열지 않음.
			fscanf(fp, "%d", &temp);
			if(tempd < 10 && tempd >= 0)
				openedUnit[tempd] = 1;
			for (int i = 0; i < 9; i++)
				fprintf(units,"%d ",openedUnit[i]);
			break;
		case 'L'://새로운 레벨을 연다. 단순히 levels.txt에 추가하면 되는 식.
			fscanf(fp, "%s", ch);
			strcmp(temp, "MAP\\");
			strcat(temp, ch);
			strcat(temp, ".level");
			ftest = fopen(temp, "rt");
			if (ftest != NULL)
			{
				fprintf(level, "%s 0\n", ch + 2);
				fclose(ftest);
			}
		case 'r'://길 파기
			willBegin = makeroad();
			break;
		case 's'://게임 시작
			if(willBegin)
				startGame();
			break;
		case 'e'://게임 끝. 별 갯수를 계산하고 레벨에 저장해야 한다.
			if (0.9 * healthMax <= currentHealth)//체력이 90% 이상이면: cur/max>=0.9
				star = 3;
			if (0.5 * healthMax <= currentHealth)//체력이 50% 이상이면
				star = 2;
			if (currentHealth > 0)//체력이 50% 이하이면
				star = 1;
			else star = 0;//클리어하지 못했으면
			break;
		}
	}
	fclose(config);
	fclose(level);
}
int startGame() //본 게임. 배열만 이용하고 파일은 건들지 않는다.반환값은 성의 남은 체력. 0이면 게임오버고 체력이 낮으면 별도 적은 방식. 
{
	int ch[2] = { 0 };
	time_t t1, t2; //게임 속도 조절에 사용한다.
	int currentTick = 0; //현재까지 지난 틱. 0부터 시작해서 999에서 1000으로 바뀔 때 끝.
	int isArrEnd[5] = { 0 };//지금 출력해야 하는 적 배열. 귀찮게 5개를 쓰는 이유는 이미 -1이 나온 배열에서는 읽어서는 안되기 때문.
	int beforeMoney = money; //이전 상태의 돈 상태. 둘이 다를 때만 출력하자.
	int beforeGameSpeed = gameSpeed;
	int isUnitSelected = 0; //유닛이 선택된 상태. 선택되었을 때와 되어있지 않을 때의 오른쪽 레이아웃이 살짝 다르고 ESC키 동작도 다르다.
	int unitPos = 0; //현재 스페이스를 눌러서 설치할 수 있는 있는 유닛.
	int temp = 0;
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
	showHealthBar(currentHealth, healthMax);
	setColor(yellow);
	gotoxy(start.x + MAX_LR, start.y - 3); printf("●게임 속도:");
	gotoxy(start.x + 2 * MAX_LR - 1, start.y - 3); printf("%1d●", gameSpeed + 1);
	gotoxy(start.x + MAX_LR, start.y - 2); printf("●돈:");
	gotoxy(start.x + 2 * MAX_LR - 14, start.y - 2); printf("%10u코인●", money);
	pt = castleMid;
	gotoxy(start.x + 2 * pt.x, start.y + pt.y);
	setColor(sky);
	printf("▣");
	while (1)
	{
		/***************이쪽에서는 한 틱당 한번만 하면 되는 것들을 담당합니다********************/
		gotoxy(start.x, start.y - 2);
		printf("%d", currentTick);
		eraseEnemyGraphic();
		attackCastle();
		moveEnemy();
		//적 유닛 출현
		for (int i = 0; i < 5; i++)
			if (road[i].here.x != -1 && road[i].enemyIndex < 10 && !isArrEnd[i])
			{
				if (enemyarr[i][currentTick] != eEnd && enemyarr[i][currentTick] != eEmpty)
				{
					road[i].enemyStart[road[i].enemyIndex] = (enemy*)malloc(sizeof(enemy));
					initEnemy((road[i].enemyStart[road[i].enemyIndex]), enemyarr[i][currentTick]);
					road[i].enemyIndex++;
				}
				else if(enemyarr[i][currentTick] == eEnd)
					isArrEnd[i] = 1;
			}
		drawEnemy();
		attackEnemy(currentTick);
		currentTick++;
		if (currentTick >= 1000 || currentTick < 0 || currentHealth <= 0)
			return currentHealth;
		do {
			/***************여기서는 상시로 해야 하는 것들을 담당합니다***********************/
			ch[0] = 0;
			ch[1] = 0;
			if (_kbhit())
			{
				ch[0] = _getch();
				ch[1] = 0;
				if (_kbhit())
					ch[1] = _getch();

				if (isUnitSelected) //유닛을 선택하고 있는 경우에는 ESC랑 방향키, Space말고는 쓸 일이 없다.
				{
					//오른쪽에 파란 네모를 업데이트하고

					//그 아래에 왼쪽: 업그레이드, 오른쪽: 제거 딱 두개만 두면 될듯


				}
				else //선택된 유닛이 없는 경우
				{
					if (ch[0] == keyCommon[0][0] && ch[1] == keyCommon[0][1]) //UP
					{
						if (map[pt.y][pt.x] == CASTLE || map[pt.y][pt.x] == CASTLE_INV)
							drawCastle();
						else clearCurEnemy(pt, map[pt.y][pt.x]);
						if (pt.y > 0 && map[pt.y - 1][pt.x] != INVALID)
							pt.y--;
						gotoxy(start.x + 2 * pt.x, start.y + pt.y);
						setColor(sky);
						printf("▣");
					}
					else if (ch[0] == keyCommon[1][0] && ch[1] == keyCommon[1][1]) //LEFT
					{
						if (map[pt.y][pt.x] == CASTLE || map[pt.y][pt.x] == CASTLE_INV)
							drawCastle();
						else clearCurEnemy(pt, map[pt.y][pt.x]);
						if (pt.x > 0 && map[pt.y][pt.x - 1] != INVALID)
							pt.x--;
						gotoxy(start.x + 2 * pt.x, start.y + pt.y);
						setColor(sky);
						printf("▣");
					}
					else if (ch[0] == keyCommon[2][0] && ch[1] == keyCommon[2][1]) //RIGHT
					{
						if (map[pt.y][pt.x] == CASTLE || map[pt.y][pt.x] == CASTLE_INV)
							drawCastle();
						else clearCurEnemy(pt, map[pt.y][pt.x]);
						if (pt.x < MAX_LR - 1 && map[pt.y][pt.x + 1] != INVALID)
							pt.x++;
						gotoxy(start.x + 2 * pt.x, start.y + pt.y);
						setColor(sky);
						printf("▣");
					}
					else if (ch[0] == keyCommon[3][0] && ch[1] == keyCommon[3][1]) //DOWN
					{
						if (map[pt.y][pt.x] == CASTLE || map[pt.y][pt.x] == CASTLE_INV)
							drawCastle();
						else clearCurEnemy(pt, map[pt.y][pt.x]);
						if (pt.y < MAX_UD - 1 && map[pt.y + 1][pt.x] != INVALID)
							pt.y++;
						gotoxy(start.x + 2 * pt.x, start.y + pt.y);
						setColor(sky);
						printf("▣");
					}
					else if (ch[0] == keyCommon[4][0] && ch[1] == keyCommon[4][1])//Select
					{
						if (map[pt.y][pt.x] == UNIT)
						{
							for (int i = 0; i < 900; i++)
							{
								if (unitArr[i].pos.x == pt.x && unitArr[i].pos.y == pt.y)
								{
									switch (unitArr[i].type)
									{
									case uVillager:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(" >>시민<< ");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식: 약하지만 싸다");
										break;
									case uArchery:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(" >>궁수<< ");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식: 시민보다는 세다");
										break;
									case uCannon:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(" >>포병<< ");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식:무난하다");
										break;
									case uSniper:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(">>저격수<<");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식:아무나 하나를 강하게!");
										break;
									case uIce:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(">얼음법사<");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식: 이 주변을 얼릴 거예요.");
										break;
									case uLaser:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(">>레이저<<");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식:직선 위를 한번에 정리");
										break;
									case uFarm:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(" >>농장<< ");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("농사방식:맛있는 식단을 위해");
										break;
									case uWarrior:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(" >>전사<< ");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식:주변에 강하게 공격");
										break;
									case uTank:
										gotoxy(start.x + 2 * MAX_LR + 12, start.y + 5); printf(" >>전차<< ");
										gotoxy(start.x + 2 * MAX_LR + 5, start.y + 9); printf("공격방식: 무작위한 한 점 주변을 공격");
										break;
									}
									gotoxy(start.x + 2 * MAX_LR + 12, start.y + 6); printf("공격력:%d", unitArr[i].str);
									gotoxy(start.x + 2 * MAX_LR + 12, start.y + 7); printf("사거리:%d", unitArr[i].rng);
									gotoxy(start.x + 2 * MAX_LR + 12, start.y + 8); printf("공격딜레이:%d", unitArr[i].delay);
								}
							}
							//오른쪽 파란 네모를 유지하고
							//현재 유닛을 선택한 유닛으로 넣어준다.
							isUnitSelected = 1;
						}
						//이 외의 경우에는 굳이 해야 하는게 없다.
					}
					else if (ch[0] == keyCommon[5][0] && ch[1] == keyCommon[5][1])//exit
					{
						setColor(gray);
						gotoxy(start.x, start.y - 3);
						printf(">일시정지<\t\t");
						while (1)
						{
							if (_kbhit())
								break;
						}
						gotoxy(start.x, start.y - 3);
						printf("\t\t\t\t\t");

						showHealthBar(currentHealth,healthMax);
					}
					if (keyType == normalKey)
					{
						for (int i = 0; i < 8; i++)
							if (ch[0] == keyNormal[i][0] && ch[1] == keyNormal[i][1])
							{
								//그 위치가 비어있다면
								if (map[pt.y][pt.x] == EMPTY)
								{
									//유닛을 새로 설치한다.
									initUnit(&unitArr[currentUnit], (unitT)i, pt, currentTick);
									currentUnit++;
									map[pt.y][pt.x] = UNIT;
									for (int i = 0; i < 5; i++)
									{
										roadEnd[i] = &(road[i]);
										while (roadEnd[i]->next != NULL)
											roadEnd[i] = roadEnd[i]->next;
									}
									//유닛의 공격 범위를 정해서 넣어준다.
								}
								for (int i = 0; i < 5; i++)
									while (roadEnd[i]->before != NULL)
									{
										if ((roadEnd[i]->here.x - pt.x, roadEnd[i]->here.y - pt.y) < unitArr[currentUnit].rng)
											unitArr[currentUnit].road[temp++] = roadEnd[i];
										roadEnd[i] = roadEnd[i]->before;
									}
							}
					}
					else //shortenKey
					{
						if (ch[0] == keyShort[0][0] && ch[1] == keyShort[0][1])
						{

						}
						if (ch[1] == keyShort[1][0] && ch[1] == keyShort[1][1])
						{

						}
					}
					beforeGameSpeed = gameSpeed;
					for (int i = 0; i < 5;i++)
						if (ch[0] == keySpeed[i][0] && ch[1] == keySpeed[i][1])
							gameSpeed = i;
				}
			}
			setColor(yellow);
			if (beforeGameSpeed != gameSpeed)
			{
				gotoxy(start.x + 2 * MAX_LR - 1, start.y - 3); printf("%1d●", gameSpeed + 1);
				beforeGameSpeed = gameSpeed;
			}
			if (money != beforeMoney)
			{
				gotoxy(start.x + 2 * MAX_LR - 14, start.y - 2); printf("%10u코인●", money);
				beforeMoney = money;
			}
			t2 = clock();
		} while (t2 - t1 < tick[gameSpeed]);
		t1 = clock();
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
	int ch[2] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		if (enemystart[i].x != -1)
		{
			pt.x = enemystart[0].x;
			pt.y = enemystart[0].y;
			for (int i = 0; i < 5; i++)
				road[i].enemyIndex = 0;

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
			ch[1] = _getch();
		}
		if (ch[0] == keyCommon[5][0] && ch[1] == keyCommon[5][1]) //esc
		{
			gotoxy((int)(cmdcol * 0.6 - 10), cmdrow / 2 - 10);
			setColor(gray);
			printf("시작 화면으로 돌아갑니다. 정말로 끝낼까요?");
			ch[0] = _getch();
			ch[1] = 0;
			if (_kbhit())
			{
				ch[0] = 0;
				ch[1] = _getch();
			}
			if (ch[0] == keyCommon[4][0] && ch[1] == keyCommon[4][1])
			{
				for (int i = 0; i < 5; i++)//길을 초기화.
				{
					if (road[i].here.x != -1)
					{
						roadEnd[i] = &(road[i]);
						while (roadEnd[i]->next != NULL)
							roadEnd[i] = roadEnd[i]->next;
						while (roadEnd[i] != &(road[i]))
						{
							roadEnd[i] = roadEnd[i]->before;
							free(roadEnd[i]->next);
							roadEnd[i]->next = NULL;
						}
					}
				}
				currentRoad = 0;
				return 0;
			}
		}
		if (ch[0] == keyCommon[4][0] && ch[1] == keyCommon[4][1])
		{
			do {
				currentRoad = (currentRoad + 1) % 5;
			} while (roadEnd[currentRoad]->here.x == -1 || map[roadEnd[currentRoad]->here.y][roadEnd[currentRoad]->here.x] == CASTLE_DOOR);
			clearCur(pt, map[pt.y][pt.x]);
			pt = roadEnd[currentRoad]->here; 
			showSelectedRoad();
		}
		if (pt.y > 0 && ch[0] == keyCommon[0][0] && ch[1] == keyCommon[0][1])//커서가 위로 움직일 수 있는 상황에서 UP키가 눌림.
		{
			clearCur(pt, map[pt.y][pt.x]); //전에 있던 커서를 지운다.
			ret = updateRoad(UP);
		}
		else if (pt.x > 0 && ch[0] == keyCommon[1][0] && ch[1] == keyCommon[1][1])
		{
			clearCur(pt, map[pt.y][pt.x]);
			ret = updateRoad(LEFT);
		}
		else if (pt.x < MAX_LR - 1 && ch[0] == keyCommon[2][0] && ch[1] == keyCommon[2][1])
		{
			clearCur(pt, map[pt.y][pt.x]);
			ret = updateRoad(RIGHT);
		}
		else if (pt.y < MAX_UD - 1 && ch[0] == keyCommon[3][0] && ch[1] == keyCommon[3][1])
		{
			clearCur(pt, map[pt.y][pt.x]);
			ret = updateRoad(DOWN);
		}
		if (ret == 1)
			break;
	}
	return 1;
}
void showHealthBar(int health, int maxHealth)
{
	setColor(gray);
	gotoxy(start.x, start.y - 4);printf("체력:%05d/%05d", health, maxHealth);
	gotoxy(start.x, start.y - 3);
	setColor(bloody);
	for (int i = 0; i < (int)(10.0 * health / maxHealth); i++)
		printf("■");
	for (int i = 0; i < 10 - (int)(10.0 * health / maxHealth) && i < 10; i++)
		printf("□");
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
	case STONE:
		setColor(gray);
		printf("■");
		break;
	case ENEMYPOS:
		setColor(bloody);
		printf("◆");
		break;
	case CASTLE_DOOR:
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
void clearCurEnemy(pos pt, int block)
{
	_road* roadEndTemp;
	int end = 0;
	gotoxy(start.x + 2 * pt.x, start.y + pt.y);
	switch (block)
	{
	case EMPTY:
		setColor(gray);
		printf(". ");
		break;
	case STONE:
		setColor(gray);
		printf("■");
		break;
	case ENEMYPOS:
		setColor(bloody);
		printf("◆");
		break;
	case CASTLE_DOOR:
		setColor(yellow);
		printf("▤");
		break;
	case ROAD:
		for (int i = 0; i < 5; i++)
		{
			if (road[i].here.x == -1)
				continue;
			roadEndTemp = &(road[i]);
			while (roadEndTemp != NULL)
			{
				if (pt.x == roadEndTemp->here.x && pt.y == roadEndTemp->here.y)
				{
					if (roadEndTemp->enemyIndex != 0)
					{
						setColor(roadEndTemp->enemyStart[0]->color);
						printf("%s", roadEndTemp->enemyStart[0]->pic);
					}
					else
					{
						setColor(yellow);
						printf("▤");
					}
					end = 1;
					break;
				}
				else roadEndTemp = roadEndTemp->next;
			}
			if (end == 1)
				break;
		}
		break;
	case INVALID:
		setColor(bloody);
		printf("IV");
		break;
	case UNIT:
		for (int i = 0; i <= currentUnit; i++)
		{
			if (pt.x == unitArr[i].pos.x && pt.y == unitArr[i].pos.y)
			{
				setColor(unitArr[i].color);
				printf("%s", unitArr[i].pic);
				break;
			}
		}
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
		roadEnd[currentRoad]->next = NULL;
		roadEnd[currentRoad]->enemyIndex = 0;
		for (int i = 0; i < 10;i++)
			roadEnd[currentRoad]->enemyStart[i] = NULL;
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
	else if ((direction == UP && map[pt.y - 1][pt.x] == ENEMYPOS && roadEnd[currentRoad]->before->direct == DOWN) || \
		(direction == LEFT && map[pt.y][pt.x - 1] == ENEMYPOS&& roadEnd[currentRoad]->before->direct == RIGHT) || \
		(direction == RIGHT && map[pt.y][pt.x + 1] == ENEMYPOS&& roadEnd[currentRoad]->before->direct == LEFT) || \
		(direction == DOWN && map[pt.y + 1][pt.x] == ENEMYPOS&& roadEnd[currentRoad]->before->direct == UP))//돌아가려고 하는 경우에는
	{//이전 블럭으로 간 경우에는 길을 지운다.
		map[pt.y][pt.x] = EMPTY;
		clearCur(pt, map[pt.y][pt.x]);
		if (direction == UP)pt.y--;
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
		|| (direction == DOWN &&  map[pt.y + 1][pt.x] == CASTLE_DOOR))//끝내야 하는 경우
	{
		if (direction == UP)pt.y--;
		if (direction == LEFT)pt.x--;
		if (direction == RIGHT)pt.x++;
		if (direction == DOWN)pt.y++;
		roadEnd[currentRoad]->next = (_road*)malloc(sizeof(road));
		roadEnd[currentRoad]->next->before = roadEnd[currentRoad];
		roadEnd[currentRoad] = roadEnd[currentRoad]->next;
		roadEnd[currentRoad]->next = NULL;
		roadEnd[currentRoad]->enemyIndex = 0;
		for (int i = 0; i < 10;i++)
			roadEnd[currentRoad]->enemyStart[i] = NULL;
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
	drawCastle();
}
void initEnemy(enemy* ep, enemyT type)
{
	if (type != eEmpty)
	{
		ep->type = type;
		ep->isFrozen = 0;
		ep->speed = 1; //한 턴마다 한 번 이동
	}
	switch (type)
	{
	case eEmpty:
		break;
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
		strcpy(ep->pic, "%%");
		ep->color = sky;
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
	switch (type)
	{
	case uVillager:
		if (money > 10)
			money -= 10;
		else return;
		break;
	case uArchery:
		if (money > 15)
			money -= 15;
		else return;
		break;
	case uCannon:
		if (money >= 15)
			money -= 15;
		else return;
		break;
	case uSniper:
		if (money >= 100)
			money -= 100;
		else return;
		break;
	case uIce:
		if (money >= 50)
			money -= 50;
		else return;
		break;
	case uLaser:
		if (money >= 70)
			money -= 70;
		else return;
		break;
	case uFarm:
		if (money >= 150)
			money -= 150;
		else return;
		break;
	case uWarrior:
		if (money >= 30)
			money -= 30;
		else return;
		break;
	case uTank:
		if (money > 200)
			money -= 200;
		else return;
		break;
	}
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
		u->str = 2;
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
}
void upgradeUnit(unit *u)
{
	if (u->upgrade = 1)
	{
		switch (u->type)
		{
		case uVillager:
			if (money > 10)
				money -= 10;
			else return;
			break;
		case uArchery:
			if (money > 15)
				money -= 15;
			else return;
			break;
		case uCannon:
			if (money >= 15)
				money -= 15;
			else return;
			break;
		case uSniper:
			if (money >= 100)
				money -= 100;
			else return;
			break;
		case uIce:
			if (money >= 50)
				money -= 50;
			else return;
			break;
		case uLaser:
			if (money >= 70)
				money -= 70;
			else return;
			break;
		case uFarm:
			if (money >= 150)
				money -= 150;
			else return;
			break;
		case uWarrior:
			if (money >= 30)
				money -= 30;
			else return;
			break;
		case uTank:
			if (money > 200)
				money -= 200;
			else return;
			break;
		}
		switch (u->type)
		{
		case uVillager:
			u->str = 3;
			u->delay = 1;
			u->rng = 3;
			u->cost = 10;
			break;
		case uArchery:
			u->str = 5;
			u->delay = 2;
			u->rng = 6;
			u->cost = 15;
			strcpy(u->pic, ">>");
			u->color = yellow;
			break;
		case uCannon:
			u->str = 10;
			u->delay = 2;
			u->rng = 4;
			u->cost = 15;
			strcpy(u->pic, "ㄸ");
			u->color = gray;
			break;
		case uSniper:
			u->str = 35;
			u->delay = 5;
			u->rng = 17;
			u->cost = 100;
			strcpy(u->pic, "▣");
			u->color = violet;
			break;
		case uIce:
			u->str = 1;
			u->delay = 2;
			u->rng = 5;
			u->cost = 50;
			strcpy(u->pic, "※");
			u->color = sky;
			break;
		case uLaser:
			u->str = 3;
			u->delay = 2;
			u->rng = 12;
			u->cost = 70;
			strcpy(u->pic, "=");
			u->color = bloody;
			break;
		case uFarm:
			u->str = 5;
			u->delay = 1;
			u->rng = 0;
			u->cost = 150;
			strcpy(u->pic, "▤");
			u->color = green;
			break;
		case uWarrior:
			u->str = 20;
			u->delay = 2;
			u->rng = 1;
			u->cost = 30;
			strcpy(u->pic, "｜ ");
			u->color = white;
			break;
		case uTank:
			u->str = 15;
			u->delay = 10;
			u->rng = 7;
			u->cost = 200;
			strcpy(u->pic, "⊇");
			u->color = ivory;
			break;
		}
	}
	if (u->upgrade == 2)
	{
		switch (u->type)
		{
		case uVillager:
			if (money > 20)
				money -= 20;
			else return;
			break;
		case uArchery:
			if (money > 15)
				money -= 15;
			else return;
			break;
		case uCannon:
			if (money >= 15)
				money -= 15;
			else return;
			break;
		case uSniper:
			if (money >= 100)
				money -= 100;
			else return;
			break;
		case uIce:
			if (money >= 50)
				money -= 50;
			else return;
			break;
		case uLaser:
			if (money >= 70)
				money -= 70;
			else return;
			break;
		case uFarm:
			if (money >= 200)
				money -= 200;
			else return;
			break;
		case uWarrior:
			if (money >= 50)
				money -= 50;
			else return;
			break;
		case uTank:
			if (money > 200)
				money -= 200;
			else return;
			break;
		}
		switch (u->type)
		{
		case uVillager:
			u->str = 5;
			u->delay = 1;
			u->rng = 3;
			u->cost = 20;
			strcpy(u->pic, "ㅆ");
			u->color = ivory;
			break;
		case uArchery:
			u->str = 10;
			u->delay = 2;
			u->rng = 8;
			u->cost = 15;
			strcpy(u->pic, "}}");
			u->color = yellow;
			break;
		case uCannon:
			u->str = 12;
			u->delay = 2;
			u->rng = 6;
			u->cost = 15;
			strcpy(u->pic, "ㅌ");
			u->color = gray;
			break;
		case uSniper:
			u->str = 40;
			u->delay = 4;
			u->rng = 18;
			u->cost = 100;
			strcpy(u->pic, "◈");
			u->color = violet;
			break;
		case uIce:
			u->str = 2;
			u->delay = 1;
			u->rng = 7;
			u->cost = 50;
			strcpy(u->pic, "♣");
			u->color = sky;
			break;
		case uLaser:
			u->str = 4;
			u->delay = 2;
			u->rng = 12;
			u->cost = 70;
			strcpy(u->pic, "≡");
			u->color = bloody;
			break;
		case uFarm:
			u->str = 10;
			u->delay = 1;
			u->rng = 0;
			u->cost = 200;
			strcpy(u->pic, "▤");
			u->color = green;
			break;
		case uWarrior:
			u->str = 30;
			u->delay = 2;
			u->rng = 1;
			u->cost = 50;
			strcpy(u->pic, "†");
			u->color = white;
			break;
		case uTank:
			u->str = 25;
			u->delay = 10;
			u->rng = 7;
			u->cost = 200;
			strcpy(u->pic, "⊇");
			u->color = ivory;
			break;
		}
	}
}
void drawCastle()
{
	setColor(sky);
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y - 2); printf("/|  /|  /|");
	setColor(ivory);
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y - 1); printf("□  □  □");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y); printf("□□□□□");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 1); printf("□++〓++□");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 2); printf("□□∩□□");
}
void eraseEnemyGraphic()
{
	for (int i = 0; i < 5; i++)	//적군 그래픽을 지워가면서 roadEnd를 맨 뒤로 보낸 뒤
	{
		if (road[i].here.x != -1)
		{
			roadEnd[i] = &(road[i]);
			while (roadEnd[i]->next != NULL)
			{
				roadEnd[i] = roadEnd[i]->next;
				if (roadEnd[i]->here.x != pt.x || roadEnd[i]->here.y != pt.y)
					clearCur(roadEnd[i]->here, ROAD);
			}
		}
	}
	
}
void attackCastle()
{
	//맨 마지막에 있는 적은 무조건 지우고 성의 체력을 깎는다.
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < roadEnd[i]->enemyIndex; j++)
		{
			if (road[i].here.x != -1)
			{
				currentHealth -= roadEnd[i]->enemyStart[j]->hp * roadEnd[i]->enemyStart[j]->damage;
				showHealthBar(currentHealth, healthMax);
				free(roadEnd[i]->enemyStart[j]);
				roadEnd[i]->enemyStart[j] = NULL;
				roadEnd[i]->enemyIndex = 0;
			}
		}
}
void moveEnemy()
{
	//적 유닛이 있을 경우 이동
	for (int i = 0; i < 5; i++)// 앞으로 한 칸씩 옮기면서 얼어있지 않은 모든 적들을 뒤로 옮긴다. (current -> next)
	{
		if (road[i].here.x != -1) //무효한 배열이 아닐 경우에는
		{
			while (roadEnd[i] != &(road[i]))//길을 뒤에서부터 읽어가면서
			{
				for (int j = 0; j < roadEnd[i]->before->enemyIndex; j++) //적 갯수만큼
				{
					if (roadEnd[i]->before->enemyStart[j]->isFrozen <= 0)//얼어있지 않다면
					{
						if (roadEnd[i]->before->enemyStart[j]->type == eRunner) //적 타입이 eRunner
						{
							if (roadEnd[i]->next != NULL && roadEnd[i]->next->enemyIndex < 10)//2칸 앞의 배열에 자리가 남는 경우
							{
								roadEnd[i]->next->enemyStart[roadEnd[i]->enemyIndex] = roadEnd[i]->before->enemyStart[j]; //그 블럭만 떼어서 앞쪽으로 옮기고
								roadEnd[i]->next->enemyIndex++;
								for (int k = j; k < roadEnd[i]->enemyIndex - 1; k++)//뒤쪽에 있는 애들은 한칸씩 앞으로 당긴다.
								{
									roadEnd[i]->before->enemyStart[k] = roadEnd[i]->before->enemyStart[k + 1];
								}
								roadEnd[i]->before->enemyIndex--;
							}
							else if (roadEnd[i]->next != NULL);//앞 배열에 자리가 없는 경우 옮기지 않는다.
							else//앞 배열이 NULL: 끝지점일 경우
							{
								currentHealth -= roadEnd[i]->before->enemyStart[j]->hp * roadEnd[i]->before->enemyStart[j]->damage;
								showHealthBar(currentHealth, healthMax);
								roadEnd[i]->before->enemyStart[j] = NULL; //그 블럭만 떼어서 앞쪽으로 옮기고
								for (int k = j; k < roadEnd[i]->enemyIndex - 1; k++)//뒤쪽에 있는 애들은 한칸씩 앞으로 당긴다.
								{
									roadEnd[i]->before->enemyStart[k] = roadEnd[i]->before->enemyStart[k + 1];
								}
								roadEnd[i]->before->enemyIndex--;
							}
						}
						else if (roadEnd[i]->enemyIndex < 10)//적 타입이 runner가 아니고 앞 배열에 자리도 남는 경우
						{
							roadEnd[i]->enemyStart[roadEnd[i]->enemyIndex] = roadEnd[i]->before->enemyStart[j]; //그 블럭만 떼어서 앞쪽으로 옮기고
							roadEnd[i]->enemyIndex++;
							for (int k = j; k < roadEnd[i]->enemyIndex - 1; k++)//뒤쪽에 있는 애들은 한칸씩 앞으로 당긴다.
							{
								roadEnd[i]->before->enemyStart[k] = roadEnd[i]->before->enemyStart[k + 1];
							}
							roadEnd[i]->before->enemyIndex--;
						}
						else;//앞에 자리가 없는 경우는 움직이지 않는다.
					}
					else//얼어있는 경우는 움직이지 않는다.
						roadEnd[i]->enemyStart[j]->isFrozen--;
				}
				roadEnd[i] = roadEnd[i]->before;
			}
		}
	}
}
void drawEnemy()
{

	//적 유닛 그리기


	for (int i = 0; i < 5; i++)
	{
		if (road[i].here.x != -1)
		{
			roadEnd[i] = &(road[i]);
			while (roadEnd[i]->next != NULL)
			{
				roadEnd[i] = roadEnd[i]->next;
				if (roadEnd[i]->enemyIndex)
				{
					if (roadEnd[i]->enemyStart[0]->type != eEmpty)
					{
						if (roadEnd[i]->here.x != pt.x || roadEnd[i]->here.y != pt.y)
						{
							gotoxy(start.x + 2 * (roadEnd[i]->here.x), start.y + (roadEnd[i]->here.y));
							setColor(roadEnd[i]->enemyStart[0]->color);
							printf("%s", roadEnd[i]->enemyStart[0]->pic);
						}
					}
				}
			}
		}
	}

}
void attackEnemy(int currentTick)
{
	//유닛이 적을 공격
	for (int i = 0; i < currentUnit; i++)
	{
		switch (unitArr[i].type)
		{
		case uVillager:
		case uArchery:
		case uCannon:
		case uSniper:
			for (int j = 0; j < 100; j++)
			{
				if (currentTick - unitArr[i].lastTick > unitArr[i].delay && unitArr[i].road[j] != NULL && unitArr[i].road[j]->enemyIndex != 0)
				{
				gotoxy(start.x + 2 * unitArr[i].road[j]->here.x, start.y + unitArr[i].road[j]->here.y);
				setColor(bloody);
				printf("%s", unitArr[i].road[j]->enemyStart[0]->pic);
					unitArr[i].road[j]->enemyStart[0]->hp -= unitArr[i].str;
					if (unitArr[i].road[j]->enemyStart[0]->hp < 0)
					{
						money += unitArr[i].road[j]->enemyStart[0]->money;
						for (int k = 0; k < unitArr[i].road[j]->enemyIndex - 1; i++)
							unitArr[i].road[j]->enemyStart[k] = unitArr[i].road[j]->enemyStart[k + 1];
						free(unitArr[i].road[j]->enemyStart[unitArr[i].road[j]->enemyIndex-1]);
						unitArr[i].road[j]->enemyIndex--;
					}
					unitArr[i].lastTick = currentTick;
					break;
				}
			}
			break;
		case uIce:
			for (int j = 0; j < 100; j++)
			{
				if (currentTick - unitArr[i].lastTick > unitArr[i].delay && unitArr[i].road[j] != NULL && unitArr[i].road[j]->enemyIndex != 0)//시간이 되고 길은 유효하며 적이 있을 때
				{
					gotoxy(start.x + 2 * unitArr[i].road[j]->here.x, start.y + unitArr[i].road[j]->here.y);
					setColor(sky);
					printf("%s", unitArr[i].road[j]->enemyStart[0]->pic);
					unitArr[i].road[j]->enemyStart[0]->hp -= unitArr[i].str;
					if (unitArr[i].road[j]->enemyStart[0]->hp < 0)
					{
						money += unitArr[i].road[j]->enemyStart[0]->money;
						for (int k = 0; k < unitArr[i].road[j]->enemyIndex - 1; i++)
							unitArr[i].road[j]->enemyStart[k] = unitArr[i].road[j]->enemyStart[k + 1];
						free(unitArr[i].road[j]->enemyStart[unitArr[i].road[j]->enemyIndex - 1]);
						unitArr[i].road[j]->enemyIndex--;
					}
					if(unitArr[i].road[j]->enemyStart[0]->isFrozen == 0)
						unitArr[i].road[j]->enemyStart[0]->isFrozen = 1;
					unitArr[i].lastTick = currentTick;
				break;
				}
			}
			break;
		case uLaser:
			break;
		case uFarm:
			if (currentTick - unitArr[i].lastTick > unitArr[i].delay)
				money += unitArr[i].str;
			if (currentTick % 2)
				setColor(rime);
			else setColor(unitArr[i].color);
			gotoxy(start.x + 2 * unitArr[i].pos.x, start.y + unitArr[i].pos.y);
			printf("%s", unitArr[i].pic);
			break;
		case uWarrior:

			break;
		case uTank:
			break;
		}
	}
}