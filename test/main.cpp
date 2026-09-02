#include <chrono>
#include <iostream>
#include <thread>
#include <Windows.h>

#include "Core/Game.h"
#include "Global.h"
#include "Core/Graphic.h"


LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd,msg,wParam,lParam);
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,int nCmdShow)
{

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindow";

    RegisterClass(&wc);

    AllocConsole();
    freopen("CONOUT$","w",stdout);
    // std::cout << "开始图" << std::endl;


    HWND hWnd = CreateWindow(
        "MyWindow",
        "GAME",
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


    ShowWindow(hWnd,nCmdShow);

    // 初始化
    Global::game = new Game();
    // 创建Graphic
    Global::graphic->Initialize(hWnd);
    Global::game->Setup();

    MSG msg = {};
    const double frameTime = 1.0 / 60.0;

    auto nowTime = GetNowTime();

    while (true)
    {
        auto frameStart = std::chrono::high_resolution_clock::now();
        // 处理窗口消息
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                return 0;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }


        float deltaTime = GetNowTime()-nowTime;
        nowTime = GetNowTime();
        // 游戏循环
        Global::game->Mainloop(deltaTime);


        // 帧率控制
        auto frameEnd = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(frameEnd - frameStart).count();

        if(elapsed < frameTime)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(frameTime - elapsed));
        }
    }


    return 0;
}