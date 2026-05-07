#include <windows.h>
#include <winuser.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // paint the window

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);
            
        } break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } break;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const char* class_name = "MinChess";
    const char* window_text = "MinChess";

    // create window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = class_name;

    RegisterClass(&wc);
    
    // create a window

    int width = 800;
    int height = 600;

    HWND hwnd = CreateWindowEx(
        0,
        class_name,
        window_text,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width, height,
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
