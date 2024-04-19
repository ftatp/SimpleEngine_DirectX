#pragma once

#include "ObjectManager.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <directxtk/SimpleMath.h>

#include <windows.h>
#include <memory>
#include <iostream>

#include <wrl.h> // ComPtr

#include <vector>

using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//struct ConstantData
//{
//    Matrix model;
//    Matrix view;
//    Matrix projection;
//};

class MainWindow
{
public:
    static MainWindow* GetInstance()
    {
        if (m_instancePtr == nullptr)
        {
            m_instancePtr = new MainWindow();
        }

        return m_instancePtr;
    }
	
	MainWindow();
    ~MainWindow();

	void MakeWindow();
    void InitWindowApiProperties();
    void RunWindow();

    ComPtr<ID3D11Device> GetDevice();

private:
	MainWindow& operator=(const MainWindow&) = delete;

    void InitWindowD3D11();
    void InitWindowImGui();

    void MakeScene();
    void UpdateScene();
    void RenderScene();

public:

	HWND m_mainWindow;
	int m_windowWidth;
	int	m_windowHeight;
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

private:
    static MainWindow* m_instancePtr;

    ComPtr<IDXGISwapChain> m_swapChain;                  // Made by D3D11CreateDeviceAndSwapChain()
    ComPtr<ID3D11Device> m_device;                       // Made by D3D11CreateDeviceAndSwapChain()
    ComPtr<ID3D11DeviceContext> m_deviceContext;         // Made by D3D11CreateDeviceAndSwapChain()
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;   // Made by device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    D3D11_VIEWPORT m_viewport;
};