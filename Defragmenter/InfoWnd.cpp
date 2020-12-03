#include "Defragmenter.h"

const wchar_t* headers[] = {
        { L"Drive: " },
        { L"1. Volume Serial Number: " },
        { L"2. Number of sectors: " },
        { L"3. Total number of clusters: " },
        { L"4. Number of free clusters: " },
        { L"5. Number of reserved clusters: " },
        { L"6. Number of bytes in sector: " },
        { L"7. Number of bytes in cluster: " },
        { L"8. Number of bytes in a file record segment: " },
        { L"9. Number of clusters in a file record segment: " },
        { L"10. Length of master file table (in bytes): " },
        { L"11. The starting logical cluster number of the master file table: " },
        { L"12. The starting logical cluster number of the master file table mirror: " },
        { L"13. The starting logical cluster number of the master file table zone: " },
        { L"14. The ending logical cluster number of the master file table zone: " }
};

LRESULT CALLBACK WNDProc_Info(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HFONT hFont;
    LOGFONT lf;

    switch (message) {
    case WM_CREATE: {
        WndMainInfo = true;
        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        hFont = CreateFont(15, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 400,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);

        char c = 'd';
        DriveInfo* driveInfo = getDriveInfo(c);
        std::vector<DriveData*> drives = getDrives();

        for (int i = 0; i < 15; i++) {
            HWND hLabel = CreateWindow(L"static", L"LBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 20, 10 + i * 30, 400, 30, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
            HWND hText = CreateWindow(L"static", L"LBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 350, 10 + i * 30, 100, 30, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
            SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
            wcsncpy_s(driveInfo->Drive, L"Hui", 10);
            wcsncpy_s(driveInfo->BytesPerFileRecordSegment, L"Hui", 10);
            SetWindowText(hLabel, headers[i]);
            SetWindowText(hText, (LPCWSTR)((size_t)driveInfo + i * sizeof(wchar_t) * 10));
        }
        break;
    }
    case WM_DESTROY:
        WndMainInfo = false;
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}