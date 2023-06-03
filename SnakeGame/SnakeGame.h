#pragma once

#include <Windows.h>
#include <queue>
#include "GameWindow.h"
#include "Resource.h"

#define CATCHFOOD 0x8003
#define GAMEOVER 0x8002
#define NORMAL 0x8001
#define SLEEPTIME 200

struct STRUCTXY
{
	SHORT x;
	SHORT y;
};

enum State
{
	NoState = 0, StateL = 1, StateR = 2, StateU = 4, StateD = 8
};

enum Block
{
	Nothing = 0, SnakeHead = 1, SnakeBody = 2, Wall = 4, Food = 8, Trap = 16
};

struct SnakeBodyData
{
	STRUCTXY site;
	State state;
};

HBITMAP WallBitmap, FoodBitmap, TrapBitmap, NothingBitmap;
HBITMAP StateLRBitmap, StateUDBitmap, StateULBitmap, StateURBitmap, StateDLBitmap, StateDRBitmap;
HBITMAP HeadLBitmap, HeadRBitmap, HeadUBitmap, HeadDBitmap;
HBITMAP TailLBitmap, TailRBitmap, TailUBitmap, TailDBitmap;

class thesnake
{
private:
	SHORT length;
	SnakeBodyData body[512];
	STRUCTXY GetHeadXY();
	void Add(SnakeBodyData newbody, bool head = FALSE);
	bool GameOver();
	bool CatchFood();
public:
	UINT Move();
	State GetNowState();
	bool SetNowState(State newstate);
	void SetSnake();
	void PrintSnake();
	void DeleteSnake();
	//void EatFood();
};

class thefood
{
private:
	STRUCTXY NowSite;
public:
	void Initialization();
	void UpdateXY();
};

extern thesnake SNAKE;
extern thefood FOOD;
extern Block GameMap[MAXX + 1][MAXY + 1];
extern STRUCTXY GetWindowXY(STRUCTXY Pos);
extern STRUCTXY GetWindowXY(SHORT x, SHORT y);
extern STRUCTXY Set(SHORT _sitex, SHORT _sitey);
extern SnakeBodyData Set(STRUCTXY _site, State _state);
extern SnakeBodyData Set(SHORT _sitex, SHORT _sitey, State _state);
extern State wParamToState(WPARAM wParam);
extern std::queue<State> Buffer;
extern std::queue<STRUCTXY> SetXY;

void LoadAllBitmap();
void PrintBitmap(HDC hdc, HBITMAP hbmp, STRUCTXY xy);
void PrintBitmap(HDC hdc, HBITMAP hbmp, SHORT x, SHORT y);
void SetGameMap(STRUCTXY xy, Block block);
void SetGameMap();
void PrintGameMap();
void RePrintGameMap();
void SetGame();
void OnceGame();