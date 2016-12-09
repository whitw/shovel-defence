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
	system("cls");
	setColor(yellow);
	initSquare(start, "��");
	gotoxy(start.x, start.y);
	printArrMap();
	//������ ���� �κе��� �о �����Ѵ�. ���� ���� ��ġ, ���� �迭, �־��� ��, �ʱ� ���� ü��.
	for (int i = 0; i < 5; i++)//���� ���� ��ġ
		fscanf(fp,"%d %d", &enemystart[i].x,&enemystart[i].y);

	for (int i = 0; i < 5; i++)//���� �迭
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
																																															talk(talkp, "���ƾ�", 1000);
																																															if (makeroad()){
																																																talk(talkp, "�ȳ��Ͻÿ�?", 500);
																																																startGame();
																																															}
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
			startGame();
			break;
		case 'e'://���� ��. �� ������ ����ϰ� ������ �����ؾ� �Ѵ�.
			
			break;
		}
	}
	fclose(config);
	fclose(level);
}
int startGame() //�� ����. �迭�� �̿��ϰ� ������ �ǵ��� �ʴ´�.��ȯ���� ���� ���� ü��. 0�̸� ���ӿ����� ü���� ������ ���� ���� ���. ��ȯ���� ����...? ��������
{
	pos castleMid = { -1,-1 };
	char ch[2] = { 0 };
	time_t t1, t2; //���� �ӵ� ������ ����Ѵ�.
	int currentTick = 0; //������� ���� ƽ. 0���� �����ؼ� 999���� 1000���� �ٲ� �� ��.
	int enemyIndex[5] = { 0 };//���� ����ؾ� �ϴ� �� �迭. ������ 5���� ���� ������ �̹� -1�� ���� �迭������ �о�� �ȵǱ� ����.
	t1 = clock();
	setColor(yellow);
	gotoxy(cmdcol / 3, cmdrow / 3 + 0); printf("�̢̢̢̢�");
	gotoxy(cmdcol / 3, cmdrow / 3 + 1); printf("        ��");
	gotoxy(cmdcol / 3, cmdrow / 3 + 2); printf("�̢̢̢̢�");
	gotoxy(cmdcol / 3, cmdrow / 3 + 3); printf("        ��");
	gotoxy(cmdcol / 3, cmdrow / 3 + 4); printf("�̢̢̢̢�");
	Sleep(1000);

	gotoxy(cmdcol / 3, cmdrow / 3 + 0); printf("�̢̢̢̢�");
	gotoxy(cmdcol / 3, cmdrow / 3 + 1); printf("        ��");
	gotoxy(cmdcol / 3, cmdrow / 3 + 2); printf("�̢̢̢̢�");
	gotoxy(cmdcol / 3, cmdrow / 3 + 3); printf("��        ");
	gotoxy(cmdcol / 3, cmdrow / 3 + 4); printf("�̢̢̢̢�");
	Sleep(1000);

	gotoxy(cmdcol / 3, cmdrow / 3 + 0); printf("        ��");
	gotoxy(cmdcol / 3, cmdrow / 3 + 1); printf("        ��");
	gotoxy(cmdcol / 3, cmdrow / 3 + 2); printf("        ��");
	gotoxy(cmdcol / 3, cmdrow / 3 + 3); printf("        ��");
	gotoxy(cmdcol / 3, cmdrow / 3 + 4); printf("        ��");
	Sleep(1000);
	system("cls");
	initSquare(start, "��");
	printgame();
	gotoxy(start.x, start.y);
	printArrMap();
	setColor(sky);
	gotoxy(start.x + 2 * MAX_LR + 10, start.y + 4); printf("��������");
	for (int i = 0; i < 5; i++){
		gotoxy(start.x + 2 * MAX_LR + 10, start.y + 5 + i); printf("��          ��");
	}
	gotoxy(start.x + 2 * MAX_LR + 10, start.y + 10); printf("��������");
	gotoxy(cmdcol / 4 * 3 - 9, start.y + 4); printf(">>              <<");
	while (1)
	{
		/***************���ʿ����� �� ƽ�� �ѹ��� �ϸ� �Ǵ� �͵��� ����մϴ�********************/
		//�� ������ ���� ��� �̵�
												//���� �׷��� �����, �� �������� ���� �׷��ȵ� �����.
		for (int i = 0; i < 5; i++)	//roadEnd�� �� �ڷ� ���� �� ������ �� ĭ�� �ű�鼭 ������ �ڷ� ��ĭ�� �ű��.
		{
			roadEnd[i] = &(road[i]);
			while (roadEnd[i]->next != NULL)
			{
				roadEnd[i] = roadEnd[i]->next;
				clearCur(roadEnd[i]->here, ROAD);
			}
		}
		//�� �������� �ִ� ������ ����� ���� ü���� ��´�.
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
		//�� ���� ����. enemyIndex ����
		//for (int i = 0; i < 5;i++)
		

		//�� ���� �׸���
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
		//�Ʊ� ������ ����


		do {
			/***************���⼭�� ��÷� �ؾ� �ϴ� �͵��� ����մϴ�***********************/
			if (_kbhit())
			{
				ch[0] = _getch();
				ch[1] = 0;
				if (ch[0] == 0x80 || ch[0] == 0)
					ch[1] = _getch();
			}
			//������ �����
			//������ �̵�
			//������ ǥ��
			//�Ķ� �簢���� �� ���� ǥ��
			t2 = clock();
		} while (t2 - t1 < tick[gameSpeed]);
	}
	return 1;
}
void showSelectedRoad()
{
	gotoxy(start.x + 2 * pt.x, start.y + pt.y);
	setColor(colorRoad[currentRoad]);
	printf("��");
}
int makeroad() //���ı�. ���Ͽ��� �о�� �迭�� ����ϵ� ������ �ǵ�� �ȵȴ�. ��ȯ���� ���������� ������ �������� 0
{//pos enemystart[5]�� �̿��Ѵ�. �ʱ�ȭ�� �Ǿ� ������ �״�� ���� ��.
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
	//�� �ı�
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
			printf("���� ȭ������ ���ư��ϴ�. ������ �������?(ENTER�� ������ Ȯ��)");
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
void printArrMap()
{
	pos castleMid;
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
				printf("  ");
				break;
			case CASTLE:
				printf("  ");
				castleMid.y = i;
				castleMid.x = j;
				break;
			case ROAD:
				setColor(yellow);
				printf("��");
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
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y - 1); printf("��  ��  ��");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y); printf("������");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 1); printf("��++��++��");
	gotoxy(start.x + 2 * castleMid.x - 4, start.y + castleMid.y + 2); printf("�������");
}
void initEnemy(enemy* ep, enemyT type)
{
	ep->type = type;
	ep->isFrozen = 0;
	ep->speed = 1; //�� �ϸ��� �� �� �̵�
	switch (type)
	{
	case eChicken:
		ep->damage = 1;//���ػ��
		ep->hp = 10;
		ep->money = 1;
		strcpy(ep->pic, "O>");
		ep->color = ivory;
		break;
	case eThief:
		ep->damage = 1;
		ep->hp = 40;
		ep->money = 2;
		strcpy(ep->pic, "��");
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
		strcpy(ep->pic, "��");
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
		strcpy(ep->pic, "��");
		ep->color = sky;
		break; 
	case eTank:
		ep->damage = 2;
		ep->hp = 100;
		ep->money = 5;
		strcpy(ep->pic, "��");
		ep->color = green;
		break;
	case	eKnight:
		ep->damage = 10;
		ep->hp = 50;
		ep->money = 20;
		strcpy(ep->pic, "��");
		ep->color = bloody;
		break;
	case eKing:
		ep->damage = 1;
		ep->hp = 1000;
		ep->money = 100;
		strcpy(ep->pic, "��");
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
		strcpy(u->pic, "��");
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
		strcpy(u->pic, "��");
		u->color = gray;
		break;
	case uSniper:
		u->str = 20;
		u->delay = 6;
		u->rng = 15;
		u->cost = 100;
		strcpy(u->pic, "��");
		u->color = violet;
		break;
	case uIce:
		u->str = 0;
		u->delay = 1;
		u->rng = 2;
		u->cost = 50;
		strcpy(u->pic, "��");
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
		strcpy(u->pic, "��");
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
		strcpy(u->pic, "��");
		u->color = ivory;
		break;
	}
	//���⼭ _road* ����� �����ؾ� �Ѵ�. �˴� �迭/��ũ�� ����Ʈ�� �ھƳְ� �տ������� �д� ��¼�� �ؾ� ��.
}
void upgradeUnit(unit *u)
{
	if (u->upgrade < 3)
		u->upgrade++;
}