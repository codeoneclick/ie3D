#include "CMainWindow.h"
#include "QResource.h"
#include "QFile.h"
#include "QTextStream.h"
#include "QLogging.h"
#include "QDebug.h"
#include "ui_CMainWindow.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    
    QResource common(":/Bundle/transition.main.xml");
    QFile commonFile(common.absoluteFilePath());
    
    if (!commonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Unable to open file: " << commonFile.fileName() << " besause of error " << commonFile.errorString() << endl;
        return;
    }
    
    QTextStream in(&commonFile);
    
    QString content = in.readAll();
    qDebug() <<content<<endl;
}

CMainWindow::~CMainWindow()
{
    delete ui;
}
