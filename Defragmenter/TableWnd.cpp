#include "Defragmenter.h"

HWND hListView;

HWND CreateListView(HWND parent, int columns);
const wchar_t* lst_headers[] = {
        { L"Drive" },
        { L"Drive Type" },
        { L"Sector Per Cluster" },
        { L"Bytes Per Sector" },
        { L"Free Space(GB)" },
        { L"Full Space(GB)" }
};

LRESULT CALLBACK WNDProc_Table(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HFONT hFont;
    LOGFONT lf;
    switch (message) {
    case WM_CREATE: {
        WndMainTable = true;
        HWND hLabel = CreateWindow(L"static", L"LBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 20, 10, 400, 72, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        hFont = CreateFont(30, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 700,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hLabel, L"Available Drives");
        HWND hInfoBtn = CreateWindowEx(NULL, L"BUTTON", L"Open Window 3", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 50, 0, 150, 24, hwnd, (HMENU)IDC_INFOBTN, GetModuleHandle(NULL), NULL);
        HWND hDefragBtn = CreateWindowEx(NULL, L"BUTTON", L"Open Window 4", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 220, 0, 150, 24, hwnd, (HMENU)IDC_DEFRAGBTN, GetModuleHandle(NULL), NULL);
       hListView = CreateListView(hwnd, 6);
        break;
    }
    case WM_DESTROY:
        WndMainTable = false;
        break;
    case WM_COMMAND:
        switch LOWORD(wParam) {
        case (int)IDC_INFOBTN: {
            RegisterWindowClass(WndClassInfo, hwndInfo, hInstance, nShow, L"WCInfo", L"Drive Info", isRegInfoWnd, (WNDPROC)WNDProc_Info, 300, 200, 500, 550);
            break;
        }
        case (int)IDC_DEFRAGBTN: {
            RegisterWindowClass(WndClassDefrag, hwndDefrag, hInstance, nShow, L"WCDefrag", L"Defragmentation process", isRegDefragWnd, (WNDPROC)WNDProc_Defrag, 200, 200, 600, 500);
            break;
        }
         break;
        }
    /*case WM_NOTIFY: {
        if ((((LPNMHDR)lParam)->hwndFrom) == SINConsole)
        {
            switch (((LPNMHDR)lParam)->code)
            {
            case NM_DBLCLK:
            {
                int ItemIndex = SendMessage(SINConsole, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
                LVITEM SelectedItem;
                SelectedItem.iItem = ItemIndex;
                ListView_GetItem(SINConsole, (LVITEM*)&SelectedItem);
                Victim* ThisVictim = NULL;
                ThisVictim = (Victim*)SelectedItem.lParam;
                // do something e.g. ThisVictim->MyMethod();
            }
            break;
            }
        }
        break;
    }*/
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

 HWND CreateListView(HWND parent, int columns) {

        RECT rcClient;
        GetClientRect(parent, &rcClient);
        HWND hWndListView = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE, 0,50,
            rcClient.right - rcClient.left, rcClient.bottom - rcClient.top - 100, parent, IDC_LISTVIEW, hInstance, NULL);

        LVCOLUMN c = { 0 };
        c.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

        for (int i = 0; i < columns; i++) {
            c.fmt = LVCFMT_LEFT;
            c.pszText = (LPWSTR)lst_headers[i];
            c.iSubItem = i;
            c.cx = 110;
            ListView_InsertColumn(hWndListView, i, &c);
        }
        return hWndListView;
 }

 void UpdateListView(HWND hListView, std::vector<DriveData*> driveList) {
     ListView_DeleteAllItems(hListView);

     LVITEM lvI;

     lvI.mask = LVIF_TEXT;
     lvI.pszText = LPSTR_TEXTCALLBACK;
     lvI.iSubItem = 0;

     for (int i = 0; i < driveList.size(); i++) {
         lvI.iItem = i;
         ListView_InsertItem(hListView, &lvI);
     }
 }