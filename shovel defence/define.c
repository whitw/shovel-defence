#include "define.h"
#pragma warning (disable:4996)

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
}
void intro()//인트로 영상을 출력합니다.
{

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

void option()
{
}