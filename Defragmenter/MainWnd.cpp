#include "Defragmenter.h"

HFONT hFont;
LOGFONT lf;

LRESULT CALLBACK WNDProc_Main(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch (message) {
    case WM_CREATE: {
        WndMainOpen = true;
        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        hFont = CreateFont(30, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 700,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
        HWND hBtn = CreateWindow(L"BUTTON", L"START", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 70, 275, 150, 50, hwnd, (HMENU)IDC_STARTBTN, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        }
        break;
    case WM_DESTROY:
        WndMainOpen = false;
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);   
        HBITMAP imageBmp = LoadBitmap((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_BACKGROUND_IMAGE));//(HBITMAP)LoadImageA(NULL, (LPCSTR)MAKEINTRESOURCE(IDR_BACKGROUND_IMAGE), IMAGE_BITMAP,0, 0,LR_DEFAULTSIZE | LR_LOADFROMFILE);
        HDC imageDC = CreateCompatibleDC(NULL);
        SelectObject(imageDC, imageBmp);
        BitBlt(hdc, 0, 0, 300, 400, imageDC, 0, 0, SRCCOPY);
        SelectObject(hdc, hFont);
        SetTextColor(hdc, RGB(0, 0, 255));
        RECT rc;
        rc.left = 40;
        rc.top = 0;
        rc.right = 240;
        rc.bottom = 100;
        DrawText(hdc, L"Welcome to defragmenter", 23, &rc, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
        EndPaint(hwnd, &ps);                 
    }
    break;

    case WM_COMMAND:
        switch LOWORD(wParam) {
        case (int)IDC_STARTBTN:
            if (isUserAdmin()) {
                RegisterWindowClass(WndClassTable, hwndTable, hInstance, nShow, L"WCTable", L"Available drives", isRegTableWnd, (WNDPROC)WNDProc_Table, 200, 210, 1000, 500);
                DestroyWindow(hwnd);
            }
            else
                MessageBox(hwnd, L"please, start defragmenter as administrator", NULL, NULL);
            break;
        }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

