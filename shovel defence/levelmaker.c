#include "define.h"
#include "levelmaker.h"
#pragma warning(disable:4996)

pos enemystart[5]; //레이드가 시작되는 지점, 최대 5개
char location[25] = "MAP\\";
char string[70][100];

void initMapEdit()
{
	const pos start = { 2,4 }; //맵을 출력하기 시작하는 위치. 
	system("cls");
	setColor(gray);
	gotoxy(cmdcol / 3, 1); printf("레벨 편집하기");
	setColor(yellow);
	initSquare(start, "♣");
	setColor(ivory);
	for (int i = 0; i < MAX_UD; i++)
	{
		gotoxy(start.x, start.y + i);
		for (int j = 0; j < MAX_LR; j++)printf("* ");
	}
	setColor(gray);
	gotoxy(2 * MAX_LR + 10, 7); printf("↑↓←→로 포인터를 이동할 수 있습니다.");
	gotoxy(2 * MAX_LR + 10, 11); printf("버튼을 눌러서 놓을 수 있습니다.");
	gotoxy(2 * MAX_LR + 10, 13); printf("1: 빈칸");
	gotoxy(2 * MAX_LR + 10, 15); printf("2: 돌");
	gotoxy(2 * MAX_LR + 10, 17); printf("3: 성. 단 한번만 놓을 수 있으니 신중히 놓으세요.");
	gotoxy(2 * MAX_LR + 10, 19); printf("4: 적군 출발 위치→길은 여기서부터 출발합니다.");
	gotoxy(2 * MAX_LR + 10, 20); printf("맵의 가장자리에서 시작하며 적어도 하나는 있어야 합니다. ");
	gotoxy(2 * MAX_LR + 10, 22); printf("5:성문→길은 여기에서 끝납니다.반드시 성 바로 옆에 있어야 하며");
	gotoxy(2 * MAX_LR + 10, 23); printf("적군 출발 위치의 갯수보다는 많아야 합니다.");
	gotoxy(2 * MAX_LR + 10, 25); printf("6: 맵 바깥(비어 보이지만 유닛을 놓을 수 없습니다.)");
	gotoxy(2 * MAX_LR + 10, 27); printf("9: 모든 정보를 날려버립니다.");
	gotoxy(2 * MAX_LR + 10, 29); printf("ENTER를 눌러서 완료할 수 있습니다.");//이 때 3, 4와 5이 있는지 체크한다.
}
int mapEdit(FILE* fp) //현재 wt로 열려있음. 닫을 필요는 없다.
{
	const pos start = { 2,4 }; //맵을 출력하기 시작하는 위치. 
	int map[MAX_UD][MAX_LR] = { 0 };
	int castleExist = 0, enemyExist = 0, doorExist = 0;
	pos pt = { 0,0 }; //맵의 가장 왼쪽 위 좌표는 (0,0), 한 칸당 2byte글자 하나 크기를 차지
	char ch;
	initMapEdit();
	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	printf("☜");
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

	while (1) //지우기-> 포인터 옮기기-> 그리기를 반복.
	{
		gotoxy(2 * MAX_LR + 10, 32); printf("성이%s",castleExist?"있습니다":"없습니다");
		gotoxy(2 * MAX_LR + 10, 33); printf("%d개의 적이 나올 곳이 있습니다.",enemyExist);
		gotoxy(2 * MAX_LR + 10, 34); printf("%d개의 성문이 있습니다.",doorExist);
		ch = _getch();
		gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
		if (pt.x == MAX_LR - 1)//포인터를 지우는 작업
		{
			printf("♣");
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
				printf("■");
				break;
			case CASTLE:
				setColor(sky);
				printf("城");
				break;
			case ENEMYPOS:
				setColor(sky);
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
			}//포인터 지우기 끝
		}
		if (_kbhit())//키 받기 + 처리
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
								gotoxy(2 * MAX_LR + 10, 31); printf("\"적이 나올 위치가 문 갯수보다 많습니다!\"");
								Sleep(500);
								gotoxy(2 * MAX_LR + 10, 31); printf("                                        ");
							}
						}
						else
						{
							gotoxy(2 * MAX_LR + 10, 31); printf("\"성문이 없습니다!\"");
							Sleep(500);
							gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
						}
					}
					else
					{
						gotoxy(2 * MAX_LR + 10, 31); printf("\"적이 나올 곳이 없습니다!\"");
						Sleep(500);
						gotoxy(2 * MAX_LR + 10, 31); printf("                                                  ");
					}
				}
				else
				{
					gotoxy(2 * MAX_LR + 10, 31); printf("\"성이 없습니다!\"");
					Sleep(500);
					gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
				}
				break;
			case '9': //맵 갈아 엎기
				for (int i = 0; i < MAX_UD; i++)
					for (int j = 0; j < MAX_LR; j++)
						map[i][j] = INVALID;
				castleExist = 0,enemyExist = 0, doorExist = 0;
				initMapEdit();
				break;
			default:



				if (ch >= '1' && ch <= '6' && map[pt.y][pt.x] != CASTLE && map[pt.y][pt.x] != CASTLE_INV) //문 갯수, 적 입구 갯수 업데이트
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
					case EMPTY://빈칸
						map[pt.y][pt.x] = EMPTY;
						printf("  ");
						break;
					case STONE://돌
						map[pt.y][pt.x] = STONE;
						setColor(gray);
						printf("■");
						break;
					case CASTLE://성
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
										map[pt.y + i][pt.x + j] = CASTLE_INV; //성 주변에 유닛을 놓으면 안된다. 기능적으로는 BLOCK과 같으나 성문을 체크할 때 더 쉬움.
									}
								map[pt.y][pt.x] = CASTLE;
								setColor(gray);
								for (int i = -2; i <= 2; i++)
									for (int j = -2; j <= 2; j++)
									{
										gotoxy(start.x + 2 * (pt.x + i), start.y + pt.y + j); printf("XX");
									}
								setColor(sky);
								gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("城");
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
					case ENEMYPOS://적 시작
						if (enemyExist < 5)
						{
							enemyExist++;
							map[pt.y][pt.x] = ENEMYPOS;
							setColor(sky);
							printf("■");
						}
						break;
					case CASTLE_DOOR://성문
						for (int i = -1; i <= 1; i++)
							for (int j = -1; j <= 1; j++)
							{
								if (map[pt.y + i][pt.x + j] == CASTLE_INV)
								{
									doorExist++;
									map[pt.y][pt.x] = CASTLE_DOOR; //성문 유효성 체크.
									setColor(violet);
									gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("∩");
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
		printf("☜");
	}
BREAK_GET_KEY://ENTER로 편집모드를 나갔음.
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
		printf("♣");
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
			printf("■");
			break;
		case CASTLE:
			setColor(sky);
			printf("城");
			break;
		case ENEMYPOS:
			setColor(sky);
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
		}
	}
	return 1;
}
void waveEdit(FILE* fp)
{
	const pos start = { 2,4 }; //맵을 출력하기 시작하는 위치.  테두리 아님.
	enemyT enemyarr[5][1000] = { 0 }; //[튀어나오는 위치][나오는 시간], -1이 나오면 전부 끝난것으로 친다. x4에서도 4분 10초나 저장할 수 있는 양이므로 1000이면 충분함.
	int index = 0, indexTime = 0;
	char ch;
	setColor(gray);
	for (int i = 0; i < 5; i++)
		enemyarr[i][0] = -1;
	gotoxy(cmdcol / 3 - 2, 1); printf("웨이브 편집하기");
	for (int i = 7; i < 35; i++)
	{
		gotoxy(2 * MAX_LR + 10, i); printf("                                                                    ");//설명 지우기.
	}
	gotoxy(2 * MAX_LR + 10, 7); printf("↑↓로 웨이브가 시작되는 점을 선택합니다.");
	gotoxy(2 * MAX_LR + 10, 9); printf("←→로 나오는 시간대를 설정할 수 있습니다.");
	gotoxy(2 * MAX_LR + 10, 11); printf("Q: 빈칸(0)");
	gotoxy(2 * MAX_LR + 10, 13); printf("W: 웨이브 끝내기(-1)");
	gotoxy(2 * MAX_LR + 10, 15); printf("A: 닭(1)");
	gotoxy(2 * MAX_LR + 10, 17); printf("S: 좀도둑(2)");
	gotoxy(2 * MAX_LR + 10, 19); printf("D: 달리기 선수(3)");
	gotoxy(2 * MAX_LR + 10, 21); printf("F: 해적(4)");
	gotoxy(2 * MAX_LR + 10, 23); printf("G: 방화범(5) ");
	gotoxy(2 * MAX_LR + 10, 25); printf("Z: 방패병(6)");
	gotoxy(2 * MAX_LR + 10, 27); printf("X:비행선(7)");
	gotoxy(2 * MAX_LR + 10, 29); printf("C:전차(8)");
	gotoxy(2 * MAX_LR + 10, 31); printf("V:정예병(9)");
	gotoxy(2 * MAX_LR + 10, 33); printf("B:이웃 왕(10)");
	gotoxy(2 * MAX_LR + 68, 38); printf("■");
	gotoxy(2 * MAX_LR + 8, 38); printf("■");
	while (1)//아래쪽 테이프 처리 시작
	{
		setColor(gray);
		gotoxy(2 * MAX_LR + 40, 37); printf("▼이곳 위치: %3d", indexTime);
		gotoxy(2 * MAX_LR + 10, 38);
		for (int i = -10; i < 10; i++)
			printf("%02d ", enemyarr[index][(indexTime + 1000 + i) % 1000]);
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(bloody); printf("■");
		ch = _getch();
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(sky); printf("■");
		if (_kbhit())//키 입력받기
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
	if (fp == NULL)//없을 경우 새로 만든다.
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
	printf("현재 플레이할 수 있는 맵은 다음과 같습니다.");
	for (int i = 0; 5 + 2 * i < 170 && strcmp(name[i], "") != 0; i++)
	{
		gotoxy(10 + 20 * (i / 30), 3 + (i % 30)); printf("%3d.%s", i + 1, &name[i][0]);
	}
	gotoxy(8, 34); printf("재 정렬하거나 현재 플레이 할 수 없는 맵을 불러올 수 있습니다.");
	gotoxy(8, 35); printf("1을 눌러 두 파일의 순서를 바꾸거나, 2를 눌러 맵을 불러올 수 있습니다.그 이외에는 이 화면을 나갑니다.>"); temp = _getch();
	if (temp == '1')
	{
		while (1)
		{
			gotoxy(8, 36); printf("서로 바꿀 두 파일의 번호를 입력해주세요.(0을 눌러 취소)");
			gotoxy(8, 37); printf("첫번째:"); scanf("%d", &temp);
			if (temp != 0)
			{
				gotoxy(8, 37); printf("두번째:"); scanf("%d", &temp2);
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
				printf("현재 플레이할 수 있는 맵은 다음과 같습니다.");
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
			gotoxy(8, 9 + num); printf("불러올 파일 이름은 무엇인가요?(을 눌러 나가기)");
			gotoxy(8, 10 + num); printf(">"); scanf("%s", string);
			strcat(location, string);
			strcat(location, ".level");
			fptemp = fopen(location, "rt"); //파일 유효성 체크.
			if (fptemp == NULL)
			{
				//fptemp를 닫으면 안된다.
				gotoxy(8, 11 + num); printf("그런 파일이 없는 것 같습니다.");
				Sleep(500);
				gotoxy(8, 11 + num); printf("\t\t\t\t");
				gotoxy(9, 10 + num); printf("\t\t\t\t");
			}
			else
			{
				//깨진 파일인지 아직 체크하지 않음. 반드시 추가해야 된다 나중이라도
				fclose(fptemp);
				leveladd(string);
				gotoxy(8, 11 + num); printf("파일을 추가했습니다!");
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
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("새로 만들 레벨 이름을 입력해주십시오(최대 14자, 공백 없이)");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 1); printf(">");
	while (1)
	{
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("\t\t\t\t\t\t\t\t\t\t");
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); scanf("%s", string); getchar();
		if (strlen(string) > 14)//이름이 너무 김.
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("이름이 너무 깁니다. 다시 시도해주세요.");
			strcpy(location, "MAP\\");
			Sleep(1000);
			continue;
		}
		strcat(location, string);
		strcat(location, ".level");
		fp = fopen(location, "rt");
		if (fp != NULL)//이미 같은 이름의 파일이 있음.
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("이미 같은 이름의 파일이 있습니다. 그래도 새로 만들까요?(원 파일이 지워집니다.)(Y/N)");
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 2); printf(">"); scanf("%c", &ch);
			if (ch == 'Y' || ch == 'y')
			{
				gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 3); printf("원 파일을 지웠습니다.");
				fp = fopen(location, "wt");
				Sleep(2000);
				rewritedFile = 1;
				break;
			}
			else
			{
				gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 3); printf("원래 파일을 유지했습니다.");
				strcpy(location, "MAP\\");
				Sleep(2000);
				return;
			}
		}
		else //같은 이름을 가진 파일이 없음.
		{
			//열린게 없으니 fclose를 쓰지 않는다.
			fp = fopen(location, "wt");
			break;
		}
	}
	//맵 편집화면
	if (mapEdit(fp))
	{
		//웨이브 편집화면
		waveEdit(fp);
		system("cls");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 2);
		printf("돈은 얼마나 줄까요?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);
		gotoxy(cmdcol / 2 - 10, cmdrow / 2);
		printf("성 체력은 얼마로 할까요?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 + 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);
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
		gotoxy(cmdcol / 2 - 23, cmdrow / 2); printf("%s에 저장되었습니다. 레벨 선택 화면 마지막에 배치됩니다.", location);
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
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("편집할 레벨 이름을 입력해주십시오(최대 14자, 공백 없이)");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 1); printf(">");
	while (1)
	{
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("\t\t\t\t\t\t\t\t\t\t");
		gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); scanf("%s", string); getchar();
		if (strlen(string) > 14)//이름이 너무 김.
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("이름이 너무 깁니다. 다시 시도해주세요.");
			Sleep(1000);
			continue;
		}
		strcpy(location, "MAP\\");
		strcat(location, string);
		strcat(location, ".level");
		fp = fopen(location, "rt");
		if (fp == NULL)
		{
			gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 1); printf("그런 이름을 가진 파일이 없습니다.");
			Sleep(1000);
		}
		else break;
	}
	//맵 편집화면
	if (editMapEdit(fp,wave))
	{
		//웨이브 편집화면
		editWaveEdit(fp,wave);
		system("cls");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 2);
		printf("돈은 얼마나 줄까요?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 - 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);
		gotoxy(cmdcol / 2 - 10, cmdrow / 2);
		printf("성 체력은 얼마로 할까요?");
		gotoxy(cmdcol / 2 - 10, cmdrow / 2 + 1);
		printf(">");
		scanf("%d", &num);
		fprintf(fp, "%d\n", num);

		system("cls");
		setColor(gray);
		fclose(fp);
		gotoxy(cmdcol / 2 - 23, cmdrow / 2); printf("%s에 저장되었습니다. 레벨 선택 화면 마지막에 배치됩니다.", location);
		Sleep(500);
	}
}
int editMapEdit(FILE* fp, enemyT (*enemyarr)[1000])
{
	int temp;
	const pos start = { 2,4 }; //맵을 출력하기 시작하는 위치. 
	int map[MAX_UD][MAX_LR] = { 0 };
	int castleExist = 0, enemyExist = 0, doorExist = 0;
	pos pt = { 0,0 }; //맵의 가장 왼쪽 위 좌표는 (0,0), 한 칸당 2byte글자 하나 크기를 차지
	char ch;
	initMapEdit();
	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	printf("☜");
	rewind(fp);
	for (int i = 0; i < MAX_UD; i++) //파일에서 받아오기 시작
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
				printf("■");
				break;
			case CASTLE:
				setColor(sky);
				printf("城");
				break;
			case ENEMYPOS:
				setColor(sky);
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
			}//포인터 지우기 끝
		}
	}
	for (int i = 0; i < 5; i++)//적의 시작 위치
		fscanf(fp, "%d %d", &enemystart[i].x, &enemystart[i].y);

	for (int i = 0; i < 5; i++)//적군 배열
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
	while (1) //지우기-> 포인터 옮기기-> 그리기를 반복.
	{
		gotoxy(2 * MAX_LR + 10, 32); printf("성이%s", castleExist ? "있습니다" : "없습니다");
		gotoxy(2 * MAX_LR + 10, 33); printf("%d개의 적이 나올 곳이 있습니다.", enemyExist);
		gotoxy(2 * MAX_LR + 10, 34); printf("%d개의 성문이 있습니다.", doorExist);
		ch = _getch();
		gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
		if (pt.x == MAX_LR - 1)//포인터를 지우는 작업
		{
			printf("♣");
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
				printf("■");
				break;
			case CASTLE:
				setColor(sky);
				printf("城");
				break;
			case ENEMYPOS:
				setColor(sky);
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
			}//포인터 지우기 끝
		}
		if (_kbhit())//키 받기 + 처리
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
								gotoxy(2 * MAX_LR + 10, 31); printf("\"적이 나올 위치가 문 갯수보다 많습니다!\"");
								Sleep(500);
								gotoxy(2 * MAX_LR + 10, 31); printf("                                        ");
							}
						}
						else
						{
							gotoxy(2 * MAX_LR + 10, 31); printf("\"성문이 없습니다!\"");
							Sleep(500);
							gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
						}
					}
					else
					{
						gotoxy(2 * MAX_LR + 10, 31); printf("\"적이 나올 곳이 없습니다!\"");
						Sleep(500);
						gotoxy(2 * MAX_LR + 10, 31); printf("                                                  ");
					}
				}
				else
				{
					gotoxy(2 * MAX_LR + 10, 31); printf("\"성이 없습니다!\"");
					Sleep(500);
					gotoxy(2 * MAX_LR + 10, 31); printf("                                    ");
				}
				break;
			case '9': //맵 갈아 엎기
				for (int i = 0; i < MAX_UD; i++)
					for (int j = 0; j < MAX_LR; j++)
						map[i][j] = INVALID;
				castleExist = 0, enemyExist = 0, doorExist = 0;
				initMapEdit();
				break;
			default:



				if (ch >= '1' && ch <= '6' && map[pt.y][pt.x] != CASTLE && map[pt.y][pt.x] != CASTLE_INV) //문 갯수, 적 입구 갯수 업데이트
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
					case EMPTY://빈칸
						map[pt.y][pt.x] = EMPTY;
						printf("  ");
						break;
					case STONE://돌
						map[pt.y][pt.x] = STONE;
						setColor(gray);
						printf("■");
						break;
					case CASTLE://성
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
										map[pt.y + i][pt.x + j] = CASTLE_INV; //성 주변에 유닛을 놓으면 안된다. 기능적으로는 BLOCK과 같으나 성문을 체크할 때 더 쉬움.
									}
								map[pt.y][pt.x] = CASTLE;
								setColor(gray);
								for (int i = -2; i <= 2; i++)
									for (int j = -2; j <= 2; j++)
									{
										gotoxy(start.x + 2 * (pt.x + i), start.y + pt.y + j); printf("XX");
									}
								setColor(sky);
								gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("城");
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
					case ENEMYPOS://적 시작
						if (enemyExist < 5)
						{
							enemyExist++;
							map[pt.y][pt.x] = ENEMYPOS;
							setColor(sky);
							printf("■");
						}
						break;
					case CASTLE_DOOR://성문
						for (int i = -1; i <= 1; i++)
							for (int j = -1; j <= 1; j++)
							{
								if (map[pt.y + i][pt.x + j] == CASTLE_INV)
								{
									doorExist++;
									map[pt.y][pt.x] = CASTLE_DOOR; //성문 유효성 체크.
									setColor(violet);
									gotoxy(start.x + 2 * pt.x, start.y + pt.y); printf("∩");
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
		printf("☜");
	}
BREAK_GET_KEY://ENTER로 편집모드를 나갔음.
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
		printf("♣");
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
			printf("■");
			break;
		case CASTLE:
			setColor(sky);
			printf("城");
			break;
		case ENEMYPOS:
			setColor(sky);
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
		}
	}
	return 1;
}
void editWaveEdit(FILE* fp, enemyT (*enemyarr)[1000])
{
	const pos start = { 2,4 }; //맵을 출력하기 시작하는 위치.  테두리 아님.
	int index = 0, indexTime = 0;
	char ch;
	setColor(gray);
	gotoxy(cmdcol / 3 - 2, 1); printf("웨이브 편집하기");
	for (int i = 7; i < 35; i++)
	{
		gotoxy(2 * MAX_LR + 10, i); printf("                                                                    ");//설명 지우기.
	}
	gotoxy(2 * MAX_LR + 10, 7); printf("↑↓로 웨이브가 시작되는 점을 선택합니다.");
	gotoxy(2 * MAX_LR + 10, 9); printf("←→로 나오는 시간대를 설정할 수 있습니다.");
	gotoxy(2 * MAX_LR + 10, 11); printf("Q: 빈칸(0)");
	gotoxy(2 * MAX_LR + 10, 13); printf("W: 웨이브 끝내기(-1)");
	gotoxy(2 * MAX_LR + 10, 15); printf("A: 닭(1)");
	gotoxy(2 * MAX_LR + 10, 17); printf("S: 좀도둑(2)");
	gotoxy(2 * MAX_LR + 10, 19); printf("D: 달리기 선수(3)");
	gotoxy(2 * MAX_LR + 10, 21); printf("F: 해적(4)");
	gotoxy(2 * MAX_LR + 10, 23); printf("G: 방화범(5) ");
	gotoxy(2 * MAX_LR + 10, 25); printf("Z: 방패병(6)");
	gotoxy(2 * MAX_LR + 10, 27); printf("X:비행선(7)");
	gotoxy(2 * MAX_LR + 10, 29); printf("C:전차(8)");
	gotoxy(2 * MAX_LR + 10, 31); printf("V:정예병(9)");
	gotoxy(2 * MAX_LR + 10, 33); printf("B:이웃 왕(10)");
	gotoxy(2 * MAX_LR + 68, 38); printf("■");
	gotoxy(2 * MAX_LR + 8, 38); printf("■");
	while (1)//아래쪽 테이프 처리 시작
	{
		setColor(gray);
		gotoxy(2 * MAX_LR + 40, 37); printf("▼이곳 위치: %3d", indexTime);
		gotoxy(2 * MAX_LR + 10, 38);
		for (int i = -10; i < 10; i++)
			printf("%02d ", enemyarr[index][(indexTime + 1000 + i) % 1000]);
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(bloody); printf("■");
		ch = _getch();
		gotoxy(start.x + 2 * enemystart[index].x, start.y + enemystart[index].y); setColor(sky); printf("■");
		if (_kbhit())//키 입력받기
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
//void fileEdit(FILE* fp)//게임 순서를 지정하는 파일을 만듬.
//{
//	int ch = 0;
//	int cur = 0;
//	orderT order[70] = {0};
//	char string[70][100];
//	system("cls");
//	gotoxy(cmdcol / 9 - 5, 2); printf("┌───────┐");
//	gotoxy(cmdcol / 9 - 5, 3); printf("│게임 순서 지정│");
//	gotoxy(cmdcol / 9 - 5, 4); printf("└───────┘");
//	for (int i = 0; i < cmdrow; i++)
//	{
//		gotoxy(cmdcol / 3, i); printf("│");
//		gotoxy(cmdcol / 3 * 2, i); printf("│");
//	}
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7); printf("이제 마지막 단계입니다!");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 2); printf("게임을 실행할 순서를 정해주세요");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 4); printf("반드시 길 만들기→");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 6); printf("게임 실행하기→");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 8); printf("클리어 했을 때 명령→");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 10); printf("끝 순서를 지켜주세요.");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 12); printf("위 아래 방향키로 커서를 움직일 수 있습니다.");
//	setColor(rime);
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 14); printf("1: 현재 위치에 메모하기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 16); printf("2:플레이어에게 말하기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 18); printf("3:길 파기(하나만)");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 20); printf("4: 게임 시작");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 22); printf("5: 클리어하면 여기서부터");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 24); printf("6: 실패하면 여기서부터");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 26); printf("7: 새로운 유닛 열기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 28); printf("8: 레벨 열기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 30); printf("9: 끝");
//	setColor(sky);
//	for (int i = 0; i < 70; i++)
//	{
//		gotoxy(cmdcol / 3 * (i/ 35) + 3, i % 35 + 5); printf("%d", i);
//	}
//	gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//	setColor(rime);
//	printf("▶");
//	while (1)
//	{
//		ch = _getch();
//		if (_kbhit())
//		{
//			ch = _getch();
//			if (ch == UP || ch == DOWN)
//			{
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//			}
//
//			if (ch == UP)
//				cur = (cur + 69) % 70;
//			else if (ch == DOWN)
//				cur = (cur + 1) % 70;
//			gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//			setColor(rime);
//			printf("▶");
//		}
//		else
//		{
//			if (ch == '1')
//			{
//				order[cur] = memo;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("메모:");
//				gotoxy(cmdcol / 3 * (cur / 35) + 15, cur % 35 + 5);
//				gets(string[cur]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '2')
//			{
//				order[cur] = orderTalk;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("말하기:");
//				gotoxy(cmdcol / 3 * (cur / 35) + 17, cur % 35 + 5); 
//				gets(string[cur]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '3')
//			{
//				order[cur] = mkroad;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>길파기");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '4')
//			{
//				order[cur] = gamestart;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>게임 시작");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '5')
//			{
//				order[cur] = clear;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>클리어하면 여기서부터");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '6')
//			{
//				order[cur] = fail;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>실패하면 여기서부터");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '7')
//			{
//				order[cur] = openUnit;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>유닛 언락:  번");
//				gotoxy(cmdcol / 3 * (cur / 35) + 22, cur % 35 + 5);
//				scanf("%d", &string[cur][0]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '8')
//			{
//				order[cur] = openLevel;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>레벨 언락:");
//				gets(string[cur]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '9')
//			{
//				order[cur] = endRead;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>읽기 끝");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == ENTER)
//				break;
//		}
//	}
//	for (int i = 0; i < 70;i++)
//	{
//		if (order[i] == memo || order[i] == orderTalk || order[i] == openLevel) //문자열이 필요한 명령어
//		{
//			fprintf(fp,"%d %s\n", order[i], string[i]);
//		}
//		else if (order[i] == openUnit)//숫자가 필요한 명령어
//		{
//			fprintf(fp, "%d %d\n", order[i], string[i][0]);
//		}
//		else//다른 명령이 필요 없는 명령어, 아무것도 쓰이지 않는 0도 포함.
//		{
//			fprintf(fp, "%d 0\n", order[i]);
//		}
//	}
//}
//void editFileEdit(FILE* fp)
//{
//	int ch = 0;
//	int cur = 0;
//	system("cls");
//	gotoxy(cmdcol / 9 - 5, 2); printf("┌───────┐");
//	gotoxy(cmdcol / 9 - 5, 3); printf("│게임 순서 지정│");
//	gotoxy(cmdcol / 9 - 5, 4); printf("└───────┘");
//	for (int i = 0; i < cmdrow; i++)
//	{
//		gotoxy(cmdcol / 3, i); printf("│");
//		gotoxy(cmdcol / 3 * 2, i); printf("│");
//	}
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7); printf("이제 마지막 단계입니다!");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 2); printf("게임을 실행할 순서를 정해주세요");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 4); printf("반드시 길 만들기→");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 6); printf("게임 실행하기→");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 8); printf("클리어 했을 때 명령→");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 10); printf("끝 순서를 지켜주세요.");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 12); printf("위 아래 방향키로 커서를 움직일 수 있습니다.");
//	setColor(rime);
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 14); printf("1: 현재 위치에 메모하기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 16); printf("2:플레이어에게 말하기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 18); printf("3:길 파기(하나만)");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 20); printf("4: 게임 시작");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 22); printf("5: 클리어하면 여기서부터");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 24); printf("6: 실패하면 여기서부터");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 26); printf("7: 새로운 유닛 열기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 28); printf("8: 레벨 열기");
//	gotoxy(cmdcol * 2 / 3 + 4, cmdrow / 7 + 30); printf("9: 끝");
//	setColor(sky);
//	for (int i = 0; i < 70; i++)
//	{
//		gotoxy(cmdcol / 3 * (i / 35) + 3, i % 35 + 5); printf("%d", i);
//		if (order[i] == '1')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf("메모:");
//			printf("%s", string[i]);
//		}
//		if (ch == '2')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf("말하기:");
//			printf("%s", string[i]);
//		}
//		if (ch == '3')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf(">>길파기");
//		}
//		if (ch == '4')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf(">>게임 시작");
//		}
//		if (ch == '5')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf(">>클리어하면 여기서부터");
//		}
//		if (ch == '6')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf(">>실패하면 여기서부터");
//		}
//		if (ch == '7')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf(">>유닛 언락:  %d번", string[i][0]);
//		}
//		if (ch == '8')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf(">>레벨 언락:");
//			printf("%s", string[i]);
//		}
//		if (ch == '9')
//		{
//			gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//			printf(">>읽기 끝");
//		}
//		gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//	}
//	gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//	setColor(rime);
//	printf("▶");
//	while (1)
//	{
//		ch = _getch();
//		if (_kbhit())
//		{
//			ch = _getch();
//			if (ch == UP || ch == DOWN)
//			{
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//			}
//
//			if (ch == UP)
//				cur = (cur + 69) % 70;
//			else if (ch == DOWN)
//				cur = (cur + 1) % 70;
//			gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//			setColor(rime);
//			printf("▶");
//		}
//		else
//		{
//			if (ch == '1')
//			{
//				order[cur] = memo;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("메모:");
//				gotoxy(cmdcol / 3 * (cur / 35) + 15, cur % 35 + 5);
//				gets(string[cur]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '2')
//			{
//				order[cur] = orderTalk;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("말하기:");
//				gotoxy(cmdcol / 3 * (cur / 35) + 17, cur % 35 + 5);
//				gets(string[cur]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '3')
//			{
//				order[cur] = mkroad;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>길파기");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '4')
//			{
//				order[cur] = gamestart;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>게임 시작");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '5')
//			{
//				order[cur] = clear;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>클리어하면 여기서부터");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '6')
//			{
//				order[cur] = fail;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>실패하면 여기서부터");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '7')
//			{
//				order[cur] = openUnit;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>유닛 언락:  번");
//				gotoxy(cmdcol / 3 * (cur / 35) + 22, cur % 35 + 5);
//				scanf("%d", &string[cur][0]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '8')
//			{
//				order[cur] = openLevel;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>레벨 언락:");
//				gets(string[cur]);
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == '9')
//			{
//				order[cur] = endRead;
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf("\t\t\t\t\t");
//				gotoxy(cmdcol / 3 * (cur / 35) + 10, cur % 35 + 5);
//				printf(">>읽기 끝");
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				printf("  ");
//				cur = (cur + 1) % 70;
//				gotoxy(cmdcol / 3 * (cur / 35) + 5, cur % 35 + 5);
//				setColor(rime);
//				printf("▶");
//			}
//			if (ch == ENTER)
//				break;
//		}
//	}
//	for (int i = 0; i < 70; i++)
//	{
//		if (order[i] == memo || order[i] == orderTalk || order[i] == openLevel) //문자열이 필요한 명령어
//		{
//			fprintf(fp, "%d %s\n", order[i], string[i]);
//		}
//		else if (order[i] == openUnit)//숫자가 필요한 명령어
//		{
//			fprintf(fp, "%d %d\n", order[i], string[i][0]);
//		}
//		else//다른 명령이 필요 없는 명령어
//		{
//			fprintf(fp, "%d 0\n", order[i]);
//		}
//	}
//}