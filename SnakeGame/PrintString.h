#pragma once

#include <Windows.h>

void PrintStringXY(int x, int y, LPCWSTR string);
void PrintStringL(int y, LPCWSTR string);
void PrintStringR(int y, LPCWSTR string);
void PrintStringM(int y, LPCWSTR string);
HFONT SetFont(int size, int cWeight, DWORD bItalic,
	DWORD bUnderline, DWORD bStrikeOut,LPCWSTR string);