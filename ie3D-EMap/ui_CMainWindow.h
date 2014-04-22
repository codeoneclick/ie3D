/********************************************************************************
** Form generated from reading UI file 'CMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMAINWINDOW_H
#define UI_CMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *pushButton;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QWidget *m_oglWindow;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *CMainWindow)
    {
        if (CMainWindow->objectName().isEmpty())
            CMainWindow->setObjectName(QStringLiteral("CMainWindow"));
        CMainWindow->resize(1024, 768);
        CMainWindow->setMinimumSize(QSize(1024, 768));
        CMainWindow->setMaximumSize(QSize(1024, 768));
        CMainWindow->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        centralWidget = new QWidget(CMainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setStyleSheet(QStringLiteral("background-color: rgb(64, 64, 64);"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(760, 12, 251, 721));
        QFont font;
        font.setFamily(QStringLiteral("Menlo"));
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        tabWidget->setFont(font);
        tabWidget->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: rgb(64, 64, 64);"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(70, 20, 114, 32));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 740, 735));
        groupBox->setFont(font);
        groupBox->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        m_oglWindow = new QWidget(groupBox);
        m_oglWindow->setObjectName(QStringLiteral("m_oglWindow"));
        m_oglWindow->setGeometry(QRect(10, 30, 721, 691));
        m_oglWindow->setStyleSheet(QStringLiteral("background-color: rgb(64, 64, 64);"));
        CMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 22));
        CMainWindow->setMenuBar(menuBar);

        retranslateUi(CMainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CMainWindow)
    {
        CMainWindow->setWindowTitle(QApplication::translate("CMainWindow", "CMainWindow", 0));
        pushButton->setText(QApplication::translate("CMainWindow", "PushButton", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CMainWindow", "Tab 1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CMainWindow", "Tab 2", 0));
        groupBox->setTitle(QApplication::translate("CMainWindow", "3D", 0));
    } // retranslateUi

};

namespace Ui {
    class CMainWindow: public Ui_CMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAINWINDOW_H
