#include "define.h"
#pragma warning(disable:4996)

void game()
{
	FILE* fp;
	char filename[25], string[15];
	int map[MAX_UD][MAX_LR];//받아온 파일은 여기에 복사하고, 여기에다 플레이시 변하는 것들을 담는다. 파일 내용은 건드리지 않는다.
	int select;
	//level select
	select = printlevel();
	if (select == -1)
		return;
	fp = fopen("levels.txt", "rt");
	for (int i = 0; i < select; i++)
	{
		fscanf(fp, "%*s");
		fscanf(fp, "%*d");//읽지 않을 부분은 버린다.
	}
	strcpy(filename, "MAP\\");
	fscanf(fp, "%s", string);
	fclose(fp);
	strcat(string, ".map");
	strcat(filename, string);
	//copy MAP file to array
	fp = fopen(filename, "rt");//여기서부터 fp는 맵 파일을 저장한다.
	fclose(fp);
	while (1)//road
	{
		//make road
		//if(good)
		break;
	}
	while (1)//game
	{
		//if(clear or fail)
		break;
	}
}
int printlevel()//초기 레벨 선택 화면을 출력합니다.
{
	FILE* fp;//levels.txt에는 저장되어 있는 맵의 목록이 저장되어 있다.
	char str[20], ch;//각각 레벨 이름, 받는 키.
	int num = 0, score = 0;
	static int select = 0; //0부터 시작, 맵이 10개 이상일 경우 페이지를 넘겨 다음 맵을 볼 수 있게 한다.
	fp = fopen("levels.txt", "rt");
	if (fp == NULL)//만약 파일이 없을 경우
	{
		fp = fopen("levels.txt", "wt");
		fclose(fp);
		fp = fopen("levels.txt", "rt");
	}
	//맵의 갯수를 받는다.
	num = 0;
	while ((ch = getc(fp)) != EOF)
	{
		if (ch == '\n')
			num++;
	}
	while (1)//출력 및 업데이트한다. ENTER를 눌러서 게임 시작.
	{
		system("cls");
		//제목 출력
		setColor(rime);
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 - 2); printf("■■■■■■■■■■");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 - 1); printf("■                ■");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5); printf("■  Level Select  ■");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 + 1); printf("■                ■");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 + 2); printf("■■■■■■■■■■");
		//초기화 끝
		if (num == 0)
		{
			setColor(bloody);
			gotoxy(cmdcol / 2 - 10, cmdrow / 2); printf("저장된 파일이 없습니다.");
			Sleep(1000);
			return -1;
		}
		setColor(yellow);
		for (int i = 0; i < (num - (select / 10) * 10) && i < 10; i++)//한줄에 다섯개 아래로 두줄까지 보이게
		{
			//네모 그리기
			gotoxy(cmdcol * (i % 5 + 1) / 6 - 8, cmdrow * (i / 5 + 2) / 4 - 3); printf("■■■■■■■■■");
			for (int j = 0; j < 5; j++)
			{
				gotoxy(cmdcol * (i % 5 + 1) / 6 - 8, cmdrow * (i / 5 + 2) / 4 + j - 2); printf("■              ■");
			}
			gotoxy(cmdcol * (i % 5 + 1) / 6 - 8, cmdrow * (i / 5 + 2) / 4 + 3); printf("■■■■■■■■■");
		}
		setColor(gray);
		//맵 이름 및 별 갯수 기록
		fseek(fp, 0L, SEEK_SET);
		for (int i = 0; i < (select / 10) * 10; i++)
		{
			fscanf(fp, "%*s");
			fscanf(fp, "%*d");//읽지 않을 부분은 버린다.
		}
		for (int i = 0; i < num - (10 * (select / 10)) && i < 10; i++)
		{
			fscanf(fp, "%s", str);
			fscanf(fp, "%d", &score);
			getc(fp);
			setColor(gray);
			gotoxy(cmdcol * (i % 5 + 1) / 6 + 1 - strlen(str) / 2, cmdrow * (i / 5 + 2) / 4 - 1); printf(str);
			setColor(yellow);
			for (int j = 0; j < 3; j++)
			{
				gotoxy(cmdcol * (i % 5 + 1) / 6 - 4 + 4 * j, cmdrow * (i / 5 + 2) / 4 + 1);
				if (j < score)
					printf("★");
				else printf("☆");
			}
		}
		//선택한 파일을 표시한다.
		setColor(bloody);
		gotoxy(cmdcol * (select % 5 + 1) / 6 - 8, cmdrow * ((select % 10) / 5 + 2) / 4 - 3); printf("■■■■■■■■■");
		for (int j = 0; j < 5; j++)
		{
			gotoxy(cmdcol * (select % 5 + 1) / 6 - 8, cmdrow * ((select % 10) / 5 + 2) / 4 + j - 2); printf("■");
			gotoxy(cmdcol * (select % 5 + 1) / 6 + 8, cmdrow * ((select % 10) / 5 + 2) / 4 + j - 2); printf("■");
		}
		gotoxy(cmdcol * (select % 5 + 1) / 6 - 8, cmdrow * ((select % 10) / 5 + 2) / 4 + 3); printf("■■■■■■■■■");
		//아래에 현 위치/최대 위치 표시
		setColor(gray);
		gotoxy((cmdcol / 2 - 5), cmdrow * 4 / 5 + 3); printf("◀%02d/%02d▶", select / 10 + 1, (num - 1) / 10 + 1);
		ch = _getch();
		if (_kbhit())
		{
			ch = _getch();
			switch (ch)
			{
			case UP:
				select = (select + num - 5) % num;
				break;
			case LEFT:
				select = (select + num - 1) % num;
				break;
			case DOWN:
				if (select + 5 >= num)
					select %= 5;
				else select += 5;
				break;
			case RIGHT:
				select = (select + 1) % num;
				break;
			}
		}
		else switch (ch)
		{
		case ENTER:
			fclose(fp);
			return select;
		case ESC:
			fclose(fp);
			return -1;
		case 'w':
		case 'W':
			select = (select + num - 5) % num;
			break;
		case 'a':
		case 'A':
			select = (select + num - 1) % num;
			break;
		case 's':
		case 'S':
			if (select + 5 >= num)
				select %= 5;
			else select += 5;
			break;
		case 'd':
		case 'D':
			select = (select + 1) % num;
			break;
		}
	}
}