#include "WindowManager.h"
#include "SceneManager.h"

#include <iostream>

int main()
{
    WindowManager::GetInstance()->MakeWindow();
    SceneManager::GetInstance()->MakeScene();
    WindowManager::GetInstance()->RunWindow();

    return 0;
}