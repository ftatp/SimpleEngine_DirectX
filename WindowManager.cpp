#include "WindowManager.h"
#include "View/ViewController.h"

#include "Scene/Object/GeometryConstructor.h"

#include <iostream>

WindowManager* WindowManager::m_instancePtr = nullptr;

WindowManager::WindowManager()
{
    ImGui::CreateContext();
}

WindowManager::~WindowManager()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void WindowManager::MakeWindow()
{
    m_mainWindowView = make_shared<view::MainWindowView>();
    m_mainWindowView->InitWindowApiProperties();
    ViewController::GetInstance()->SetEvent(m_mainWindowView);

    return;
}

void WindowManager::RunWindow()
{
    MSG message = {};

    ShowWindow(m_mainWindowView->GetWindow(), SW_SHOWDEFAULT);
    UpdateWindow(m_mainWindowView->GetWindow());

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

        m_mainWindowView->RenderImGui();
        m_mainWindowView->UpdateScene();
        m_mainWindowView->RenderScene();
    }

    delete this;
}

ComPtr<ID3D11Device> WindowManager::GetDevice()
{
    return m_mainWindowView->GetDevice();
}

ComPtr<ID3D11DeviceContext> WindowManager::GetDeviceContext()
{
    return m_mainWindowView->GetDeviceContext();
}