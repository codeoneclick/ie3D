#include "CMainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    std::shared_ptr<CMainWindow> window = std::make_shared<CMainWindow>();
    window->show();
    window->execute();
    return application.exec();
}
