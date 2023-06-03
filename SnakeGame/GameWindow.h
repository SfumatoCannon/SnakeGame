#pragma once

#include <Windows.h>

#define MAXX 20
#define MAXY 15
#define LEFTDIS 32
#define RIGHTDIS 256
#define UPDIS 32
#define DOWNDIS 32
#define FUCKX 20 //乱改后果自负
#define FUCKY 23 //乱改后果自负
#define WINDOWX 200
#define WINDOWY 50
#define WINDOWWIDTH (MAXX*32+LEFTDIS+RIGHTDIS) //可以稍稍改一下
#define WINDOWHEIGHT (MAXY*32+UPDIS+DOWNDIS) //可以稍稍改一下
#define WINDOWTRUEWIDTH (WINDOWWIDTH+FUCKX) //乱改后果自负
#define WINDOWTRUEHEIGHT (WINDOWHEIGHT+FUCKY) //乱改后果自负
extern HDC WindowHDC;