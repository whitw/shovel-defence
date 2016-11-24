#include "define.h"
#pragma warning(disable:4996)

pos enemystart[5]; //레이드가 시작되는 지점, 최대 4개

void levelmaker()
{
	int num;
	char string[15] = "",location[25] = "MAP\\";
	char name[100][15] = { "" };
	FILE* fp = NULL;
	system("cls");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("숫자를 입력해 주세요.");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 2); printf("1. 파일 정렬하기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 3); printf("2. 새 파일 만들기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 4); printf("3. 파일 편집하기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 5); printf("4. 나가기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 6); printf(">"); scanf("%d", &num);
	system("cls");
	if (num == 1) //맵 파일 정렬
	{
		fp = fopen("levels.txt", "rt");
		if (fp == NULL)//없을 경우 새로 만든다.
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
		printf("현재 플레이할 수 있는 맵은 다음과 같습니다.");
		for (int i = 0; strcmp(name[i],"") != 0; i++)
		{
			gotoxy(8,5+2 * i);printf("%3d.%s", i+1,&name[i][0]);
		}
		system("pause");
		fclose(fp);
	}
	else if (num == 2) //새 파일 만들기
	{
		mkdir("MAP");
		gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("새로 만들 레벨 이름을 입력해주십시오(최대 14자).");
		gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 1); printf(">");
		while(1)
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
				gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 2); printf(">"); scanf("%d", &num);
				if (num == 'Y' || num == 'y')
				{
					fp = fopen(location, "wt");
					break;
				}
				else
				{
					gotoxy(cmdcol / 2 - 14, cmdrow / 2 + 3); printf("원래 파일을 유지했습니다.");
					strcpy(location, "MAP\\");
					Sleep(2000);
					continue;
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
			scanf("%d",&num);
			fprintf(fp, "%d\n", num);
			gotoxy(cmdcol / 2 - 10, cmdrow / 2);
			printf("성 체력은 얼마로 할까요?");
			gotoxy(cmdcol / 2 - 10, cmdrow / 2 + 1);
			printf(">");
			scanf("%d", &num);
			fprintf(fp, "%d\n", num);
			//이벤트 편집 장면
			//

			system("cls");
			setColor(gray);
			fclose(fp);
			fp = fopen("levels.txt", "at");
			fprintf(fp, "%s 0\n", string);
			fclose(fp);
			system("cls");
			gotoxy(cmdcol / 2 - 23, cmdrow / 2); printf("%s에 저장되었습니다. 레벨 선택 화면 마지막에 배치됩니다.", location);
			Sleep(500);
		}
	}
	else if (num == 3) //파일 편집
	{

	}
	//그 이외의 경우에는 숫자에 상관 없이 나가기
}

void initMapEdit()
{
	const pos start = { 2,4 }; //맵을 출력하기 시작하는 위치. 
	system("cls");
	setColor(gray);
	gotoxy(cmdcol / 3, 1); printf("레벨 편집하기");
	initSquare(start, "♣");
	gotoxy(2 * MAX_LR + 10, 7); printf("↑↓←→로 포인터를 이동할 수 있습니다.");
	gotoxy(2 * MAX_LR + 10, 11); printf("다음 버튼을 눌러서 포인터 위치에 이와 같은 것들을 만들 수 있습니다.");
	gotoxy(2 * MAX_LR + 10, 15); printf("1: 빈칸");
	gotoxy(2 * MAX_LR + 10, 17); printf("2: 돌");
	gotoxy(2 * MAX_LR + 10, 19); printf("3: 성. 단 한번만 놓을 수 있으니 신중히 놓으세요.");
	gotoxy(2 * MAX_LR + 10, 21); printf("4: 적군 출발 위치→길은 여기서부터 출발합니다.");
	gotoxy(2 * MAX_LR + 10, 22); printf("맵의 가장자리에서 시작하며 적어도 하나는 있어야 합니다. ");
	gotoxy(2 * MAX_LR + 10, 24); printf("5:성문→길은 여기에서 끝납니다.반드시 성 바로 옆에 있어야 하며");
	gotoxy(2 * MAX_LR + 10, 25); printf("적군 출발 위치의 갯수보다는 많아야 합니다.");
	gotoxy(2 * MAX_LR + 10, 27); printf("9: 모든 정보를 날려버립니다.");
	gotoxy(2 * MAX_LR + 10, 29); printf("ENTER를 눌러서 완료할 수 있습니다.");//이 때 3, 4와 5이 있는지 체크한다.
}


int mapEdit(FILE* fp) //현재 wt로 열려있음. 닫을 필요는 없다.
{
	const pos start = { 2,4 }; //맵을 출력하기 시작하는 위치. 
	int map[MAX_LR][MAX_UD] = {0};
	int castleExist = 0,enemyExist = 0,doorExist = 0;
	pos pt = {0,0}; //맵의 가장 왼쪽 위 좌표는 (0,0), 한 칸당 2byte글자 하나 크기를 차지
	char ch;
	initMapEdit();
	gotoxy(start.x + 2 * (pt.x + 1), start.y + pt.y);
	printf("☜");
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
			switch (map[pt.x + 1][pt.y])
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
				for (int i = 0; i < MAX_LR; i++)
					for (int j = 0; j < MAX_UD; j++)
						map[i][j] = EMPTY;
				castleExist = 0,enemyExist = 0, doorExist = 0;
				initMapEdit();
				break;
			default:



				if (ch >= '1' && ch <= '5' && map[pt.x][pt.y] != CASTLE && map[pt.x][pt.y] != CASTLE_INV) //문 갯수, 적 입구 갯수 업데이트
				{
					if (map[pt.x][pt.y] == CASTLE_DOOR)
						doorExist--;
					if (map[pt.x][pt.y] == ENEMYPOS)
						enemyExist--;
					gotoxy(start.x + 2 * pt.x, start.y + pt.y);
					switch (ch - '0' - 1)
					{
					case EMPTY://빈칸
						map[pt.x][pt.y] = EMPTY;
						printf("  ");
						break;
					case STONE://돌
						map[pt.x][pt.y] = STONE;
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
										if (map[pt.x+i][pt.y+j] == CASTLE_DOOR)
											doorExist--;
										if (map[pt.x+i][pt.y+j] == ENEMYPOS)
											enemyExist--;
										map[pt.x + i][pt.y + j] = CASTLE_INV; //성 주변에 유닛을 놓으면 안된다. 기능적으로는 BLOCK과 같으나 성문을 체크할 때 더 쉬움.
									}
								map[pt.x][pt.y] = CASTLE;
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
							if (map[pt.x][pt.y] == CASTLE_DOOR)
								doorExist++;
							if (map[pt.x][pt.y] == ENEMYPOS)
								enemyExist++;
						}
						break;
					case ENEMYPOS://적 시작
						if (enemyExist < 5)
						{
							enemyExist++;
							map[pt.x][pt.y] = ENEMYPOS;
							setColor(sky);
							printf("■");
						}
						break;
					case CASTLE_DOOR://성문
						for (int i = -1; i <= 1; i++)
							for (int j = -1; j <= 1; j++)
							{
								if (map[pt.x + i][pt.y + j] == CASTLE_INV)
								{
									doorExist++;
									map[pt.x][pt.y] = CASTLE_DOOR; //성문 유효성 체크.
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
BREAK_GET_KEY://ESC로 편집모드를 나갔음.
	enemyExist = 0;
	for (int i = 0;i < MAX_LR; i++)
	{
		for (int j = 0; j < MAX_UD; j++)
		{
			fprintf(fp, "%d ", map[j][i]); //솔직히 말하자면 실수한 부분 맞음.
			if (map[j][i] == ENEMYPOS)// 어느순간부턴가 앞뒤, 위아래가 순서가 바뀌었는데
			{
				enemystart[enemyExist].x = j;// 건들기는 너무 커졌으니 나중에 버그 픽스때 시간 나면 고친다.
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
		switch (map[pt.x + 1][pt.y])
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
	for (int i = 0; i < 5;i++)
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
	while (1)
	{
		setColor(gray);//아래쪽 테이프
	gotoxy(2 * MAX_LR + 40, 37); printf("▼이곳 위치: %3d", indexTime);
		gotoxy(2 * MAX_LR + 10, 38);
		for (int i = -10; i < 10; i++)
			printf("%02d ", enemyarr[index][(indexTime +1000 + i) % 1000]);
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