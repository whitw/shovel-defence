#include "define.h"
#pragma warning(disable:4996)

int printlevel()//�ʱ� ���� ���� ȭ���� ����մϴ�.
{
	FILE* fp;//levels.txt���� ����Ǿ� �ִ� ���� ����� ����Ǿ� �ִ�.
	char str[20], ch;//���� ���� �̸�, �޴� Ű.
	int num = 0, score = 0;
	static int select = 0; //0���� ����, ���� 10�� �̻��� ��� �������� �Ѱ� ���� ���� �� �� �ְ� �Ѵ�.
	fp = fopen("levels.txt", "rt");
	if (fp == NULL)//���� ������ ���� ���
	{
		fp = fopen("levels.txt", "wt");
		fclose(fp);
		fp = fopen("levels.txt", "rt");
	}
	//���� ������ �޴´�.
	num = 0;
	while ((ch = getc(fp)) != EOF)
	{
		if (ch == '\n')
			num++;
	}
	if (num > 1000)
		num = 1000;//1000�������� ǥ���Ѵ�. �ٸ� �ֵ鵵 �� ������ �־��ָ� �ٸ� �κп����� 1000�������� �˻��ϰ� �������� ���������� ������.
	while (1)//��� �� ������Ʈ�Ѵ�. ENTER�� ������ ���� ����.
	{
		system("cls");
		//���� ���
		setColor(rime);
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 - 2); printf("�����������");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 - 1); printf("��                ��");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5); printf("��  Level Select  ��");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 + 1); printf("��                ��");
		gotoxy((cmdcol - strlen("  Level Select  ")) / 2, cmdrow / 5 + 2); printf("�����������");
		//�ʱ�ȭ ��
		if (num == 0)
		{
			setColor(bloody);
			gotoxy(cmdcol / 2 - 10, cmdrow / 2); printf("����� ������ �����ϴ�.");
			Sleep(1000);
			return -1;
		}
		setColor(yellow);
		for (int i = 0; i < (num - (select / 10) * 10) && i < 10; i++)//���ٿ� �ټ��� �Ʒ��� ���ٱ��� ���̰�
		{
			//�׸� �׸���
			gotoxy(cmdcol * (i % 5 + 1) / 6 - 8, cmdrow * (i / 5 + 2) / 4 - 3); printf("����������");
			for (int j = 0; j < 5; j++)
			{
				gotoxy(cmdcol * (i % 5 + 1) / 6 - 8, cmdrow * (i / 5 + 2) / 4 + j - 2); printf("��              ��");
			}
			gotoxy(cmdcol * (i % 5 + 1) / 6 - 8, cmdrow * (i / 5 + 2) / 4 + 3); printf("����������");
		}
		setColor(gray);
		//�� �̸� �� �� ���� ���
		fseek(fp, 0L, SEEK_SET);
		for (int i = 0; i < (select / 10) * 10; i++)
		{
			fscanf(fp, "%*s");
			fscanf(fp, "%*d");//���� ���� �κ��� ������.
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
					printf("��");
				else printf("��");
			}
		}
		//������ ������ ǥ���Ѵ�.
		setColor(bloody);
		gotoxy(cmdcol * (select % 5 + 1) / 6 - 8, cmdrow * ((select % 10) / 5 + 2) / 4 - 3); printf("����������");
		for (int j = 0; j < 5; j++)
		{
			gotoxy(cmdcol * (select % 5 + 1) / 6 - 8, cmdrow * ((select % 10) / 5 + 2) / 4 + j - 2); printf("��");
			gotoxy(cmdcol * (select % 5 + 1) / 6 + 8, cmdrow * ((select % 10) / 5 + 2) / 4 + j - 2); printf("��");
		}
		gotoxy(cmdcol * (select % 5 + 1) / 6 - 8, cmdrow * ((select % 10) / 5 + 2) / 4 + 3); printf("����������");
		//�Ʒ��� �� ��ġ/�ִ� ��ġ ǥ��
		setColor(gray);
		gotoxy((cmdcol / 2 - 5), cmdrow * 4 / 5 + 3); printf("��%02d/%02d��", select / 10 + 1, (num - 1) / 10 + 1);
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
void printgame()
{
	pos start = { 8, 6 };
	gotoxy(start.x + MAX_LR, start.y - 4);
	setColor(yellow);
	for (int i = 0; i < MAX_LR / 2 + 1; i++)
		printf("��");
	gotoxy(start.x + MAX_LR, start.y - 3); printf("�ܰ��� �ӵ�:");
	gotoxy(start.x + 2 * MAX_LR, start.y - 3); printf("��");
	gotoxy(start.x + MAX_LR, start.y - 2); printf("�ܵ�:");
	gotoxy(start.x + 2 * MAX_LR - 4, start.y - 2); printf("���Ρ�");
	setColor(yellow);
	gotoxy(start.x + 2 * MAX_LR + 2, cmdrow * 2 / 3); for (int i = 0; i < cmdcol / 4; i++)printf("��");
	setColor(ivory);
	for (int i = 0; i < 5; i++)
		for (int j = 1; j <= 2; j++)
		{
			gotoxy(cmdcol * (i + 7) / 12 - 6, cmdrow * 2 / 3 + 5 * j - 3); printf("��%2d��", 5 * (j-1) + i + 1);
			gotoxy(cmdcol * (i + 7) / 12 - 6, cmdrow * 2 / 3 + 5 * j - 2); printf("������");
			gotoxy(cmdcol * (i + 7) / 12 - 6, cmdrow * 2 / 3 + 5 * j - 1); printf("������");
		}
}
