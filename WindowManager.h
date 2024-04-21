#pragma once

#include "ObjectManager.h"
#include "View/MainWindowView.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <windows.h>
#include <memory>
#include <iostream>

#include <wrl.h> // ComPtr

#include <vector>

using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
using namespace std;

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
    void RunWindow();

    ComPtr<ID3D11Device> GetDevice();

private:
	WindowManager& operator=(const WindowManager&) = delete;

    shared_ptr<view::MainWindowView> m_mainWindowView;

    void MakeScene();
    void UpdateScene();
    void RenderScene();

    static WindowManager* m_instancePtr;
};