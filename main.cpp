#include "MainWindow.h"

int main()
{
    MainWindow::GetInstance()->MakeWindow();
    MainWindow::GetInstance()->InitWindowApiProperties();
    MainWindow::GetInstance()->DisplayWindow();

    return 0;
}