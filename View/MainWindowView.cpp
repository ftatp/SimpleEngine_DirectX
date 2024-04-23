#include "MainWindowView.h"
#include "../SceneManager.h"

#include <iostream>
#include <string>

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

namespace view
{
	MainWindowView::MainWindowView()
	{
        m_windowWidth = 1280;
        m_windowHeight = 720;

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
        m_renderTargetView->Release();
        m_swapChain->Release();
        m_deviceContext->Release();
        m_device->Release();

        DestroyWindow(m_window);
	}

    void MainWindowView::InitWindowApiProperties()
    {
        InitWindowD3D11();
        InitWindowImGui();
    }

    void MainWindowView::UpdateScene()
    {
        auto objects = SceneManager::GetInstance()->GetCurrentScene()->GetObjectList();

        for (int i = 0; i < objects.size(); i++)
        {
            ConstantData constantData = {};

            constantData.model = objects[i]->GetModelTransform();
            constantData.model = constantData.model.Transpose();

            constantData.view = DirectX::XMMatrixLookToLH({ 0.0f, 0.0f, -2.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
            constantData.view = constantData.view.Transpose();

            constantData.projection = DirectX::XMMatrixPerspectiveFovLH(
                DirectX::XMConvertToRadians(70.0f), float(m_windowWidth) / m_windowHeight, 0.5f, 100.0f);

            constantData.projection = constantData.projection.Transpose();

            objects[i]->SetConstantData(constantData);
        }
    }

    void MainWindowView::RenderImGui()
    {
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is some text");
        if (ImGui::Button("Add Object"))
        {
            OnAddObjectButtonClickEvent.Trigger();
        }

        auto objects = SceneManager::GetInstance()->GetCurrentScene()->GetObjectList();

        for (int i = 0; i < objects.size(); i++)
        {
            auto object = objects[i];
            ImGui::BeginGroup();
            ImGui::SliderFloat3(("Translation" + std::to_string(i)).data(), &(objects[i]->GetTranslationPtr()->x), -2.0f, 2.0f);
            ImGui::SliderFloat3(("Rotation" + std::to_string(i)).data(), &(objects[i]->GetRotationPtr()->x), -2.0f, 2.0f);
            ImGui::SliderFloat3(("Scale" + std::to_string(i)).data(), &(objects[i]->GetScalePtr()->x), -2.0f, 2.0f);
            ImGui::EndGroup();
        }

        ImGui::End();
        ImGui::Render();
    }

    void MainWindowView::RenderScene()
    {
        m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), (float*)&clearColor);
        m_deviceContext->ClearDepthStencilView(
            m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_deviceContext->RSSetViewports(1, &m_viewport);
        m_deviceContext->RSSetState(m_rasterizerState.Get());

        m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
        m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

        auto objects = SceneManager::GetInstance()->GetCurrentScene()->GetObjectList();

        for (int i = 0; i < objects.size(); i++)
        {

            auto vertexBuffer = objects[i]->GetVertexBuffer();
            auto inputLayout = objects[i]->GetInputLayout();
            auto indexBuffer = objects[i]->GetIndexBuffer();
            auto vertexShader = objects[i]->GetVertexShader();
            auto constantBuffer = objects[i]->GetConstantBuffer();
            auto pixelShader = objects[i]->GetPixelShader();

            auto constantData = objects[i]->GetConstantData();

            UINT stride = sizeof(object::Vertex);
            UINT offset = 0;
            D3D11_MAPPED_SUBRESOURCE ms;
            m_deviceContext->Map(constantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
            memcpy(ms.pData, &constantData, sizeof(constantData));
            m_deviceContext->Unmap(constantBuffer.Get(), NULL);

            m_deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
            m_deviceContext->IASetInputLayout(inputLayout.Get());
            m_deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

            m_deviceContext->VSSetShader(vertexShader.Get(), 0, 0);
            m_deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
            m_deviceContext->PSSetShader(pixelShader.Get(), 0, 0);

            m_deviceContext->DrawIndexed(objects[i]->GetMesh()->GetIndices().size(), 0, 0);
        }

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        m_swapChain->Present(1, 0);
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

    ComPtr<ID3D11Device> MainWindowView::GetDevice()
    {
        return m_device;
    }

    ComPtr<ID3D11DeviceContext> MainWindowView::GetDeviceContext()
    {
        return m_deviceContext;
    }

    void MainWindowView::InitWindowD3D11()
    {
#pragma region Device&SwapChain
        const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };

        DXGI_SWAP_CHAIN_DESC swapChainDescription;
        swapChainDescription.BufferCount = 2;
        swapChainDescription.BufferDesc.Width = m_windowWidth;
        swapChainDescription.BufferDesc.Height = m_windowHeight;
        swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDescription.OutputWindow = m_window;
        swapChainDescription.SampleDesc.Count = 1;
        swapChainDescription.SampleDesc.Quality = 0;
        swapChainDescription.Windowed = TRUE;
        swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching
        swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

        HRESULT result = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 1, D3D11_SDK_VERSION, &swapChainDescription, //inputs
            &m_swapChain, &m_device, nullptr, &m_deviceContext); //ouputs

        if (FAILED(result))
        {
            std::cout << "D3D11CreateDeviceAndSwapChain() error" << std::endl;
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

        m_viewport.Width = m_windowWidth;
        m_viewport.Height = m_windowHeight;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0;
        m_viewport.TopLeftY = 0;
        m_deviceContext->RSSetViewports(1, &m_viewport);

#pragma endregion

#pragma region RasterizerState
        D3D11_RASTERIZER_DESC rasterizerDesc;
        ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
        rasterizerDesc.FillMode = D3D11_FILL_SOLID;
        rasterizerDesc.CullMode = D3D11_CULL_BACK;
        rasterizerDesc.FrontCounterClockwise = false;
        rasterizerDesc.DepthClipEnable = true;

        m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
#pragma endregion

#pragma region DepthBuffer
        D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
        ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
        depthStencilBufferDesc.Width = m_windowWidth;
        depthStencilBufferDesc.Height = m_windowHeight;
        depthStencilBufferDesc.MipLevels = 1;
        depthStencilBufferDesc.ArraySize = 1;
        depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilBufferDesc.SampleDesc.Count = 1;
        depthStencilBufferDesc.SampleDesc.Quality = 0;
        depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilBufferDesc.CPUAccessFlags = 0;
        depthStencilBufferDesc.MiscFlags = 0;

        if (FAILED(m_device->CreateTexture2D(&depthStencilBufferDesc, 0, m_depthStencilBuffer.GetAddressOf())))
        {
            std::cout << "CreateTexture2D() failed." << std::endl;
        }

        if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView)))
        {
            std::cout << "CreateDepthStencilView() failed." << std::endl;
        }

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
        depthStencilDesc.DepthEnable = true; // false
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

        if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc,
            m_depthStencilState.GetAddressOf()))) {
            std::cout << "CreateDepthStencilState() failed." << std::endl;
        }
#pragma endregion
    }

    void MainWindowView::InitWindowImGui()
    {
        ImGui_ImplDX11_Init(m_device.Get(), m_deviceContext.Get());
        ImGui_ImplWin32_Init(m_window);
    }
    
}


