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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CMaterialSettingsGUI
{
public:
    QGroupBox *gb_settings;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QCheckBox *cb_cull_face_status;
    QComboBox *cb_cull_face_mode;
    QCheckBox *cb_depth_test_status;
    QCheckBox *cb_depth_mask_status;
    QCheckBox *cb_blending_status;
    QComboBox *cb_blending_function_source;
    QComboBox *cb_blending_function_destination;
    QCheckBox *cb_clipping_status;
    QLineEdit *eb_clipping_x;
    QLineEdit *eb_clipping_y;
    QLineEdit *eb_clipping_z;
    QLineEdit *eb_clipping_w;
    QLineEdit *eb_vs_shader;
    QPushButton *btn_vs_browse;
    QLineEdit *eb_fs_shader;
    QPushButton *btn_fs_browse;
    QLineEdit *eb_texture_slot_1;
    QPushButton *btn_texture_slot1_browse;
    QLineEdit *eb_texture_slot_2;
    QPushButton *btn_texture_slot2_browse;
    QLineEdit *eb_texture_slot_3;
    QLineEdit *eb_texture_slot_4;
    QPushButton *btn_texture_slot3_browse;
    QPushButton *btn_texture_slot4_browse;
    QComboBox *cb_texture_slot1_wrap;
    QComboBox *cb_texture_slot2_wrap;
    QComboBox *cb_texture_slot3_wrap;
    QComboBox *cb_texture_slot4_wrap;
    QLineEdit *eb_material_name;
    QGroupBox *gb_options;
    QPushButton *btn_create_material;
    QPushButton *btn_open;
    QPushButton *btn_save;
    QPushButton *btn_saveas;
    QPushButton *btn_validate;
    QPushButton *btn_delete_material;

    void setupUi(QDialog *CMaterialSettingsGUI)
    {
        if (CMaterialSettingsGUI->objectName().isEmpty())
            CMaterialSettingsGUI->setObjectName(QStringLiteral("CMaterialSettingsGUI"));
        CMaterialSettingsGUI->resize(971, 612);
        CMaterialSettingsGUI->setStyleSheet(QLatin1String("background-color: rgb(64, 64, 64);\n"
"color: rgb(255, 255, 255);"));
        gb_settings = new QGroupBox(CMaterialSettingsGUI);
        gb_settings->setObjectName(QStringLiteral("gb_settings"));
        gb_settings->setGeometry(QRect(10, 0, 601, 601));
        QFont font;
        font.setFamily(QStringLiteral("Menlo"));
        gb_settings->setFont(font);
        gb_settings->setStyleSheet(QLatin1String("background-color: rgb(64, 64, 64);\n"
"color: rgb(255, 255, 255);"));
        label = new QLabel(gb_settings);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 60, 41, 19));
        label->setFont(font);
        label_2 = new QLabel(gb_settings);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 90, 141, 19));
        label_2->setFont(font);
        label_3 = new QLabel(gb_settings);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 120, 121, 19));
        label_3->setFont(font);
        label_4 = new QLabel(gb_settings);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 150, 151, 19));
        label_4->setFont(font);
        label_5 = new QLabel(gb_settings);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 180, 151, 19));
        label_5->setFont(font);
        label_6 = new QLabel(gb_settings);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 210, 131, 19));
        label_6->setFont(font);
        label_7 = new QLabel(gb_settings);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 240, 201, 19));
        label_7->setFont(font);
        label_8 = new QLabel(gb_settings);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 270, 241, 19));
        label_8->setFont(font);
        label_9 = new QLabel(gb_settings);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 300, 131, 19));
        label_9->setFont(font);
        label_10 = new QLabel(gb_settings);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(20, 330, 211, 19));
        label_10->setFont(font);
        label_11 = new QLabel(gb_settings);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(20, 360, 131, 19));
        label_11->setFont(font);
        label_12 = new QLabel(gb_settings);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(20, 440, 121, 19));
        label_12->setFont(font);
        label_13 = new QLabel(gb_settings);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(20, 480, 121, 19));
        label_13->setFont(font);
        label_14 = new QLabel(gb_settings);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(20, 520, 121, 19));
        label_14->setFont(font);
        label_15 = new QLabel(gb_settings);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(20, 560, 121, 19));
        label_15->setFont(font);
        cb_cull_face_status = new QCheckBox(gb_settings);
        cb_cull_face_status->setObjectName(QStringLiteral("cb_cull_face_status"));
        cb_cull_face_status->setGeometry(QRect(268, 90, 16, 16));
        cb_cull_face_status->setFont(font);
        cb_cull_face_mode = new QComboBox(gb_settings);
        cb_cull_face_mode->setObjectName(QStringLiteral("cb_cull_face_mode"));
        cb_cull_face_mode->setGeometry(QRect(268, 115, 211, 26));
        cb_cull_face_mode->setFont(font);
        cb_cull_face_mode->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        cb_depth_test_status = new QCheckBox(gb_settings);
        cb_depth_test_status->setObjectName(QStringLiteral("cb_depth_test_status"));
        cb_depth_test_status->setGeometry(QRect(268, 150, 16, 16));
        cb_depth_test_status->setFont(font);
        cb_depth_mask_status = new QCheckBox(gb_settings);
        cb_depth_mask_status->setObjectName(QStringLiteral("cb_depth_mask_status"));
        cb_depth_mask_status->setGeometry(QRect(268, 180, 16, 16));
        cb_depth_mask_status->setFont(font);
        cb_blending_status = new QCheckBox(gb_settings);
        cb_blending_status->setObjectName(QStringLiteral("cb_blending_status"));
        cb_blending_status->setGeometry(QRect(268, 210, 16, 16));
        cb_blending_status->setFont(font);
        cb_blending_function_source = new QComboBox(gb_settings);
        cb_blending_function_source->setObjectName(QStringLiteral("cb_blending_function_source"));
        cb_blending_function_source->setGeometry(QRect(268, 235, 211, 26));
        cb_blending_function_source->setFont(font);
        cb_blending_function_source->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        cb_blending_function_destination = new QComboBox(gb_settings);
        cb_blending_function_destination->setObjectName(QStringLiteral("cb_blending_function_destination"));
        cb_blending_function_destination->setGeometry(QRect(268, 265, 211, 26));
        cb_blending_function_destination->setFont(font);
        cb_blending_function_destination->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        cb_clipping_status = new QCheckBox(gb_settings);
        cb_clipping_status->setObjectName(QStringLiteral("cb_clipping_status"));
        cb_clipping_status->setGeometry(QRect(268, 300, 16, 16));
        cb_clipping_status->setFont(font);
        eb_clipping_x = new QLineEdit(gb_settings);
        eb_clipping_x->setObjectName(QStringLiteral("eb_clipping_x"));
        eb_clipping_x->setGeometry(QRect(268, 330, 31, 21));
        eb_clipping_x->setFont(font);
        eb_clipping_x->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_clipping_y = new QLineEdit(gb_settings);
        eb_clipping_y->setObjectName(QStringLiteral("eb_clipping_y"));
        eb_clipping_y->setGeometry(QRect(308, 330, 31, 21));
        eb_clipping_y->setFont(font);
        eb_clipping_y->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_clipping_z = new QLineEdit(gb_settings);
        eb_clipping_z->setObjectName(QStringLiteral("eb_clipping_z"));
        eb_clipping_z->setGeometry(QRect(348, 330, 31, 21));
        eb_clipping_z->setFont(font);
        eb_clipping_z->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_clipping_w = new QLineEdit(gb_settings);
        eb_clipping_w->setObjectName(QStringLiteral("eb_clipping_w"));
        eb_clipping_w->setGeometry(QRect(388, 330, 31, 21));
        eb_clipping_w->setFont(font);
        eb_clipping_w->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_vs_shader = new QLineEdit(gb_settings);
        eb_vs_shader->setObjectName(QStringLiteral("eb_vs_shader"));
        eb_vs_shader->setGeometry(QRect(268, 360, 151, 21));
        eb_vs_shader->setFont(font);
        eb_vs_shader->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        btn_vs_browse = new QPushButton(gb_settings);
        btn_vs_browse->setObjectName(QStringLiteral("btn_vs_browse"));
        btn_vs_browse->setGeometry(QRect(428, 355, 81, 32));
        btn_vs_browse->setFont(font);
        btn_vs_browse->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        eb_fs_shader = new QLineEdit(gb_settings);
        eb_fs_shader->setObjectName(QStringLiteral("eb_fs_shader"));
        eb_fs_shader->setGeometry(QRect(268, 400, 151, 21));
        eb_fs_shader->setFont(font);
        eb_fs_shader->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        btn_fs_browse = new QPushButton(gb_settings);
        btn_fs_browse->setObjectName(QStringLiteral("btn_fs_browse"));
        btn_fs_browse->setGeometry(QRect(428, 395, 81, 32));
        btn_fs_browse->setFont(font);
        btn_fs_browse->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        eb_texture_slot_1 = new QLineEdit(gb_settings);
        eb_texture_slot_1->setObjectName(QStringLiteral("eb_texture_slot_1"));
        eb_texture_slot_1->setGeometry(QRect(268, 440, 151, 21));
        eb_texture_slot_1->setFont(font);
        eb_texture_slot_1->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_texture_slot_1->setReadOnly(true);
        btn_texture_slot1_browse = new QPushButton(gb_settings);
        btn_texture_slot1_browse->setObjectName(QStringLiteral("btn_texture_slot1_browse"));
        btn_texture_slot1_browse->setGeometry(QRect(428, 435, 81, 32));
        btn_texture_slot1_browse->setFont(font);
        btn_texture_slot1_browse->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        eb_texture_slot_2 = new QLineEdit(gb_settings);
        eb_texture_slot_2->setObjectName(QStringLiteral("eb_texture_slot_2"));
        eb_texture_slot_2->setGeometry(QRect(268, 480, 151, 21));
        eb_texture_slot_2->setFont(font);
        eb_texture_slot_2->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_texture_slot_2->setReadOnly(true);
        btn_texture_slot2_browse = new QPushButton(gb_settings);
        btn_texture_slot2_browse->setObjectName(QStringLiteral("btn_texture_slot2_browse"));
        btn_texture_slot2_browse->setGeometry(QRect(428, 475, 81, 32));
        btn_texture_slot2_browse->setFont(font);
        btn_texture_slot2_browse->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        eb_texture_slot_3 = new QLineEdit(gb_settings);
        eb_texture_slot_3->setObjectName(QStringLiteral("eb_texture_slot_3"));
        eb_texture_slot_3->setGeometry(QRect(268, 520, 151, 21));
        eb_texture_slot_3->setFont(font);
        eb_texture_slot_3->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_texture_slot_3->setReadOnly(true);
        eb_texture_slot_4 = new QLineEdit(gb_settings);
        eb_texture_slot_4->setObjectName(QStringLiteral("eb_texture_slot_4"));
        eb_texture_slot_4->setGeometry(QRect(268, 560, 151, 21));
        eb_texture_slot_4->setFont(font);
        eb_texture_slot_4->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_texture_slot_4->setReadOnly(true);
        btn_texture_slot3_browse = new QPushButton(gb_settings);
        btn_texture_slot3_browse->setObjectName(QStringLiteral("btn_texture_slot3_browse"));
        btn_texture_slot3_browse->setGeometry(QRect(428, 515, 81, 32));
        btn_texture_slot3_browse->setFont(font);
        btn_texture_slot3_browse->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        btn_texture_slot4_browse = new QPushButton(gb_settings);
        btn_texture_slot4_browse->setObjectName(QStringLiteral("btn_texture_slot4_browse"));
        btn_texture_slot4_browse->setGeometry(QRect(428, 555, 81, 32));
        btn_texture_slot4_browse->setFont(font);
        btn_texture_slot4_browse->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        cb_texture_slot1_wrap = new QComboBox(gb_settings);
        cb_texture_slot1_wrap->setObjectName(QStringLiteral("cb_texture_slot1_wrap"));
        cb_texture_slot1_wrap->setGeometry(QRect(520, 435, 65, 26));
        cb_texture_slot1_wrap->setFont(font);
        cb_texture_slot1_wrap->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        cb_texture_slot2_wrap = new QComboBox(gb_settings);
        cb_texture_slot2_wrap->setObjectName(QStringLiteral("cb_texture_slot2_wrap"));
        cb_texture_slot2_wrap->setGeometry(QRect(520, 475, 65, 26));
        cb_texture_slot2_wrap->setFont(font);
        cb_texture_slot2_wrap->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        cb_texture_slot3_wrap = new QComboBox(gb_settings);
        cb_texture_slot3_wrap->setObjectName(QStringLiteral("cb_texture_slot3_wrap"));
        cb_texture_slot3_wrap->setGeometry(QRect(520, 515, 65, 26));
        cb_texture_slot3_wrap->setFont(font);
        cb_texture_slot3_wrap->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        cb_texture_slot4_wrap = new QComboBox(gb_settings);
        cb_texture_slot4_wrap->setObjectName(QStringLiteral("cb_texture_slot4_wrap"));
        cb_texture_slot4_wrap->setGeometry(QRect(520, 555, 65, 26));
        cb_texture_slot4_wrap->setFont(font);
        cb_texture_slot4_wrap->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        eb_material_name = new QLineEdit(gb_settings);
        eb_material_name->setObjectName(QStringLiteral("eb_material_name"));
        eb_material_name->setGeometry(QRect(273, 60, 211, 21));
        eb_material_name->setFont(font);
        eb_material_name->setFocusPolicy(Qt::StrongFocus);
        eb_material_name->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        gb_options = new QGroupBox(CMaterialSettingsGUI);
        gb_options->setObjectName(QStringLiteral("gb_options"));
        gb_options->setGeometry(QRect(620, 0, 341, 601));
        gb_options->setFont(font);
        btn_create_material = new QPushButton(gb_options);
        btn_create_material->setObjectName(QStringLiteral("btn_create_material"));
        btn_create_material->setGeometry(QRect(30, 50, 281, 32));
        btn_create_material->setFont(font);
        btn_create_material->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        btn_open = new QPushButton(gb_options);
        btn_open->setObjectName(QStringLiteral("btn_open"));
        btn_open->setGeometry(QRect(30, 90, 281, 32));
        btn_open->setFont(font);
        btn_open->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        btn_save = new QPushButton(gb_options);
        btn_save->setObjectName(QStringLiteral("btn_save"));
        btn_save->setGeometry(QRect(30, 130, 281, 32));
        btn_save->setFont(font);
        btn_save->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        btn_saveas = new QPushButton(gb_options);
        btn_saveas->setObjectName(QStringLiteral("btn_saveas"));
        btn_saveas->setGeometry(QRect(30, 170, 281, 32));
        btn_saveas->setFont(font);
        btn_saveas->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        btn_validate = new QPushButton(gb_options);
        btn_validate->setObjectName(QStringLiteral("btn_validate"));
        btn_validate->setGeometry(QRect(30, 250, 281, 32));
        btn_validate->setFont(font);
        btn_validate->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));
        btn_delete_material = new QPushButton(gb_options);
        btn_delete_material->setObjectName(QStringLiteral("btn_delete_material"));
        btn_delete_material->setGeometry(QRect(30, 210, 281, 32));
        btn_delete_material->setFont(font);
        btn_delete_material->setStyleSheet(QLatin1String("background-color: rgb(128, 128, 128);\n"
"color: rgb(255, 255, 255);\n"
"text-align:left;"));

        retranslateUi(CMaterialSettingsGUI);

        QMetaObject::connectSlotsByName(CMaterialSettingsGUI);
    } // setupUi

    void retranslateUi(QDialog *CMaterialSettingsGUI)
    {
        CMaterialSettingsGUI->setWindowTitle(QApplication::translate("CMaterialSettingsGUI", "Dialog", 0));
        gb_settings->setTitle(QApplication::translate("CMaterialSettingsGUI", "Settings", 0));
        label->setText(QApplication::translate("CMaterialSettingsGUI", "Name:", 0));
        label_2->setText(QApplication::translate("CMaterialSettingsGUI", "Cull Face on/off:", 0));
        label_3->setText(QApplication::translate("CMaterialSettingsGUI", "Cull Face mode:", 0));
        label_4->setText(QApplication::translate("CMaterialSettingsGUI", "Depth test on/off:", 0));
        label_5->setText(QApplication::translate("CMaterialSettingsGUI", "Depth mask on/off:", 0));
        label_6->setText(QApplication::translate("CMaterialSettingsGUI", "Blending on/off:", 0));
        label_7->setText(QApplication::translate("CMaterialSettingsGUI", "Blending function source:", 0));
        label_8->setText(QApplication::translate("CMaterialSettingsGUI", "Belnding function destination:", 0));
        label_9->setText(QApplication::translate("CMaterialSettingsGUI", "Clipping on/off:", 0));
        label_10->setText(QApplication::translate("CMaterialSettingsGUI", "Clipping values (x,y,z,w):", 0));
        label_11->setText(QApplication::translate("CMaterialSettingsGUI", "Shader (vs, fs):", 0));
        label_12->setText(QApplication::translate("CMaterialSettingsGUI", "Texture slot 1:", 0));
        label_13->setText(QApplication::translate("CMaterialSettingsGUI", "Texture slot 2:", 0));
        label_14->setText(QApplication::translate("CMaterialSettingsGUI", "Texture slot 3:", 0));
        label_15->setText(QApplication::translate("CMaterialSettingsGUI", "Texture slot 4:", 0));
        cb_cull_face_status->setText(QString());
        cb_depth_test_status->setText(QString());
        cb_depth_mask_status->setText(QString());
        cb_blending_status->setText(QString());
        cb_clipping_status->setText(QString());
        eb_clipping_x->setPlaceholderText(QApplication::translate("CMaterialSettingsGUI", "x", 0));
        eb_clipping_y->setPlaceholderText(QApplication::translate("CMaterialSettingsGUI", "y", 0));
        eb_clipping_z->setPlaceholderText(QApplication::translate("CMaterialSettingsGUI", "z", 0));
        eb_clipping_w->setPlaceholderText(QApplication::translate("CMaterialSettingsGUI", "w", 0));
        btn_vs_browse->setText(QApplication::translate("CMaterialSettingsGUI", "  Browse", 0));
        btn_fs_browse->setText(QApplication::translate("CMaterialSettingsGUI", "  Browse", 0));
        btn_texture_slot1_browse->setText(QApplication::translate("CMaterialSettingsGUI", "  Browse", 0));
        btn_texture_slot2_browse->setText(QApplication::translate("CMaterialSettingsGUI", "  Browse", 0));
        btn_texture_slot3_browse->setText(QApplication::translate("CMaterialSettingsGUI", "  Browse", 0));
        btn_texture_slot4_browse->setText(QApplication::translate("CMaterialSettingsGUI", "  Browse", 0));
        gb_options->setTitle(QApplication::translate("CMaterialSettingsGUI", "Options", 0));
        btn_create_material->setText(QApplication::translate("CMaterialSettingsGUI", "  Create", 0));
        btn_open->setText(QApplication::translate("CMaterialSettingsGUI", "  Open...", 0));
        btn_save->setText(QApplication::translate("CMaterialSettingsGUI", "  Save", 0));
        btn_saveas->setText(QApplication::translate("CMaterialSettingsGUI", "  Save as...", 0));
        btn_validate->setText(QApplication::translate("CMaterialSettingsGUI", "  Validate", 0));
        btn_delete_material->setText(QApplication::translate("CMaterialSettingsGUI", "  Delete", 0));
    } // retranslateUi

};

namespace Ui {
    class CMaterialSettingsGUI: public Ui_CMaterialSettingsGUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMATERIALSETTINGSGUI_H
