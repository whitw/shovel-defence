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
_road* roadEnd[5]; //���� �۾����� ���� �������� ������.
int currentRoad = 0; //���� �����ϰ� �ִ� ��

					 //MAP & config

int map[MAX_UD][MAX_LR];//�޾ƿ� ������ ���⿡ �����ϰ�, ���⿡�� �÷��̽� ���ϴ� �͵��� ��´�. ���� ������ �ǵ帮�� �ʴ´�.
pos enemystart[5];
pos pt = { 0,0 };
int enemyarr[5][1000], money, healthMax, healthCur;
int gameSpeed = 1;

void game()
{
	FILE* fp;
	char filename[25], string[15];
	int select;
	//level select
	select = printlevel();
	if (select == -1)
		return;
	fp = fopen("config.txt", "rt"); //Ű �Է� �������� ����
	if (fp == NULL)
	{
		fp = fopen("config.txt", "wt");
		fprintf(fp, "0 113\n0 119\n0 101\n0 114\n0 97\n0 115\n0 100\n0 102\n0 97\n0 100\n0 32\n0 27\n-32 72\n-32 75\n-32 77\n-32 80\n0 49\n0 50\n0 51\n");//�ʱ� ���·� ����.
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
	//Ű ���� �������� ��
	fp = fopen("levels.txt", "rt");//���� �̸� �������� ����
	if (fp == NULL)
	{
		fp = fopen("levels.txt", "wt");
		fclose(fp);
		fp = fopen("levels.txt", "rt");
	}
	for (int i = 0; i < select; i++)
	{
		fscanf(fp, "%*s");
		fscanf(fp, "%*d");//���� ���� �κ��� ������.
	}
	strcpy(filename, "MAP\\");
	fscanf(fp, "%s", string);
	strcat(string, ".level");
	strcat(filename, string);
	fclose(fp);//���� �̸� �������� ��
	//copy MAP file to array
	fp = fopen(filename, "rt");//���⼭���� fp�� �� ������ �����Ѵ�.
	printmap(fp);//������ �о �ʿ� �����Ѵ�. �� �Լ��� ���� �ڿ� fp�� �̺�Ʈ ��ġ�� ����Ű�� �־�� �Ѵ�.
	printgame();
	readfile(fp);//���⼭ ������ �о���鼭 �� �پ� ����. �׻� road(���ı�) -> start(���� ����) -> if(ifclear) -> end������ ����� ��.
	fclose(fp);
}
void printmap(FILE* fp)
{
	pos castleMid = { -1,-1 };
	int temp;
	rewind(fp);
	for (int i = 0; i < MAX_UD; i++) //���Ͽ��� �޾ƿ��� ����
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			fscanf(fp, "%d", &map[i][j]);
		}
		fscanf(fp, "\n");
	}
	for (int i = 0; i < MAX_LR;i++)
	system("cls");
	setColor(yellow);
	initSquare(start, "��");
	gotoxy(start.x, start.y);
	for (int i = 0; i < MAX_UD; i++) //����ϱ� ����
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
				printf("��");
				break;
			case EMPTY:
				setColor(gray);
				printf(". ");
				break;
			case STONE:
				setColor(gray);
				printf("��");
				break;
			case CASTLE_DOOR:
				setColor(violet);
				printf("��");
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
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y - 1); printf("��  ��  ��");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y); printf("������");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 1); printf("��++��++��");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 2); printf("�������");
	//������ ���� �κе��� �о �����Ѵ�. ���� ���� ��ġ, ���� �迭, �־��� ��, �ʱ� ���� ü��.
	for (int i = 0; i < 5; i++)//���� ���� ��ġ
		fscanf(fp,"%d %d", &enemystart[i].x,&enemystart[i].y);

	for (int i = 0; i < 5; i++)//���� �迭
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
void readfile(FILE* fp) //������ �����鼭 ���� ������ �����ϰ� �����Ѵ�.
{
	FILE* config;
	FILE* level;
	FILE* ftest;
	char ch[256];
	char temp[128];
	pos talkp = { start.x + 2 * MAX_LR + 10, start.y - 4 };
	//���� �а� ������ ����.
	setColor(yellow);
	gotoxy(start.x + MAX_LR, start.y - 3); printf("�ܰ��� �ӵ�:");
	gotoxy(start.x + 2 * MAX_LR - 1, start.y - 3); printf("%1d��",gameSpeed);
	gotoxy(start.x + MAX_LR, start.y - 2); printf("�ܵ�:");
	gotoxy(start.x + 2 * MAX_LR - 14, start.y - 2); printf("%10u���Ρ�",money);
	config = fopen("config.txt", "rt");
	gotoxy(0, cmdrow - 2); system("pause");
	makeroad();
	level = fopen("levels.txt", "at");//�̹� game���� ��������Ƿ� ���� ���� ����.
	if (config == NULL)
	{
		config = fopen("config.txt", "wt");
		fclose(config);
		config = fopen("config.txt", "rt");
	}
	while (feof(fp) != 0)//������ �� ���� �� ����
	{
		fgets(ch, 256, fp);//�� �� �� ���� �� �� �ձ��ڸ� �о
		switch (ch[0])
		{
		case ';'://�ּ�ó�� ;
			break;
		case '\"'://�񼭰� �ϴ� ��. "
			talk(talkp, ch + 1, 500);
			break;
		case 'O'://���ο� ������ ����. �̹� ���������� ���� ����.
			break;
		case 'L'://���ο� ������ ����. �ܼ��� levels.txt�� �߰��ϸ� �Ǵ� ��.
			strcmp(temp, "MAP\\");
			strcat(temp, ch + 2);
			strcat(temp, ".level");
			ftest = fopen(temp, "rt");
			if (ftest == NULL)
			{
				fprintf(level, "%s 0\n", ch + 2);
				fclose(ftest);
			}
		case 'r'://�� �ı�
			makeroad();
			break;
		case 's'://���� ����
			startGame(fp);
			break;
		case 'e'://���� ��. �� ������ ����ϰ� ������ �����ؾ� �Ѵ�.
			
			break;
		}
	}
	fclose(config);
	fclose(level);
}
int startGame() //�� ����. �迭�� �̿��ϰ� ������ �ǵ��� �ʴ´�.��ȯ���� ���� ���� ü��. 0�̸� ���ӿ����� ü���� ������ ���� ���� ���.
{
	return 1;
}
void showSelectedRoad()
{
	gotoxy(start.x + 2 * pt.x, start.y + pt.y);
	setColor(colorRoad[currentRoad]);
	printf("��");
}
void makeroad() //���ı�. ���Ͽ��� �о�� �迭�� ����ϵ� ������ �ǵ�� �ȵȴ�.
{//pos enemystart[5]�� �̿��Ѵ�. �ʱ�ȭ�� �Ǿ� ������ �״�� ���� ��.
	int ret = 0;
	for (int i = 0; i < 5; i++)
	{
		if (enemystart[i].x != -1)
		{
			pt.x = enemystart[0].x;
			pt.y = enemystart[0].y;
			break;
		}
	}
	char ch[2] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		road[i].here.x = enemystart[i].x;
		road[i].here.y = enemystart[i].y;
		road[i].next = NULL;
		road[i].before = NULL;
		roadEnd[i] = &road[i];
	}
	//�� �ı�
	while (1)
	{
		ch[0] = _getch();
		ch[1] = 0;
		if (_kbhit())
		{
			ch[0] = 0;
			ch[1] = _getch();
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
		if (pt.y > 0 && ch[1] == UP || ch[0] == 'w' || ch[0] == 'W')//Ŀ���� ���� ������ �� �ִ� ��Ȳ���� UPŰ�� ����.
		{
			clearCur(pt, map[pt.y][pt.x]); //���� �ִ� Ŀ���� �����.
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
		printf("��");
		break;
	case ROAD:
		setColor(yellow);
		printf("��");
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
		||(direction == DOWN &&  map[pt.y + 1][pt.x] == EMPTY))//���� ���� ���� �� �ִٸ�
	{
		//���� ���� �����.
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
		(direction == DOWN && map[pt.y + 1][pt.x] == ROAD&& roadEnd[currentRoad]->before->direct == UP))//���ư����� �ϴ� ��쿡��
		{//���� ������ �� ��쿡�� ���� �����.
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
		//�� ���� ��쿡�� ���� �ٸ� ���� ��ġ�ų� �ϳ��� ���� ���̴� ���ۿ� �����Ƿ� �ƹ��͵� ���� �ʴ´�. 
	}
	else if ((direction == UP && map[pt.y - 1][pt.x] == ROAD)||
		(direction == LEFT && map[pt.y][pt.x - 1] == ROAD) || \
		(direction == RIGHT && map[pt.y][pt.x + 1] == ROAD) || \
		(direction == DOWN && map[pt.y + 1][pt.x] == ROAD))//���� ��ġ�ų� ���̴� ���
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
			if (i >= 5 || roadEnd[i]->here.x == -1)//��� ���� ����.
			{
				Sleep(1000);
				system("cls");
				return 1;//�ϼ� ��ȣ
			}
			else if (map[roadEnd[i]->here.y][roadEnd[i]->here.x] != CASTLE_DOOR) //�ϼ����� ���� ���� ã�Ҵ�
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