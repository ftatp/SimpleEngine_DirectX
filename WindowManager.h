#pragma once

#include "SceneManager.h"
#include "View/MainWindowView.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <windows.h>
#include <memory>
#include <iostream>

#include <wrl.h>

#include <vector>

using namespace Microsoft::WRL;

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
    ComPtr<ID3D11DeviceContext> GetDeviceContext();

private:
    static WindowManager* m_instancePtr;
	WindowManager& operator=(const WindowManager&) = delete;

    shared_ptr<view::MainWindowView> m_mainWindowView;
};