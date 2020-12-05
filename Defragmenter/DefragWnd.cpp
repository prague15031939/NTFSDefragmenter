#include "Defragmenter.h"

HWND hEdit;
HWND hBtn;
HANDLE hDefragThread;
void AppendTextToEditCtrl(HWND hWndEdit, LPCTSTR pszText);
StartDefragInfo* GetStartDefragInfo(char drive = 'E');

LRESULT CALLBACK WNDProc_Defrag(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        InitCS();
        hDefragThread = CreateThread(NULL, 0, WorkIn, GetStartDefragInfo(/* pass a drive letter here */), 0, NULL);

        SetTimer(hwnd, 1, 40, NULL);
        hEdit = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, 10, 20, 600, 400, hwnd, (HMENU)IDC_DEFRAGOUTPUT, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)L"");
        hBtn = CreateWindow(L"BUTTON", L"STOP", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 200, 450, 100, 30, hwnd, (HMENU)IDC_STARTBTN, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        SetThreadPriority(hBtn, THREAD_PRIORITY_HIGHEST);

        WndMainDefrag = true;
        break;
    }

    case WM_DESTROY:
        StopDefrager(hDefragThread);
        DeleteCS();
        WndMainDefrag = false;
        break;

    case WM_TIMER: {
        std::queue<DefragmentationLogItem*> log = getDefragmentationLogs();
        int size = log.size();
        if (log.size() > 0) {
            for (int i = 0; i < size; i++) {
                DefragmentationLogItem* item = log.front();
                log.pop();
                AppendTextToEditCtrl(hEdit, L"\r\n");
                AppendTextToEditCtrl(hEdit, item->fullName);
                AppendTextToEditCtrl(hEdit, L"      ");
                AppendTextToEditCtrl(hEdit, item->result);
                delete item;
            }
        }
        DWORD dwThreadID = GetWindowThreadProcessId(hBtn, NULL);
        DWORD dwThisThreadID = GetWindowThreadProcessId(hEdit, NULL);
        AttachThreadInput(dwThisThreadID, dwThreadID, TRUE);
        SetFocus(hBtn);
        AttachThreadInput(dwThisThreadID, dwThreadID, FALSE);

        break;
    }
    case WM_COMMAND:
        switch LOWORD(wParam) {
        case (int)IDC_STARTBTN:
            DestroyWindow(hwnd);
            break;
        }
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void AppendTextToEditCtrl(HWND hWndEdit, LPCTSTR pszText)
{
    int nLength = GetWindowTextLength(hWndEdit);
    if (nLength > 10000) {
        SetWindowText(hWndEdit, L"");
    }
    SendMessage(hWndEdit, EM_SETSEL, (WPARAM)nLength, (LPARAM)nLength);
    SendMessage(hWndEdit, EM_REPLACESEL, (WPARAM)FALSE, (LPARAM)pszText);
}

StartDefragInfo* GetStartDefragInfo(char drive) 
{
    StartDefragInfo* info = new StartDefragInfo();
    char* dr = new char[7]{ "\\\\.\\C:" };
    dr[4] = drive;
    info->directory = drive;
    info->drive = dr;
    info->first = true;
    return info;
}
