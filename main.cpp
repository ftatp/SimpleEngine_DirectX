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
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

const int m_screenWidth = 1280;
const int m_screenHeight = 960;

const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

IDXGISwapChain* swapChain;                  // Made by D3D11CreateDeviceAndSwapChain()
ID3D11Device* device;                       // Made by D3D11CreateDeviceAndSwapChain()
ID3D11DeviceContext* deviceContext;         // Made by D3D11CreateDeviceAndSwapChain()
ID3D11RenderTargetView* renderTargetView;   // Made by device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);

HWND MakeWindow();

void InitD3D11(HWND hWnd);
void InitImGui(HWND hWnd);

int main()
{
    MSG message = {};

    HWND hWnd = MakeWindow();

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    InitD3D11(hWnd);
    InitImGui(hWnd);

    while (message.message != WM_QUIT)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
            continue;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is some text");
        ImGui::End();
        ImGui::Render();

        // Rendering
        deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
        deviceContext->ClearRenderTargetView(renderTargetView, (float*)&clearColor);
        
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        swapChain->Present(1, 0);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    renderTargetView->Release();
    swapChain->Release();
    deviceContext->Release();
    device->Release();
    
    DestroyWindow(hWnd);

    return 0;
}

HWND MakeWindow() 
{
    int canvasWidth = m_screenWidth / 80;
    int canvasHeight = m_screenHeight / 80;

    WNDCLASSEX wndClass = {
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

    RECT windowRange = { 0, 0, m_screenWidth, m_screenHeight };

    RegisterClassEx(&wndClass);
    AdjustWindowRect(&windowRange, WS_OVERLAPPEDWINDOW, TRUE);

    HWND hWnd = CreateWindow(wndClass.lpszClassName, wndClass.lpszClassName, WS_OVERLAPPEDWINDOW,
        50, 50, windowRange.right - windowRange.left, windowRange.bottom - windowRange.top,
        nullptr, nullptr, wndClass.hInstance, nullptr);

    return hWnd;
}

void InitD3D11(HWND hWnd)
{
#pragma region SwapChain
    const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };

    DXGI_SWAP_CHAIN_DESC swapChainDescription;
    swapChainDescription.BufferCount = 2;
    swapChainDescription.BufferDesc.Width = m_screenWidth;
    swapChainDescription.BufferDesc.Height = m_screenHeight;
    swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescription.OutputWindow = hWnd;
    swapChainDescription.SampleDesc.Count = 1;
    swapChainDescription.SampleDesc.Quality = 0;
    swapChainDescription.Windowed = TRUE;
    swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching
    swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0; // D3D11_CREATE_DEVICE_DEBUG;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevelArray, 1, D3D11_SDK_VERSION, &swapChainDescription, //inputs
        &swapChain, &device, nullptr, &deviceContext); //ouputs

    if (result < 0)
    {
        std::cout << "D3D11CreateDeviceAndSwapChain() error" << std::endl;
        exit(-1);
    }
#pragma endregion

#pragma region RenderTarget
    ID3D11Texture2D* pBackBuffer = nullptr;
    
    swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    
    if (pBackBuffer != nullptr) 
    {
        device->CreateRenderTargetView(pBackBuffer, nullptr, &renderTargetView);
        pBackBuffer->Release();
    }
    else
    {
        std::cout << "CreateRenderTargetView() error" << std::endl;
        exit(-1);
    }
#pragma endregion

#pragma region Viewport
    // Setup viewport
    D3D11_VIEWPORT viewport;
    viewport.Width = 1280;
    viewport.Height = 720;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    deviceContext->RSSetViewports(1, &viewport);

#pragma endregion
}

void InitImGui(HWND hWnd)
{
    ImGui::CreateContext();
    ImGui_ImplDX11_Init(device, deviceContext);
    ImGui_ImplWin32_Init(hWnd);
}


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
