#include "define.h"
int main()
{
	int select = 0;
	//play.intro
	init();//�ʱ�ȭ�� �ʿ��� �Լ����� �ʱ�ȭ.
	intro();//��Ʈ�� ������ ����մϴ�.
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