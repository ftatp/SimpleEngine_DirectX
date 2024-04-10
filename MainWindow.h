#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>

#include <windows.h>
#include <memory>
#include <iostream>

#include <vector>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class MainWindow
{
public:
    static MainWindow* GetInstance()
    {
        if (instancePtr == nullptr)
        {
            instancePtr = new MainWindow();
        }

        return instancePtr;
    }
	
	MainWindow();
    ~MainWindow();

	void MakeWindow();
    void InitWindowApiProperties();
    void DisplayWindow();
	
private:
	MainWindow& operator=(const MainWindow&) = delete;

    void InitWindowD3D11();
    void InitWindowImGui();

public:

	HWND m_mainWindow;
	int m_windowWidth;
	int	m_windowHeight;
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    IDXGISwapChain* m_swapChain;                  // Made by D3D11CreateDeviceAndSwapChain()
    ID3D11Device* m_device;                       // Made by D3D11CreateDeviceAndSwapChain()
    ID3D11DeviceContext* m_deviceContext;         // Made by D3D11CreateDeviceAndSwapChain()
    ID3D11RenderTargetView* m_renderTargetView;   // Made by device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);


private:
    static MainWindow* instancePtr;
};