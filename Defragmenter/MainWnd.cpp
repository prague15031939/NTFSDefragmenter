﻿#include "Defragmenter.h"

HFONT hFont;
LOGFONT lf;

LRESULT CALLBACK WNDProc_Main(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message) {
    case WM_CREATE: {
        WndMainOpen = true;
        HWND hLabel = CreateWindow(L"static", L"StartLBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 20, 0, 200, 60, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        hFont = CreateFont(30, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 700,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hLabel, L"Welcome to defragmenter");
        HWND hBtn = CreateWindow(L"BUTTON", L"START", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 50, 220, 150, 50, hwnd, (HMENU)IDC_STARTBTN, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        }
        break;
    case WM_DESTROY:
        WndMainOpen = false;
        break;

    case WM_CTLCOLORSTATIC:
    {
            if(GetDlgItem(hwnd,(int)IDC_STARTLBL) == (HWND)lParam)
            {
                SetTextColor((HDC)wParam, RGB(0, 0, 255));
                SetBkMode((HDC)wParam, TRANSPARENT);
                return (INT_PTR)CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
            }
            break;
     }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);   
        HBITMAP imageBmp = (HBITMAP)LoadImageA(NULL,(LPCSTR)".\\defragicon.bmp",IMAGE_BITMAP,0, 0,LR_DEFAULTSIZE | LR_LOADFROMFILE);
        HDC imageDC = CreateCompatibleDC(NULL);
        SelectObject(imageDC, imageBmp);
        BitBlt(hdc,0, 60,300,400,imageDC,0, 0,SRCCOPY);
        EndPaint(hwnd, &ps);                 
    }break;

    case WM_COMMAND:
        switch LOWORD(wParam) {
        case (int)IDC_STARTBTN:
            //if (isUserAdmin()) {
                RegisterWindowClass(WndClassTable, hwndTable, hInstance, nShow, L"WCTable", L"Available drives", isRegTableWnd, (WNDPROC)WNDProc_Table, 200, 210, 1000, 500);
                DestroyWindow(hwnd);
            //}
                
            //else
               // MessageBox(hwnd,L"You are not admin, start defrager as admin",NULL,NULL);
            break;
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);

}

