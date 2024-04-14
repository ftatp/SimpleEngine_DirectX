#pragma once
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
    void RunWindow();
	
private:
	MainWindow& operator=(const MainWindow&) = delete;

    void InitWindowD3D11();
    void InitWindowImGui();

    void MakeScene();
    void UpdateScene();

    struct ConstantData
    {
        Matrix model;
        Matrix view;
        Matrix projection;
    };

    void CreateVertexBuffer(vector<Vector3> vertices);
    void CreateIndiceBuffer(vector<uint16_t> indices);
    void CreateConstantBuffer(ConstantData constantData);
    
public:

	HWND m_mainWindow;
	int m_windowWidth;
	int	m_windowHeight;
    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

private:
    static MainWindow* instancePtr;

    ComPtr<IDXGISwapChain> m_swapChain;                  // Made by D3D11CreateDeviceAndSwapChain()
    ComPtr<ID3D11Device> m_device;                       // Made by D3D11CreateDeviceAndSwapChain()
    ComPtr<ID3D11DeviceContext> m_deviceContext;         // Made by D3D11CreateDeviceAndSwapChain()
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;   // Made by device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
    ComPtr<ID3D11RasterizerState> m_rasterizerState;
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    D3D11_VIEWPORT m_viewport;

    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11Buffer> m_constantBuffer;

    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;


    ConstantData m_constantData = {};
};