#include "define.h"
int main()
{
	int select = 0;
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