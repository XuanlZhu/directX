#include "myHead.h"


//默认的窗口消息处理函数
LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_MOUSEMOVE:  
        
        break;
        case WM_LBUTTONDOWN:
			
            break;
        case WM_LBUTTONUP:
            
            
            break;
        case WM_RBUTTONDOWN:
			
            break;
        case WM_RBUTTONUP:
           
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        /*case WM_MOUSEHWHEEL:*/
            break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    HWND hwnd= CGraphic::GetSingleObjPtr()->InitWindow(WndProc);

    //加入dx环境
    CGraphic::GetSingleObjPtr()->InitDX(hwnd);

	
    MSG msg;
    
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
           
            CGraphic::GetSingleObjPtr()->BeginDraw();
           
            CGraphic::GetSingleObjPtr()->EndDraw();
        }
    }
    return 0;
}