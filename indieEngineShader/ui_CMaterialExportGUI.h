/********************************************************************************
** Form generated from reading UI file 'CMaterialExportGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMATERIALEXPORTGUI_H
#define UI_CMATERIALEXPORTGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_CMaterialExportGUI
{
public:

    void setupUi(QDialog *CMaterialExportGUI)
    {
        if (CMaterialExportGUI->objectName().isEmpty())
            CMaterialExportGUI->setObjectName(QStringLiteral("CMaterialExportGUI"));
        CMaterialExportGUI->resize(400, 300);

        retranslateUi(CMaterialExportGUI);

        QMetaObject::connectSlotsByName(CMaterialExportGUI);
    } // setupUi

    void retranslateUi(QDialog *CMaterialExportGUI)
    {
        CMaterialExportGUI->setWindowTitle(QApplication::translate("CMaterialExportGUI", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class CMaterialExportGUI: public Ui_CMaterialExportGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMATERIALEXPORTGUI_H
