#include "define.h"
#pragma warning(disable:4996)

extern int keyNormal[8][2]; // qwertasdf
extern int keyCommon[6][2]; // w a s d select escape
extern int keySpeed[5][2];// speedUp speedDown

int printlevel()//초기 레벨 선택 화면을 출력합니다.
{
	FILE* fp;//levels.txt에는 저장되어 있는 맵의 목록이 저장되어 있다.
	char str[20],ch;//레벨 이름.
	int key[2];
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
	if (num > 1000)
		num = 1000;//1000개까지만 표시한다. 다른 애들도 이 조건을 넣어주면 다른 부분에서도 1000개까지만 검사하고 나머지는 날려버려도 괜찮다.
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
		key[0] = _getch();
		key[1] = 0;
		if (_kbhit())
			key[1] = _getch();
		if (key[0] == keyCommon[0][0] && key[1] == keyCommon[0][1])
			select = (select + num - 5) % num;
		if (key[0] == keyCommon[1][0] && key[1] == keyCommon[1][1])
			select = (select + num - 1) % num;
		if (key[0] == keyCommon[2][0] && key[1] == keyCommon[2][1])
			select = (select + 1) % num;
		if (key[0] == keyCommon[3][0] && key[1] == keyCommon[3][1]){
			if (select + 5 >= num)
				select %= 5;
			else select += 5;
		}
		if (key[0] == keyCommon[4][0] && key[1] == keyCommon[4][1]){
			fclose(fp);
			return select;
		}
		if(key[0] == keyCommon[5][0] && key[1] == keyCommon[5][1]){
			fclose(fp);
			return -1;
		}
	}
}
void printgame()
{
	pos start = { 8, 6 };
	gotoxy(start.x + MAX_LR, start.y - 4);
	setColor(yellow);
	for (int i = 0; i < MAX_LR / 2 + 1; i++)
		printf("●");
	gotoxy(start.x + MAX_LR, start.y - 3); printf("●게임 속도:");
	gotoxy(start.x + 2 * MAX_LR, start.y - 3); printf("●");
	gotoxy(start.x + MAX_LR, start.y - 2); printf("●돈:");
	gotoxy(start.x + 2 * MAX_LR - 4, start.y - 2); printf("코인●");
	setColor(yellow);
	gotoxy(start.x + 2 * MAX_LR + 2, cmdrow * 2 / 3); for (int i = 0; i < cmdcol / 4; i++)printf("●");
}
