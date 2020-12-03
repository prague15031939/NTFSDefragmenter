#include "GUI.h"

bool WndMainOpen, WndMainTable, WndMainInfo, WndMainDefrag=false;
bool isRegMainWnd, isRegTableWnd, isRegInfoWnd, isRegDefragWnd=false;
int nShow = 0;
HINSTANCE hInstance = NULL;
HWND hwndTable = NULL;
HWND hwndInfo=NULL;
HWND hwndDefrag=NULL;
HWND hWndMain=NULL;

WNDCLASSEX WndClassMain = {};
WNDCLASSEX WndClassTable = {};
WNDCLASSEX WndClassInfo = {};
WNDCLASSEX WndClassDefrag = {};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
    
    bool endprogram = false;
    nShow = nShowCmd;
    hInstance = hInst;
    RegisterWindowClass(WndClassMain, hWndMain, hInst, nShowCmd, L"WCMain", L"Spizzennyi Defragmentator", isRegMainWnd, (WNDPROC)WNDProc_Main, 300, 300, 300, 400);

    MSG msg;
    bool endloop = false;
    while (endloop == false) {
        if (GetMessage(&msg, NULL, 0, 0));
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (WndMainOpen == false && WndMainTable == false && WndMainInfo == false && WndMainDefrag == false)
            endloop = true;
    }
    MessageBox(NULL,
        L"All Windows are closed.  Program will now close.",
        L"Message",
        MB_ICONINFORMATION);
}


void RegisterWindowClass(WNDCLASSEX& wc, HWND& hwnd, HINSTANCE hInst, int nShowCmd, LPCWSTR className, LPCWSTR windowTitle, bool &isReg, WNDPROC wndProc, int x, int y, int w, int h) {
    if (isReg == false) {
        ZeroMemory(&wc, sizeof(WNDCLASSEX));
        wc.cbClsExtra = NULL;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = NULL;
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DEFRAGMENTATOR));
        wc.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
        wc.hInstance = hInst;
        wc.lpfnWndProc = wndProc;
        wc.lpszClassName = className;
        wc.lpszMenuName = MAKEINTRESOURCEW(IDC_DEFRAGMENTATOR_MAIN);
        wc.style = CS_HREDRAW | CS_VREDRAW;

        if (!RegisterClassEx(&wc))
        {
            int nResult = GetLastError();
            MessageBox(NULL,
                L"Window class creation failed",
                L"Window Class Failed",
                MB_ICONERROR);
        }
        else
            isReg = true;
    }
    hwnd = CreateWindowEx(NULL,
        wc.lpszClassName,
        windowTitle,
        WS_OVERLAPPEDWINDOW,
        x, y, w, h,
        NULL,
        NULL,
        hInst,
        NULL
    );

    if (!hwnd)
    {
        int nResult = GetLastError();

        MessageBox(NULL,
            L"Window creation failed",
            L"Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(hwnd, nShowCmd);
}