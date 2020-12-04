#include "Defragmenter.h"

LRESULT CALLBACK WNDProc_Defrag(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        WndMainDefrag = true;
        break;
    case WM_DESTROY:
        WndMainDefrag = false;
        break;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}