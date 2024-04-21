#include "WindowManager.h"

int main()
{
    WindowManager::GetInstance()->MakeWindow();
    WindowManager::GetInstance()->RunWindow();

    return 0;
}