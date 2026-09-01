#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd,msg,wParam,lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int)
{

    // 注册窗口
    WNDCLASS wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = reinterpret_cast<LPCSTR>(L"MyWindow");

    RegisterClass(&wc);
    // 创建窗口
    HWND hWnd = CreateWindow(
        reinterpret_cast<LPCSTR>(L"MyWindow"),
        reinterpret_cast<LPCSTR>(L"Hello Window"),

        WS_OVERLAPPEDWINDOW,

        100,
        100,

        800,
        600,

        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    // 显示
    ShowWindow(hWnd,SW_SHOW);

    // 消息循环
    MSG msg = {};
    while(GetMessage(&msg,nullptr,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}