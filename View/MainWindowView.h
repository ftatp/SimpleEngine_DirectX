#pragma once
#include "../EventSystem/Event.h"

#include <imgui.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <directxtk/SimpleMath.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgidebug.h>

#include <Windows.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace view 
{
	class MainWindowView
	{
	public:
		MainWindowView();
		~MainWindowView();

		void InitWindowApiProperties();
		void UpdateScene();
		void RenderImGui();
		void RenderScene();

		HWND GetWindow();
		int GetWindowWidth();
		int GetWindowHeight();

		ComPtr<ID3D11Device> GetDevice();
		ComPtr<ID3D11DeviceContext> GetDeviceContext();

		EventSystem::Event<> OnAddObjectButtonClickEvent;

	private:
		void InitWindowD3D11();
		void InitWindowImGui();

		const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		WNDCLASSEX m_wndClass;
		HWND m_window;
		int m_windowWidth;
		int	m_windowHeight;

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

	
}