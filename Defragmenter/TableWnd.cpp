#include "Defragmenter.h"

HWND hListView;
std::vector<DriveData*> Drives;
int pressed=0;

HWND CreateListView(HWND parent, int columns);
void UpdateListView(HWND hListView, std::vector<DriveData*> driveList);

const wchar_t* lst_headers[] = {
        { L"Drive" },
        { L"Drive Type" },
        { L"Sector Per Cluster" },
        { L"Bytes Per Sector" },
        { L"Free Space(GB)" },
        { L"Full Space(GB)" }
};

LRESULT CALLBACK WNDProc_Table(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HFONT hFont1, hFont2;
    LOGFONT lf;
    Drives = getDrives();
    switch (message) {
    case WM_CREATE: {
        WndMainTable = true;
        HWND hLabel = CreateWindow(L"static", L"LBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 20, 10, 400, 72, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
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
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont1, TRUE);
        SetWindowText(hLabel, L"Available Drives");
       hListView = CreateListView(hwnd, 6);
       SendMessage(hListView, WM_SETFONT, (WPARAM)hFont2, TRUE);
       UpdateListView(hListView, Drives);
        break;
    }
    case WM_DESTROY:
        WndMainTable = false;
        break;
    case WM_NOTIFY:
    {
        NMLVDISPINFO* plvdi;

        if ((((LPNMHDR)lParam)->hwndFrom) == hListView)
        {
            switch (((LPNMHDR)lParam)->code)
            {
            case LVN_GETDISPINFO:

                plvdi = (NMLVDISPINFO*)lParam;

                switch (plvdi->item.iSubItem)
                {
                case 0:
                    plvdi->item.pszText = Drives[plvdi->item.iItem]->Drive;
                    break;
                case 1:
                    plvdi->item.pszText = Drives[plvdi->item.iItem]->DriveType;
                    break;
                case 2:
                    plvdi->item.pszText = Drives[plvdi->item.iItem]->SectorPerCluster;
                    break;
                case 3:
                    plvdi->item.pszText = Drives[plvdi->item.iItem]->BytesPerSector;
                    break;
                case 4:
                    plvdi->item.pszText = Drives[plvdi->item.iItem]->FreeSpace;
                    break;
                case 5:
                    plvdi->item.pszText = Drives[plvdi->item.iItem]->FullSpace;
                    break;
                default:
                    break;
                }
                break;
            }
            case BN_CLICKED:
            {
                if ((((LPNMHDR)lParam)->hwndFrom) == hListView)
                {
                    POINT pt = {};
                    GetCursorPos(&pt);
                        int ItemIndex = (pt.y-335)/25;
                        if (ItemIndex > -1 && ItemIndex < Drives.size()) {
                            if (!WndMainSelected) {
                                if (pressed > 1) {
                                    currDrive = Drives[ItemIndex];
                                    RegisterWindowClass(WndClassSelected, hwndSelected, hInstance, nShow, L"WCSelected", L"Selected Drive", isRegSelectedWnd, (WNDPROC)WNDProc_Selected, 200, 200, 350, 320);
                                    SetWindowPos(hwndSelected, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                                }
                                else
                                    pressed++;
                         }
                    }

                    /*int ItemIndex = SendMessage(hListView, LVM_GETNEXTITEM, -1, LVIS_SELECTED);
                    
                    if (ItemIndex > -1 && ItemIndex < Drives.size()) {
                        if (!WndMainSelected) {
                            currDrive = Drives[ItemIndex];
                            RegisterWindowClass(WndClassSelected, hwndSelected, hInstance, nShow, L"WCSelected", L"Selected Drive", isRegSelectedWnd, (WNDPROC)WNDProc_Selected, 200, 200, 350, 320);
                            SetWindowPos(hwndSelected, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                        }
                        
                    }*/
                }
                
            }
        }
    break;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

 HWND CreateListView(HWND parent, int columns) {

        RECT rcClient;
        GetClientRect(parent, &rcClient);
        HWND hWndListView = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS | WS_EX_CLIENTEDGE, 0,50,
            rcClient.right - rcClient.left, 35 * Drives.size(), parent, IDC_LISTVIEW, hInstance, NULL);

        LVCOLUMN c = { 0 };
        c.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

        for (int i = 0; i < columns; i++) {
            c.fmt = LVCFMT_LEFT;
            c.pszText = (LPWSTR)lst_headers[i];
            c.iSubItem = i;
            c.cx = 150;
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