#include "CMainWindow.h"
#include "ui_CMainWindow.h"
#include "QFileDialog.h"
#include "QMessageBox.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "CMEGameController.h"
#include "CMEGameTransition.h"
#include "CMEScene.h"
#include "IOGLWindow.h"
#include "CMEUIToSceneCommands.h"
#include "CMESceneToUICommands.h"
#include "CConfigurationAccessor.h"
#include "CTexture.h"
#include "IRenderTechniqueImporter.h"

#endif

#if defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

CMainWindow::CMainWindow(QWidget *parent) :
QMainWindow(parent),
#if defined(__OSX__) || defined(__WIN32__)

m_editableGameController(nullptr),
m_editableGameTransition(nullptr),
m_recentFilename(""),

#endif
ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    
    std::ostringstream stream;
    stream<<"Brush size: "<<ui->m_brushSizeSlider->value()<<" [4:32]";
    ui->m_brushSizeLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_previousBrushSize = ui->m_brushSizeSlider->value();
    
    stream.str("");
    stream.clear();
    stream<<"Brush strength: "<<ui->m_brushStrengthSlider->value()<<" [1:10]";
    ui->m_brushStrengthLabel->setText(QString::fromUtf8(stream.str().c_str()));
    
    stream.str("");
    stream.clear();
    stream<<"Falloff coefficient: "<<ui->m_falloffSlider->value()<<" [0:99]";
    ui->m_falloffLabel->setText(QString::fromUtf8(stream.str().c_str()));
    
    stream.str("");
    stream.clear();
    stream<<"Smooth coefficient: "<<ui->m_smoothSlider->value()<<" [0:3]";
    ui->m_smoothLabel->setText(QString::fromUtf8(stream.str().c_str()));
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::execute(void)
{
#if defined(__OSX__) || defined(__WIN32__)
    
    NSView* view = reinterpret_cast<NSView*>(ui->m_oglWindow->winId());
    m_sceneToUICommands = std::make_shared<CMESceneToUICommands>();
    m_sceneToUICommands->connectSetBrushSizeCommand(std::bind(&CMainWindow::setBrushSize, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetBrushStrengthCommand(std::bind(&CMainWindow::setBrushStrength, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetFalloffCoefficientCommand(std::bind(&CMainWindow::setFalloffCoefficient, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetSmoothCoefficientCommand(std::bind(&CMainWindow::setSmoothCoefficient, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetTextureSamplerCommand(std::bind(&CMainWindow::setTextureSampler, this, std::placeholders::_1, std::placeholders::_2));
    
    std::shared_ptr<IOGLWindow> window = std::make_shared<IOGLWindow>((__bridge void*)view);
    m_editableGameController = std::make_shared<CMEGameController>();
    m_editableGameTransition = std::static_pointer_cast<CMEGameTransition>(m_editableGameController->createEditableGameTransition("transition.main.xml", window));
    
    m_editableGameTransition->getConfigurationAccessor()->loadMaterialConfiguration("material.texture2D.xml", shared_from_this());
    
    m_editableGameController->RegisterTransition(m_editableGameTransition);
    m_editableGameController->GoToTransition("transition.main.xml");
    m_editableGameTransition->setSceneToUICommands(m_sceneToUICommands);
    
#endif
}

void CMainWindow::on_m_brushSizeSlider_valueChanged(int value)
{
    if(m_previousBrushSize < value)
    {
        m_previousBrushSize = value;
        m_previousBrushSize = m_previousBrushSize % 2 != 0 ? m_previousBrushSize + 1 : m_previousBrushSize;
        ui->m_brushSizeSlider->setValue(m_previousBrushSize);
    }
    else if(m_previousBrushSize > value)
    {
        m_previousBrushSize = value;
        m_previousBrushSize = m_previousBrushSize % 2 != 0 ? m_previousBrushSize - 1 : m_previousBrushSize;
        ui->m_brushSizeSlider->setValue(m_previousBrushSize);
    }
    
    std::ostringstream stream;
    stream<<"Brush size: "<<m_previousBrushSize<<" [4:32]";
    ui->m_brushSizeLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_editableGameTransition->getUIToSceneCommands()->executeSetBrushSizeCommand(m_previousBrushSize);
}

void CMainWindow::on_m_brushStrengthSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Brush strength: "<<value<<" [1:10]";
    ui->m_brushStrengthLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_editableGameTransition->getUIToSceneCommands()->executeSetBrushStrengthCommand(value);
}

void CMainWindow::on_m_falloffSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Falloff coefficient: "<<value<<" [0:99]";
    ui->m_falloffLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_editableGameTransition->getUIToSceneCommands()->executeSetFalloffCoefficientCommand(value);
}

void CMainWindow::on_m_smoothSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Smooth coefficient: "<<value<<" [0:3]";
    ui->m_smoothLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_editableGameTransition->getUIToSceneCommands()->executeSetSmoothCoefficientCommand(value);
}

void CMainWindow::on_m_texture01Btn_pressed()
{

}

void CMainWindow::on_m_texture01Btn_clicked()
{
    QString recentFilename = m_recentFilename.length() != 0 ? QString(m_recentFilename.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentFilename, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture01Img->setPixmap(pixmap);
        m_editableGameTransition->getUIToSceneCommands()->executeSetTextureSamplerCommand(filename.toUtf8().constData(), E_SHADER_SAMPLER_01);
    }
}

void CMainWindow::on_m_texture02Btn_clicked()
{
    QString recentFilename = m_recentFilename.length() != 0 ? QString(m_recentFilename.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentFilename, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture02Img->setPixmap(pixmap);
        m_editableGameTransition->getUIToSceneCommands()->executeSetTextureSamplerCommand(filename.toUtf8().constData(), E_SHADER_SAMPLER_02);
    }
}

void CMainWindow::on_m_texture03Btn_clicked()
{
    QString recentFilename = m_recentFilename.length() != 0 ? QString(m_recentFilename.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentFilename, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture03Img->setPixmap(pixmap);
        m_editableGameTransition->getUIToSceneCommands()->executeSetTextureSamplerCommand(filename.toUtf8().constData(), E_SHADER_SAMPLER_03);
    }
}

void CMainWindow::setBrushSize(ui32 value)
{
    m_previousBrushSize = value;
    std::ostringstream stream;
    stream<<"Brush size: "<<m_previousBrushSize<<" [4:32]";
    ui->m_brushSizeLabel->setText(QString::fromUtf8(stream.str().c_str()));
    ui->m_brushSizeSlider->setValue(m_previousBrushSize);
}

void CMainWindow::setBrushStrength(ui32)
{
    
}

void CMainWindow::setFalloffCoefficient(ui32)
{
    
}

void CMainWindow::setSmoothCoefficient(ui32)
{
    
}

void CMainWindow::setTextureSampler(CSharedTextureRef texture, E_SHADER_SAMPLER sampler)
{
    std::stringstream stringstream;
    stringstream<<"image_"<<sampler<<".png";
    std::string filename(stringstream.str());
    m_editableGameTransition->getRenderTechniqueImporter()->saveTexture(texture, filename, 256, 256);
    
    switch (sampler) {
        case E_SHADER_SAMPLER_01:
        {
            QPixmap pixmap(QString(filename.c_str()));
            ui->m_texture01Img->setPixmap(pixmap);
        }
            break;
            
        case E_SHADER_SAMPLER_02:
        {
            QPixmap pixmap(QString(filename.c_str()));
            ui->m_texture02Img->setPixmap(pixmap);
        }
            break;
            
        case E_SHADER_SAMPLER_03:
        {
            QPixmap pixmap(QString(filename.c_str()));
            ui->m_texture03Img->setPixmap(pixmap);
        }
            break;
            
        default:
            break;
    }
}

void CMainWindow::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    
}

void CMainWindow::on_m_textureTilling01SpinBox_valueChanged(int value)
{
    m_editableGameTransition->getUIToSceneCommands()->executeSetTillingTexcoordCommand(value, E_SHADER_SAMPLER_01);
}

void CMainWindow::on_m_textureTilling02SpinBox_valueChanged(int value)
{
    m_editableGameTransition->getUIToSceneCommands()->executeSetTillingTexcoordCommand(value, E_SHADER_SAMPLER_02);
}

void CMainWindow::on_m_textureTilling03SpinBox_valueChanged(int value)
{
    m_editableGameTransition->getUIToSceneCommands()->executeSetTillingTexcoordCommand(value, E_SHADER_SAMPLER_03);
}
