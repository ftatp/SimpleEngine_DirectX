#include "ViewController.h"
#include "../Scene/SceneManager.h"

#include <iostream>

ViewController* ViewController::m_instancePtr = nullptr;

ViewController::ViewController()
{
}

ViewController::~ViewController()
{
}

void ViewController::SetEvent(std::shared_ptr<view::MainWindowView> mainWindow)
{
	mainWindow->OnAddObjectButtonClickEvent.AddListener(
		[]() {
			std::cout << "Button clicked!" << std::endl; 
			SceneManager::GetInstance()->GetCurrentScene()->MakeObject();
		}
	);
}