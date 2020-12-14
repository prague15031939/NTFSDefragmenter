#pragma once
#include "resource.h"
#include "framework.h"
#include "../DefragmenterCore/Core.h"

LRESULT CALLBACK WNDProc_Main(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WNDProc_Table(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WNDProc_Info(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WNDProc_Defrag(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WNDProc_Selected(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WNDProc_Stats(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern int nShow;
extern HINSTANCE hInstance;

extern bool WndMainOpen, WndMainTable, WndMainInfo, WndMainDefrag, WndMainSelected, WndMainStats;
extern bool isRegMainWnd, isRegTableWnd, isRegInfoWnd, isRegDefragWnd, isRegSelectedWnd, isRegStatsWnd;

extern HWND hwndTable;
extern HWND hwndInfo;
extern HWND hwndDefrag;
extern HWND hWndMain;
extern HWND hwndSelected;
extern HWND hwndStats;

extern WNDCLASSEX WndClassMain;
extern WNDCLASSEX WndClassTable;
extern WNDCLASSEX WndClassInfo;
extern WNDCLASSEX WndClassDefrag;
extern WNDCLASSEX WndClassSelected;
extern WNDCLASSEX WndClassStats;

extern DriveData* currDrive;

void RegisterWindowClass(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd, LPCWSTR className, LPCWSTR windowTitle, bool& isReg, WNDPROC wndProc, int x, int y, int w, int h);