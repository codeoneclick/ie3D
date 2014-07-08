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
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QSlider *m_brushStrengthSlider;
    QSlider *m_falloffSlider;
    QSlider *m_smoothSlider;
    QSlider *m_brushSizeSlider;
    QLabel *m_brushSizeLabel;
    QLabel *m_brushStrengthLabel;
    QLabel *m_falloffLabel;
    QLabel *m_smoothLabel;
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
        font.setPointSize(13);
        font.setBold(true);
        font.setWeight(75);
        tabWidget->setFont(font);
        tabWidget->setStyleSheet(QLatin1String("color: rgb(255, 255, 255);\n"
"background-color: rgb(64, 64, 64);"));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        m_brushStrengthSlider = new QSlider(tab);
        m_brushStrengthSlider->setObjectName(QStringLiteral("m_brushStrengthSlider"));
        m_brushStrengthSlider->setGeometry(QRect(10, 110, 225, 22));
        m_brushStrengthSlider->setMinimum(1);
        m_brushStrengthSlider->setMaximum(10);
        m_brushStrengthSlider->setOrientation(Qt::Horizontal);
        m_brushStrengthSlider->setTickPosition(QSlider::TicksAbove);
        m_brushStrengthSlider->setTickInterval(1);
        m_falloffSlider = new QSlider(tab);
        m_falloffSlider->setObjectName(QStringLiteral("m_falloffSlider"));
        m_falloffSlider->setGeometry(QRect(10, 160, 225, 22));
        m_falloffSlider->setOrientation(Qt::Horizontal);
        m_falloffSlider->setTickPosition(QSlider::TicksAbove);
        m_falloffSlider->setTickInterval(10);
        m_smoothSlider = new QSlider(tab);
        m_smoothSlider->setObjectName(QStringLiteral("m_smoothSlider"));
        m_smoothSlider->setGeometry(QRect(10, 210, 225, 22));
        m_smoothSlider->setMaximum(3);
        m_smoothSlider->setOrientation(Qt::Horizontal);
        m_smoothSlider->setInvertedAppearance(false);
        m_smoothSlider->setInvertedControls(false);
        m_smoothSlider->setTickPosition(QSlider::TicksAbove);
        m_smoothSlider->setTickInterval(1);
        m_brushSizeSlider = new QSlider(tab);
        m_brushSizeSlider->setObjectName(QStringLiteral("m_brushSizeSlider"));
        m_brushSizeSlider->setGeometry(QRect(10, 60, 225, 22));
        m_brushSizeSlider->setMinimum(4);
        m_brushSizeSlider->setMaximum(32);
        m_brushSizeSlider->setSingleStep(2);
        m_brushSizeSlider->setOrientation(Qt::Horizontal);
        m_brushSizeSlider->setTickPosition(QSlider::TicksAbove);
        m_brushSizeSlider->setTickInterval(2);
        m_brushSizeLabel = new QLabel(tab);
        m_brushSizeLabel->setObjectName(QStringLiteral("m_brushSizeLabel"));
        m_brushSizeLabel->setGeometry(QRect(10, 40, 225, 20));
        m_brushSizeLabel->setFont(font);
        m_brushStrengthLabel = new QLabel(tab);
        m_brushStrengthLabel->setObjectName(QStringLiteral("m_brushStrengthLabel"));
        m_brushStrengthLabel->setGeometry(QRect(10, 90, 225, 20));
        m_brushStrengthLabel->setFont(font);
        m_falloffLabel = new QLabel(tab);
        m_falloffLabel->setObjectName(QStringLiteral("m_falloffLabel"));
        m_falloffLabel->setGeometry(QRect(10, 140, 225, 20));
        m_falloffLabel->setFont(font);
        m_smoothLabel = new QLabel(tab);
        m_smoothLabel->setObjectName(QStringLiteral("m_smoothLabel"));
        m_smoothLabel->setGeometry(QRect(10, 190, 225, 20));
        m_smoothLabel->setFont(font);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 740, 735));
        QFont font1;
        font1.setFamily(QStringLiteral("Menlo"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        groupBox->setFont(font1);
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
        m_brushSizeLabel->setText(QApplication::translate("CMainWindow", "Brush Size:", 0));
        m_brushStrengthLabel->setText(QApplication::translate("CMainWindow", "Brush Strength:", 0));
        m_falloffLabel->setText(QApplication::translate("CMainWindow", "Falloff Coefficient:", 0));
        m_smoothLabel->setText(QApplication::translate("CMainWindow", "Smooth Coefficient:", 0));
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