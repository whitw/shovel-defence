#include "define.h"
#pragma warning (disable:4996)
int keyNormal[8][2];
int keyCommon[6][2];
int keySpeed[5][2];

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
	FILE* fp;
	fp = fopen("config.txt", "rt");
	if (fp == NULL)
	{
		fp = fopen("config.txt", "wt");
		fprintf(fp, "113 0\n119 0\n101 0\n114 0\n97 0 \n115 0\n100 0\n102 0\n224 72\n224 75\n224 77\n224 80\n32 0\n27 0\n49 0\n50 0\n51 0\n52 0\n53 0\n");//초기 상태로 복구.
		fclose(fp);
		fp = fopen("config.txt", "rt");
	}
	for (int i = 0; i < 8; i++)
		fscanf(fp, "%d %d\n", &keyNormal[i][0], &keyNormal[i][1]);
	for (int i = 0; i < 6; i++)
		fscanf(fp, "%d %d\n", &keyCommon[i][0], &keyCommon[i][1]);
	for (int i = 0; i < 5; i++)
		fscanf(fp, "%d %d\n", &keySpeed[i][0], &keySpeed[i][1]);
	fclose(fp);

	ColorInit();
	cmdsize(cmdcol, cmdrow);
	setcursortype(NOCURSOR);
	srand((unsigned)time(NULL));
	system("title shovel-defence");
}
int getselect()
{
	int select = 0;
	int ch[2];
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
		ch[0] = _getch();
		ch[1] = 0;
		if (_kbhit())
			ch[1] = _getch();
		if ((ch[0] == keyCommon[0][0] && ch[1] == keyCommon[0][1]) || ch[1] == UP)
			select = (select + 3) % 4;
		else if ((ch[0] == keyCommon[3][0] && ch[1] == keyCommon[3][1]) || ch[1] == DOWN)
			select = (select + 1) % 4;
		else if (ch[0] == ENTER || ch[0] == SPACE || ch[0] == ESC ||(ch[0] == keyCommon[4][0] && ch[1] == keyCommon[4][1]) || (ch[0] == keyCommon[5][0] && ch[1] == keyCommon[5][1]))
		{
			if ((ch[0] == keyCommon[5][0] && ch[1] == keyCommon[5][1]) || ch[0] == ESC)
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
	int ch[2];
	FILE* fp = fopen("config.txt", "rt");
	if (fp == NULL)
	{
		fp = fopen("config.txt", "wt");
		fprintf(fp, "113 0\n119 0\n101 0\n114 0\n97 0 \n115 0\n100 0\n102 0\n224 72\n224 75\n224 77\n224 80\n32 0\n27 0\n49 0\n50 0\n51 0\n52 0\n53 0\n");//초기 상태로 복구.
		fclose(fp);
		fp = fopen("config.txt", "rt");
	}
	else
	{
		for (int i = 0; i < 8; i++)
			fscanf(fp, "%d %d\n", &keyNormal[i][0], &keyNormal[i][1]);
		for (int i = 0; i < 6; i++)
			fscanf(fp, "%d %d\n", &keyCommon[i][0], &keyCommon[i][1]);
		for (int i = 0; i < 5; i++)
			fscanf(fp, "%d %d\n", &keySpeed[i][0], &keySpeed[i][1]);
	}
	system("cls");
	gotoxy(cmdcol / 2 - 5, cmdrow / 5); printf(">>키 설정<<");
	gotoxy(cmdcol / 2 - 20, cmdrow / 2); printf("재설정할까요? (SPACE)");
	ch[0] = _getch();
	if (ch[0] == SPACE)
	{
		if (_kbhit())
			_getch();
		gotoxy(cmdcol / 2 - 20, cmdrow / 2); printf("\t\t\t\t\t");
		gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 2); printf("다음 키를 하나씩 눌러주세요. ");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("위방향 키\t\t\t");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keyCommon[0]);
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("왼쪽 방향 키\t");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keyCommon[1]);
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("오른쪽 방향 키\t");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keyCommon[2]);
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("아래 방향 키\t");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keyCommon[3]);
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("선택 키\t");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keyCommon[4]);
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("취소 키\t");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keyCommon[5]);
			for (int i = 0; i < 8; i++)
			{
				gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("유닛 키 %d\t", i + 1);
				gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keyNormal[i]);
			}
		for (int i = 0; i < 5; i++)
		{
			gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 3); printf("속도 설정 키 %d\t", i + 1);
			gotoxy(cmdcol / 2 - 20, cmdrow / 2 + 4); printf(">"); getKey(keySpeed[i]);
		}
		fclose(fp);
		fp = fopen("config.txt", "wt");
		for (int i = 0; i < 8; i++)
			fprintf(fp, "%d %d\n", keyNormal[i][0], keyNormal[i][1]);
		for (int i = 0; i < 6; i++)
			fprintf(fp, "%d %d\n", keyCommon[i][0], keyCommon[i][1]);
		for (int i = 0; i < 5; i++)
			fprintf(fp, "%d %d\n", keySpeed[i][0], keySpeed[i][1]);
		system("cls");
		gotoxy(cmdcol / 2 - 20, cmdrow / 2); printf("설정했습니다. 게임에서 확인할 수 있습니다.");
		fclose(fp);
	}
	
}
void talk(pos start,char* str)
{
	setColor(gray);
	gotoxy(start.x + 1, start.y); printf("┌─┐");
	gotoxy(start.x + 1, start.y + 1); printf("┴─┴");
	gotoxy(start.x - 1, start.y + 2); printf("(○_.._◑)");
	setColor(white);
	gotoxy(start.x + 10, start.y + 2); printf("{  %s  )", str);
	_getch();
	for (int i = 0; i < 3; i++)
	{
		gotoxy(start.x - 1, start.y + i); printf("\t\t\t");
	}
	gotoxy(start.x + 10, start.y + 2);
	for (unsigned int i = 0; i < strlen(str)+8;i++)
		printf(" ");
}
void leveladd(char* string)
{
	FILE* fptemp = fopen("levels.txt", "at");
	fptemp = fopen("levels.txt", "at");
	fseek(fptemp, 0L, SEEK_END);
	fprintf(fptemp, "%s 0\n", string);
	fclose(fptemp);
}
void getKey(int* ch)
{
	ch[0] = _getch();
	ch[1] = 0;
	if (_kbhit())
	{
		ch[1] = _getch();
	}
}