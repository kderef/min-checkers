#include <minwindef.h>
#include <windef.h>
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>

const int WINDOW_W = 600;
const int WINDOW_H = WINDOW_W + 25;

typedef struct {
    HWND hwnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
} WindowState;

void WindowPaint(const WindowState* ws) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(ws->hwnd, &ps);

    // brushes
    COLORREF white = RGB(240, 240, 240);
    COLORREF black = RGB(20, 20, 20);

    HBRUSH br_white = CreateSolidBrush(white);
    HBRUSH br_black = CreateSolidBrush(black);

    // begin paint
    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));

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
            
            HBRUSH brush = (white_square)? br_white : br_black;
        
            FillRect(hdc, &ps.rcPaint, brush);
        }
    }
    
    
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
    const char* class_name = "MinChess";
    const char* window_text = "MinChess";

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
