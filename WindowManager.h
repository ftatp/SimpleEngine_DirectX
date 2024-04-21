#pragma once

#include "ObjectManager.h"
#include "View/MainWindowView.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

//#include <directxtk/SimpleMath.h>

#include <windows.h>
#include <memory>
#include <iostream>

#include <wrl.h> // ComPtr

#include <vector>

using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

//struct ConstantData
//{
//    Matrix model;
//    Matrix view;
//    Matrix projection;
//};

class WindowManager
{
public:
    static WindowManager* GetInstance()
    {
        if (m_instancePtr == nullptr)
        {
            m_instancePtr = new WindowManager();
        }

        return m_instancePtr;
    }
	
	WindowManager();
    ~WindowManager();

	void MakeWindow();
    //void InitWindowApiProperties();
    void RunWindow();

    ComPtr<ID3D11Device> GetDevice();

private:
	WindowManager& operator=(const WindowManager&) = delete;

    //void InitWindowD3D11();
    //void InitWindowImGui();

    void MakeScene();
    void UpdateScene();
    void RenderScene();


    shared_ptr<view::MainWindowView> m_mainWindowView;

    static WindowManager* m_instancePtr;

    //ComPtr<IDXGISwapChain> m_swapChain;                  // Made by D3D11CreateDeviceAndSwapChain()
    //ComPtr<ID3D11Device> m_device;                       // Made by D3D11CreateDeviceAndSwapChain()
    //ComPtr<ID3D11DeviceContext> m_deviceContext;         // Made by D3D11CreateDeviceAndSwapChain()
    //ComPtr<ID3D11RenderTargetView> m_renderTargetView;   // Made by device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
    //ComPtr<ID3D11RasterizerState> m_rasterizerState;
    //ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    //ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    //ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    //D3D11_VIEWPORT m_viewport;
};