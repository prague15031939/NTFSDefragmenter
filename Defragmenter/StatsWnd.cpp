#include "Defragmenter.h"
#define PI 3.14159265 

DefragmentationStats stats;
void DrawPie(HDC hdc, double radius, int cx, int cy, int& x0, int& y0, int& x1, int& y1, double angle);
double TransformStatToAngle(long param, DefragmentationStats sts);
void MoveRect(RECT* rc, int offset);

LRESULT CALLBACK WNDProc_Stats(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HFONT hFont;
    LOGFONT lf;
    switch (message) {
    case WM_CREATE: {
        WndMainStats = true;
        stats = getDefragmentationStats();
        stats.filesDefragmented = 10;
        stats.filesErrorDefragmented = 1; //Test DataSet
        stats.filesNotTouched = 200;
        HWND hLabel = CreateWindow(L"static", L"StartLBL", WS_CHILD | WS_VISIBLE | WS_TABSTOP, 20, 20, 200, 30, hwnd, (HMENU)IDC_STARTLBL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);
        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
        hFont = CreateFont(30, lf.lfWidth,
            lf.lfEscapement, lf.lfOrientation, 700,
            lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
            lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
            lf.lfPitchAndFamily, lf.lfFaceName);
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        SetWindowText(hLabel, L"Disk Statistics");
    }
                  break;
    case WM_DESTROY:
        WndMainStats = false;
        break;

    case WM_PAINT:
    {
        double radius = 150;
        int x0 = 200;
        int y0 = 80;
        int x1 = 200;
        int y1 = 80;
        int cx = 50 + radius;
        int cy = 80 + radius;
        PAINTSTRUCT ps;
        RECT rc_rect;
        RECT rc_text;

        rc_rect.top = 80;
        rc_rect.left = 400;
        rc_rect.bottom = 120;
        rc_rect.right = 440;

        rc_text.top = 80;
        rc_text.left = 445;
        rc_text.bottom = 120;
        rc_text.right = 650;

        if (stats.filesDefragmented + stats.filesErrorDefragmented + stats.filesNotTouched < 1)
            DestroyWindow(hwnd);
        else {
            HDC hdc = BeginPaint(hwnd, &ps);
            HPEN hpenOld = static_cast<HPEN>(SelectObject(hdc, GetStockObject(DC_PEN)));
            HBRUSH hbrushOld = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(NULL_BRUSH)));

            SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 255)));
            DrawPie(hdc, radius, cx, cy, x0, y0, x1, y1, TransformStatToAngle(stats.filesNotTouched, stats));
            FillRect(hdc, &rc_rect, CreateSolidBrush(RGB(0, 0, 255)));
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, TEXT("Files Not Touched"), lstrlen(TEXT("Files Not Touched")), &rc_text, DT_CENTER);

            SelectObject(hdc, CreateSolidBrush(RGB(0, 255, 0)));
            DrawPie(hdc, radius, cx, cy, x0, y0, x1, y1, TransformStatToAngle(stats.filesDefragmented, stats) + TransformStatToAngle(stats.filesNotTouched, stats));
            MoveRect(&rc_rect, 80);
            FillRect(hdc, &rc_rect, CreateSolidBrush(RGB(0, 255, 0)));
            SetBkMode(hdc, TRANSPARENT);
            MoveRect(&rc_text, 80);
            DrawText(hdc, TEXT("Files Defragmented"), lstrlen(TEXT("Files Defragmented")), &rc_text, DT_CENTER);

            SelectObject(hdc, CreateSolidBrush(RGB(255, 0, 0)));
            DrawPie(hdc, radius, cx, cy, x0, y0, x1, y1, TransformStatToAngle(stats.filesErrorDefragmented, stats) + TransformStatToAngle(stats.filesDefragmented, stats) + TransformStatToAngle(stats.filesNotTouched, stats) + 1);
            MoveRect(&rc_rect, 80);
            FillRect(hdc, &rc_rect, CreateSolidBrush(RGB(255, 0, 0)));
            SetBkMode(hdc, TRANSPARENT);
            MoveRect(&rc_text, 80);
            DrawText(hdc, TEXT("Files Error Defragmented"), lstrlen(TEXT("Files Error Defragmented")), &rc_text, DT_CENTER);


            SelectObject(hdc, hpenOld);
            SelectObject(hdc, hbrushOld);
            EndPaint(hwnd, &ps);
        }
        
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);

}

void DrawPie(HDC hdc,double radius, int cx, int cy, int &x0, int &y0, int &x1, int &y1, double angle) {
    x1 = cx + radius * sin(angle * PI / 180);
    y1 = cy - radius * cos(angle * PI / 180);
    Pie(hdc, 50, 80, 50 + radius * 2, 80 + radius * 2, x1, y1, x0, y0);
    x0 = x1;
    y0 = y1;
}

double TransformStatToAngle(long param, DefragmentationStats sts) {
    long sum = sts.filesDefragmented + sts.filesNotTouched + sts.filesErrorDefragmented;
    double angle = (360 * param) / sum;
    return angle;
}

void MoveRect(RECT* rc, int offset) {
    rc->top = rc->top + offset;
    rc->bottom = rc->bottom + offset;
}
