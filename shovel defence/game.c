#include "define.h"
#pragma warning(disable:4996)
extern char keyNormal[8][2];
extern char keyShort[2][2];
extern char keyCommon[6][2];
extern char keySpeed[3][2];
int map[MAX_LR][MAX_UD];//받아온 파일은 여기에 복사하고, 여기에다 플레이시 변하는 것들을 담는다. 파일 내용은 건드리지 않는다.
pos enemystart[5];
int enemyarr[5][1000], money, healthMax, healthCur;

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
	pos start = { 8, 6 };
	pos castleMid = { -1,-1 };
	int i, j;
	int temp;
	rewind(fp);
	for (i = 0; i < MAX_LR; i++) //파일에서 받아오기 시작
	{
		for (j = 0; j < MAX_UD; j++)
		{
			fscanf(fp, "%d", &map[i][j]);
		}
		fscanf(fp, "\n");
	}
	system("cls");
	setColor(yellow);
	initSquare(start, "●");
	gotoxy(start.x, start.y);
	for (i = 0; i < MAX_UD; i++) //출력하기 시작
	{
		for (j = 0; j < MAX_LR; j++)
		{
			switch (map[i][j])
			{
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
				printf("XX");
				break;
			case CASTLE:
				printf("XX");
				castleMid.y = i;
				castleMid.x = j;
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
	healthCur = healthMax;
}
void readfile(FILE* fp) //파일을 읽으면서 게임 순서를 지정하고 실행한다.
{
	FILE* config;
	FILE* level;
	FILE* ftest;
	char ch[256];
	char temp[128];
	pos start = { 8, 6 };
	pos talkp = { start.x + 2 * MAX_LR + 10, start.y - 4 };
	//파일 읽고 돌리기 시작.

	config = fopen("config.txt", "rt");
	level = fopen("level.txt", "at");//이미 game에서 만들었으므로 없을 리가 없다.
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
			talk(talkp, ch + 1);
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
			startGame(fp);
			break;
		case 'e'://게임 끝. 별 갯수를 계산하고 레벨에 저장해야 한다.
			
			break;
		}
	}
	fclose(config);
	fclose(level);
}

void makeroad() //길파기. 파일에서 읽어온 배열을 사용하되 파일은 건들면 안된다.
{//pos enemystart[5]을 이용한다. 초기화는 되어 있으니 그대로 쓰면 됨.
	road* r[5];
	char ch[2] = 0;
	for (int i = 0; i < 5; i++)
	{
		r[i] = (road*)malloc(sizeof(road));
	}
	while (1)
	{
		ch[0] = _getch();
		ch[1] = 0;
		if (_kbhit())
		{
			ch[0] = 0;
			ch[1] = _getch();
		}
		if (ch[1] == UP || ch[0] == 'w' || ch[0] == 'W')
		{

		}
		else if(ch[1] == LEFT|| ch[0] == 'a' || ch[0] == 'a')
		{

		}
		else if (ch[1] == RIGHT || ch[0] == 'd' || ch[0] == 'd')
		{

		}
		else if (ch[1] == DOWN || ch[0] == 's' || ch[0] == 's')
		{

		}
	}
}
int startGame() //본 게임. 배열만 이용하고 파일은 건들지 않는다.반환값은 성의 남은 체력. 0이면 게임오버고 체력이 낮으면 별도 적은 방식.
{
	return 1;
}