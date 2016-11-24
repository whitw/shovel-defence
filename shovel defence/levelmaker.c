#include "define.h"
#pragma warning(disable:4996)

pos enemystart[5]; //���̵尡 ���۵Ǵ� ����, �ִ� 4��

void levelmaker()
{
	int num;
	char string[15] = "",location[25] = "MAP\\";
	char name[100][15] = { "" };
	FILE* fp = NULL;
	system("cls");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("���ڸ� �Է��� �ּ���.");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 2); printf("1. ���� �����ϱ�");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 3); printf("2. �� ���� �����");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 4); printf("3. ���� �����ϱ�");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 5); printf("4. ������");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 6); printf(">"); scanf("%d", &num);
	system("cls");
	if (num == 1) //�� ���� ����
	{
		fp = fopen("levels.txt", "rt");
		if (fp == NULL)//���� ��� ���� �����.
		{
			fp = fopen("levels,txt", "wt");
			fclose(fp);
			fp = fopen("levels.txt", "rt");
		}
		num = 0;
		while (!feof(fp))
		{
			fscanf(fp, "%s %*d\n", &name[num][0]);
			num++;
		}
		gotoxy(8, 2);
		printf("���� �÷����� �� �ִ� ���� ������ �����ϴ�.");
		for (int i = 0; strcmp(name[i],"") != 0; i++)
		{
			gotoxy(8,5+2 * i);printf("%3d.%s", i+1,&name[i][0]);
		}
		system("pause");
		fclose(fp);
	}
	else if (num == 2) //�� ���� �����
	{
		mkdir("MAP");
		gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("���� ���� ���� �̸��� �Է����ֽʽÿ�(�ִ� 14��).");
		gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 1); printf(">");
		while(1)
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
				gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 2); printf(">"); scanf("%d", &num);
				if (num == 'Y' || num == 'y')
				{
					fp = fopen(location, "wt");
					break;
				}
				else
				{
					gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 3); printf("���� ������ �����߽��ϴ�.");
					strcpy(location, "MAP\\");
					Sleep(2000);
					continue;
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
			scanf("%d",&num);
			fprintf(fp, "%d\n", num);
			gotoxy(cmdcol / 2 - 10, cmdrow / 2);
			printf("�� ü���� �󸶷� �ұ��?");
			gotoxy(cmdcol / 2 - 10, cmdrow / 2 + 1);
			printf(">");
			scanf("%d", &num);
			fprintf(fp, "%d\n", num);
			//�̺�Ʈ ���� ���
			//

			system("cls");
			setColor(gray);
			fclose(fp);
			fp = fopen("levels.txt", "at");
			fprintf(fp, "%s 0\n", string);
			fclose(fp);
			system("cls");
			gotoxy(cmdcol / 2 - 23, cmdrow / 2); printf("%s�� ����Ǿ����ϴ�. ���� ���� ȭ�� �������� ��ġ�˴ϴ�.", location);
			Sleep(500);
		}
	}
	else if (num == 3) //���� ����
	{

	}
	//�� �̿��� ��쿡�� ���ڿ� ��� ���� ������
}

void initMapEdit()
{
	const pos start = { 2,4 }; //���� ����ϱ� �����ϴ� ��ġ. 
	system("cls");
	setColor(gray);
	gotoxy(cmdcol / 3, 1); printf("���� �����ϱ�");
	initSquare(start, "��");
	gotoxy(2 * MAX_LR + 10, 7); printf("������ �����͸� �̵��� �� �ֽ��ϴ�.");
	gotoxy(2 * MAX_LR + 10, 11); printf("���� ��ư�� ������ ������ ��ġ�� �̿� ���� �͵��� ���� �� �ֽ��ϴ�.");
	gotoxy(2 * MAX_LR + 10, 15); printf("1: ��ĭ");
	gotoxy(2 * MAX_LR + 10, 17); printf("2: ��");
	gotoxy(2 * MAX_LR + 10, 19); printf("3: ��. �� �ѹ��� ���� �� ������ ������ ��������.");
	gotoxy(2 * MAX_LR + 10, 21); printf("4: ���� ��� ��ġ����� ���⼭���� ����մϴ�.");
	gotoxy(2 * MAX_LR + 10, 22); printf("���� �����ڸ����� �����ϸ� ��� �ϳ��� �־�� �մϴ�. ");
	gotoxy(2 * MAX_LR + 10, 24); printf("5:��������� ���⿡�� �����ϴ�.�ݵ�� �� �ٷ� ���� �־�� �ϸ�");
	gotoxy(2 * MAX_LR + 10, 25); printf("���� ��� ��ġ�� �������ٴ� ���ƾ� �մϴ�.");
	gotoxy(2 * MAX_LR + 10, 27); printf("9: ��� ������ ���������ϴ�.");
	gotoxy(2 * MAX_LR + 10, 29); printf("ENTER�� ������ �Ϸ��� �� �ֽ��ϴ�.");//�� �� 3, 4�� 5�� �ִ��� üũ�Ѵ�.
}


int mapEdit(FILE* fp) //���� wt�� ��������. ���� �ʿ�� ����.
{
	const pos start = { 2,4 }; //���� ����ϱ� �����ϴ� ��ġ. 
	int map[MAX_LR][MAX_UD] = {0};
	int castleExist = 0,enemyExist = 0,doorExist = 0;
	pos pt = {0,0}; //���� ���� ���� �� ��ǥ�� (0,0), �� ĭ�� 2byte���� �ϳ� ũ�⸦ ����
	char ch;
	initMapEdit();
	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	printf("��");
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
			switch (map[pt.x + 1][pt.y])
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
				for (int i = 0; i < MAX_LR; i++)
					for (int j = 0; j < MAX_UD; j++)
						map[i][j] = EMPTY;
				castleExist = 0,enemyExist = 0, doorExist = 0;
				initMapEdit();
				break;
			default:



				if (ch >= '1' && ch <= '5' && map[pt.x][pt.y] != CASTLE && map[pt.x][pt.y] != CASTLE_INV) //�� ����, �� �Ա� ���� ������Ʈ
				{
					if (map[pt.x][pt.y] == CASTLE_DOOR)
						doorExist--;
					if (map[pt.x][pt.y] == ENEMYPOS)
						enemyExist--;
					gotoxy(start.x + 2 * pt.x, start.y + pt.y);
					switch (ch - '0' - 1)
					{
					case EMPTY://��ĭ
						map[pt.x][pt.y] = EMPTY;
						printf("  ");
						break;
					case STONE://��
						map[pt.x][pt.y] = STONE;
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
										if (map[pt.x+i][pt.y+j] == CASTLE_DOOR)
											doorExist--;
										if (map[pt.x+i][pt.y+j] == ENEMYPOS)
											enemyExist--;
										map[pt.x + i][pt.y + j] = CASTLE_INV; //�� �ֺ��� ������ ������ �ȵȴ�. ��������δ� BLOCK�� ������ ������ üũ�� �� �� ����.
									}
								map[pt.x][pt.y] = CASTLE;
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
							if (map[pt.x][pt.y] == CASTLE_DOOR)
								doorExist++;
							if (map[pt.x][pt.y] == ENEMYPOS)
								enemyExist++;
						}
						break;
					case ENEMYPOS://�� ����
						if (enemyExist < 5)
						{
							enemyExist++;
							map[pt.x][pt.y] = ENEMYPOS;
							setColor(sky);
							printf("��");
						}
						break;
					case CASTLE_DOOR://����
						for (int i = -1; i <= 1; i++)
							for (int j = -1; j <= 1; j++)
							{
								if (map[pt.x + i][pt.y + j] == CASTLE_INV)
								{
									doorExist++;
									map[pt.x][pt.y] = CASTLE_DOOR; //���� ��ȿ�� üũ.
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
BREAK_GET_KEY://ESC�� ������带 ������.
	enemyExist = 0;
	for (int i = 0;i < MAX_LR; i++)
	{
		for (int j = 0; j < MAX_UD; j++)
		{
			fprintf(fp, "%d ", map[j][i]); //������ �����ڸ� �Ǽ��� �κ� ����.
			if (map[j][i] == ENEMYPOS)// ����������ϰ� �յ�, ���Ʒ��� ������ �ٲ���µ�
			{
				enemystart[enemyExist].x = j;// �ǵ��� �ʹ� Ŀ������ ���߿� ���� �Ƚ��� �ð� ���� ��ģ��.
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
		switch (map[pt.x + 1][pt.y])
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
	for (int i = 0; i < 5;i++)
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
	while (1)
	{
		setColor(gray);//�Ʒ��� ������
	gotoxy(2 * MAX_LR + 40, 37); printf("���̰� ��ġ: %3d", indexTime);
		gotoxy(2 * MAX_LR + 10, 38);
		for (int i = -10; i < 10; i++)
			printf("%02d ", enemyarr[index][(indexTime +1000 + i) % 1000]);
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
				while(1)
					if(enemystart[index].x == -1)
						index = (index + 4) % 5;
					else break;
				break;
			case DOWN:
				index = (index + 1) % 5;
				while (1)
					if (enemystart[index].x == -1)
						index =(index + 1) % 5;
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
			if(ch == 'q' || ch == 'Q')
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
			if (ch == 'g' || ch == 'V')
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