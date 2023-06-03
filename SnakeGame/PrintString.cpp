#include "GameWindow.h"
#include "PrintString.h"

void PrintStringXY(int x, int y, LPCWSTR string)
{
	TextOutW(WindowHDC, x, y, string, wcslen(string));
}

void PrintStringL(int y, LPCWSTR string)
{
	TextOutW(WindowHDC, 0, y, string, wcslen(string));
}

void PrintStringR(int y, LPCWSTR string)
{
	SIZE size;
	GetTextExtentPoint32W(WindowHDC, string, wcslen(string), &size);
	TextOutW(WindowHDC, WINDOWWIDTH - size.cx, y, string, wcslen(string));
}

void PrintStringM(int y, LPCWSTR string)
{
	SIZE size;
	GetTextExtentPoint32W(WindowHDC, string, wcslen(string), &size);
	TextOutW(WindowHDC, (WINDOWWIDTH - size.cx) / 2, y, string, wcslen(string));
}

HFONT SetFont(int size, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut,LPCWSTR string)
{
	HFONT font = ::CreateFont(size, 0, 0, 0, cWeight, bItalic, bUnderline, bStrikeOut, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS,
		string);
	return font;
}
