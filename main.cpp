#include "WindowManager.h"

void MakeScene()
{
    SceneManager::GetInstance()->MakeObject();
    SceneManager::GetInstance()->MakeObject();
    SceneManager::GetInstance()->SetObjectTranslation(1, Matrix::CreateTranslation(Vector3(1.0f, 0.0f, 0.0f)));
}

int main()
{
    WindowManager::GetInstance()->MakeWindow();
    MakeScene();
    WindowManager::GetInstance()->RunWindow();

    return 0;
}