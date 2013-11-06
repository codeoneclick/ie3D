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
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_CShaderCompileGUI
{
public:

    void setupUi(QDialog *CShaderCompileGUI)
    {
        if (CShaderCompileGUI->objectName().isEmpty())
            CShaderCompileGUI->setObjectName(QStringLiteral("CShaderCompileGUI"));
        CShaderCompileGUI->setWindowModality(Qt::ApplicationModal);
        CShaderCompileGUI->resize(1024, 768);
        CShaderCompileGUI->setMaximumSize(QSize(1024, 768));

        retranslateUi(CShaderCompileGUI);

        QMetaObject::connectSlotsByName(CShaderCompileGUI);
    } // setupUi

    void retranslateUi(QDialog *CShaderCompileGUI)
    {
        CShaderCompileGUI->setWindowTitle(QApplication::translate("CShaderCompileGUI", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class CShaderCompileGUI: public Ui_CShaderCompileGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CSHADERCOMPILEGUI_H
