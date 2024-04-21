#include "WindowManager.h"
#include "Object/GeometryConstructor.h"

#include <iostream>

using namespace object;

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

    return;
}

void WindowManager::RunWindow()
{
    MSG message = {};

    ShowWindow(m_mainWindowView->GetWindow(), SW_SHOWDEFAULT);
    UpdateWindow(m_mainWindowView->GetWindow());

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

        m_mainWindowView->RenderImGui();

        UpdateScene();
        RenderScene();
    }

    delete this;
}

ComPtr<ID3D11Device> WindowManager::GetDevice()
{
    return m_mainWindowView->GetDevice();
}

void WindowManager::MakeScene()
{
    ObjectManager::GetInstance()->MakeObject();
    ObjectManager::GetInstance()->MakeObject();
    ObjectManager::GetInstance()->SetObjectTranslation(1, Matrix::CreateTranslation(Vector3(1.0f, 0.0f, 0.0f)));
}

void WindowManager::UpdateScene()
{
    auto objects = ObjectManager::GetInstance()->GetObjectList();

    for (int i = 0; i < objects.size(); i++)
    {
        ConstantData constantData = {};

        constantData.model = objects[i]->GetModelTransform();
            //Matrix::CreateScale(Vector3(1.0f)) * Matrix::CreateRotationX(0.0f) *
            //Matrix::CreateRotationY(0.0f) * Matrix::CreateRotationZ(0.0f) *
            //Matrix::CreateTranslation(Vector3(0.0f));
        constantData.model = constantData.model.Transpose();

        constantData.view = DirectX::XMMatrixLookToLH({ 0.0f, 0.0f, -2.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
        constantData.view = constantData.view.Transpose();

        constantData.projection = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(70.0f), float(m_mainWindowView->GetWindowWidth()) / m_mainWindowView->GetWindowHeight(), 0.5f, 100.0f);

        constantData.projection = constantData.projection.Transpose();

        objects[i]->SetConstantData(constantData);
    }
}

void WindowManager::RenderScene()
{
    m_mainWindowView->RenderScene();
}