#include "Defragmenter.h"

HWND hEdit;
HWND hBtn;
HWND hlstHead;
HANDLE hDefragThread;

void AppendTextToEditCtrl(HWND hWndEdit, LPCTSTR pszText,bool clear=false);
StartDefragInfo* GetStartDefragInfo(char drive = 'E');
const wchar_t* SwitchDefragStatus(wchar_t result[2]);
HWND CreateListView(HWND parent);

LRESULT CALLBACK WNDProc_Defrag(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HFONT hFont1, hFont2;
    LOGFONT lf;

    switch (message) {
    case WM_CREATE: {
        InitCS();
        /*char c = currDrive->Drive[0];
        hDefragThread = CreateThread(NULL, 0, WorkIn, GetStartDefragInfo(c), 0, NULL);*/
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
        hDefragThread = CreateThread(NULL, 0, Defragmentation, NULL, 0, NULL);
        SetTimer(hwnd, 1, 40, NULL);
        hEdit = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY, 12, 45, 572, 500, hwnd, (HMENU)IDC_DEFRAGOUTPUT, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        
        hBtn = CreateWindow(L"BUTTON", L"STOP", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 200, 560, 130, 50, hwnd, (HMENU)IDC_STARTBTN, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

        hlstHead = CreateListView(hwnd);

        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        hFont1 = CreateFont(30, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 700,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
        hFont2 = CreateFont(20, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 500,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);

        SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont1, TRUE);
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont2, TRUE);
        SendMessage(hlstHead, WM_SETFONT, (WPARAM)hFont2, TRUE);
        SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)L"");
        WndMainDefrag = true;
        break;
    }

    case WM_DESTROY:
        StopDefrager(hDefragThread);
        DeleteCS();
        WndMainDefrag = false;
        break;

        
    case WM_TIMER: {
        //std::queue<DefragmentationLogItem*> log = getDefragmentationLogs();
        std::queue<DefragmentationLogItem*> log = getTestDefragmentationLogs();
        int size = log.size();
        if (log.size() > 0) {
            for (int i = 0; i < size; i++) {
                DefragmentationLogItem* item = log.front();
                log.pop();
                AppendTextToEditCtrl(hEdit, L"\r\n");
                AppendTextToEditCtrl(hEdit, SwitchDefragStatus(item->result));
                AppendTextToEditCtrl(hEdit, L"              ");
                AppendTextToEditCtrl(hEdit, item->fullName);
                AppendTextToEditCtrl(hEdit, L"",true);
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
            //MessageBox(hwnd, L"Hello", NULL, NULL);
            StopDefrager(hDefragThread);
            break;
        }
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void AppendTextToEditCtrl(HWND hWndEdit, LPCTSTR pszText, bool clear)
{
    int nLength = GetWindowTextLength(hWndEdit);
    if (nLength > 10000 && clear) {
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

const wchar_t* SwitchDefragStatus(wchar_t result[2]) {
    if (!wcscmp(result, L"="))
        return L"Defragmented";
    if (!wcscmp(result, L"-"))
        return L"Error       ";
    if (!wcscmp(result, L"+"))
        return L"Clear       ";
}

HWND CreateListView(HWND parent) {

    RECT rcClient;
    GetClientRect(parent, &rcClient);
    HWND hWndListView = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE, 10, 20,
        600, 50, parent, IDC_LISTVIEW, hInstance, NULL);

    LVCOLUMN c = { 0 };
    c.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

    c.fmt = LVCFMT_LEFT;
    c.pszText = (LPWSTR)L"Status";
    c.iSubItem = 0;
    c.cx = 150;
    ListView_InsertColumn(hWndListView, 0, &c);

    c.fmt = LVCFMT_LEFT;
    c.pszText = (LPWSTR)L"File Name";
    c.iSubItem = 1;
    c.cx = 450;
    ListView_InsertColumn(hWndListView, 1, &c);

    return hWndListView;
}