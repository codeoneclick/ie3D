#include "CMainWindowGUI.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    CMainWindowGUI window;
    window.show();
    window.Execute();

    return application.exec();
}
