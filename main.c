#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <winuser.h>

#include "checkers.c"

static Board board;
static BOOL white_to_move = 1;

const int WINDOW_W = 600;
const int WINDOW_H = WINDOW_W + 25;


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
    static COLORREF color_square_white = RGB(238, 238, 210);
    static COLORREF color_square_black = RGB(118, 150, 86);
    static COLORREF color_white = RGB(255, 255, 255);
    static COLORREF color_black = RGB(0, 0, 0);

    HBRUSH br_sq_white = CreateSolidBrush(color_square_white);
    HBRUSH br_sq_black = CreateSolidBrush(color_square_black);
    HBRUSH br_white = CreateSolidBrush(color_white);
    HBRUSH br_black = CreateSolidBrush(color_black);

    HPEN pen_white = CreatePen(PS_SOLID, 2, color_white);
    HPEN pen_black = CreatePen(PS_SOLID, 2, color_black);


    // begin paint
    // FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));

    // --- 1. The checkerboard
    int nsquares = 8;
    int square_size = WINDOW_W / nsquares;

    for (int row = 0; row < nsquares; row++) {
        int y = row * square_size;

        for (int col = 0; col < nsquares; col++) {
            int x = col * square_size;

            ps.rcPaint.left = x;
            ps.rcPaint.right = x + square_size;
            ps.rcPaint.top = y;
            ps.rcPaint.bottom = y + square_size;

            // Figured this out through trial and error
            BOOL white_square = (col % 2 != 0) ^ (row % 2 == 0);
            
            HBRUSH brush = (white_square)? br_sq_white : br_sq_black;
        
            FillRect(hdc, &ps.rcPaint, brush);
        }
    }

    // --- 2. The pieces
    SelectBrush(hdc, br_black);

    Ellipse(hdc, 10, 10, square_size - 10, square_size - 10);

    
    
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

    ShowWindow(hwnd, nShowCmd);

    // Now setup the chess board
    board_setup(&board);
    
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
