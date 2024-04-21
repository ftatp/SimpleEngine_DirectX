#include "MainWindowView.h"

#include <iostream>

namespace view
{
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

	MainWindowView::MainWindowView()
	{
        m_windowWidth = 1280;
        m_windowHeight = 960;

        WNDCLASSEX m_wndClass = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(nullptr),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        L"SimpleEngine_DirectX",
        nullptr
        };

        RECT windowRange = { 0, 0, m_windowWidth, m_windowHeight };

        RegisterClassEx(&m_wndClass);
        AdjustWindowRect(&windowRange, WS_OVERLAPPEDWINDOW, TRUE);

        m_window = CreateWindow(m_wndClass.lpszClassName, m_wndClass.lpszClassName,
            WS_OVERLAPPEDWINDOW,
            50, 50, windowRange.right - windowRange.left, windowRange.bottom - windowRange.top,
            nullptr, nullptr, m_wndClass.hInstance, nullptr);
	}

	MainWindowView::~MainWindowView()
	{
        DestroyWindow(m_window);
	}

    HWND MainWindowView::GetWindow()
    {
        return m_window;
    }

    int MainWindowView::GetWindowWidth()
    {
        return m_windowWidth;
    }

    int MainWindowView::GetWindowHeight()
    {
        return m_windowHeight;
    }
}