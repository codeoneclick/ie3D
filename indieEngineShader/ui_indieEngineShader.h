/********************************************************************************
** Form generated from reading UI file 'indieEngineShader.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDIEENGINESHADER_H
#define UI_INDIEENGINESHADER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_indieEngineShader
{
public:
    QWidget *centralWidget;
    QWidget *window;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *indieEngineShader)
    {
        if (indieEngineShader->objectName().isEmpty())
            indieEngineShader->setObjectName(QStringLiteral("indieEngineShader"));
        indieEngineShader->resize(628, 426);
        centralWidget = new QWidget(indieEngineShader);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        window = new QWidget(centralWidget);
        window->setObjectName(QStringLiteral("window"));
        window->setGeometry(QRect(20, 20, 591, 331));
        window->setAutoFillBackground(true);
        pushButton = new QPushButton(window);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 270, 114, 32));
        indieEngineShader->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(indieEngineShader);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 628, 22));
        indieEngineShader->setMenuBar(menuBar);
        mainToolBar = new QToolBar(indieEngineShader);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        indieEngineShader->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(indieEngineShader);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        indieEngineShader->setStatusBar(statusBar);

        retranslateUi(indieEngineShader);

        QMetaObject::connectSlotsByName(indieEngineShader);
    } // setupUi

    void retranslateUi(QMainWindow *indieEngineShader)
    {
        indieEngineShader->setWindowTitle(QApplication::translate("indieEngineShader", "indieEngineShader", 0));
        pushButton->setText(QApplication::translate("indieEngineShader", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class indieEngineShader: public Ui_indieEngineShader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDIEENGINESHADER_H
