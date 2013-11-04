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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_indieEngineShader
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QGroupBox *groupBox_2;
    QWidget *window;
    QGroupBox *groupBox_3;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *indieEngineShader)
    {
        if (indieEngineShader->objectName().isEmpty())
            indieEngineShader->setObjectName(QStringLiteral("indieEngineShader"));
        indieEngineShader->resize(882, 710);
        indieEngineShader->setMaximumSize(QSize(882, 710));
        centralWidget = new QWidget(indieEngineShader);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(680, 10, 191, 531));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 30, 171, 32));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 661, 531));
        window = new QWidget(groupBox_2);
        window->setObjectName(QStringLiteral("window"));
        window->setGeometry(QRect(10, 30, 640, 480));
        window->setAutoFillBackground(true);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 540, 861, 141));
        plainTextEdit = new QPlainTextEdit(groupBox_3);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 30, 841, 91));
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        plainTextEdit->setUndoRedoEnabled(false);
        plainTextEdit->setReadOnly(true);
        indieEngineShader->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(indieEngineShader);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 882, 22));
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
        indieEngineShader->setWindowTitle(QApplication::translate("indieEngineShader", "indieEngine-Shader", 0));
        groupBox->setTitle(QApplication::translate("indieEngineShader", "Settings", 0));
        pushButton->setText(QApplication::translate("indieEngineShader", "PushButton", 0));
        groupBox_2->setTitle(QApplication::translate("indieEngineShader", "3D", 0));
        groupBox_3->setTitle(QApplication::translate("indieEngineShader", "Console", 0));
        plainTextEdit->setPlainText(QString());
    } // retranslateUi

};

namespace Ui {
    class indieEngineShader: public Ui_indieEngineShader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDIEENGINESHADER_H
