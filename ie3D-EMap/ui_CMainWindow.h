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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CMainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *landscape_heightmap;
    QSlider *m_brushStrengthSlider;
    QSlider *m_falloffSlider;
    QSlider *m_smoothSlider;
    QSlider *m_brushSizeSlider;
    QLabel *m_brushSizeLabel;
    QLabel *m_brushStrengthLabel;
    QLabel *m_falloffLabel;
    QLabel *m_smoothLabel;
    QWidget *landscape_textures;
    QPushButton *m_texture01Btn;
    QLabel *m_texture01Label;
    QLabel *m_texture02Label;
    QPushButton *m_texture02Btn;
    QLabel *m_texture03Label;
    QPushButton *m_texture03Btn;
    QLabel *m_texture01Img;
    QLabel *m_texture02Img;
    QLabel *m_texture03Img;
    QSpinBox *m_textureTilling01SpinBox;
    QLabel *m_textureTilling01Label;
    QLabel *m_textureTilling02Label;
    QSpinBox *m_textureTilling02SpinBox;
    QLabel *m_textureTilling03Label;
    QSpinBox *m_textureTilling03SpinBox;
    QWidget *models;
    QWidget *m_modelsOpenGLView;
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
        landscape_heightmap = new QWidget();
        landscape_heightmap->setObjectName(QStringLiteral("landscape_heightmap"));
        m_brushStrengthSlider = new QSlider(landscape_heightmap);
        m_brushStrengthSlider->setObjectName(QStringLiteral("m_brushStrengthSlider"));
        m_brushStrengthSlider->setGeometry(QRect(10, 110, 225, 22));
        m_brushStrengthSlider->setMinimum(1);
        m_brushStrengthSlider->setMaximum(10);
        m_brushStrengthSlider->setOrientation(Qt::Horizontal);
        m_brushStrengthSlider->setTickPosition(QSlider::TicksAbove);
        m_brushStrengthSlider->setTickInterval(1);
        m_falloffSlider = new QSlider(landscape_heightmap);
        m_falloffSlider->setObjectName(QStringLiteral("m_falloffSlider"));
        m_falloffSlider->setGeometry(QRect(10, 160, 225, 22));
        m_falloffSlider->setOrientation(Qt::Horizontal);
        m_falloffSlider->setTickPosition(QSlider::TicksAbove);
        m_falloffSlider->setTickInterval(10);
        m_smoothSlider = new QSlider(landscape_heightmap);
        m_smoothSlider->setObjectName(QStringLiteral("m_smoothSlider"));
        m_smoothSlider->setGeometry(QRect(10, 210, 225, 22));
        m_smoothSlider->setMaximum(3);
        m_smoothSlider->setOrientation(Qt::Horizontal);
        m_smoothSlider->setInvertedAppearance(false);
        m_smoothSlider->setInvertedControls(false);
        m_smoothSlider->setTickPosition(QSlider::TicksAbove);
        m_smoothSlider->setTickInterval(1);
        m_brushSizeSlider = new QSlider(landscape_heightmap);
        m_brushSizeSlider->setObjectName(QStringLiteral("m_brushSizeSlider"));
        m_brushSizeSlider->setGeometry(QRect(10, 60, 225, 22));
        m_brushSizeSlider->setMinimum(4);
        m_brushSizeSlider->setMaximum(32);
        m_brushSizeSlider->setSingleStep(2);
        m_brushSizeSlider->setOrientation(Qt::Horizontal);
        m_brushSizeSlider->setTickPosition(QSlider::TicksAbove);
        m_brushSizeSlider->setTickInterval(2);
        m_brushSizeLabel = new QLabel(landscape_heightmap);
        m_brushSizeLabel->setObjectName(QStringLiteral("m_brushSizeLabel"));
        m_brushSizeLabel->setGeometry(QRect(10, 40, 225, 20));
        m_brushSizeLabel->setFont(font);
        m_brushStrengthLabel = new QLabel(landscape_heightmap);
        m_brushStrengthLabel->setObjectName(QStringLiteral("m_brushStrengthLabel"));
        m_brushStrengthLabel->setGeometry(QRect(10, 90, 225, 20));
        m_brushStrengthLabel->setFont(font);
        m_falloffLabel = new QLabel(landscape_heightmap);
        m_falloffLabel->setObjectName(QStringLiteral("m_falloffLabel"));
        m_falloffLabel->setGeometry(QRect(10, 140, 225, 20));
        m_falloffLabel->setFont(font);
        m_smoothLabel = new QLabel(landscape_heightmap);
        m_smoothLabel->setObjectName(QStringLiteral("m_smoothLabel"));
        m_smoothLabel->setGeometry(QRect(10, 190, 225, 20));
        m_smoothLabel->setFont(font);
        tabWidget->addTab(landscape_heightmap, QString());
        landscape_textures = new QWidget();
        landscape_textures->setObjectName(QStringLiteral("landscape_textures"));
        m_texture01Btn = new QPushButton(landscape_textures);
        m_texture01Btn->setObjectName(QStringLiteral("m_texture01Btn"));
        m_texture01Btn->setGeometry(QRect(142, 40, 91, 32));
        m_texture01Label = new QLabel(landscape_textures);
        m_texture01Label->setObjectName(QStringLiteral("m_texture01Label"));
        m_texture01Label->setGeometry(QRect(10, 10, 221, 16));
        QFont font1;
        font1.setFamily(QStringLiteral("Menlo"));
        font1.setBold(true);
        font1.setWeight(75);
        m_texture01Label->setFont(font1);
        m_texture02Label = new QLabel(landscape_textures);
        m_texture02Label->setObjectName(QStringLiteral("m_texture02Label"));
        m_texture02Label->setGeometry(QRect(10, 180, 221, 16));
        m_texture02Label->setFont(font1);
        m_texture02Btn = new QPushButton(landscape_textures);
        m_texture02Btn->setObjectName(QStringLiteral("m_texture02Btn"));
        m_texture02Btn->setGeometry(QRect(142, 210, 91, 32));
        m_texture03Label = new QLabel(landscape_textures);
        m_texture03Label->setObjectName(QStringLiteral("m_texture03Label"));
        m_texture03Label->setGeometry(QRect(10, 350, 221, 16));
        m_texture03Label->setFont(font1);
        m_texture03Btn = new QPushButton(landscape_textures);
        m_texture03Btn->setObjectName(QStringLiteral("m_texture03Btn"));
        m_texture03Btn->setGeometry(QRect(142, 380, 91, 32));
        m_texture01Img = new QLabel(landscape_textures);
        m_texture01Img->setObjectName(QStringLiteral("m_texture01Img"));
        m_texture01Img->setGeometry(QRect(10, 40, 128, 128));
        m_texture02Img = new QLabel(landscape_textures);
        m_texture02Img->setObjectName(QStringLiteral("m_texture02Img"));
        m_texture02Img->setGeometry(QRect(10, 210, 128, 128));
        m_texture03Img = new QLabel(landscape_textures);
        m_texture03Img->setObjectName(QStringLiteral("m_texture03Img"));
        m_texture03Img->setGeometry(QRect(10, 380, 128, 128));
        m_textureTilling01SpinBox = new QSpinBox(landscape_textures);
        m_textureTilling01SpinBox->setObjectName(QStringLiteral("m_textureTilling01SpinBox"));
        m_textureTilling01SpinBox->setGeometry(QRect(150, 100, 91, 24));
        m_textureTilling01SpinBox->setMinimum(1);
        m_textureTilling01SpinBox->setMaximum(128);
        m_textureTilling01Label = new QLabel(landscape_textures);
        m_textureTilling01Label->setObjectName(QStringLiteral("m_textureTilling01Label"));
        m_textureTilling01Label->setGeometry(QRect(150, 80, 91, 20));
        m_textureTilling01Label->setFont(font1);
        m_textureTilling02Label = new QLabel(landscape_textures);
        m_textureTilling02Label->setObjectName(QStringLiteral("m_textureTilling02Label"));
        m_textureTilling02Label->setGeometry(QRect(150, 250, 91, 20));
        m_textureTilling02Label->setFont(font1);
        m_textureTilling02SpinBox = new QSpinBox(landscape_textures);
        m_textureTilling02SpinBox->setObjectName(QStringLiteral("m_textureTilling02SpinBox"));
        m_textureTilling02SpinBox->setGeometry(QRect(150, 270, 91, 24));
        m_textureTilling02SpinBox->setMinimum(1);
        m_textureTilling02SpinBox->setMaximum(128);
        m_textureTilling03Label = new QLabel(landscape_textures);
        m_textureTilling03Label->setObjectName(QStringLiteral("m_textureTilling03Label"));
        m_textureTilling03Label->setGeometry(QRect(150, 420, 91, 20));
        m_textureTilling03Label->setFont(font1);
        m_textureTilling03SpinBox = new QSpinBox(landscape_textures);
        m_textureTilling03SpinBox->setObjectName(QStringLiteral("m_textureTilling03SpinBox"));
        m_textureTilling03SpinBox->setGeometry(QRect(150, 440, 91, 24));
        m_textureTilling03SpinBox->setMinimum(1);
        m_textureTilling03SpinBox->setMaximum(128);
        tabWidget->addTab(landscape_textures, QString());
        models = new QWidget();
        models->setObjectName(QStringLiteral("models"));
        m_modelsOpenGLView = new QWidget(models);
        m_modelsOpenGLView->setObjectName(QStringLiteral("m_modelsOpenGLView"));
        m_modelsOpenGLView->setGeometry(QRect(10, 10, 226, 181));
        m_modelsOpenGLView->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        tabWidget->addTab(models, QString());
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 740, 735));
        QFont font2;
        font2.setFamily(QStringLiteral("Menlo"));
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setWeight(75);
        groupBox->setFont(font2);
        groupBox->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        m_oglWindow = new QWidget(groupBox);
        m_oglWindow->setObjectName(QStringLiteral("m_oglWindow"));
        m_oglWindow->setGeometry(QRect(10, 30, 721, 691));
        m_oglWindow->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        CMainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CMainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 22));
        CMainWindow->setMenuBar(menuBar);

        retranslateUi(CMainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(CMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CMainWindow)
    {
        CMainWindow->setWindowTitle(QApplication::translate("CMainWindow", "CMainWindow", 0));
        m_brushSizeLabel->setText(QApplication::translate("CMainWindow", "Brush Size:", 0));
        m_brushStrengthLabel->setText(QApplication::translate("CMainWindow", "Brush Strength:", 0));
        m_falloffLabel->setText(QApplication::translate("CMainWindow", "Falloff Coefficient:", 0));
        m_smoothLabel->setText(QApplication::translate("CMainWindow", "Smooth Coefficient:", 0));
        tabWidget->setTabText(tabWidget->indexOf(landscape_heightmap), QApplication::translate("CMainWindow", "Landscape", 0));
        m_texture01Btn->setText(QApplication::translate("CMainWindow", "Open...", 0));
        m_texture01Label->setText(QApplication::translate("CMainWindow", "Texture sampler 1:", 0));
        m_texture02Label->setText(QApplication::translate("CMainWindow", "Texture sampler 2:", 0));
        m_texture02Btn->setText(QApplication::translate("CMainWindow", "Open...", 0));
        m_texture03Label->setText(QApplication::translate("CMainWindow", "Texture sampler 3:", 0));
        m_texture03Btn->setText(QApplication::translate("CMainWindow", "Open...", 0));
        m_texture01Img->setText(QString());
        m_texture02Img->setText(QString());
        m_texture03Img->setText(QString());
        m_textureTilling01Label->setText(QApplication::translate("CMainWindow", "Tilling:", 0));
        m_textureTilling02Label->setText(QApplication::translate("CMainWindow", "Tilling:", 0));
        m_textureTilling03Label->setText(QApplication::translate("CMainWindow", "Tilling:", 0));
        tabWidget->setTabText(tabWidget->indexOf(landscape_textures), QApplication::translate("CMainWindow", "Textures", 0));
        tabWidget->setTabText(tabWidget->indexOf(models), QApplication::translate("CMainWindow", "Models", 0));
        groupBox->setTitle(QApplication::translate("CMainWindow", "3D", 0));
    } // retranslateUi

};

namespace Ui {
    class CMainWindow: public Ui_CMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CMAINWINDOW_H
