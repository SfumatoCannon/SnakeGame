#pragma once

#include <Windows.h>

#define MAXX 20
#define MAXY 15
#define LEFTDIS 32
#define RIGHTDIS 256
#define UPDIS 32
#define DOWNDIS 32
#define FUCKX 20 //�Ҹĺ���Ը�
#define FUCKY 23 //�Ҹĺ���Ը�
#define WINDOWX 200
#define WINDOWY 50
#define WINDOWWIDTH (MAXX*32+LEFTDIS+RIGHTDIS) //�������Ը�һ��
#define WINDOWHEIGHT (MAXY*32+UPDIS+DOWNDIS) //�������Ը�һ��
#define WINDOWTRUEWIDTH (WINDOWWIDTH+FUCKX) //�Ҹĺ���Ը�
#define WINDOWTRUEHEIGHT (WINDOWHEIGHT+FUCKY) //�Ҹĺ���Ը�
extern HDC WindowHDC;