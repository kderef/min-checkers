#include <debugapi.h>
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <winuser.h>

#include "checkers.c"

static BOOL white_to_move = 1;

const int WINDOW_W = 600;
const int WINDOW_H = WINDOW_W + 25;

static const COLORREF
    color_square_white = RGB(238, 238, 210),
    color_square_black = RGB(118, 150, 86),
    color_white = RGB(255, 255, 255),
    color_black = RGB(0, 0, 0)
;

static HBRUSH br_sq_white, br_sq_black, br_white, br_black;


typedef struct {
    HWND hwnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
} WindowState;

void WindowPaint(const WindowState* ws) {
    static PAINTSTRUCT ps;
    HDC hdc = BeginPaint(ws->hwnd, &ps);

    // brushes

    HPEN pen_white = CreatePen(PS_SOLID, 2, color_white);
    HPEN pen_black = CreatePen(PS_SOLID, 2, color_black);

    HBRUSH old_brush;


    // begin paint

    FillRect(hdc, &ps.rcPaint, br_sq_white);

    // --- 1. The checkerboard
    int nsquares = 8;
    int square_size = WINDOW_W / nsquares;

    for (int row = 0; row < nsquares; row++) {
        int y = row * square_size;

        for (int col = row % 2; col < nsquares; col += 2) {
            int x = col * square_size;

            ps.rcPaint.left = x;
            ps.rcPaint.right = x + square_size;
            ps.rcPaint.top = y;
            ps.rcPaint.bottom = y + square_size;
            
            FillRect(hdc, &ps.rcPaint, br_sq_black);

            // Draw the piece            
            int board_x = (col / 2);
            int board_y = row;

            switch (board_get(board_x, board_y)) {
                case PIECE_NONE: break;
                case PIECE_BLACK:
                    old_brush = SelectBrush(hdc, br_black);
                    Ellipse(hdc, x + 10, y + 10, x + square_size - 10, y + square_size - 10);
                    SelectBrush(hdc, old_brush);
                    break;
                case PIECE_WHITE:
                    old_brush = SelectBrush(hdc, br_white);
                    Ellipse(hdc, x + 10, y + 10, x + square_size - 10, y + square_size - 10);
                    SelectBrush(hdc, old_brush);
                    break;
            }
        }
    }

    // --- 2. The pieces

    
    
    EndPaint(ws->hwnd, &ps);
    // end paint
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    WindowState window_state = {
        .hwnd = hwnd,
        .msg = uMsg,
        .wParam = wParam,
        .lParam = lParam
    };
    
    switch (uMsg) {
        case WM_PAINT:
            WindowPaint(&window_state);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            break;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const char* class_name = "MinCheckers";
    const char* window_text = "MinCheckers";

    // create window class

    WNDCLASS wc = {0};

#if 0
    SHSTOCKICONINFO sii = {0};
    sii.cbSize = sizeof(SHSTOCKICONINFO);
    SHGetStockIconInfo(SIID_SOFTWARE, SHGSI_ICON, &sii);
    wc.hIcon = sii.hIcon;
    SHGetStockIconInfo(SIID_SOFTWARE, SHGSI_ICON | SHGSI_SMALLICON, &sii);
    wc.hIconSm = sii.hIcon;    
#endif

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = class_name;

    RegisterClass(&wc);
    
    // create a window


    long window_style = WS_OVERLAPPED     |
                        WS_CAPTION        |
                        WS_SYSMENU        |
                        WS_MINIMIZEBOX;

    HWND hwnd = CreateWindowEx(
        0,
        class_name,
        window_text,
        window_style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WINDOW_W + 6, // HACK: window width is not enough
        WINDOW_H,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == 0) {
        return 1;
    }

    // Set brushes
    br_sq_black = CreateSolidBrush(color_square_black);
    br_sq_white = CreateSolidBrush(color_square_white);
    br_white = CreateSolidBrush(color_white);
    br_black = CreateSolidBrush(color_black);

    ShowWindow(hwnd, nShowCmd);
    
    // message loop

    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void WinMainCRTStartup(void) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    int result = WinMain(hInstance, NULL, NULL, SW_SHOWDEFAULT);
    ExitProcess(result);
}
