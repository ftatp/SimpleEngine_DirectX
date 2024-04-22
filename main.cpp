#include "WindowManager.h"
#include "SceneManager.h"

int main()
{
    WindowManager::GetInstance()->MakeWindow();
    SceneManager::GetInstance()->MakeScene();
    WindowManager::GetInstance()->RunWindow();

    return 0;
}