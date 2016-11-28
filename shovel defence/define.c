#include "define.h"
#pragma warning (disable:4996)
int keyType = 0;
char keyNormal[8][2] = { {'q',0} ,{ 'w',0 },{ 'e',0 },{ 'r',0 },{ 'a',0 },{ 's',0 },{ 'd',0 },{ 'f',0 } };
char keyShort[2][2] = { {'a',0},{'d',0} };
char keyCommon[6][2] = { { SPACE,0 },{ ESC,0 },{ -32,UP },{ -32,LEFT },{ -32,RIGHT },{ -32,DOWN } };
char keySpeed[3][2] = { {'1',0} ,{ '2',0 } ,{ '3',0 } };

void gotoxy(int x, int y)
{
	COORD Pos = { x , y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void setcursortype(CURSOR_TYPE c)
{
	CONSOLE_CURSOR_INFO CurInfo;
	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
void init()//초기화가 필요한 함수들의 초기화.
{
	ColorInit();
	cmdsize(cmdcol, cmdrow);
	setcursortype(NOCURSOR);
	srand((unsigned)time(NULL));
}
void intro()//인트로 영상을 출력합니다.
{
	//그딴거 없다
	//나중에 시간 나면
}
int getselect()
{
	int select = 0;
	char ch;
	while (1)//get key and select menu
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				gotoxy(cmdcol / 2 - 16, cmdrow * (i + 5) / 10 + j);
				printf("          ");
			}
		}
		setColor(ivory);
		gotoxy(cmdcol / 2 - 16, cmdrow * (select + 5) / 10 - 1);
		printf("■");
		gotoxy(cmdcol / 2 - 16, cmdrow * (select + 5) / 10);
		printf("■■");
		gotoxy(cmdcol / 2 - 16, cmdrow * (select + 5) / 10 + 1);
		printf("■");
		setColor(gray);
		gotoxy(cmdcol / 2 - 12, cmdrow * (select + 5) / 10 - 1);
		printf("■■");
		gotoxy(cmdcol / 2 - 12, cmdrow * (select + 5) / 10);
		printf("■■■");
		gotoxy(cmdcol / 2 - 12, cmdrow * (select + 5) / 10 + 1);
		printf("■■");
		ch = _getch();
		if (ch == -32 || ch == 0)
		{
			ch = _getch();
			switch (ch)
			{
			case UP:
				select = (select + 3) % 4;
				break;
			case DOWN:
				select = (select + 1) % 4;
				break;
			}
		}
		else if (ch == ENTER || ch == SPACE || ch == ESC)
		{
			if (ch == ESC)
				select = 3;
			for (int i = 0; i < 5; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					gotoxy(cmdcol / 2 - 16, cmdrow * (i + 5) / 10 + j);
					printf("          ");
				}
			}
			setColor(ivory);
			gotoxy(cmdcol / 2 - 14, cmdrow * (select + 5) / 10 - 1);
			printf("■");
			gotoxy(cmdcol / 2 - 14, cmdrow * (select + 5) / 10);
			printf("■■");
			gotoxy(cmdcol / 2 - 14, cmdrow * (select + 5) / 10 + 1);
			printf("■");
			setColor(gray);
			gotoxy(cmdcol / 2 - 10, cmdrow * (select + 5) / 10 - 1);
			printf("■■");
			gotoxy(cmdcol / 2 - 10, cmdrow * (select + 5) / 10);
			printf("■■■");
			gotoxy(cmdcol / 2 - 10, cmdrow * (select + 5) / 10 + 1);
			printf("■■");
			Sleep(300);
			break;
		}
	}
	return select;
}
void printmain()//초기 메인 화면을 출력합니다.
{
	setColor(green);
	gotoxy(cmdcol / 2 - 22, cmdrow * 2 / 10);printf("♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣");
	gotoxy(cmdcol / 2 - 22, cmdrow * 2 / 10 + 1);printf("♣                                        ♣");
	gotoxy(cmdcol / 2 - 22, cmdrow * 2 / 10 + 2);printf("♣                                        ♣");
	gotoxy(cmdcol / 2 - 22, cmdrow * 2 / 10 + 3);printf("♣                                        ♣");
	gotoxy(cmdcol / 2 - 22, cmdrow * 2 / 10 + 4);printf("♣                                        ♣");
	gotoxy(cmdcol / 2 - 22, cmdrow * 2 / 10 + 5);printf("♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣♣");
	setColor(lyellow);
	gotoxy(cmdcol / 2 - 18, cmdrow * 2 / 10 + 1);printf("삽질");
	gotoxy(cmdcol  / 2 - 3, cmdrow * 2 / 10 + 3);printf("디펜스");
	setColor(ivory);
	gotoxy(cmdcol / 2 + 10, cmdrow * 2 / 10 + 1);printf("■■■");
	gotoxy(cmdcol / 2 + 12, cmdrow * 2 / 10 + 2);printf("■");
	gotoxy(cmdcol / 2 + 12, cmdrow * 2 / 10 + 3);printf("■");
	setColor(gray);
	gotoxy(cmdcol / 2 + 10, cmdrow * 2 / 10 + 4);printf("■■■");
	gotoxy(cmdcol / 2 + 10, cmdrow * 2 / 10 + 5);printf("■■■");
	gotoxy(cmdcol / 2 + 12, cmdrow * 2 / 10 + 6);printf("■");
	setColor(sky);
	gotoxy(cmdcol / 2 - 4, cmdrow * 5 / 10);printf("게임 시작");
	gotoxy(cmdcol / 2 - 4, cmdrow * 6 / 10);printf("맵 만들기");
	gotoxy(cmdcol / 2 - 2, cmdrow * 7 / 10);printf("설정");
	gotoxy(cmdcol / 2 - 3, cmdrow * 8 / 10); printf("나가기");
}
void initSquare(pos p, char* str)
{
	pos start = p;
	gotoxy(start.x - 2, start.y - 1);
	for (int i = 0; i < MAX_LR + 2; i++)printf("%s",str);
	for (int i = 0; i <= MAX_UD; i++)
	{
		gotoxy(start.x - 2, start.y - 1 + i); printf("%s",str);
		gotoxy(start.x + MAX_LR * 2, start.y - 1 + i); printf("%s",str);
	}
	gotoxy(start.x - 2, MAX_UD + start.y);
	for (int i = 0; i < MAX_LR + 2; i++)printf("%s",str);
}
void option()
{
	FILE* fpR = fopen("config.txt", "rt");
	FILE* fpW = NULL;
	char ch;
	if (fpR == NULL)
	{
		fpW = fopen("config.txt", "wt");
		fpR = fopen("config.txt", "rt");
		fprintf(fpW, "0 113\n0 119\n0 101\n0 114\n0 97\n0 115\n0 100\n0 102\n0 97\n0 100\n0 32\n0 27\n-32 72\n-32 75\n-32 77\n-32 80\n0 49\n0 50\n0 51\n");//초기 상태로 복구.
	}
	else
	{
		for (int i = 0; i < 8; i++)
			fscanf(fpR, "%c %c\n", &keyNormal[i][0], &keyNormal[i][1]);
		for (int i = 0; i < 2; i++)
			fscanf(fpR, "%c %c\n", &keyShort[i][0], &keyShort[i][1]);
		for (int i = 0; i < 6; i++)
			fscanf(fpR, "%c %c\n", &keyCommon[i][0], &keyCommon[i][1]);
		for (int i = 0; i < 3; i++)
			fscanf(fpR, "%c %c\n", &keySpeed[i][0], &keySpeed[i][1]);
	}
	printf("키 설정/ ");
}

void talk(pos start,char* str)
{
	setColor(gray);
	gotoxy(start.x, start.y); printf("┌─┐");
	gotoxy(start.x, start.y + 1); printf("┴─┴");
	gotoxy(start.x - 1, start.y + 2); printf("(○_.._◑)");
	setColor(white);
	gotoxy(start.x + 10, start.y + 2); printf("{%s)", str);
	Sleep(500);
}

void leveladd(char* string)
{
	FILE* fptemp = fopen("levels.txt", "at");
	fptemp = fopen("levels.txt", "at");
	fseek(fptemp, 0L, SEEK_END);
	fprintf(fptemp, "%s 0\n", string);
	fclose(fptemp);
}