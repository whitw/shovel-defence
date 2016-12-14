#include "define.h"
#include "levelmaker.h"
#pragma warning(disable:4996)

pos enemystart[5]; //���̵尡 ���۵Ǵ� ����, �ִ� 5��
char location[25] = "MAP\\";
orderT order[70] = { 0 };
char string[70][100];

void initMapEdit()
{
	const pos start = { 2,4 }; //���� ����ϱ� �����ϴ� ��ġ. 
	system("cls");
	setColor(gray);
	gotoxy(cmdcol / 3, 1); printf("���� �����ϱ�");
	setColor(yellow);
	initSquare(start, "��");
	setColor(ivory);
	for (int i = 0; i < MAX_UD; i++)
	{
		gotoxy(start.x, start.y + i);
		for (int j = 0; j < MAX_LR; j++)printf("* ");
	}
	setColor(gray);
	gotoxy(2 * MAX_LR + 10, 7); printf("������ �����͸� �̵��� �� �ֽ��ϴ�.");
	gotoxy(2 * MAX_LR + 10, 11); printf("��ư�� ������ ���� �� �ֽ��ϴ�.");
	gotoxy(2 * MAX_LR + 10, 13); printf("1: ��ĭ");
	gotoxy(2 * MAX_LR + 10, 15); printf("2: ��");
	gotoxy(2 * MAX_LR + 10, 17); printf("3: ��. �� �ѹ��� ���� �� ������ ������ ��������.");
	gotoxy(2 * MAX_LR + 10, 19); printf("4: ���� ��� ��ġ����� ���⼭���� ����մϴ�.");
	gotoxy(2 * MAX_LR + 10, 20); printf("���� �����ڸ����� �����ϸ� ��� �ϳ��� �־�� �մϴ�. ");
	gotoxy(2 * MAX_LR + 10, 22); printf("5:��������� ���⿡�� �����ϴ�.�ݵ�� �� �ٷ� ���� �־�� �ϸ�");
	gotoxy(2 * MAX_LR + 10, 23); printf("���� ��� ��ġ�� �������ٴ� ���ƾ� �մϴ�.");
	gotoxy(2 * MAX_LR + 10, 25); printf("6: �� �ٱ�(��� �������� ������ ���� �� �����ϴ�.)");
	gotoxy(2 * MAX_LR + 10, 27); printf("9: ��� ������ ���������ϴ�.");
	gotoxy(2 * MAX_LR + 10, 29); printf("ENTER�� ������ �Ϸ��� �� �ֽ��ϴ�.");//�� �� 3, 4�� 5�� �ִ��� üũ�Ѵ�.
}
int mapEdit(FILE* fp) //���� wt�� ��������. ���� �ʿ�� ����.
{
	const pos start = { 2,4 }; //���� ����ϱ� �����ϴ� ��ġ. 
	int map[MAX_UD][MAX_LR] = { 0 };
	int castleExist = 0, enemyExist = 0, doorExist = 0;
	pos pt = { 0,0 }; //���� ���� ���� �� ��ǥ�� (0,0), �� ĭ�� 2byte���� �ϳ� ũ�⸦ ����
	char ch;
	initMapEdit();
	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	printf("��");
	for (int i = 0; i < MAX_UD; i++)
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			map[i][j] = INVALID;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		enemystart[i].x = -1;
		enemystart[i].y = -1;
	}

	while (1) //�����-> ������ �ű��-> �׸��⸦ �ݺ�.
	{
		gotoxy(2 * MAX_LR + 10, 32); printf("����%s",castleExist?"�ֽ��ϴ�":"�����ϴ�");
		gotoxy(2 * MAX_LR + 10, 33); printf("%d���� ���� ���� ���� �ֽ��ϴ�.",enemyExist);
		gotoxy(2 * MAX_LR + 10, 34); printf("%d���� ������ �ֽ��ϴ�.",doorExist);
		ch = _getch();
		gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
		if (pt.x == MAX_LR - 1)//�����͸� ����� �۾�
		{
			printf("��");
		}
		else
		{
			switch (map[pt.y][pt.x + 1])
			{
			case INVALID:
				setColor(ivory);
				printf("* ");
				break;
			case EMPTY:
				printf("  ");
				break;
			case STONE:
				setColor(gray);
				printf("��");
				break;
			case CASTLE:
				setColor(sky);
				printf("��");
				break;
			case ENEMYPOS:
				setColor(sky);
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
			}//������ ����� ��
		}
		if (_kbhit())//Ű �ޱ� + ó��
		{
			ch = _getch();
			switch (ch)
			{
			case UP:
				pt.y = (pt.y + MAX_UD - 1) % MAX_UD;
				break;
			case LEFT:
				pt.x = (pt.x + MAX_LR - 1) % MAX_LR;
				break;
			case RIGHT:
				pt.x = (pt.x + 1) % MAX_LR;
				break;
			case DOWN:
				pt.y = (pt.y + 1) % MAX_UD;
				break;
			}
		}
		else
		{
			switch (ch)
			{
			case ESC:
				return 0;
			case ENTER:
				setColor(bloody);
				if(castleExist)
				{
					if (enemyExist > 0)
					{
						if (doorExist > 0)
						{
							if (enemyExist - doorExist <= 0)
							{
								goto BREAK_GET_KEY;
							}
							else
							{
								gotoxy(2 * MAX_LR + 10, 31); printf("\"���� ���� ��ġ�� �� �������� �����ϴ�!\"");
								Sleep(500);
								gotoxy(2 * MAX_LR + 10, 31); printf("                                        ");
							}
						}
						else
						{
							gotoxy(2 * MAX_LR + 10, 31); printf("\"������ �����ϴ�!\"");
							Sleep(500);
							gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
						}
					}
					else
					{
						gotoxy(2 * MAX_LR + 10, 31); printf("\"���� ���� ���� �����ϴ�!\"");
						Sleep(500);
						gotoxy(2 * MAX_LR + 10, 31); printf("                                                  ");
					}
				}
				else
				{
					gotoxy(2 * MAX_LR + 10, 31); printf("\"���� �����ϴ�!\"");
					Sleep(500);
					gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
				}
				break;
			case '9': //�� ���� ����
				for (int i = 0; i < MAX_UD; i++)
					for (int j = 0; j < MAX_LR; j++)
						map[i][j] = INVALID;
				castleExist = 0,enemyExist = 0, doorExist = 0;
				initMapEdit();
				break;
			default:



				if (ch >= '1' && ch <= '6' && map[pt.y][pt.x] != CASTLE && map[pt.y][pt.x] != CASTLE_INV) //�� ����, �� �Ա� ���� ������Ʈ
				{
					if (map[pt.y][pt.x] == CASTLE_DOOR)
						doorExist--;
					if (map[pt.y][pt.x] == ENEMYPOS)
						enemyExist--;
					gotoxy(start.x + 2 * pt.x, start.y + pt.y);
					switch (ch - '0' - 1)
					{
					case 5:
						map[pt.y][pt.x] = INVALID;
						setColor(ivory);
						printf("* ");
						break;
					case EMPTY://��ĭ
						map[pt.y][pt.x] = EMPTY;
						printf("  ");
						break;
					case STONE://��
						map[pt.y][pt.x] = STONE;
						setColor(gray);
						printf("��");
						break;
					case CASTLE://��
						if (!castleExist)
						{
							if (pt.x >= 2 && pt.x <= MAX_LR - 3 && pt.y >= 2 && pt.y <= MAX_UD - 3)
							{
								castleExist = 1;
								for (int i = -2; i <= 2; i++)
									for (int j = -2; j <= 2; j++)
									{
										if (map[pt.y+i][pt.x+j] == CASTLE_DOOR)
											doorExist--;
										if (map[pt.y+i][pt.x+j] == ENEMYPOS)
											enemyExist--;
										map[pt.y + i][pt.x + j] = CASTLE_INV; //�� �ֺ��� ������ ������ �ȵȴ�. ��������δ� BLOCK�� ������ ������ üũ�� �� �� ����.
									}
								map[pt.y][pt.x] = CASTLE;
								setColor(gray);
								for (int i = -2; i <= 2; i++)
									for (int j = -2; j <= 2; j++)
									{
										gotoxy(start.x + 2 * (pt.x + i), start.y + pt.y + j); printf("XX");
									}
								setColor(sky);
								gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("��");
							}
						}
						else
						{
							if (map[pt.y][pt.x] == CASTLE_DOOR)
								doorExist++;
							if (map[pt.y][pt.x] == ENEMYPOS)
								enemyExist++;
						}
						break;
					case ENEMYPOS://�� ����
						if (enemyExist < 5)
						{
							enemyExist++;
							map[pt.y][pt.x] = ENEMYPOS;
							setColor(sky);
							printf("��");
						}
						break;
					case CASTLE_DOOR://����
						for (int i = -1; i <= 1; i++)
							for (int j = -1; j <= 1; j++)
							{
								if (map[pt.y + i][pt.x + j] == CASTLE_INV)
								{
									doorExist++;
									map[pt.y][pt.x] = CASTLE_DOOR; //���� ��ȿ�� üũ.
									setColor(violet);
									gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("��");
									goto BREAK_CASE_CASTLE_DOOR;
								}
							}
						BREAK_CASE_CASTLE_DOOR:
						break;
					}
				}
				break;
			}
		}
		gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
		setColor(gray);
		printf("��");
	}
BREAK_GET_KEY://ENTER�� ������带 ������.
	enemyExist = 0;
	for (int i = 0;i < MAX_UD; i++)
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			fprintf(fp, "%d ", map[i][j]);
			if (map[i][j] == ENEMYPOS)
			{
				enemystart[enemyExist].x = j;
				enemystart[enemyExist].y = i;
				enemyExist++;
			}
		}
		fprintf(fp,"\n");
	}
	for (int i = 0; i < 5; i++)
		fprintf(fp, "%d %d\n", enemystart[i].x, enemystart[i].y);

	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	if (pt.x == MAX_LR - 1)
	{
		printf("��");
	}
	else
	{
		switch (map[pt.y][pt.x + 1])
		{
		case EMPTY:
			printf("  ");
			break;
		case STONE:
			setColor(gray);
			printf("��");
			break;
		case CASTLE:
			setColor(sky);
			printf("��");
			break;
		case ENEMYPOS:
			setColor(sky);
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
		}
	}
	return 1;
}
void waveEdit(FILE* fp)
{
	const pos start = { 2,4 }; //���� ����ϱ� �����ϴ� ��ġ.  �׵θ� �ƴ�.
	enemyT enemyarr[5][1000] = { 0 }; //[Ƣ����� ��ġ][������ �ð�], -1�� ������ ���� ���������� ģ��. x4������ 4�� 10�ʳ� ������ �� �ִ� ���̹Ƿ� 1000�̸� �����.
	int index = 0, indexTime = 0;
	char ch;
	setColor(gray);
	for (int i = 0; i < 5; i++)
		enemyarr[i][0] = -1;
	gotoxy(cmdcol / 3 - 2, 1); printf("���̺� �����ϱ�");
	for (int i = 7; i < 35; i++)
	{
		gotoxy(2 * MAX_LR + 10, i); printf("                                                                    ");//���� �����.
	}
	gotoxy(2 * MAX_LR + 10, 7); printf("���� ���̺갡 ���۵Ǵ� ���� �����մϴ�.");
	gotoxy(2 * MAX_LR + 10, 9); printf("���� ������ �ð��븦 ������ �� �ֽ��ϴ�.");
	gotoxy(2 * MAX_LR + 10, 11); printf("Q: ��ĭ(0)");
	gotoxy(2 * MAX_LR + 10, 13); printf("W: ���̺� ������(-1)");
	gotoxy(2 * MAX_LR + 10, 15); printf("A: ��(1)");
	gotoxy(2 * MAX_LR + 10, 17); printf("S: ������(2)");
	gotoxy(2 * MAX_LR + 10, 19); printf("D: �޸��� ����(3)");
	gotoxy(2 * MAX_LR + 10, 21); printf("F: ����(4)");
	gotoxy(2 * MAX_LR + 10, 23); printf("G: ��ȭ��(5) ");
	gotoxy(2 * MAX_LR + 10, 25); printf("Z: ���к�(6)");
	gotoxy(2 * MAX_LR + 10, 27); printf("X:���༱(7)");
	gotoxy(2 * MAX_LR + 10, 29); printf("C:����(8)");
	gotoxy(2 * MAX_LR + 10, 31); printf("V:������(9)");
	gotoxy(2 * MAX_LR + 10, 33); printf("B:�̿� ��(10)");
	gotoxy(2 * MAX_LR + 68, 38); printf("��");
	gotoxy(2 * MAX_LR + 8, 38); printf("��");
	while (1)//�Ʒ��� ������ ó�� ����
	{
		setColor(gray);
		gotoxy(2 * MAX_LR + 40, 37); printf("���̰� ��ġ: %3d", indexTime);
		gotoxy(2 * MAX_LR + 10, 38);
		for (int i = -10; i < 10; i++)
			printf("%02d ", enemyarr[index][(indexTime + 1000 + i) % 1000]);
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(bloody); printf("��");
		ch = _getch();
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(sky); printf("��");
		if (_kbhit())//Ű �Է¹ޱ�
		{
			ch = _getch();
			switch (ch)
			{
			case UP:
				index = (index + 4) % 5;
				while (1)
					if (enemystart[index].x == -1)
						index = (index + 4) % 5;
					else break;
					break;
			case DOWN:
				index = (index + 1) % 5;
				while (1)
					if (enemystart[index].x == -1)
						index = (index + 1) % 5;
					else break;
					break;
			case RIGHT:
				indexTime = (indexTime + 1) % 1000;
				break;
			case LEFT:
				indexTime = (indexTime + 999) % 1000;
				break;
			}
		}
		else
		{
			if (ch == ENTER)
			{
				break;
			}
			if (ch == 'q' || ch == 'Q')
				enemyarr[index][indexTime] = eEmpty;
			if (ch == 'w' || ch == 'W')
				enemyarr[index][indexTime] = eEnd;
			if (ch == 'a' || ch == 'A')
				enemyarr[index][indexTime] = eChicken;
			if (ch == 's' || ch == 'S')
				enemyarr[index][indexTime] = eThief;
			if (ch == 'd' || ch == 'D')
				enemyarr[index][indexTime] = eRunner;
			if (ch == 'f' || ch == 'F')
				enemyarr[index][indexTime] = ePirates;
			if (ch == 'g' || ch == 'G')
				enemyarr[index][indexTime] = eBomb;
			if (ch == 'z' || ch == 'Z')
				enemyarr[index][indexTime] = eShield;
			if (ch == 'x' || ch == 'X')
				enemyarr[index][indexTime] = eAirship;
			if (ch == 'c' || ch == 'C')
				enemyarr[index][indexTime] = eTank;
			if (ch == 'v' || ch == 'V')
				enemyarr[index][indexTime] = eKnight;
			if (ch == 'b' || ch == 'B')
				enemyarr[index][indexTime] = eKing;
			indexTime = (indexTime + 1) % 1000;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			fprintf(fp, "%d ", enemyarr[i][j]);
			if (enemyarr[i][j] == -1)
				break;
		}
		fprintf(fp, "\n");
	}
}
void sortMapFile()
{
	FILE* fp = NULL, *fptemp = NULL;
	int num, temp = 0, temp2 = 0, temp3 = 0;
	char string[20] = "", location[25] = "MAP\\";
	char name[1000][15] = { "" };
	int score[1000];
	fp = fopen("levels.txt", "rt");
	if (fp == NULL)//���� ��� ���� �����.
	{
		fp = fopen("levels.txt", "wt");
		fclose(fp);
		fp = fopen("levels.txt", "rt");
	}
	num = 0;
	while (!feof(fp))
	{
		fscanf(fp, "%s %d\n", &name[num], &score[num]);
		num++;
	}
	gotoxy(8, 2);
	setColor(gray);
	printf("���� �÷����� �� �ִ� ���� ������ �����ϴ�.");
	for (int i = 0; 5 + 2 * i < 170 && strcmp(name[i], "") != 0; i++)
	{
		gotoxy(10 + 20 * (i / 30), 3 + (i % 30)); printf("%3d.%s", i + 1, &name[i][0]);
	}
	gotoxy(8, 34); printf("�� �����ϰų� ���� �÷��� �� �� ���� ���� �ҷ��� �� �ֽ��ϴ�.");
	gotoxy(8, 35); printf("1�� ���� �� ������ ������ �ٲٰų�, 2�� ���� ���� �ҷ��� �� �ֽ��ϴ�.�� �̿ܿ��� �� ȭ���� �����ϴ�.>"); temp = _getch();
	if (temp == '1')
	{
		while (1)
		{
			gotoxy(8, 36); printf("���� �ٲ� �� ������ ��ȣ�� �Է����ּ���.(0�� ���� ���)");
			gotoxy(8, 37); printf("ù��°:"); scanf("%d", &temp);
			if (temp != 0)
			{
				gotoxy(8, 37); printf("�ι�°:"); scanf("%d", &temp2);
			}
			else break;
			if (temp2 != 0)
			{
				if (temp>0 && temp2> 0 && temp - 1 < 1000 && temp - 1< num && temp2 - 1 < 1000 && temp2 - 1 < num && strcmp(name[temp - 1], "") != 0 && strcmp(name[temp2 - 1], "") != 0)
				{
					strcpy(string, name[temp - 1]);
					strcpy(name[temp - 1], name[temp2 - 1]);
					strcpy(name[temp2 - 1], string);

					temp3 = score[temp - 1];
					score[temp - 1] = score[temp2 - 1];
					score[temp2 - 1] = temp3;
					fclose(fp);
					fp = fopen("levels.txt", "wt");
					for (int i = 0; i < num; i++)
						fprintf(fp, "%s %d\n", name[i], score[i]);
				}
				system("cls");
				gotoxy(8, 2);
				printf("���� �÷����� �� �ִ� ���� ������ �����ϴ�.");
				for (int i = 0; 5 + 2 * i < 170 && strcmp(name[i], "") != 0; i++)
				{
					gotoxy(10 + 20 * (i / 30), 3 + (i % 30)); printf("%3d.%s", i + 1, &name[i][0]);
				}
			}
			else break;
		}
	}
	else if (temp == '2')
	{
		while (1)
		{
			gotoxy(8, 9 + num); printf("�ҷ��� ���� �̸��� �����ΰ���?(�� ���� ������)");
			gotoxy(8, 10 + num); printf(">"); scanf("%s", string);
			strcat(location, string);
			strcat(location, ".level");
			fptemp = fopen(location, "rt"); //���� ��ȿ�� üũ.
			if (fptemp == NULL)
			{
				//fptemp�� ������ �ȵȴ�.
				gotoxy(8, 11 + num); printf("�׷� ������ ���� �� �����ϴ�.");
				Sleep(500);
				gotoxy(8, 11 + num); printf("\t\t\t\t");
				gotoxy(9, 10 + num); printf("\t\t\t\t");
			}
			else
			{
				//���� �������� ���� üũ���� ����. �ݵ�� �߰��ؾ� �ȴ� �����̶�
				fclose(fptemp);
				leveladd(string);
				gotoxy(8, 11 + num); printf("������ �߰��߽��ϴ�!");
				Sleep(500);
				gotoxy(8, 11 + num); printf("\t\t\t\t");
				break;
			}
		}
	}
	fclose(fp);
}
void newMapFile()
{
	int num, temp = 0, temp2 = 0, temp3 = 0;
	char string[20] = "", location[25] = "MAP\\";
	char name[1000][15] = { "" };
	char ch;
	int rewritedFile = 0;
	FILE* fp = NULL;
	FILE* fptemp = NULL;
	mkdir("MAP");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("���� ���� ���� �̸��� �Է����ֽʽÿ�(�ִ� 14��, ���� ����)");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 1); printf(">");
	while (1)
	{
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("\t\t\t\t\t\t\t\t\t\t");
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); scanf("%s", string); getchar();
		if (strlen(string) > 14)//�̸��� �ʹ� ��.
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("�̸��� �ʹ� ��ϴ�. �ٽ� �õ����ּ���.");
			strcpy(location, "MAP\\");
			Sleep(1000);
			continue;
		}
		strcat(location, string);
		strcat(location, ".level");
		fp = fopen(location, "rt");
		if (fp != NULL)//�̹� ���� �̸��� ������ ����.
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("�̹� ���� �̸��� ������ �ֽ��ϴ�. �׷��� ���� ������?(�� ������ �������ϴ�.)(Y/N)");
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 2); printf(">"); scanf("%c", &ch);
			if (ch == 'Y' || ch == 'y')
			{
				gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 3); printf("�� ������ �������ϴ�.");
				fp = fopen(location, "wt");
				Sleep(2000);
				rewritedFile = 1;
				break;
			}
			else
			{
				gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 3); printf("���� ������ �����߽��ϴ�.");
				strcpy(location, "MAP\\");
				Sleep(2000);
				return;
			}
		}
		else //���� �̸��� ���� ������ ����.
		{
			//������ ������ fclose�� ���� �ʴ´�.
			fp = fopen(location, "wt");
			break;
		}
	}
	//�� ����ȭ��
	if (mapEdit(fp))
	{
		//���̺� ����ȭ��
		waveEdit(fp);
		system("cls");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 2);
		printf("���� �󸶳� �ٱ��?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);
		gotoxy(cmdcol / 2 - 10, cmdrow / 2);
		printf("�� ü���� �󸶷� �ұ��?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 + 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);
		//�̺�Ʈ ���� ���
		fileEdit(fp);
		system("cls");
		setColor(gray);
		fclose(fp);
		if (!rewritedFile)
		{
			fp = fopen("levels.txt", "at");
			fprintf(fp, "%s 0\n", string);
			fclose(fp);
		}
		system("cls");
		gotoxy(cmdcol / 2 - 23, cmdrow / 2); printf("%s�� ����Ǿ����ϴ�. ���� ���� ȭ�� �������� ��ġ�˴ϴ�.", location);
		Sleep(500);
	}
}
void editMapFile()
{

	int num, temp = 0, temp2 = 0, temp3 = 0;
	char string[20] = "";
	enemyT wave[5][1000] = {0};
	FILE* fp = NULL;
	mkdir("MAP");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("������ ���� �̸��� �Է����ֽʽÿ�(�ִ� 14��, ���� ����)");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 1); printf(">");
	while (1)
	{
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("\t\t\t\t\t\t\t\t\t\t");
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); scanf("%s", string); getchar();
		if (strlen(string) > 14)//�̸��� �ʹ� ��.
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("�̸��� �ʹ� ��ϴ�. �ٽ� �õ����ּ���.");
			Sleep(1000);
			continue;
		}
		strcpy(location, "MAP\\");
		strcat(location, string);
		strcat(location, ".level");
		fp = fopen(location, "rt");
		if (fp == NULL)
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("�׷� �̸��� ���� ������ �����ϴ�.");
			Sleep(1000);
		}
		else break;
	}
	//�� ����ȭ��
	if (editMapEdit(fp,wave))
	{
		//���̺� ����ȭ��
		editWaveEdit(fp,wave);
		system("cls");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 2);
		printf("���� �󸶳� �ٱ��?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);
		gotoxy(cmdcol / 2 - 10, cmdrow / 2);
		printf("�� ü���� �󸶷� �ұ��?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 + 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);
		//�̺�Ʈ ���� ���
		editFileEdit(fp);

		system("cls");
		setColor(gray);
		fclose(fp);
		gotoxy(cmdcol / 2 - 23, cmdrow / 2); printf("%s�� ����Ǿ����ϴ�. ���� ���� ȭ�� �������� ��ġ�˴ϴ�.", location);
		Sleep(500);
	}
}
int editMapEdit(FILE* fp, enemyT (*enemyarr)[1000])
{
	int temp;
	const pos start = { 2,4 }; //���� ����ϱ� �����ϴ� ��ġ. 
	int map[MAX_UD][MAX_LR] = { 0 };
	int castleExist = 0, enemyExist = 0, doorExist = 0;
	pos pt = { 0,0 }; //���� ���� ���� �� ��ǥ�� (0,0), �� ĭ�� 2byte���� �ϳ� ũ�⸦ ����
	char ch;
	initMapEdit();
	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	printf("��");
	rewind(fp);
	for (int i = 0; i < MAX_UD; i++) //���Ͽ��� �޾ƿ��� ����
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			fscanf(fp, "%d", &map[i][j]);
		if (map[i][j] == CASTLE_DOOR)
			doorExist++;
		else if (map[i][j] == CASTLE)
			castleExist = 1;
		else if (map[i][j] == ENEMYPOS)
			enemyExist++;
		}
		fscanf(fp, "\n");
	}
	for (int i = 0; i < MAX_UD; i++)
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			gotoxy(start.x + 2 * j, start.y + i);
			switch (map[i][j])
			{
			case INVALID:
				setColor(ivory);
				printf("* ");
				break;
			case EMPTY:
				printf("  ");
				break;
			case STONE:
				setColor(gray);
				printf("��");
				break;
			case CASTLE:
				setColor(sky);
				printf("��");
				break;
			case ENEMYPOS:
				setColor(sky);
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
			}//������ ����� ��
		}
	}
	for (int i = 0; i < 5; i++)//���� ���� ��ġ
		fscanf(fp, "%d %d", &enemystart[i].x, &enemystart[i].y);

	for (int i = 0; i < 5; i++)//���� �迭
	{
		for (int j = 0; j < 1000; j++)
		{
			fscanf(fp, "%d", &temp);
			enemyarr[i][j] = temp;
			if (temp == -1)
			{
				break;
			}
		}
	}
	fscanf(fp, "%*d %*d");//���� ����
	for (int i = 0; i < 70; i++)
	{
		fscanf(fp, "%d", &order[i]);
		if (order[i] == memo || order[i] == orderTalk || order[i] == openLevel) //���ڿ��� �ʿ��� ��ɾ�
		{
			fscanf(fp, "%s\n", string[i]);
		}
		else if (order[i] == openUnit)//���ڰ� �ʿ��� ��ɾ�
		{
			fscanf(fp, "%d\n", &string[i][0]);
		}
		else//�ٸ� ����� �ʿ� ���� ��ɾ�
		{
			fscanf(fp, "0\n");
		}
	}
	while (1) //�����-> ������ �ű��-> �׸��⸦ �ݺ�.
	{
		gotoxy(2 * MAX_LR + 10, 32); printf("����%s", castleExist ? "�ֽ��ϴ�" : "�����ϴ�");
		gotoxy(2 * MAX_LR + 10, 33); printf("%d���� ���� ���� ���� �ֽ��ϴ�.", enemyExist);
		gotoxy(2 * MAX_LR + 10, 34); printf("%d���� ������ �ֽ��ϴ�.", doorExist);
		ch = _getch();
		gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
		if (pt.x == MAX_LR - 1)//�����͸� ����� �۾�
		{
			printf("��");
		}
		else
		{
			switch (map[pt.y][pt.x + 1])
			{
			case INVALID:
				setColor(ivory);
				printf("* ");
				break;
			case EMPTY:
				printf("  ");
				break;
			case STONE:
				setColor(gray);
				printf("��");
				break;
			case CASTLE:
				setColor(sky);
				printf("��");
				break;
			case ENEMYPOS:
				setColor(sky);
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
			}//������ ����� ��
		}
		if (_kbhit())//Ű �ޱ� + ó��
		{
			ch = _getch();
			switch (ch)
			{
			case UP:
				pt.y = (pt.y + MAX_UD - 1) % MAX_UD;
				break;
			case LEFT:
				pt.x = (pt.x + MAX_LR - 1) % MAX_LR;
				break;
			case RIGHT:
				pt.x = (pt.x + 1) % MAX_LR;
				break;
			case DOWN:
				pt.y = (pt.y + 1) % MAX_UD;
				break;
			}
		}
		else
		{
			switch (ch)
			{
			case ESC:
				return 0;
			case ENTER:
				setColor(bloody);
				if (castleExist)
				{
					if (enemyExist > 0)
					{
						if (doorExist > 0)
						{
							if (enemyExist - doorExist <= 0)
							{
								goto BREAK_GET_KEY;
							}
							else
							{
								gotoxy(2 * MAX_LR + 10, 31); printf("\"���� ���� ��ġ�� �� �������� �����ϴ�!\"");
								Sleep(500);
								gotoxy(2 * MAX_LR + 10, 31); printf("                                        ");
							}
						}
						else
						{
							gotoxy(2 * MAX_LR + 10, 31); printf("\"������ �����ϴ�!\"");
							Sleep(500);
							gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
						}
					}
					else
					{
						gotoxy(2 * MAX_LR + 10, 31); printf("\"���� ���� ���� �����ϴ�!\"");
						Sleep(500);
						gotoxy(2 * MAX_LR + 10, 31); printf("                                                  ");
					}
				}
				else
				{
					gotoxy(2 * MAX_LR + 10, 31); printf("\"���� �����ϴ�!\"");
					Sleep(500);
					gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
				}
				break;
			case '9': //�� ���� ����
				for (int i = 0; i < MAX_UD; i++)
					for (int j = 0; j < MAX_LR; j++)
						map[i][j] = INVALID;
				castleExist = 0, enemyExist = 0, doorExist = 0;
				initMapEdit();
				break;
			default:



				if (ch >= '1' && ch <= '6' && map[pt.y][pt.x] != CASTLE && map[pt.y][pt.x] != CASTLE_INV) //�� ����, �� �Ա� ���� ������Ʈ
				{
					if (map[pt.y][pt.x] == CASTLE_DOOR)
						doorExist--;
					if (map[pt.y][pt.x] == ENEMYPOS)
						enemyExist--;
					gotoxy(start.x + 2 * pt.x, start.y + pt.y);
					switch (ch - '0' - 1)
					{
					case 5:
						map[pt.y][pt.x] = INVALID;
						setColor(ivory);
						printf("* ");
						break;
					case EMPTY://��ĭ
						map[pt.y][pt.x] = EMPTY;
						printf("  ");
						break;
					case STONE://��
						map[pt.y][pt.x] = STONE;
						setColor(gray);
						printf("��");
						break;
					case CASTLE://��
						if (!castleExist)
						{
							if (pt.x >= 2 && pt.x <= MAX_LR - 3 && pt.y >= 2 && pt.y <= MAX_UD - 3)
							{
								castleExist = 1;
								for (int i = -2; i <= 2; i++)
									for (int j = -2; j <= 2; j++)
									{
										if (map[pt.y + i][pt.x + j] == CASTLE_DOOR)
											doorExist--;
										if (map[pt.y + i][pt.x + j] == ENEMYPOS)
											enemyExist--;
										map[pt.y + i][pt.x + j] = CASTLE_INV; //�� �ֺ��� ������ ������ �ȵȴ�. ��������δ� BLOCK�� ������ ������ üũ�� �� �� ����.
									}
								map[pt.y][pt.x] = CASTLE;
								setColor(gray);
								for (int i = -2; i <= 2; i++)
									for (int j = -2; j <= 2; j++)
									{
										gotoxy(start.x + 2 * (pt.x + i), start.y + pt.y + j); printf("XX");
									}
								setColor(sky);
								gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("��");
							}
						}
						else
						{
							if (map[pt.y][pt.x] == CASTLE_DOOR)
								doorExist++;
							if (map[pt.y][pt.x] == ENEMYPOS)
								enemyExist++;
						}
						break;
					case ENEMYPOS://�� ����
						if (enemyExist < 5)
						{
							enemyExist++;
							map[pt.y][pt.x] = ENEMYPOS;
							setColor(sky);
							printf("��");
						}
						break;
					case CASTLE_DOOR://����
						for (int i = -1; i <= 1; i++)
							for (int j = -1; j <= 1; j++)
							{
								if (map[pt.y + i][pt.x + j] == CASTLE_INV)
								{
									doorExist++;
									map[pt.y][pt.x] = CASTLE_DOOR; //���� ��ȿ�� üũ.
									setColor(violet);
									gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("��");
									goto BREAK_CASE_CASTLE_DOOR;
								}
							}
					BREAK_CASE_CASTLE_DOOR:
						break;
					}
				}
				break;
			}
		}
		gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
		setColor(gray);
		printf("��");
	}
BREAK_GET_KEY://ENTER�� ������带 ������.
	enemyExist = 0;
	fclose(fp);
	fp = fopen(location, "wt");
	for (int i = 0; i < MAX_UD; i++)
	{
		for (int j = 0; j < MAX_LR; j++)
		{
			fprintf(fp, "%d ", map[i][j]);
			if (map[i][j] == ENEMYPOS)
			{
				enemystart[enemyExist].x = j;
				enemystart[enemyExist].y = i;
				enemyExist++;
			}
		}
		fprintf(fp, "\n");
	}
	for (int i = 0; i < 5; i++)
		fprintf(fp, "%d %d\n", enemystart[i].x, enemystart[i].y);

	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	if (pt.x == MAX_LR - 1)
	{
		printf("��");
	}
	else
	{
		switch (map[pt.y][pt.x + 1])
		{
		case EMPTY:
			printf("  ");
			break;
		case STONE:
			setColor(gray);
			printf("��");
			break;
		case CASTLE:
			setColor(sky);
			printf("��");
			break;
		case ENEMYPOS:
			setColor(sky);
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
		}
	}
	return 1;
}
void editWaveEdit(FILE* fp, enemyT (*enemyarr)[1000])
{
	const pos start = { 2,4 }; //���� ����ϱ� �����ϴ� ��ġ.  �׵θ� �ƴ�.
	int index = 0, indexTime = 0;
	char ch;
	setColor(gray);
	gotoxy(cmdcol / 3 - 2, 1); printf("���̺� �����ϱ�");
	for (int i = 7; i < 35; i++)
	{
		gotoxy(2 * MAX_LR + 10, i); printf("                                                                    ");//���� �����.
	}
	gotoxy(2 * MAX_LR + 10, 7); printf("���� ���̺갡 ���۵Ǵ� ���� �����մϴ�.");
	gotoxy(2 * MAX_LR + 10, 9); printf("���� ������ �ð��븦 ������ �� �ֽ��ϴ�.");
	gotoxy(2 * MAX_LR + 10, 11); printf("Q: ��ĭ(0)");
	gotoxy(2 * MAX_LR + 10, 13); printf("W: ���̺� ������(-1)");
	gotoxy(2 * MAX_LR + 10, 15); printf("A: ��(1)");
	gotoxy(2 * MAX_LR + 10, 17); printf("S: ������(2)");
	gotoxy(2 * MAX_LR + 10, 19); printf("D: �޸��� ����(3)");
	gotoxy(2 * MAX_LR + 10, 21); printf("F: ����(4)");
	gotoxy(2 * MAX_LR + 10, 23); printf("G: ��ȭ��(5) ");
	gotoxy(2 * MAX_LR + 10, 25); printf("Z: ���к�(6)");
	gotoxy(2 * MAX_LR + 10, 27); printf("X:���༱(7)");
	gotoxy(2 * MAX_LR + 10, 29); printf("C:����(8)");
	gotoxy(2 * MAX_LR + 10, 31); printf("V:������(9)");
	gotoxy(2 * MAX_LR + 10, 33); printf("B:�̿� ��(10)");
	gotoxy(2 * MAX_LR + 68, 38); printf("��");
	gotoxy(2 * MAX_LR + 8, 38); printf("��");
	while (1)//�Ʒ��� ������ ó�� ����
	{
		setColor(gray);
		gotoxy(2 * MAX_LR + 40, 37); printf("���̰� ��ġ: %3d", indexTime);
		gotoxy(2 * MAX_LR + 10, 38);
		for (int i = -10; i < 10; i++)
			printf("%02d ", enemyarr[index][(indexTime + 1000 + i) % 1000]);
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(bloody); printf("��");
		ch = _getch();
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(sky); printf("��");
		if (_kbhit())//Ű �Է¹ޱ�
		{
			ch = _getch();
			switch (ch)
			{
			case UP:
				index = (index + 4) % 5;
				while (1)
					if (enemystart[index].x == -1)
						index = (index + 4) % 5;
					else break;
					break;
			case DOWN:
				index = (index + 1) % 5;
				while (1)
					if (enemystart[index].x == -1)
						index = (index + 1) % 5;
					else break;
					break;
			case RIGHT:
				indexTime = (indexTime + 1) % 1000;
				break;
			case LEFT:
				indexTime = (indexTime + 999) % 1000;
				break;
			}
		}
		else
		{
			if (ch == ENTER)
			{
				break;
			}
			if (ch == 'q' || ch == 'Q')
				enemyarr[index][indexTime] = eEmpty;
			if (ch == 'w' || ch == 'W')
				enemyarr[index][indexTime] = eEnd;
			if (ch == 'a' || ch == 'A')
				enemyarr[index][indexTime] = eChicken;
			if (ch == 's' || ch == 'S')
				enemyarr[index][indexTime] = eThief;
			if (ch == 'd' || ch == 'D')
				enemyarr[index][indexTime] = eRunner;
			if (ch == 'f' || ch == 'F')
				enemyarr[index][indexTime] = ePirates;
			if (ch == 'g' || ch == 'G')
				enemyarr[index][indexTime] = eBomb;
			if (ch == 'z' || ch == 'Z')
				enemyarr[index][indexTime] = eShield;
			if (ch == 'x' || ch == 'X')
				enemyarr[index][indexTime] = eAirship;
			if (ch == 'c' || ch == 'C')
				enemyarr[index][indexTime] = eTank;
			if (ch == 'v' || ch == 'V')
				enemyarr[index][indexTime] = eKnight;
			if (ch == 'b' || ch == 'B')
				enemyarr[index][indexTime] = eKing;
			indexTime = (indexTime + 1) % 1000;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			fprintf(fp, "%d ", enemyarr[i][j]);
			if (enemyarr[i][j] == -1)
				break;
		}
		fprintf(fp, "\n");
	}
}
void fileEdit(FILE* fp)//���� ������ �����ϴ� ������ ����.
{
	int ch = 0;
	int cur = 0;
	orderT order[70] = {0};
	char string[70][100];
	system("cls");
	gotoxy(cmdcol / 9 - 5, 2); printf("������������������");
	gotoxy(cmdcol / 9 - 5, 3); printf("������ ���� ������");
	gotoxy(cmdcol / 9 - 5, 4); printf("������������������");
	for (int i = 0; i < cmdrow; i++)
	{
		gotoxy(cmdcol / 3, i); printf("��");
		gotoxy(cmdcol / 3 * 2, i); printf("��");
	}
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7); printf("���� ������ �ܰ��Դϴ�!");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 2); printf("������ ������ ������ �����ּ���");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 4); printf("�ݵ�� �� ������");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 6); printf("���� �����ϱ��");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 8); printf("Ŭ���� ���� �� ��ɡ�");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 10); printf("�� ������ �����ּ���.");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 12); printf("�� �Ʒ� ����Ű�� Ŀ���� ������ �� �ֽ��ϴ�.");
	setColor(rime);
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 14); printf("1: ���� ��ġ�� �޸��ϱ�");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 16); printf("2:�÷��̾�� ���ϱ�");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 18); printf("3:�� �ı�(�ϳ���)");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 20); printf("4: ���� ����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 22); printf("5: Ŭ�����ϸ� ���⼭����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 24); printf("6: �����ϸ� ���⼭����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 26); printf("7: ���ο� ���� ����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 28); printf("8: ���� ����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 30); printf("9: ��");
	setColor(sky);
	for (int i = 0; i < 70; i++)
	{
		gotoxy(cmdcol / 3 * (i/ 35) + 3, i % 35 + 5); printf("%d", i);
	}
	gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
	setColor(rime);
	printf("��");
	while (1)
	{
		ch = _getch();
		if (_kbhit())
		{
			ch = _getch();
			if (ch == UP || ch == DOWN)
			{
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
			}

			if (ch == UP)
				cur = (cur + 69) % 70;
			else if (ch == DOWN)
				cur = (cur + 1) % 70;
			gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
			setColor(rime);
			printf("��");
		}
		else
		{
			if (ch == '1')
			{
				order[cur] = memo;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("�޸�:");
				gotoxy(cmdcol / 3 * (cur / 35) + 15, cur % 35 + 5);
				gets(string[cur]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '2')
			{
				order[cur] = orderTalk;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("���ϱ�:");
				gotoxy(cmdcol / 3 * (cur / 35) + 17, cur % 35 + 5); 
				gets(string[cur]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '3')
			{
				order[cur] = mkroad;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���ı�");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '4')
			{
				order[cur] = gamestart;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���� ����");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '5')
			{
				order[cur] = clear;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>Ŭ�����ϸ� ���⼭����");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '6')
			{
				order[cur] = fail;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>�����ϸ� ���⼭����");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '7')
			{
				order[cur] = openUnit;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���� ���:  ��");
				gotoxy(cmdcol / 3 * (cur / 35) + 22, cur % 35 + 5);
				scanf("%d", &string[cur][0]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '8')
			{
				order[cur] = openLevel;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���� ���:");
				gets(string[cur]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '9')
			{
				order[cur] = endRead;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>�б� ��");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == ENTER)
				break;
		}
	}
	for (int i = 0; i < 70;i++)
	{
		if (order[i] == memo || order[i] == orderTalk || order[i] == openLevel) //���ڿ��� �ʿ��� ��ɾ�
		{
			fprintf(fp,"%d %s\n", order[i], string[i]);
		}
		else if (order[i] == openUnit)//���ڰ� �ʿ��� ��ɾ�
		{
			fprintf(fp, "%d %d\n", order[i], string[i][0]);
		}
		else//�ٸ� ����� �ʿ� ���� ��ɾ�, �ƹ��͵� ������ �ʴ� 0�� ����.
		{
			fprintf(fp, "%d 0\n", order[i]);
		}
	}
}
void editFileEdit(FILE* fp)
{
	int ch = 0;
	int cur = 0;
	system("cls");
	gotoxy(cmdcol / 9 - 5, 2); printf("������������������");
	gotoxy(cmdcol / 9 - 5, 3); printf("������ ���� ������");
	gotoxy(cmdcol / 9 - 5, 4); printf("������������������");
	for (int i = 0; i < cmdrow; i++)
	{
		gotoxy(cmdcol / 3, i); printf("��");
		gotoxy(cmdcol / 3 * 2, i); printf("��");
	}
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7); printf("���� ������ �ܰ��Դϴ�!");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 2); printf("������ ������ ������ �����ּ���");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 4); printf("�ݵ�� �� ������");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 6); printf("���� �����ϱ��");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 8); printf("Ŭ���� ���� �� ��ɡ�");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 10); printf("�� ������ �����ּ���.");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 12); printf("�� �Ʒ� ����Ű�� Ŀ���� ������ �� �ֽ��ϴ�.");
	setColor(rime);
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 14); printf("1: ���� ��ġ�� �޸��ϱ�");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 16); printf("2:�÷��̾�� ���ϱ�");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 18); printf("3:�� �ı�(�ϳ���)");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 20); printf("4: ���� ����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 22); printf("5: Ŭ�����ϸ� ���⼭����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 24); printf("6: �����ϸ� ���⼭����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 26); printf("7: ���ο� ���� ����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 28); printf("8: ���� ����");
	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 30); printf("9: ��");
	setColor(sky);
	for (int i = 0; i < 70; i++)
	{
		gotoxy(cmdcol / 3 * (i / 35) + 3, i % 35 + 5); printf("%d", i);
		if (order[i] == '1')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf("�޸�:");
			printf("%s", string[i]);
		}
		if (ch == '2')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf("���ϱ�:");
			printf("%s", string[i]);
		}
		if (ch == '3')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf(">>���ı�");
		}
		if (ch == '4')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf(">>���� ����");
		}
		if (ch == '5')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf(">>Ŭ�����ϸ� ���⼭����");
		}
		if (ch == '6')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf(">>�����ϸ� ���⼭����");
		}
		if (ch == '7')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf(">>���� ���:  %d��", string[i][0]);
		}
		if (ch == '8')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf(">>���� ���:");
			printf("%s", string[i]);
		}
		if (ch == '9')
		{
			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
			printf(">>�б� ��");
		}
		gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
	}
	gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
	setColor(rime);
	printf("��");
	while (1)
	{
		ch = _getch();
		if (_kbhit())
		{
			ch = _getch();
			if (ch == UP || ch == DOWN)
			{
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
			}

			if (ch == UP)
				cur = (cur + 69) % 70;
			else if (ch == DOWN)
				cur = (cur + 1) % 70;
			gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
			setColor(rime);
			printf("��");
		}
		else
		{
			if (ch == '1')
			{
				order[cur] = memo;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("�޸�:");
				gotoxy(cmdcol / 3 * (cur / 35) + 15, cur % 35 + 5);
				gets(string[cur]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '2')
			{
				order[cur] = orderTalk;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("���ϱ�:");
				gotoxy(cmdcol / 3 * (cur / 35) + 17, cur % 35 + 5);
				gets(string[cur]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '3')
			{
				order[cur] = mkroad;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���ı�");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '4')
			{
				order[cur] = gamestart;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���� ����");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '5')
			{
				order[cur] = clear;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>Ŭ�����ϸ� ���⼭����");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '6')
			{
				order[cur] = fail;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>�����ϸ� ���⼭����");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '7')
			{
				order[cur] = openUnit;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���� ���:  ��");
				gotoxy(cmdcol / 3 * (cur / 35) + 22, cur % 35 + 5);
				scanf("%d", &string[cur][0]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '8')
			{
				order[cur] = openLevel;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>���� ���:");
				gets(string[cur]);
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == '9')
			{
				order[cur] = endRead;
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf("\t\t\t\t\t");
				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
				printf(">>�б� ��");
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				printf("  ");
				cur = (cur + 1) % 70;
				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
				setColor(rime);
				printf("��");
			}
			if (ch == ENTER)
				break;
		}
	}
	for (int i = 0; i < 70; i++)
	{
		if (order[i] == memo || order[i] == orderTalk || order[i] == openLevel) //���ڿ��� �ʿ��� ��ɾ�
		{
			fprintf(fp, "%d %s\n", order[i], string[i]);
		}
		else if (order[i] == openUnit)//���ڰ� �ʿ��� ��ɾ�
		{
			fprintf(fp, "%d %d\n", order[i], string[i][0]);
		}
		else//�ٸ� ����� �ʿ� ���� ��ɾ�
		{
			fprintf(fp, "%d 0\n", order[i]);
		}
	}
}