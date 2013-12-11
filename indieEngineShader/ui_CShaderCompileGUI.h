/********************************************************************************
** Form generated from reading UI file 'CShaderCompileGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CSHADERCOMPILEGUI_H
#define UI_CSHADERCOMPILEGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CShaderCompileGUI
{
public:
    QGroupBox *groupBox;
    QPlainTextEdit *source_vs;
    QGroupBox *groupBox_2;
    QPlainTextEdit *source_fs;
    QGroupBox *groupBox_3;
    QPlainTextEdit *console;
    QGroupBox *groupBox_4;
    QPushButton *btn_compile;
    QPushButton *btn_back;
    QPushButton *btn_save_vs;
    QPushButton *btn_save_fs;
    QPushButton *btn_save_vs_as;
    QPushButton *btn_open_vs;
    QPushButton *btn_open_fs;
    QPushButton *btn_save_fs_as;

    void setupUi(QDialog *CShaderCompileGUI)
    {
        if (CShaderCompileGUI->objectName().isEmpty())
            CShaderCompileGUI->setObjectName(QStringLiteral("CShaderCompileGUI"));
        CShaderCompileGUI->setWindowModality(Qt::ApplicationModal);
        CShaderCompileGUI->resize(1024, 768);
        CShaderCompileGUI->setMaximumSize(QSize(1024, 768));
        QFont font;
        font.setFamily(QStringLiteral("Menlo"));
        font.setBold(false);
        font.setWeight(50);
        CShaderCompileGUI->setFont(font);
        CShaderCompileGUI->setStyleSheet(QLatin1String("background-color: rgb(64, 64, 64);\n"
"color: rgb(255, 255, 255);"));
        groupBox = new QGroupBox(CShaderCompileGUI);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(9, 9, 751, 300));
        groupBox->setFont(font);
        source_vs = new QPlainTextEdit(groupBox);
        source_vs->setObjectName(QStringLiteral("source_vs"));
        source_vs->setGeometry(QRect(10, 30, 731, 261));
        QFont font1;
        font1.setFamily(QStringLiteral("Menlo"));
        font1.setPointSize(11);
        source_vs->setFont(font1);
        source_vs->setStyleSheet(QLatin1String("QScrollBar:vertical {               \n"
"border: 1px solid #999999;\n"
"background:white;\n"
"width:10px;\n"
"margin: 0px 0px 0px 0px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"stop: 0  rgb(128, 128, 128), stop: 0.5 rgb(128, 128, 128),  stop:1 rgb(128, 128, 128));\n"
"min-height: 0px;\n"
"}\n"
"QScrollBar::add-line:vertical {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"stop: 0  rgb(128, 128, 128), stop: 0.5 rgb(128, 128, 128),  stop:1 rgb(128, 128, 128));\n"
"height: px;\n"
"subcontrol-position: bottom;\n"
"subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-line:vertical {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"stop: 0  rgb(128, 128, 128), stop: 0.5 rgb(128, 128, 128),  stop:1 rgb(128, 128, 128));\n"
"height: 0px;\n"
"subcontrol-position: top;\n"
"subcontrol-origin: margin;\n"
"}"));
        source_vs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        groupBox_2 = new QGroupBox(CShaderCompileGUI);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 310, 751, 300));
        groupBox_2->setFont(font);
        source_fs = new QPlainTextEdit(groupBox_2);
        source_fs->setObjectName(QStringLiteral("source_fs"));
        source_fs->setGeometry(QRect(10, 30, 731, 261));
        source_fs->setFont(font1);
        source_fs->setStyleSheet(QLatin1String("QScrollBar:vertical {               \n"
"border: 1px solid #999999;\n"
"background:white;\n"
"width:10px;\n"
"margin: 0px 0px 0px 0px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"stop: 0  rgb(128, 128, 128), stop: 0.5 rgb(128, 128, 128),  stop:1 rgb(128, 128, 128));\n"
"min-height: 0px;\n"
"}\n"
"QScrollBar::add-line:vertical {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"stop: 0  rgb(128, 128, 128), stop: 0.5 rgb(128, 128, 128),  stop:1 rgb(128, 128, 128));\n"
"height: px;\n"
"subcontrol-position: bottom;\n"
"subcontrol-origin: margin;\n"
"}\n"
"QScrollBar::sub-line:vertical {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"stop: 0  rgb(128, 128, 128), stop: 0.5 rgb(128, 128, 128),  stop:1 rgb(128, 128, 128));\n"
"height: 0px;\n"
"subcontrol-position: top;\n"
"subcontrol-origin: margin;\n"
"}"));
        source_fs->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        groupBox_3 = new QGroupBox(CShaderCompileGUI);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 610, 1001, 151));
        groupBox_3->setFont(font);
        console = new QPlainTextEdit(groupBox_3);
        console->setObjectName(QStringLiteral("console"));
        console->setGeometry(QRect(10, 30, 981, 111));
        console->setFont(font1);
        groupBox_4 = new QGroupBox(CShaderCompileGUI);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(769, 9, 241, 601));
        groupBox_4->setFont(font);
        btn_compile = new QPushButton(groupBox_4);
        btn_compile->setObjectName(QStringLiteral("btn_compile"));
        btn_compile->setGeometry(QRect(10, 520, 221, 32));
        btn_compile->setFont(font);
        btn_compile->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_back = new QPushButton(groupBox_4);
        btn_back->setObjectName(QStringLiteral("btn_back"));
        btn_back->setGeometry(QRect(10, 560, 221, 32));
        btn_back->setFont(font);
        btn_back->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_save_vs = new QPushButton(groupBox_4);
        btn_save_vs->setObjectName(QStringLiteral("btn_save_vs"));
        btn_save_vs->setGeometry(QRect(10, 110, 221, 32));
        btn_save_vs->setFont(font);
        btn_save_vs->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_save_fs = new QPushButton(groupBox_4);
        btn_save_fs->setObjectName(QStringLiteral("btn_save_fs"));
        btn_save_fs->setGeometry(QRect(10, 190, 221, 32));
        btn_save_fs->setFont(font);
        btn_save_fs->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_save_vs_as = new QPushButton(groupBox_4);
        btn_save_vs_as->setObjectName(QStringLiteral("btn_save_vs_as"));
        btn_save_vs_as->setGeometry(QRect(10, 150, 221, 32));
        btn_save_vs_as->setFont(font);
        btn_save_vs_as->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_open_vs = new QPushButton(groupBox_4);
        btn_open_vs->setObjectName(QStringLiteral("btn_open_vs"));
        btn_open_vs->setGeometry(QRect(10, 30, 221, 32));
        btn_open_vs->setFont(font);
        btn_open_vs->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_open_fs = new QPushButton(groupBox_4);
        btn_open_fs->setObjectName(QStringLiteral("btn_open_fs"));
        btn_open_fs->setGeometry(QRect(10, 70, 221, 32));
        btn_open_fs->setFont(font);
        btn_open_fs->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        btn_save_fs_as = new QPushButton(groupBox_4);
        btn_save_fs_as->setObjectName(QStringLiteral("btn_save_fs_as"));
        btn_save_fs_as->setGeometry(QRect(10, 230, 221, 32));
        btn_save_fs_as->setFont(font);
        btn_save_fs_as->setStyleSheet(QLatin1String("text-align:left;\n"
"background-color: rgb(128, 128, 128);"));
        groupBox->raise();
        groupBox_2->raise();
        groupBox_3->raise();
        groupBox_4->raise();
        btn_back->raise();
        btn_compile->raise();

        retranslateUi(CShaderCompileGUI);

        QMetaObject::connectSlotsByName(CShaderCompileGUI);
    } // setupUi

    void retranslateUi(QDialog *CShaderCompileGUI)
    {
        CShaderCompileGUI->setWindowTitle(QApplication::translate("CShaderCompileGUI", "indieEngine-ShaderEditor", 0));
        groupBox->setTitle(QApplication::translate("CShaderCompileGUI", "Source VS", 0));
        groupBox_2->setTitle(QApplication::translate("CShaderCompileGUI", "Source FS", 0));
        groupBox_3->setTitle(QApplication::translate("CShaderCompileGUI", "Output", 0));
        groupBox_4->setTitle(QApplication::translate("CShaderCompileGUI", "Options", 0));
        btn_compile->setText(QApplication::translate("CShaderCompileGUI", "  Compile", 0));
        btn_back->setText(QApplication::translate("CShaderCompileGUI", "  Back", 0));
        btn_save_vs->setText(QApplication::translate("CShaderCompileGUI", "  Save VS", 0));
        btn_save_fs->setText(QApplication::translate("CShaderCompileGUI", "  Save FS", 0));
        btn_save_vs_as->setText(QApplication::translate("CShaderCompileGUI", "  Save VS as...", 0));
        btn_open_vs->setText(QApplication::translate("CShaderCompileGUI", "  Open VS...", 0));
        btn_open_fs->setText(QApplication::translate("CShaderCompileGUI", "  Open FS...", 0));
        btn_save_fs_as->setText(QApplication::translate("CShaderCompileGUI", "  Save FS as...", 0));
    } // retranslateUi

};

namespace Ui {
    class CShaderCompileGUI: public Ui_CShaderCompileGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CSHADERCOMPILEGUI_H
