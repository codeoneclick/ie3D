#include "CMainWindow.h"
#include "ui_CMainWindow.h"
#include "QFileDialog.h"
#include "QMessageBox.h"
#include "QPainter.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "CMEGameController.h"
#include "CMEMainSceneTransition.h"
#include "CMEPreviewModelSceneTransition.h"
#include "CMEMainScene.h"
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

m_mainSceneController(nullptr),
m_mainSceneTransition(nullptr),
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
    
    //m_modelsSceneView = new CMEModelsSceneView(ui->m_modelsOpenGLView);
    //m_modelsSceneView->setGeometry(QRect(0, 0,
    //                                     ui->m_modelsOpenGLView->width(),
    //                                     ui->m_modelsOpenGLView->height()));
    //m_modelsSceneView->setStyleSheet(ui->m_modelsOpenGLView->styleSheet());
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::execute(void)
{
#if defined(__OSX__) || defined(__WIN32__)
    
    m_sceneToUICommands = std::make_shared<CMESceneToUICommands>();
    m_sceneToUICommands->connectSetBrushSizeCommand(std::bind(&CMainWindow::setBrushSize, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetBrushStrengthCommand(std::bind(&CMainWindow::setBrushStrength, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetFalloffCoefficientCommand(std::bind(&CMainWindow::setFalloffCoefficient, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetSmoothCoefficientCommand(std::bind(&CMainWindow::setSmoothCoefficient, this, std::placeholders::_1));
    m_sceneToUICommands->connectSetTextureSamplerCommand(std::bind(&CMainWindow::setTextureSampler, this, std::placeholders::_1, std::placeholders::_2));
    m_sceneToUICommands->connectSetTillingTexcoordCommand(std::bind(&CMainWindow::setTillingTexcoord, this, std::placeholders::_1, std::placeholders::_2));
    
    NSView *mainSceneView = reinterpret_cast<NSView*>(ui->m_oglWindow->winId());
    NSOpenGLView *mainSceneOpenGLView = [[NSOpenGLView alloc] initWithFrame:CGRectMake(0.0,
                                                                                       0.0,
                                                                                       mainSceneView.frame.size.width,
                                                                                       mainSceneView.frame.size.height)];
    [mainSceneView addSubview:mainSceneOpenGLView];
    std::shared_ptr<IOGLWindow> mainSceneWindow = std::make_shared<IOGLWindow>((__bridge void*)mainSceneOpenGLView);
    
    m_mainSceneController = std::make_shared<CMEGameController>(mainSceneWindow);
    m_mainSceneTransition = std::make_shared<CMEMainSceneTransition>("transition.map.editor.main.scene.xml", false);
    m_mainSceneController->addTransition(m_mainSceneTransition);
    m_mainSceneController->gotoTransition("transition.map.editor.main.scene.xml");
    m_mainSceneTransition->setSceneToUICommands(m_sceneToUICommands);
    
    NSView *modelPreviewView =reinterpret_cast<NSView*>(ui->m_modelsOpenGLView->winId());
    NSOpenGLView *modelPreviewOpenGLView = [[NSOpenGLView alloc] initWithFrame:CGRectMake(0.0,
                                                                                          0.0,
                                                                                          modelPreviewView.frame.size.width,
                                                                                          modelPreviewView.frame.size.height)];
    [modelPreviewView addSubview:modelPreviewOpenGLView];
    std::shared_ptr<IOGLWindow> modelPreviewWindow = std::make_shared<IOGLWindow>((__bridge void*)modelPreviewOpenGLView);
    
    m_modelPreviewController = std::make_shared<CMEGameController>(modelPreviewWindow);
    m_modelPreviewTransition = std::make_shared<CMEPreviewModelSceneTransition>("transition.map.editor.preview.model.xml", false);
    m_modelPreviewController->addTransition(m_modelPreviewTransition);
    m_modelPreviewController->gotoTransition("transition.map.editor.preview.model.xml");
    
    //m_previewModelSceneTransition = std::make_shared<CMEPreviewModelSceneTransition>("transition.map.editor.preview.model.xml", true);
    //m_gameController->addChildTransition(m_previewModelSceneTransition);
    //m_gameController->activateChildTransition("transition.map.editor.preview.model.xml");
    //m_previewModelSceneTransition->getRenderTechniqueImporter()->addRenderTechninqueOperationTextureHandler("render.operation.world.base", shared_from_this());
    
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
    m_mainSceneTransition->getUIToSceneCommands()->executeSetBrushSizeCommand(m_previousBrushSize);
}

void CMainWindow::on_m_brushStrengthSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Brush strength: "<<value<<" [1:10]";
    ui->m_brushStrengthLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_mainSceneTransition->getUIToSceneCommands()->executeSetBrushStrengthCommand(value);
}

void CMainWindow::on_m_falloffSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Falloff coefficient: "<<value<<" [0:99]";
    ui->m_falloffLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_mainSceneTransition->getUIToSceneCommands()->executeSetFalloffCoefficientCommand(value);
}

void CMainWindow::on_m_smoothSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Smooth coefficient: "<<value<<" [0:3]";
    ui->m_smoothLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_mainSceneTransition->getUIToSceneCommands()->executeSetSmoothCoefficientCommand(value);
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
        m_mainSceneTransition->getUIToSceneCommands()->executeSetTextureSamplerCommand(filename.toUtf8().constData(), E_SHADER_SAMPLER_01);
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
        m_mainSceneTransition->getUIToSceneCommands()->executeSetTextureSamplerCommand(filename.toUtf8().constData(), E_SHADER_SAMPLER_02);
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
        m_mainSceneTransition->getUIToSceneCommands()->executeSetTextureSamplerCommand(filename.toUtf8().constData(), E_SHADER_SAMPLER_03);
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
    m_mainSceneTransition->getRenderTechniqueImporter()->saveTexture(texture, filename, 256, 256);
    
    switch (sampler)
    {
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

void CMainWindow::setTillingTexcoord(f32 value, E_SHADER_SAMPLER sampler)
{
    switch (sampler) {
        case E_SHADER_SAMPLER_01:
        {
            ui->m_textureTilling01SpinBox->setValue(value);
        }
            break;
            
        case E_SHADER_SAMPLER_02:
        {
            ui->m_textureTilling02SpinBox->setValue(value);
        }
            break;
            
        case E_SHADER_SAMPLER_03:
        {
            ui->m_textureTilling03SpinBox->setValue(value);
        }
            break;
        default:
            break;
    }
}

void CMainWindow::on_m_textureTilling01SpinBox_valueChanged(int value)
{
    m_mainSceneTransition->getUIToSceneCommands()->executeSetTillingTexcoordCommand(value, E_SHADER_SAMPLER_01);
}

void CMainWindow::on_m_textureTilling02SpinBox_valueChanged(int value)
{
    m_mainSceneTransition->getUIToSceneCommands()->executeSetTillingTexcoordCommand(value, E_SHADER_SAMPLER_02);
}

void CMainWindow::on_m_textureTilling03SpinBox_valueChanged(int value)
{
    m_mainSceneTransition->getUIToSceneCommands()->executeSetTillingTexcoordCommand(value, E_SHADER_SAMPLER_03);
}

void CMainWindow::onTextureRendered(const std::string& techniqueName, const ui8 *rawdata, ui32 width, ui32 height)
{
    QImage image(rawdata, width, height, QImage::Format_RGBA8888);
    QTransform transform;
    transform.rotate(180);
    image = image.transformed(transform);
    m_modelsSceneImage = image;
    m_modelsSceneView->setImage(m_modelsSceneImage);
    m_modelsSceneView->update();
}

bool CMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Polish)
    {
        CMainWindow::execute();
    }
    return QWidget::event(event);
}

void CMainWindow::onConfigurationLoaded(ISharedConfigurationRef configuration, bool success)
{
    
}

