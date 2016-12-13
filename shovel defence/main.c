#include "define.h"
#include "game.h"
#include "levelmaker.h"
int main()
{
	int select = 0;
	pos start = { 8,4 };
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

void levelmaker()
{
	int num;
	system("cls");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2); printf("���ڸ� �Է��� �ּ���.");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 2); printf("1. ���� �����ϱ�");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 3); printf("2. �� ���� �����");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 4); printf("3. ���� �����ϱ�");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 5); printf("4. ������");
	gotoxy(cmdcol / 2 - 15, cmdrow / 2 + 6); printf(">"); scanf("%d", &num);
	if (_kbhit())_getch();
	system("cls");
	if (num == 1) //�� ���� ����
		sortMapFile();
	else if (num == 2) //�� ���� �����
	{
		newMapFile();
	}
	else if (num == 3) //���� ����
	{
		editMapFile();
	}
	//�� �̿��� ��쿡�� ���ڿ� ��� ���� ������
}
