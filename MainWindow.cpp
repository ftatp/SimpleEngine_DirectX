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

void MainWindow::RunWindow()
{
    MSG message = {};

    ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
    UpdateWindow(m_mainWindow);

    MakeScene();

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

        UpdateScene();

        // Rendering
        m_deviceContext->RSSetViewports(1, &m_viewport);

        m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), (float*)&clearColor);
        m_deviceContext->ClearDepthStencilView(
            m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(),
            m_depthStencilView.Get());
        m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
        m_deviceContext->VSSetShader(m_vertexShader.Get(), 0, 0);
        m_deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
        m_deviceContext->RSSetState(m_rasterizerState.Get());
        m_deviceContext->PSSetShader(m_pixelShader.Get(), 0, 0);

        UINT stride = sizeof(Vector3);
        UINT offset = 0;
        m_deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
        m_deviceContext->IASetInputLayout(m_inputLayout.Get());
        m_deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
        m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_deviceContext->DrawIndexed(3, 0, 0);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        m_swapChain->Present(1, 0);
    }

    delete this;
}

void MainWindow::InitWindowD3D11()
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
    swapChainDescription.OutputWindow = m_mainWindow;
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
    
    m_viewport.Width = 1280;
    m_viewport.Height = 720;
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
        cout << "CreateTexture2D() failed." << endl;
    }

    if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView))) 
    {
        cout << "CreateDepthStencilView() failed." << endl;
    }

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true; // false
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc,
        m_depthStencilState.GetAddressOf()))) {
        cout << "CreateDepthStencilState() failed." << endl;
    }
#pragma endregion
}

void MainWindow::InitWindowImGui()
{
    ImGui::CreateContext();
    ImGui_ImplDX11_Init(m_device.Get(), m_deviceContext.Get());
    ImGui_ImplWin32_Init(m_mainWindow);
}

struct MeshData
{
    vector<Vector3> vertices;
    vector<uint16_t> indices;
};

shared_ptr<MeshData> MakeTriangle()
{
    shared_ptr<MeshData> triangleMeshData = make_shared<MeshData>();

    triangleMeshData->vertices.push_back({-1.0, -1.0, 0.0});
    triangleMeshData->vertices.push_back({ 0.0, 1.0, 0.0 });
    triangleMeshData->vertices.push_back({ 1.0, -1.0, 0.0 });

    triangleMeshData->indices.push_back(0);
    triangleMeshData->indices.push_back(1);
    triangleMeshData->indices.push_back(2);

    return triangleMeshData;
}

void MainWindow::CreateVertexBuffer(vector<Vector3> vertices)
{
    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.ByteWidth = UINT(sizeof(Vector3) * vertices.size());
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.StructureByteStride = UINT(sizeof(Vector3));

    D3D11_SUBRESOURCE_DATA vertexBufferSubresource;
    ZeroMemory(&vertexBufferSubresource, sizeof(D3D11_SUBRESOURCE_DATA));
    vertexBufferSubresource.pSysMem = vertices.data();

    const HRESULT isVertexBufferCreated =
        m_device->CreateBuffer(&vertexBufferDesc, &vertexBufferSubresource, m_vertexBuffer.GetAddressOf());

    if (FAILED(isVertexBufferCreated)) {
        std::cout << __FUNCTION__ << "failed. " << std::hex << isVertexBufferCreated << std::endl;
    };
}

void MainWindow::CreateIndiceBuffer(vector<uint16_t> indices)
{
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.StructureByteStride = UINT(sizeof(uint16_t));

    D3D11_SUBRESOURCE_DATA indexBufferSubresource;
    ZeroMemory(&indexBufferSubresource, sizeof(D3D11_SUBRESOURCE_DATA));
    indexBufferSubresource.pSysMem = indices.data();

    const HRESULT isIndexBufferCreated =
        m_device->CreateBuffer(&indexBufferDesc, &indexBufferSubresource, m_indexBuffer.GetAddressOf());

    if (FAILED(isIndexBufferCreated)) {
        std::cout << __FUNCTION__ << "failed. " << std::hex << isIndexBufferCreated << std::endl;
    };
}

void MainWindow::CreateConstantBuffer(ConstantData constantData)
{
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.ByteWidth = sizeof(constantData);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    constantBufferDesc.MiscFlags = 0;
    constantBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA constantBufferSubresource;
    ZeroMemory(&constantBufferSubresource, sizeof(D3D11_SUBRESOURCE_DATA));
    constantBufferSubresource.pSysMem = &constantBufferDesc;

    const HRESULT isConstantxBufferCreated =
        m_device->CreateBuffer(&constantBufferDesc, &constantBufferSubresource, m_constantBuffer.GetAddressOf());

    if (FAILED(isConstantxBufferCreated)) {
        std::cout << __FUNCTION__ << "failed. " << std::hex << isConstantxBufferCreated << std::endl;
    };
}

void MainWindow::MakeScene()
{
    shared_ptr<MeshData> exampleTriangle = MakeTriangle();

    m_constantData.model = Matrix();
    m_constantData.view = Matrix();
    m_constantData.projection = Matrix();

    CreateVertexBuffer(exampleTriangle->vertices);
    CreateIndiceBuffer(exampleTriangle->indices);
    CreateConstantBuffer(m_constantData);

    vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutElements = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    HRESULT isVertexShaderCompiled = 
        D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &shaderBlob, &errorBlob);

    if (FAILED(isVertexShaderCompiled)) {
        // 파일이 없을 경우
        if ((isVertexShaderCompiled & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
            cout << "File not found." << endl;
        }

        // 에러 메시지가 있으면 출력
        if (errorBlob) {
            cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
        }
    }

    m_device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 
        nullptr, &m_vertexShader);
    m_device->CreateInputLayout(inputLayoutElements.data(), UINT(inputLayoutElements.size()), 
        shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &m_inputLayout);

    HRESULT isPixelShaderCompiled =
        D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &shaderBlob, &errorBlob);

    if (FAILED(isPixelShaderCompiled)) {
        // 파일이 없을 경우
        if ((isPixelShaderCompiled & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
            cout << "File not found." << endl;
        }

        // 에러 메시지가 있으면 출력
        if (errorBlob) {
            cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
        }
    }

    m_device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
        nullptr, &m_pixelShader);
}

void MainWindow::UpdateScene()
{
    m_constantData.model =
        Matrix::CreateScale(Vector3(1.0f)) * Matrix::CreateRotationX(0.0f) *
        Matrix::CreateRotationY(0.0f) * Matrix::CreateRotationZ(0.0f) *
        Matrix::CreateTranslation(Vector3(0.0f));
    m_constantData.model = m_constantData.model.Transpose();

    m_constantData.view = DirectX::XMMatrixLookToLH({ 0.0f, 0.0f, -2.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
    m_constantData.view = m_constantData.view.Transpose();

    m_constantData.projection = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(70.0f), float(m_windowWidth) / m_windowHeight, 0.5f, 100.0f);

    m_constantData.projection = m_constantData.projection.Transpose();

    D3D11_MAPPED_SUBRESOURCE ms;
    m_deviceContext->Map(m_constantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, &m_constantData, sizeof(m_constantData));
    m_deviceContext->Unmap(m_constantBuffer.Get(), NULL);
}