#include "indieEngineShader.h"
#include "ui_indieEngineShader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    indieEngineShader window;
    window.show();
    return application.exec();
}
