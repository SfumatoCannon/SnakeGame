#include "framework.h"
#include <time.h>
#include <conio.h>
#include <cstring>
#include "PrintString.h"
#include "File.h"
#include "SnakeGame.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
PAINTSTRUCT ps;
HWND hWnd;
int nowscore;
int len;

thesnake SNAKE;
thefood FOOD;
Block GameMap[MAXX + 1][MAXY + 1];
void PrintScore();
STRUCTXY GetWindowXY(STRUCTXY Pos);
STRUCTXY GetWindowXY(SHORT x, SHORT y);
STRUCTXY Set(SHORT _sitex, SHORT _sitey);
SnakeBodyData Set(STRUCTXY _site, State _state);
SnakeBodyData Set(SHORT _sitex, SHORT _sitey, State _state);
State wParamToState(WPARAM wParam);
std::queue<State> Buffer;
std::queue<STRUCTXY> SetXY;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SNAKEGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SNAKEGAME));

	MSG msg;
	SetGame();
	SetTimer(hWnd, 1, SLEEPTIME, NULL);
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}
	}
	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKEGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	RECT rect;
	rect.left = WINDOWX;
	rect.right = WINDOWX + WINDOWTRUEWIDTH;
	rect.top = WINDOWY;
	rect.bottom = WINDOWY + WINDOWTRUEHEIGHT;
	AdjustWindowRectEx(&rect, WS_OVERLAPPED, 1, NULL);
	hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);
	if (GetWindowRect(hWnd, &rect))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
	}
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

char str[100];
wchar_t strW[100];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		HFONT TitleFont = SetFont(32, FW_BOLD, 0, 0, 0, L"Consolas");
		HFONT Font1 = SetFont(16, FW_NORMAL, 0, 0, 0, L"Consolas");
		WindowHDC = BeginPaint(hWnd, &ps);
		ReadScore();
		RePrintGameMap();
		SelectObject(WindowHDC, TitleFont);
		PrintStringM(0, L"Snake Game");
		DeleteObject(TitleFont);
		SetTextColor(WindowHDC, RGB(127, 127, 127));
		SelectObject(WindowHDC, Font1);
		PrintStringL(WINDOWHEIGHT - 32, L"Game by SfumatoCannon_.");
		PrintStringL(WINDOWHEIGHT - 16, L"All Right reserved.");
		SetTextColor(WindowHDC, RGB(0, 0, 0));
		DeleteObject(Font1);
		PrintScore();
		break;
	}
	case WM_TIMER:
		if (!Buffer.empty())
		{
			while (!Buffer.empty() && !SNAKE.SetNowState(Buffer.front())) Buffer.pop();
			if (!Buffer.empty()) Buffer.pop();
		}
		SendMessage(hWnd, SNAKE.Move(), 0, 0);
		HFONT Font1;
		Font1 = SetFont(16, FW_NORMAL, 0, 0, 0, L"Consolas");
		SelectObject(WindowHDC, Font1);
		SetTextColor(WindowHDC, RGB(0, 0, 0));
		sprintf_s(str, "Score:%d       ", nowscore);
		len = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), strW, len);
		strW[len] = '\0';
		PrintStringL(0, strW);
		DeleteObject(Font1);
		SetTextColor(WindowHDC, RGB(255, 255, 255));
		break;
	case GAMEOVER:
		KillTimer(hWnd, 1);
		UpdateScore(nowscore);
		sprintf_s(str, "GAME OVER!\nYou score is:  %d\nWould you like to try again?", nowscore);
		len = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), strW, len);
		strW[len] = '\0';
		if (MessageBox(hWnd, strW, L"MSG", MB_YESNO) == IDNO)
			SendMessage(hWnd, WM_DESTROY, 0, 0);
		else
		{
			SetGame();
			PrintScore();
			SetTimer(hWnd, 1, SLEEPTIME, NULL);
		}
		break;
	case CATCHFOOD:
		nowscore += 20;
		//SNAKE.EatFood();
		FOOD.UpdateXY();
		break;
	case WM_KEYDOWN:
		Buffer.push(wParamToState(wParam));
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		EndPaint(hWnd, &ps);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

State thesnake::GetNowState()
{
	return body[0].state;
}

STRUCTXY thesnake::GetHeadXY()
{
	return body[0].site;
}

void thesnake::Add(SnakeBodyData newbody, bool head)
{
	length++;
	body[length - 1] = newbody;
	if (head) SetGameMap(newbody.site, SnakeHead);
	else SetGameMap(newbody.site, SnakeBody);
}

bool thesnake::SetNowState(State newstate)
{
	if (newstate == NoState)
		return false;
	if (newstate + GetNowState() == StateL + StateR || newstate + GetNowState() == StateU + StateD || newstate == GetNowState()) return false;
	body[0].state = newstate;
	return true;
}

void thesnake::SetSnake()
{
	length = 0;
	Add(Set(MAXX / 2 + 2, MAXY / 2 + 1, StateR), TRUE);
	Add(Set(MAXX / 2 + 1, MAXY / 2 + 1, StateR));
	Add(Set(MAXX / 2, MAXY / 2 + 1, StateR));
	Add(Set(MAXX / 2 - 1, MAXY / 2 + 1, StateR));
}

void thesnake::PrintSnake()
{
	int i;
	STRUCTXY WindowXY;
	WindowXY = GetWindowXY(body[0].site);
	switch (body[0].state)
	{
	case StateD:
		PrintBitmap(WindowHDC, HeadDBitmap, WindowXY);
		break;
	case StateL:
		PrintBitmap(WindowHDC, HeadLBitmap, WindowXY);
		break;
	case StateR:
		PrintBitmap(WindowHDC, HeadRBitmap, WindowXY);
		break;
	case StateU:
		PrintBitmap(WindowHDC, HeadUBitmap, WindowXY);
		break;
	default:
		break;
	}
	for (i = 1; i < length - 1; i++)
	{
		WindowXY = GetWindowXY(body[i].site);
		switch (body[i].state + body[i + 1].state)
		{
		case StateL + StateL:case StateR + StateR://2,4
			PrintBitmap(WindowHDC, StateLRBitmap, WindowXY);
			break;
		case StateU + StateU:case StateD + StateD://8,16
			PrintBitmap(WindowHDC, StateUDBitmap, WindowXY);
			break;
		case StateD + StateL://9
			if (body[i].state == StateD)
				PrintBitmap(WindowHDC, StateDRBitmap, WindowXY);
			else PrintBitmap(WindowHDC, StateULBitmap, WindowXY);
			break;
		case StateD + StateR://10
			if (body[i].state == StateD)
				PrintBitmap(WindowHDC, StateDLBitmap, WindowXY);
			else PrintBitmap(WindowHDC, StateURBitmap, WindowXY);
			break;
		case StateU + StateL://5
			if (body[i].state == StateU)
				PrintBitmap(WindowHDC, StateURBitmap, WindowXY);
			else PrintBitmap(WindowHDC, StateDLBitmap, WindowXY);
			break;
		case StateU + StateR://6
			if (body[i].state == StateU)
				PrintBitmap(WindowHDC, StateULBitmap, WindowXY);
			else PrintBitmap(WindowHDC, StateDRBitmap, WindowXY);
			break;
		default:
			break;
		}
	}
	WindowXY = GetWindowXY(body[length - 1].site);
	switch (body[length - 1].state)
	{
	case StateD:
		PrintBitmap(WindowHDC, TailUBitmap, WindowXY);
		break;
	case StateL:
		PrintBitmap(WindowHDC, TailRBitmap, WindowXY);
		break;
	case StateR:
		PrintBitmap(WindowHDC, TailLBitmap, WindowXY);
		break;
	case StateU:
		PrintBitmap(WindowHDC, TailDBitmap, WindowXY);
		break;
	default:
		break;
	}
}

void thesnake::DeleteSnake()
{
	int i;
	for (i = 0; i < length; i++)
		PrintBitmap(WindowHDC, NothingBitmap, GetWindowXY(body[i].site));
}

bool thesnake::GameOver()
{
	int i;
	bool visited[MAXX + 1][MAXY + 1];
	memset(visited, false, sizeof(visited));
	STRUCTXY SnakeXY = GetHeadXY();
	if (GameMap[SnakeXY.x][SnakeXY.y] == Wall || GameMap[SnakeXY.x][SnakeXY.y] == Trap)
		return true;
	for (i = 0; i < length; i++)
	{
		if (visited[body[i].site.x][body[i].site.y]) return true;
		visited[body[i].site.x][body[i].site.y] = true;
	}
	return false;
}

bool thesnake::CatchFood()
{
	STRUCTXY SnakeXY = GetHeadXY();
	if (GameMap[SnakeXY.x][SnakeXY.y] == Food)
		return true;
	else return false;
}

/*void thesnake::EatFood()
{
	SnakeBodyData NewBody;
	NewBody = body[length - 1];
	switch (NewBody.state)
	{
	case StateL:
		NewBody.site.x++;
		break;
	case StateR:
		NewBody.site.x--;
		break;
	case StateU:
		NewBody.site.y++;
		break;
	case StateD:
		NewBody.site.y--;
		break;
	default:
		break;
	}
	Add(NewBody);
	PrintGameMap();
}*/

UINT thesnake::Move()
{
	int i;
	SnakeBodyData Tail = body[length - 1];
	for (i = length - 1; i >= 0; i--)
	{
		switch (body[i].state)
		{
		case StateL:
			body[i].site.x--;
			break;
		case StateR:
			body[i].site.x++;
			break;
		case StateU:
			body[i].site.y--;
			break;
		case StateD:
			body[i].site.y++;
			break;
		default:
			break;
		}
		if (i != 0)
			body[i].state = body[i - 1].state;
	}
	if (GameOver()) return GAMEOVER;
	if (CatchFood())
	{
		GameMap[GetHeadXY().x][GetHeadXY().y] = SnakeHead;
		SetGameMap(Tail.site, SnakeBody);
		Add(Tail);
		PrintGameMap();
		return CATCHFOOD;
	}
	SetGameMap(Tail.site, Nothing);
	SetGameMap(GetHeadXY(), SnakeHead);
	PrintGameMap();
	return NORMAL;
}

void PrintScore()
{
	HFONT masterTitleFont = SetFont(32, FW_NORMAL, 0, 0, 0, L"System");
	HFONT masterTextFont = SetFont(16, FW_NORMAL, 0, 0, 0, L"Consolas");
	SetTextColor(WindowHDC, RGB(0, 0, 0));
	SelectObject(WindowHDC, masterTitleFont);
	PrintStringXY(LEFTDIS + MAXX * 32 + 64, 32, L"Top Score");
	DeleteObject(masterTitleFont);
	SelectObject(WindowHDC, masterTextFont);
	SetTextColor(WindowHDC, RGB(130, 130, 130));
	PrintStringXY(LEFTDIS + MAXX * 32 + 50, 70, L"-------------------");
	int i;
	if (totnum == 0)
		PrintStringXY(LEFTDIS + MAXX * 32 + 70, 96, L"No records yet");
	for (i = 0; i < totnum; i++)
	{
		if (i == 0) SetTextColor(WindowHDC, RGB(255, 31, 31));
		else if (i == 1) SetTextColor(WindowHDC, RGB(99, 97, 194));
		else if (i == 2) SetTextColor(WindowHDC, RGB(185, 120, 9));
		else SetTextColor(WindowHDC, RGB(0, 0, 0));
		sprintf_s(str, "#%d  ", i + 1);
		len = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), strW, len);
		strW[len] = '\0';
		PrintStringXY(LEFTDIS + MAXX * 32 + 20, 96 + i * 32, strW);
		sprintf_s(str, "%lld", Master[i]);
		len = MultiByteToWideChar(CP_ACP, 0, str, strlen(str), NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, str, strlen(str), strW, len);
		strW[len] = '\0';
		long long x = Master[i], temp = 0;
		if (x == 0)
			temp = 1;
		else
		{
			while (x != 0)
			{
				temp++;
				x /= 10;
			}
		}
		PrintStringXY(LEFTDIS + MAXX * 32 + 220 - 8 * (temp - 1), 96 + i * 32, strW);
	}
	DeleteObject(masterTextFont);
}

STRUCTXY GetWindowXY(STRUCTXY Pos)
{
	STRUCTXY result;
	result.x = 32 * Pos.x - 32 + LEFTDIS;
	result.y = 32 * Pos.y - 32 + UPDIS;
	return result;
}

STRUCTXY GetWindowXY(SHORT x, SHORT y)
{
	STRUCTXY result;
	result.x = 32 * x - 32 + LEFTDIS;
	result.y = 32 * y - 32 + UPDIS;
	return result;
}

STRUCTXY Set(SHORT _sitex, SHORT _sitey)
{
	STRUCTXY result;
	result.x = _sitex;
	result.y = _sitey;
	return result;
}

SnakeBodyData Set(STRUCTXY _site, State _state)
{
	SnakeBodyData result;
	result.site = _site;
	result.state = _state;
	return result;
}

SnakeBodyData Set(SHORT _sitex, SHORT _sitey, State _state)
{
	return Set(Set(_sitex, _sitey), _state);
}

State wParamToState(WPARAM wParam)
{
	switch (wParam)
	{
	case VK_UP:
		return StateU;
		break;
	case VK_LEFT:
		return StateL;
		break;
	case VK_DOWN:
		return StateD;
		break;
	case VK_RIGHT:
		return StateR;
		break;
	default:
		return NoState;
		break;
	}
}

void LoadAllBitmap()
{
	FoodBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\Food.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	HeadDBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\HeadD.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	HeadLBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\HeadL.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	HeadRBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\HeadR.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	HeadUBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\HeadU.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	NothingBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\Nothing.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	StateDLBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\StateDL.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	StateDRBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\StateDR.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	StateLRBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\StateLR.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	StateUDBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\StateUD.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	StateULBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\StateUL.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	StateURBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\StateUR.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	TailDBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\TailD.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	TailLBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\TailL.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	TailRBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\TailR.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	TailUBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\TailU.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	TrapBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\Trap.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
	WallBitmap = (HBITMAP)::LoadImage(
		NULL,
		L"Image\\Wall.bmp",
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_DEFAULTCOLOR
	);
}

void PrintBitmap(HDC hdc, HBITMAP hbmp, STRUCTXY xy)
{
	HDC mdc = CreateCompatibleDC(hdc);
	SelectObject(mdc, hbmp);
	BitBlt(
		hdc,
		xy.x, xy.y,
		32,
		32,
		mdc,
		0, 0,
		SRCCOPY
	);
	DeleteDC(mdc);
}

void PrintBitmap(HDC hdc, HBITMAP hbmp, SHORT x, SHORT y)
{
	HDC mdc = CreateCompatibleDC(hdc);
	SelectObject(mdc, hbmp);
	BitBlt(
		hdc,
		x, y,
		32,
		32,
		mdc,
		0, 0,
		SRCCOPY
	);
}

void SetGameMap(STRUCTXY xy, Block block)
{
	GameMap[xy.x][xy.y] = block;
	SetXY.push(xy);
}

void SetGameMap()
{
	SHORT i, j;
	for (i = 1; i <= MAXX; i++)
		for (j = 1; j <= MAXY; j++)
		{
			if (i == 1 || j == 1 || i == MAXX || j == MAXY) SetGameMap(Set(i, j), Wall);
			else SetGameMap(Set(i, j), Nothing);
		}
}

void PrintGameMap()
{
	while (!SetXY.empty())
	{
		switch (GameMap[SetXY.front().x][SetXY.front().y])
		{
		case Wall:
			PrintBitmap(WindowHDC, WallBitmap, GetWindowXY(SetXY.front()));
			break;
		case Food:
			PrintBitmap(WindowHDC, FoodBitmap, GetWindowXY(SetXY.front()));
			break;
		case Nothing:
			PrintBitmap(WindowHDC, NothingBitmap, GetWindowXY(SetXY.front()));
			break;
		default:
			break;
		}
		SetXY.pop();
	}
	SNAKE.PrintSnake();
}

void RePrintGameMap()
{
	SHORT i, j;
	for (i = 1; i <= MAXX; i++)
		for (j = 1; j <= MAXY; j++)
		{
			switch (GameMap[i][j])
			{
			case Wall:
				PrintBitmap(WindowHDC, WallBitmap, GetWindowXY(i, j));
				break;
			case Food:
				PrintBitmap(WindowHDC, FoodBitmap, GetWindowXY(i, j));
				break;
			case Nothing:
				PrintBitmap(WindowHDC, NothingBitmap, GetWindowXY(i, j));
				break;
			case SnakeHead:
				SNAKE.PrintSnake();
				break;
			default:
				break;
			}
		}
}

void SetGame()
{
	nowscore = 0;
	LoadAllBitmap();
	SetGameMap();
	SNAKE.SetSnake();
	FOOD.Initialization();
	FOOD.UpdateXY();
	PrintGameMap();
}

void OnceGame()
{
}

void thefood::Initialization()
{
	srand(short(time(0)));
}

void thefood::UpdateXY()
{
	NowSite.x = rand() % (MAXX - 2) + 2;
	NowSite.y = rand() % (MAXY - 2) + 2;
	while (GameMap[NowSite.x][NowSite.y] != Nothing)
	{
		NowSite.x = rand() % (MAXX - 2) + 2;
		NowSite.y = rand() % (MAXY - 2) + 2;
	}
	SetGameMap(NowSite, Food);
	PrintBitmap(WindowHDC, FoodBitmap, GetWindowXY(NowSite));
}
