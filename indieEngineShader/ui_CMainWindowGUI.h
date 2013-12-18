/********************************************************************************
** Form generated from reading UI file 'CMainWindowGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMAINWINDOWGUI_H
#define UI_CMAINWINDOWGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMainWindowGUI
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionExit;
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QPushButton *btn_shader_compile;
    QPushButton *btn_material_editor;
    QComboBox *materials_list;
    QLabel *label;
    QPushButton *btn_content_path;
    QLabel *lbl_content_path;
    QPushButton *btn_open;
    QPushButton *btn_save;
    QPushButton *btn_saveas;
    QGroupBox *groupBox_2;
    QWidget *opengl_window;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CMainWindowGUI)
    {
        if (CMainWindowGUI->objectName().isEmpty())
            CMainWindowGUI->setObjectName(QStringLiteral("CMainWindowGUI"));
        CMainWindowGUI->resize(1024, 768);
        CMainWindowGUI->setMaximumSize(QSize(1024, 768));
        QFont font;
        font.setFamily(QStringLiteral("Menlo"));
        font.setBold(true);
        font.setWeight(75);
        CMainWindowGUI->setFont(font);
        CMainWindowGUI->setStyleSheet(QLatin1String("background-color: rgb(64, 64, 64);\n"
"color: rgb(255, 255, 255);"));
        actionOpen = new QAction(CMainWindowGUI);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(CMainWindowGUI);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionExit = new QAction(CMainWindowGUI);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        centralWidget = new QWidget(CMainWindowGUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(669, 10, 341, 691));
        groupBox->setFont(font);
        groupBox->setStyleSheet(QStringLiteral(""));
        btn_shader_compile = new QPushButton(groupBox);
        btn_shader_compile->setObjectName(QStringLiteral("btn_shader_compile"));
        btn_shader_compile->setGeometry(QRect(20, 340, 301, 32));
        QFont font1;
        font1.setFamily(QStringLiteral("Menlo"));
        btn_shader_compile->setFont(font1);
        btn_shader_compile->setAutoFillBackground(false);
        btn_shader_compile->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_material_editor = new QPushButton(groupBox);
        btn_material_editor->setObjectName(QStringLiteral("btn_material_editor"));
        btn_material_editor->setGeometry(QRect(20, 380, 301, 32));
        btn_material_editor->setFont(font1);
        btn_material_editor->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        materials_list = new QComboBox(groupBox);
        materials_list->setObjectName(QStringLiteral("materials_list"));
        materials_list->setGeometry(QRect(25, 300, 291, 26));
        materials_list->setFont(font);
        materials_list->setStyleSheet(QStringLiteral("background-color: rgb(127, 127, 127);s"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 270, 101, 19));
        label->setFont(font);
        btn_content_path = new QPushButton(groupBox);
        btn_content_path->setObjectName(QStringLiteral("btn_content_path"));
        btn_content_path->setGeometry(QRect(20, 40, 301, 32));
        btn_content_path->setFont(font1);
        btn_content_path->setAutoFillBackground(false);
        btn_content_path->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        lbl_content_path = new QLabel(groupBox);
        lbl_content_path->setObjectName(QStringLiteral("lbl_content_path"));
        lbl_content_path->setGeometry(QRect(30, 80, 281, 51));
        lbl_content_path->setFont(font1);
        lbl_content_path->setStyleSheet(QStringLiteral("background-color: rgb(64, 64, 64);"));
        lbl_content_path->setTextFormat(Qt::PlainText);
        lbl_content_path->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        lbl_content_path->setWordWrap(true);
        btn_open = new QPushButton(groupBox);
        btn_open->setObjectName(QStringLiteral("btn_open"));
        btn_open->setGeometry(QRect(20, 140, 301, 32));
        btn_open->setFont(font1);
        btn_open->setAutoFillBackground(false);
        btn_open->setStyleSheet(QStringLiteral("text-align:left;"));
        btn_save = new QPushButton(groupBox);
        btn_save->setObjectName(QStringLiteral("btn_save"));
        btn_save->setGeometry(QRect(20, 180, 301, 32));
        btn_save->setFont(font1);
        btn_save->setAutoFillBackground(false);
        btn_save->setStyleSheet(QStringLiteral("text-align:left;"));
        btn_saveas = new QPushButton(groupBox);
        btn_saveas->setObjectName(QStringLiteral("btn_saveas"));
        btn_saveas->setGeometry(QRect(20, 220, 301, 32));
        btn_saveas->setFont(font1);
        btn_saveas->setAutoFillBackground(false);
        btn_saveas->setStyleSheet(QStringLiteral("text-align:left;"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 9, 641, 691));
        groupBox_2->setFont(font);
        opengl_window = new QWidget(groupBox_2);
        opengl_window->setObjectName(QStringLiteral("opengl_window"));
        opengl_window->setGeometry(QRect(9, 29, 621, 651));
        CMainWindowGUI->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CMainWindowGUI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        CMainWindowGUI->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CMainWindowGUI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CMainWindowGUI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CMainWindowGUI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CMainWindowGUI->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);

        retranslateUi(CMainWindowGUI);

        QMetaObject::connectSlotsByName(CMainWindowGUI);
    } // setupUi

    void retranslateUi(QMainWindow *CMainWindowGUI)
    {
        CMainWindowGUI->setWindowTitle(QApplication::translate("CMainWindowGUI", "indieEngine-ShaderEditor", 0));
        actionOpen->setText(QApplication::translate("CMainWindowGUI", "Open", 0));
        actionSave->setText(QApplication::translate("CMainWindowGUI", "Save", 0));
        actionExit->setText(QApplication::translate("CMainWindowGUI", "Exit", 0));
        groupBox->setTitle(QApplication::translate("CMainWindowGUI", "Settings", 0));
        btn_shader_compile->setText(QApplication::translate("CMainWindowGUI", "  GLSL Editor", 0));
        btn_material_editor->setText(QApplication::translate("CMainWindowGUI", "  Material Editor", 0));
        label->setText(QApplication::translate("CMainWindowGUI", "render mode:", 0));
        btn_content_path->setText(QApplication::translate("CMainWindowGUI", "  Content Path", 0));
        lbl_content_path->setText(QApplication::translate("CMainWindowGUI", "path:", 0));
        btn_open->setText(QApplication::translate("CMainWindowGUI", "  Open...", 0));
        btn_save->setText(QApplication::translate("CMainWindowGUI", "  Save", 0));
        btn_saveas->setText(QApplication::translate("CMainWindowGUI", "  Save as...", 0));
        groupBox_2->setTitle(QApplication::translate("CMainWindowGUI", "3D", 0));
        menuFile->setTitle(QApplication::translate("CMainWindowGUI", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class CMainWindowGUI: public Ui_CMainWindowGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAINWINDOWGUI_H
