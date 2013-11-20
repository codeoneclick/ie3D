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
    QPushButton *btn_close;

    void setupUi(QDialog *CShaderCompileGUI)
    {
        if (CShaderCompileGUI->objectName().isEmpty())
            CShaderCompileGUI->setObjectName(QStringLiteral("CShaderCompileGUI"));
        CShaderCompileGUI->setWindowModality(Qt::ApplicationModal);
        CShaderCompileGUI->resize(1024, 768);
        CShaderCompileGUI->setMaximumSize(QSize(1024, 768));
        groupBox = new QGroupBox(CShaderCompileGUI);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(9, 9, 751, 300));
        source_vs = new QPlainTextEdit(groupBox);
        source_vs->setObjectName(QStringLiteral("source_vs"));
        source_vs->setGeometry(QRect(10, 30, 731, 261));
        QFont font;
        font.setFamily(QStringLiteral("Menlo"));
        font.setPointSize(11);
        source_vs->setFont(font);
        groupBox_2 = new QGroupBox(CShaderCompileGUI);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 310, 751, 300));
        source_fs = new QPlainTextEdit(groupBox_2);
        source_fs->setObjectName(QStringLiteral("source_fs"));
        source_fs->setGeometry(QRect(10, 30, 731, 261));
        source_fs->setFont(font);
        groupBox_3 = new QGroupBox(CShaderCompileGUI);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 610, 1001, 151));
        console = new QPlainTextEdit(groupBox_3);
        console->setObjectName(QStringLiteral("console"));
        console->setGeometry(QRect(10, 30, 981, 111));
        console->setFont(font);
        groupBox_4 = new QGroupBox(CShaderCompileGUI);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(769, 9, 241, 601));
        btn_compile = new QPushButton(groupBox_4);
        btn_compile->setObjectName(QStringLiteral("btn_compile"));
        btn_compile->setGeometry(QRect(10, 30, 221, 32));
        btn_close = new QPushButton(groupBox_4);
        btn_close->setObjectName(QStringLiteral("btn_close"));
        btn_close->setGeometry(QRect(10, 70, 221, 32));

        retranslateUi(CShaderCompileGUI);

        QMetaObject::connectSlotsByName(CShaderCompileGUI);
    } // setupUi

    void retranslateUi(QDialog *CShaderCompileGUI)
    {
        CShaderCompileGUI->setWindowTitle(QApplication::translate("CShaderCompileGUI", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("CShaderCompileGUI", "Source VS", 0));
        groupBox_2->setTitle(QApplication::translate("CShaderCompileGUI", "Source FS", 0));
        groupBox_3->setTitle(QApplication::translate("CShaderCompileGUI", "Console", 0));
        groupBox_4->setTitle(QApplication::translate("CShaderCompileGUI", "Settings", 0));
        btn_compile->setText(QApplication::translate("CShaderCompileGUI", "Compile", 0));
        btn_close->setText(QApplication::translate("CShaderCompileGUI", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class CShaderCompileGUI: public Ui_CShaderCompileGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CSHADERCOMPILEGUI_H
