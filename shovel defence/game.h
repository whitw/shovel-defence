#pragma once

#include <stdio.h>
#include "define.h"

void printmap(FILE* fp);
void printgame(); //게임 화면 초기화.
void readfile(FILE* fp); //파일을 읽으면서 게임 순서를 지정하고 실행한다.
void talk(pos position, char* str, int time);
void makeroad(); //길파기. 파일에서 읽어온 배열을 사용하되 파일은 건들면 안된다.
int startGame(); //역시 배열만 이용하고 파일은 건들지 않는다. 반환값은 성의 남은 체력. 0이면 게임오버고 체력이 낮으면 별도 적은 방식.
void clearCur(pos pt, int block);
int updateRoad(dir direction);
void showSelectedRoad();
