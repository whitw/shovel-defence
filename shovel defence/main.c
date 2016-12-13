#include "define.h"
#include "game.h"
#include "levelmaker.h"
int main()
{
	int select = 0;
	pos start = { 8,4 };
	//play.intro
	init();//초기화가 필요한 함수들의 초기화.
	intro();//인트로 영상을 출력합니다.
	while (1)
	{
		system("cls");
		printmain();
		select = getselect();
		if (select == 0)
			game();
		else if (select == 1)
			levelmaker();
		else if (select == 2)
			option();
		else break;
	}
	return 0;
}

void levelmaker()
{
	int num;
	system("cls");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("숫자를 입력해 주세요.");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 2); printf("1. 파일 정렬하기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 3); printf("2. 새 파일 만들기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 4); printf("3. 파일 편집하기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 5); printf("4. 나가기");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 6); printf(">"); scanf("%d", &num);
	if (_kbhit())_getch();
	system("cls");
	if (num == 1) //맵 파일 정렬
		sortMapFile();
	else if (num == 2) //새 파일 만들기
	{
		newMapFile();
	}
	else if (num == 3) //파일 편집
	{
		editMapFile();
	}
	//그 이외의 경우에는 숫자에 상관 없이 나가기
}
