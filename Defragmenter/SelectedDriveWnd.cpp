#include "Defragmenter.h"

const wchar_t* sel_headers[] = {
        { L"Drive" },
        { L"Drive Type" },
        { L"Sector Per Cluster" },
        { L"Bytes Per Sector" },
        { L"Free Space(GB)" },
        { L"Full Space(GB)" }
};

LRESULT CALLBACK WNDProc_Selected(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HFONT hFont1, hFont2;
    LOGFONT lf;

    switch (message) {
    case WM_CREATE: {
        WndMainSelected = true;
        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        hFont1 = CreateFont(15, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 400,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
        hFont2 = CreateFont(15, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 600,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
        HWND hLabel = CreateWindow(L"static", L"LBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 20, 20, 400, 30, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        HWND hInfoBtn = CreateWindowEx(NULL, L"BUTTON", L"Detail Info", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 5, 200, 150, 24, hwnd, (HMENU)IDC_INFOBTN, GetModuleHandle(NULL), NULL);
        HWND hDefragBtn = CreateWindowEx(NULL, L"BUTTON", L"Defragment", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 170, 200, 150, 24, hwnd, (HMENU)IDC_DEFRAGBTN, GetModuleHandle(NULL), NULL);
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont2, TRUE);
        SendMessage(hInfoBtn, WM_SETFONT, (WPARAM)hFont2, TRUE);
        SendMessage(hDefragBtn, WM_SETFONT, (WPARAM)hFont2, TRUE);
        SetWindowText(hLabel, L"Selected Drive");

        for (int i = 0; i < 6; i++) {
            HWND hLabel = CreateWindow(L"static", L"LBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 20, 10 + i * 30, 200, 30, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
            HWND hText = CreateWindow(L"static", L"LBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 200, 10 + i * 30, 200, 30, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
            SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont1, TRUE);
            SendMessage(hText, WM_SETFONT, (WPARAM)hFont2, TRUE);
            SetWindowText(hLabel, sel_headers[i]);
            SetWindowText(hText, (LPCWSTR)((size_t)currDrive + i * sizeof(wchar_t) * 20));
        }
        break;
    }
    case WM_COMMAND:
        switch (HIWORD(wParam)) {
        case BN_CLICKED:
            switch LOWORD(wParam) {
            case (int)IDC_INFOBTN: {
                char c = currDrive->Drive[0];
                DriveInfo* driveInfo = getDriveInfo(c);
                if (driveInfo!=NULL)
                    RegisterWindowClass(WndClassInfo, hwndInfo, hInstance, nShow, L"WCInfo", L"Drive Info", isRegInfoWnd, (WNDPROC)WNDProc_Info, 300, 200, 500, 550);
                else
                    MessageBox(hwnd, L"Current drive is unavailable (or FAT32 System)", NULL, NULL);
                break;
            }
            case (int)IDC_DEFRAGBTN: {
                RegisterWindowClass(WndClassDefrag, hwndDefrag, hInstance, nShow, L"WCDefrag", L"Defragmentation process", isRegDefragWnd, (WNDPROC)WNDProc_Defrag, 200, 200, 600, 650);
                break;
            }
            }
            DestroyWindow(hwndSelected);
            break;
    }
    case WM_DESTROY:
        WndMainSelected = false;
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}