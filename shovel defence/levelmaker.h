#pragma once

#include <stdio.h>
#include "define.h"

void sortMapFile();
void newMapFile();
int mapEdit(FILE* fp);
void initMapEdit();
void waveEdit(FILE* fp);
/*void fileEdit(FILE* fp);*///���� ������ �����ϴ� ������ ����.
void leveladd(char* str);

void editMapFile();
int editMapEdit(FILE* fp, enemyT (*var)[1000]);
void editWaveEdit(FILE* fp, enemyT (*var)[1000]);
//void editFileEdit(FILE* fp);//�̾��ؤФ�