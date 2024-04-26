#pragma once

#include "MainWindowView.h"

#include <memory>

class ViewController
{
public:
    static ViewController* GetInstance()
    {
        if (m_instancePtr == nullptr)
        {
            m_instancePtr = new ViewController();
        }

        return m_instancePtr;
    }
	ViewController();
	~ViewController();

    void SetEvent(std::shared_ptr<view::MainWindowView> mainWindow);

private:
    static ViewController* m_instancePtr;
    ViewController& operator=(const ViewController&) = delete;
};
