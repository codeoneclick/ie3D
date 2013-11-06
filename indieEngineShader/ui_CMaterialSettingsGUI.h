/********************************************************************************
** Form generated from reading UI file 'CMaterialSettingsGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CMATERIALSETTINGSGUI_H
#define UI_CMATERIALSETTINGSGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_CMaterialSettingsGUI
{
public:

    void setupUi(QDialog *CMaterialSettingsGUI)
    {
        if (CMaterialSettingsGUI->objectName().isEmpty())
            CMaterialSettingsGUI->setObjectName(QStringLiteral("CMaterialSettingsGUI"));
        CMaterialSettingsGUI->resize(400, 300);

        retranslateUi(CMaterialSettingsGUI);

        QMetaObject::connectSlotsByName(CMaterialSettingsGUI);
    } // setupUi

    void retranslateUi(QDialog *CMaterialSettingsGUI)
    {
        CMaterialSettingsGUI->setWindowTitle(QApplication::translate("CMaterialSettingsGUI", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class CMaterialSettingsGUI: public Ui_CMaterialSettingsGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMATERIALSETTINGSGUI_H
