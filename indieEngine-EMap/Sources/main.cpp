#include "CMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    CMainWindow window;
    window.show();
    window.execute();
    return application.exec();
}
