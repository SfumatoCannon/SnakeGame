#pragma once

#define FILEOPENERROR 4001
#define FILECLOSEERROR 4002

#include "framework.h"

extern long long Master[10];
extern long long totnum;

void ReadScore();
void WriteScoreToFile();
void UpdateScore(int newscore);
