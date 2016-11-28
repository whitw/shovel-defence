#include "define.h"
#pragma warning(disable:4996)
extern char keyNormal[8][2];
extern char keyShort[2][2];
extern char keyCommon[6][2];
extern char keySpeed[3][2];
int map[MAX_LR][MAX_UD];//�޾ƿ� ������ ���⿡ �����ϰ�, ���⿡�� �÷��̽� ���ϴ� �͵��� ��´�. ���� ������ �ǵ帮�� �ʴ´�.
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
	pos start = { 8, 6 };
	pos castleMid = { -1,-1 };
	int i, j;
	int temp;
	rewind(fp);
	for (i = 0; i < MAX_LR; i++) //���Ͽ��� �޾ƿ��� ����
	{
		for (j = 0; j < MAX_UD; j++)
		{
			fscanf(fp, "%d", &map[i][j]);
		}
		fscanf(fp, "\n");
	}
	system("cls");
	setColor(yellow);
	initSquare(start, "��");
	gotoxy(start.x, start.y);
	for (i = 0; i < MAX_UD; i++) //����ϱ� ����
	{
		for (j = 0; j < MAX_LR; j++)
		{
			switch (map[i][j])
			{
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
	pos start = { 8, 6 };
	pos talkp = { start.x + 2 * MAX_LR + 10, start.y - 4 };
	//���� �а� ������ ����.

	config = fopen("config.txt", "rt");
	level = fopen("level.txt", "at");//�̹� game���� ��������Ƿ� ���� ���� ����.
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
			talk(talkp, ch + 1);
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

void makeroad() //���ı�. ���Ͽ��� �о�� �迭�� ����ϵ� ������ �ǵ�� �ȵȴ�.
{//pos enemystart[5]�� �̿��Ѵ�. �ʱ�ȭ�� �Ǿ� ������ �״�� ���� ��.
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
int startGame() //�� ����. �迭�� �̿��ϰ� ������ �ǵ��� �ʴ´�.��ȯ���� ���� ���� ü��. 0�̸� ���ӿ����� ü���� ������ ���� ���� ���.
{
	return 1;
}