#include "MainWindow.h"

#include <iostream>


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

std::string GetErrorMessageFromHRESULT(HRESULT hr) {
    LPSTR messageBuffer = nullptr;
    DWORD dwBufferLength = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&messageBuffer),
        0,
        nullptr);
    if (dwBufferLength == 0) {
        // Failed to retrieve error message
        return "Unknown error.";
    }

    // Convert message buffer to string
    std::string errorMessage(messageBuffer, dwBufferLength);

    // Free the message buffer
    LocalFree(messageBuffer);

    // Remove trailing newline characters
    while (!errorMessage.empty() && (errorMessage.back() == '\r' || errorMessage.back() == '\n')) {
        errorMessage.pop_back();
    }

    return errorMessage;
}


MainWindow* MainWindow::instancePtr = nullptr;

MainWindow::MainWindow()
{
	m_windowWidth = 1280;
	m_windowHeight = 960;
}

MainWindow::~MainWindow()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    m_renderTargetView->Release();
    m_swapChain->Release();
    m_deviceContext->Release();
    m_device->Release();

    DestroyWindow(m_mainWindow);
}

void MainWindow::MakeWindow()
{
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

    RECT windowRange = { 0, 0, m_windowWidth, m_windowHeight };

    RegisterClassEx(&wndClass);
    AdjustWindowRect(&windowRange, WS_OVERLAPPEDWINDOW, TRUE);

    m_mainWindow = CreateWindow(wndClass.lpszClassName, wndClass.lpszClassName, WS_OVERLAPPEDWINDOW,
        50, 50, windowRange.right - windowRange.left, windowRange.bottom - windowRange.top,
        nullptr, nullptr, wndClass.hInstance, nullptr);

    return;
}


void MainWindow::InitWindowApiProperties()
{
    InitWindowD3D11();
    InitWindowImGui();
}

void MainWindow::DisplayWindow()
{
    MSG message = {};

    ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
    UpdateWindow(m_mainWindow);

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
        m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
        m_deviceContext->ClearRenderTargetView(m_renderTargetView, (float*)&clearColor);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        m_swapChain->Present(1, 0);
    }

    delete this;
}

void MainWindow::InitWindowD3D11()
{
#pragma region SwapChain
    const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };

    DXGI_SWAP_CHAIN_DESC swapChainDescription;
    swapChainDescription.BufferCount = 2;
    swapChainDescription.BufferDesc.Width = m_windowWidth;
    swapChainDescription.BufferDesc.Height = m_windowHeight;
    swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescription.OutputWindow = m_mainWindow;
    swapChainDescription.SampleDesc.Count = 1;
    swapChainDescription.SampleDesc.Quality = 0;
    swapChainDescription.Windowed = TRUE;
    swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching
    swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &swapChainDescription, //inputs
        &m_swapChain, &m_device, nullptr, &m_deviceContext); //ouputs

    if (FAILED(result))
    {
        std::cout << "D3D11CreateDeviceAndSwapChain() error" << std::endl;
        // Retrieve the DXGI debug interface
        std::string errorMessage = GetErrorMessageFromHRESULT(result);
        
        exit(-1);
    }
#pragma endregion

#pragma region RenderTarget
    ID3D11Texture2D* pBackBuffer = nullptr;

    m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    if (pBackBuffer != nullptr)
    {
        m_device->CreateRenderTargetView(pBackBuffer, nullptr, &m_renderTargetView);
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
    m_deviceContext->RSSetViewports(1, &viewport);

#pragma endregion
}

void MainWindow::InitWindowImGui()
{
    ImGui::CreateContext();
    ImGui_ImplDX11_Init(m_device, m_deviceContext);
    ImGui_ImplWin32_Init(m_mainWindow);
}
