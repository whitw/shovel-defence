#pragma once

#include <stdio.h>
#include "define.h"

void sortMapFile();
void newMapFile();
int mapEdit(FILE* fp);
void initMapEdit();
void waveEdit(FILE* fp);
void fileEdit(FILE* fp);//게임 순서를 지정하는 파일을 만듬.
void leveladd(char* str);