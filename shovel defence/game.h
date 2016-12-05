#pragma once

#include <stdio.h>
#include "define.h"

void printmap(FILE* fp);
void printgame(); //���� ȭ�� �ʱ�ȭ.
void readfile(FILE* fp); //������ �����鼭 ���� ������ �����ϰ� �����Ѵ�.
void talk(pos position, char* str, int time);
void makeroad(); //���ı�. ���Ͽ��� �о�� �迭�� ����ϵ� ������ �ǵ�� �ȵȴ�.
int startGame(); //���� �迭�� �̿��ϰ� ������ �ǵ��� �ʴ´�. ��ȯ���� ���� ���� ü��. 0�̸� ���ӿ����� ü���� ������ ���� ���� ���.
void clearCur(pos pt, int block);
int updateRoad(dir direction);
void showSelectedRoad();
