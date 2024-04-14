#include "MainWindow.h"

int main()
{
    MainWindow::GetInstance()->MakeWindow();
    MainWindow::GetInstance()->InitWindowApiProperties();
    MainWindow::GetInstance()->RunWindow();

    return 0;
}