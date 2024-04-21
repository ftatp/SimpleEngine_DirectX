#pragma once
#include <Windows.h>
#include <imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace view 
{
	class MainWindowView
	{
	public:
		MainWindowView();
		~MainWindowView();

		HWND GetWindow();
		int GetWindowWidth();
		int GetWindowHeight();

	private:
		WNDCLASSEX m_wndClass;
		HWND m_window;
		int m_windowWidth;
		int	m_windowHeight;
	};

	
}