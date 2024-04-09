#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <windows.h>
#include <memory>
#include <iostream>

#include <vector>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void MakeWindow();

int main() 
{
    MakeWindow();

    return 0;
}

void MakeWindow() 
{
    int width = 1280;
    int height = 960;
    int canvasWidth = width / 80;
    int canvasHeight = height / 80;
    MSG message;

    WNDCLASSEX wndClass = {
        sizeof(WNDCLASSEX),      
        CS_CLASSDC, 
        WndProc, 
        0L,   
        0L,
        GetModuleHandle(NULL),   
        NULL,       
        NULL,    
        NULL, 
        NULL,
        L"SimpleEngine_DirectX", 
        NULL
    };

    RECT windowRange = { 0, 0, 1280, 960 };

    RegisterClassEx(&wndClass);
    AdjustWindowRect(&windowRange, WS_OVERLAPPEDWINDOW, TRUE);

    HWND hWnd = CreateWindow(wndClass.lpszClassName, wndClass.lpszClassName, WS_OVERLAPPEDWINDOW,
        300, 300, windowRange.right - windowRange.left, windowRange.bottom - windowRange.top,
        NULL, NULL, wndClass.hInstance, NULL);

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    while (GetMessage(&message, NULL, 0, 0)) 
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg) 
    {
        case WM_SIZE:
        
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_MOUSEMOVE:
            std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
            break;
        case WM_LBUTTONUP:
            std::cout << "Up\tLeft mouse button" << std::endl;
            break;
        case WM_RBUTTONUP:
            std::cout << "Up\tRight mouse button" << std::endl;
            break;
        case WM_KEYDOWN:
            std::cout << "KEYDOWN\t" << (int)wParam << std::endl;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}