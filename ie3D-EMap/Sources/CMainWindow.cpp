#include "CMainWindow.h"
#include "ui_CMainWindow.h"
#include "QFileDialog.h"
#include "QMessageBox.h"
#include "QPainter.h"
#include "QStatusBar.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "CMEGameController.h"
#include "CMEmseTransition.h"
#include "CMEgopTransition.h"
#include "CMEgoeTransition.h"
#include "IOGLWindow.h"
#include "IUICommands.h"
#include "CConfigurationAccessor.h"
#include "CTexture.h"
#include "IRenderTechniqueImporter.h"
#include "HUICommands.h"
#include "CCommonOS.h"

#endif

#if defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

CMainWindow::CMainWindow(QWidget *parent) :
QMainWindow(parent),
#if defined(__OSX__) || defined(__WIN32__)

m_mseSceneToUICommands(std::make_shared<IUICommands>()),
m_goeSceneToUICommands(std::make_shared<IUICommands>()),
m_recentOpenPath(""),

#endif
ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
    
    std::ostringstream stream;
    stream<<"Brush size: "<<ui->m_brushSizeSlider->value()<<" [4:32]";
    ui->m_brushSizeLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_brushSize = ui->m_brushSizeSlider->value();
    
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
    
    statusBar()->showMessage("Copyright (c) 2015 Sergey Sergeev. All rights reserved.");
    statusBar()->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255); text-align:left");
    
    ui->m_goeSettingsTab->setCurrentWidget(ui->m_goeMainSettingTab);
    ui->m_goeMaterialsSettingTab->setDisabled(true);
    ui->m_goeAnimationsSettingTab->setDisabled(true);
    
    ui->m_drawCurrentRadioButton->setChecked(false);
    ui->m_drawAllRadioButton->setChecked(true);
    
    ui->m_cullModeComboBox->addItem("GL_FRONT");
    ui->m_cullModeComboBox->addItem("GL_BACK");
    
    ui->m_blendingSourceComboBox->addItem("GL_SRC_ALPHA");
    ui->m_blendingSourceComboBox->addItem("GL_ONE_MINUS_SRC_ALPHA");
    
    ui->m_blendingDesinationComboBox->addItem("GL_SRC_ALPHA");
    ui->m_blendingDesinationComboBox->addItem("GL_ONE_MINUS_SRC_ALPHA");
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::execute(void)
{
#if defined(__OSX__) || defined(__WIN32__)
    
    ISharedCommand command = std::make_shared<CCommand<UICommandMSESetBrushSize::COMMAND>>(std::bind(&CMainWindow::setMSEBrushSize,
                                                                                                     this,
                                                                                                     std::placeholders::_1));
    m_mseSceneToUICommands->addCommand(UICommandMSESetBrushSize::GUID,
                                       command);
    
    command = std::make_shared<CCommand<UICommandMSESetBrushStrength::COMMAND>>(std::bind(&CMainWindow::setMSEBrushStrength,
                                                                                          this,
                                                                                          std::placeholders::_1));
    m_mseSceneToUICommands->addCommand(UICommandMSESetBrushStrength::GUID,
                                       command);
    
    command = std::make_shared<CCommand<UICommandMSESetFalloffCoefficient::COMMAND>>(std::bind(&CMainWindow::setMSEFalloffCoefficient,
                                                                                               this,
                                                                                               std::placeholders::_1));
    m_mseSceneToUICommands->addCommand(UICommandMSESetFalloffCoefficient::GUID,
                                       command);
    
    command = std::make_shared<CCommand<UICommandMSESetSmoothCoefficient::COMMAND>>(std::bind(&CMainWindow::setMSESmoothCoefficient,
                                                                                              this,
                                                                                              std::placeholders::_1));
    m_mseSceneToUICommands->addCommand(UICommandMSESetSmoothCoefficient::GUID,
                                       command);
    
    command = std::make_shared<CCommand<UICommandMSESetTexture::COMMAND>>(std::bind(&CMainWindow::setMSETexture,
                                                                                    this,
                                                                                    std::placeholders::_1,
                                                                                    std::placeholders::_2));
    m_mseSceneToUICommands->addCommand(UICommandMSESetTexture::GUID,
                                       command);
    
    command = std::make_shared<CCommand<UICommandMSESetTillingTexcoord::COMMAND>>(std::bind(&CMainWindow::setMSETillingTexcoord,
                                                                                            this,
                                                                                            std::placeholders::_1,
                                                                                            std::placeholders::_2));
    
    m_mseSceneToUICommands->addCommand(UICommandMSESetTillingTexcoord::GUID,
                                       command);
    
    command = std::make_shared<CCommand<UICommandMSEUpdateHeightmapGenerationStatistic::COMMAND>>(std::bind(&CMainWindow::generatorStatisticsUpdate, this,
                                                                                                            std::placeholders::_1,
                                                                                                            std::placeholders::_2,
                                                                                                            std::placeholders::_3));
    
    m_mseSceneToUICommands->addCommand(UICommandMSEUpdateHeightmapGenerationStatistic::GUID,
                                       command);
    
    command = std::make_shared<CCommand<UICommandGOEUpdateConfigurationsMaterials::COMMAND>>(std::bind(&CMainWindow::updateGOEConfigurationsMaterials,
                                                                                                       this,
                                                                                                       std::placeholders::_1));
    m_goeSceneToUICommands->addCommand(UICommandGOEUpdateConfigurationsMaterials::GUID,
                                       command);
    
    NSView *mseView = reinterpret_cast<NSView*>(ui->m_oglWindow->winId());
    NSOpenGLView *mseOGLView = [[NSOpenGLView alloc] initWithFrame:CGRectMake(0.0,
                                                                              0.0,
                                                                              mseView.frame.size.width,
                                                                              mseView.frame.size.height)];
    [mseView addSubview:mseOGLView];
    std::shared_ptr<IOGLWindow> mseWindow = std::make_shared<IOGLWindow>((__bridge void*)mseOGLView);
    
    m_mseController = std::make_shared<CMEGameController>(mseWindow);
    m_mseTransition = std::make_shared<CMEmseTransition>("transition.mse.xml", false);
    m_mseController->addTransition(m_mseTransition);
    m_mseController->gotoTransition("transition.mse.xml");
    m_mseTransition->setSceneToUICommands(m_mseSceneToUICommands);
    ui->m_oglWindow->setVisible(false);
    
    NSView *goeView =reinterpret_cast<NSView*>(ui->m_gameObjectGLWindow->winId());
    NSOpenGLView *goeOGLView = [[NSOpenGLView alloc] initWithFrame:CGRectMake(0.0,
                                                                              0.0,
                                                                              goeView.frame.size.width,
                                                                              goeView.frame.size.height)];
    [goeView addSubview:goeOGLView];
    std::shared_ptr<IOGLWindow> goeOGLWindow = std::make_shared<IOGLWindow>((__bridge void*)goeOGLView);
    
    m_goeController = std::make_shared<CMEGameController>(goeOGLWindow);
    m_goeTransition = std::make_shared<CMEgoeTransition>("transition.goe.xml", false);
    m_goeController->addTransition(m_goeTransition);
    m_goeController->gotoTransition("transition.goe.xml");
    m_goeTransition->setSceneToUICommands(m_goeSceneToUICommands);
    
    NSView *gopView =reinterpret_cast<NSView*>(ui->m_modelsOpenGLView->winId());
    NSOpenGLView *gopOGLView = [[NSOpenGLView alloc] initWithFrame:CGRectMake(0.0,
                                                                              0.0,
                                                                              gopView.frame.size.width,
                                                                              gopView.frame.size.height)];
    [gopView addSubview:gopOGLView];
    std::shared_ptr<IOGLWindow> gopWindow = std::make_shared<IOGLWindow>((__bridge void*)gopOGLView);
    
    m_gopController = std::make_shared<CMEGameController>(gopWindow);
    m_gopTransition = std::make_shared<CMEgopTransition>("transition.gop.xml", false);
    m_gopController->addTransition(m_gopTransition);
    m_gopController->gotoTransition("transition.gop.xml");
    
    ui->m_modelsList->addItem("gameobject.human_01.xml");
    ui->m_modelsList->addItem("gameobject.human_02.xml");
    ui->m_modelsList->addItem("gameobject.orc_01.xml");
    ui->m_modelsList->addItem("gameobject.orc_02.xml");
    ui->m_modelsList->setCurrentRow(0);
    
    ui->m_mainMenuTabs->setCurrentIndex(0);
    ui->m_landscapePropertiesTab->setCurrentIndex(0);
    CMainWindow::resumeWidgets();
    
#endif
}

void CMainWindow::on_m_brushSizeSlider_valueChanged(int value)
{
    if(m_brushSize < value)
    {
        m_brushSize = value;
        m_brushSize = m_brushSize % 2 != 0 ? m_brushSize + 1 : m_brushSize;
        ui->m_brushSizeSlider->setValue(m_brushSize);
    }
    else if(m_brushSize > value)
    {
        m_brushSize = value;
        m_brushSize = m_brushSize % 2 != 0 ? m_brushSize - 1 : m_brushSize;
        ui->m_brushSizeSlider->setValue(m_brushSize);
    }
    
    std::ostringstream stream;
    stream<<"Brush size: "<<m_brushSize<<" [4:32]";
    ui->m_brushSizeLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetBrushSize::COMMAND>(UICommandMSESetBrushSize::GUID,
                                                                                        m_brushSize);
}

void CMainWindow::on_m_brushStrengthSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Brush strength: "<<value<<" [1:10]";
    ui->m_brushStrengthLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetBrushStrength::COMMAND>(UICommandMSESetBrushStrength::GUID,
                                                                                            value);
}

void CMainWindow::on_m_falloffSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Falloff coefficient: "<<value<<" [0:99]";
    ui->m_falloffLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetFalloffCoefficient::COMMAND>(UICommandMSESetFalloffCoefficient::GUID,
                                                                                                 value);
}

void CMainWindow::on_m_smoothSlider_valueChanged(int value)
{
    std::ostringstream stream;
    stream<<"Smooth coefficient: "<<value<<" [0:3]";
    ui->m_smoothLabel->setText(QString::fromUtf8(stream.str().c_str()));
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetSmoothCoefficient::COMMAND>(UICommandMSESetSmoothCoefficient::GUID,
                                                                                                value);
}

void CMainWindow::on_m_texture01Btn_pressed()
{
    
}

void CMainWindow::on_m_texture01Btn_clicked()
{
    QString recentOpenPath = m_recentOpenPath.length() != 0 ? QString(m_recentOpenPath.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentOpenPath, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture01Img->setPixmap(pixmap);
        m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetTextureFilename::COMMAND>(UICommandMSESetTextureFilename::GUID,
                                                                                                  filename.toUtf8().constData(),
                                                                                                  E_SHADER_SAMPLER_01);
    }
}

void CMainWindow::on_m_texture02Btn_clicked()
{
    QString recentOpenPath = m_recentOpenPath.length() != 0 ? QString(m_recentOpenPath.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentOpenPath, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture02Img->setPixmap(pixmap);
        m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetTextureFilename::COMMAND>(UICommandMSESetTextureFilename::GUID,
                                                                                                  filename.toUtf8().constData(),
                                                                                                  E_SHADER_SAMPLER_02);
    }
}

void CMainWindow::on_m_texture03Btn_clicked()
{
    QString recentOpenPath = m_recentOpenPath.length() != 0 ? QString(m_recentOpenPath.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentOpenPath, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture03Img->setPixmap(pixmap);
        m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetTextureFilename::COMMAND>(UICommandMSESetTextureFilename::GUID,
                                                                                                  filename.toUtf8().constData(),
                                                                                                  E_SHADER_SAMPLER_03);
    }
}

void CMainWindow::setMSEBrushSize(ui32 value)
{
    m_brushSize = value;
    std::ostringstream stream;
    stream<<"Brush size: "<<m_brushSize<<" [4:32]";
    ui->m_brushSizeLabel->setText(QString::fromUtf8(stream.str().c_str()));
    ui->m_brushSizeSlider->setValue(m_brushSize);
}

void CMainWindow::setMSEBrushStrength(ui32)
{
    
}

void CMainWindow::setMSEFalloffCoefficient(ui32)
{
    
}

void CMainWindow::setMSESmoothCoefficient(ui32)
{
    
}

void CMainWindow::setMSETexture(CSharedTextureRef texture, E_SHADER_SAMPLER sampler)
{
    std::stringstream stringstream;
    stringstream<<"image_"<<sampler<<".png";
    std::string filename(stringstream.str());
    m_mseTransition->getRenderTechniqueImporter()->saveTexture(texture, filename, 256, 256);
    
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

void CMainWindow::setMSETillingTexcoord(f32 value, E_SHADER_SAMPLER sampler)
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
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetTillingTexcoord::COMMAND>(UICommandMSESetTillingTexcoord::GUID,
                                                                                              value,
                                                                                              E_SHADER_SAMPLER_01);
}

void CMainWindow::on_m_textureTilling02SpinBox_valueChanged(int value)
{
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetTillingTexcoord::COMMAND>(UICommandMSESetTillingTexcoord::GUID,
                                                                                              value,
                                                                                              E_SHADER_SAMPLER_02);
}

void CMainWindow::on_m_textureTilling03SpinBox_valueChanged(int value)
{
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetTillingTexcoord::COMMAND>(UICommandMSESetTillingTexcoord::GUID,
                                                                                              value,
                                                                                              E_SHADER_SAMPLER_03);
}

bool CMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Polish)
    {
        CMainWindow::execute();
    }
    if (event->type() == QEvent::WindowActivate)
    {
        CMainWindow::resumeWidgets();
    }
    if(event->type() == QEvent::WindowDeactivate)
    {
        CMainWindow::pauseWidgets();
    }
    return QWidget::event(event);
}

void CMainWindow::on_generateButton_clicked()
{
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSEGenerateHeightmap::COMMAND>(UICommandMSEGenerateHeightmap::GUID,
                                                                                             glm::ivec2(ui->m_sizeXSpinBox->value(), ui->m_sizeYSpinBox->value()),
                                                                                             ui->m_frequencySpinBox->value(),
                                                                                             ui->m_octavesSpinBox->value(),
                                                                                             ui->m_seedSpinBox->value());
}

void CMainWindow::resumeWidgets(void)
{
    if(ui->m_mainMenuTabs->currentWidget() == ui->m_sceneTab)
    {
        m_mseTransition->setPaused(false);
        m_goeTransition->setPaused(true);
        
        if(ui->m_landscapePropertiesTab->currentWidget() == ui->m_landscapeHeightmapTab)
        {
            m_gopTransition->setPaused(true);
            m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetLandscapeEditMode::COMMAND>(UICommandMSESetLandscapeEditMode::GUID,
                                                                                                        E_LANDSCAPE_EDIT_MODE_HEIGHTMAP);
        }
        else if(ui->m_landscapePropertiesTab->currentWidget() == ui->m_landscapeTexturesTab)
        {
            m_gopTransition->setPaused(true);
            m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetLandscapeEditMode::COMMAND>(UICommandMSESetLandscapeEditMode::GUID,
                                                                                                        E_LANDSCAPE_EDIT_MODE_TEXTURES);
        }
        else if(ui->m_landscapePropertiesTab->currentWidget() == ui->m_landscapeGameobjectsTab)
        {
            m_gopTransition->setPaused(false);
            m_mseTransition->getUIToSceneCommands()->execute<UICommandMSESetLandscapeEditMode::COMMAND>(UICommandMSESetLandscapeEditMode::GUID,
                                                                                                        E_LANDSCAPE_EDIT_MODE_GAMEOBJECTS);
        }
    }
    else if(ui->m_mainMenuTabs->currentWidget() == ui->m_gameObjectTab)
    {
        m_goeTransition->setPaused(false);
        m_mseTransition->setPaused(true);
        m_gopTransition->setPaused(true);
    }
    else
    {
        assert(false);
    }
}

void CMainWindow::pauseWidgets(void)
{
    m_mseTransition->setPaused(true);
    m_goeTransition->setPaused(true);
    m_gopTransition->setPaused(true);
}

void CMainWindow::on_m_mainMenuTabs_currentChanged(int)
{
    CMainWindow::resumeWidgets();
}

void CMainWindow::on_m_landscapePropertiesTab_currentChanged(int)
{
    CMainWindow::resumeWidgets();
}

void CMainWindow::on_m_createGameObjectConfiguration_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), "", tr("Files (*.ie3Dmesh)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
#if defined(__OSX__) || defined(__WIN32__)
        
        m_goeTransition->getUIToSceneCommands()->execute<UICommandGOECreateConfiguration::COMMAND>(UICommandGOECreateConfiguration::GUID,
                                                                                                   filename.toUtf8().constData());
        
#endif
    }
}

void CMainWindow::updateGOEConfigurationsMaterials(std::vector<CSharedConfigurationMaterial>& configurations)
{
    m_goeConfigurationsMaterials.clear();
    
    while (ui->m_materialsComboBox->count() != 0)
    {
        ui->m_materialsComboBox->removeItem(0);
    }
    for(ui32 i = 0; i < configurations.size(); ++i)
    {
        ui->m_materialsComboBox->addItem(QString(configurations.at(i)->getRenderTechniqueName().c_str()));
    }
    ui->m_materialsComboBox->setCurrentIndex(0);
    
    CMainWindow::updateGOEUIConfigurationMaterial(configurations.at(0));
    
    ui->m_goeSettingsTab->setCurrentWidget(ui->m_goeMaterialsSettingTab);
    ui->m_goeMaterialsSettingTab->setDisabled(false);
    ui->m_goeAnimationsSettingTab->setDisabled(false);
    
    m_goeConfigurationsMaterials = configurations;
}

void CMainWindow::updateGOEUIConfigurationMaterial(CSharedConfigurationMaterialRef configuration)
{
    ui->m_materialEnabledCheckBox->setCheckState(configuration->getEnabled() ? Qt::Checked : Qt::Unchecked);
    ui->m_cullFaceCheckBox->setCheckState(configuration->getCulling() ? Qt::Checked : Qt::Unchecked);
    ui->m_cullModeComboBox->setCurrentText(QString(g_enumGLToString[configuration->getCullingMode()].c_str()));
    ui->m_depthTestCheckBox->setCheckState(configuration->getDepthTest() ? Qt::Checked : Qt::Unchecked);
    ui->m_depthMaskCheckBox->setCheckState(configuration->getDepthMask() ? Qt::Checked : Qt::Unchecked);
    ui->m_blendingCheckBox->setCheckState(configuration->getBlending() ? Qt::Checked : Qt::Unchecked);
    ui->m_blendingSourceComboBox->setCurrentText(QString(g_enumGLToString[configuration->getBlendingFunctionSource()].c_str()));
    ui->m_blendingDesinationComboBox->setCurrentText(QString(g_enumGLToString[configuration->getBlendingFunctionDestination()].c_str()));
    ui->m_clippingCheckBox->setCheckState(configuration->getClipping() ? Qt::Checked : Qt::Unchecked);
    
    ui->m_clippingXSpinBox->setValue(configuration->getClippingX());
    ui->m_clippingYSpinBox->setValue(configuration->getClippingY());
    ui->m_clippingZSpinBox->setValue(configuration->getClippingZ());
    ui->m_clippingWSpinBox->setValue(configuration->getClippingW());
    ui->m_reflectingCheckBox->setCheckState(configuration->getReflecting() ? Qt::Checked : Qt::Unchecked);
    ui->m_shadowingCheckBox->setCheckState(configuration->getShadowing() ? Qt::Checked : Qt::Unchecked);
    ui->m_debuggingCheckBox->setCheckState(configuration->getDebugging() ? Qt::Checked : Qt::Unchecked);
    
    if(configuration->getTexturesConfigurations().size() >= 1)
    {
        CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(configuration->getTexturesConfigurations().at(0));
        std::string path = configurationTexture->getTextureFilename();
        QPixmap pixmap(QString(path.c_str()));
        if(pixmap.data_ptr() == nullptr)
        {
            path = bundlepath() + configurationTexture->getTextureFilename();
            pixmap = QPixmap(QString(path.c_str()));
        }
        ui->m_texture1Image->setPixmap(pixmap);
    }
    
    if(configuration->getTexturesConfigurations().size() >= 2)
    {
        CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(configuration->getTexturesConfigurations().at(1));
        std::string path = configurationTexture->getTextureFilename();
        QPixmap pixmap(QString(path.c_str()));
        if(pixmap.data_ptr() == nullptr)
        {
            path = bundlepath() + configurationTexture->getTextureFilename();
            pixmap = QPixmap(QString(path.c_str()));
        }
        ui->m_texture2Image->setPixmap(pixmap);
    }
    
    if(configuration->getTexturesConfigurations().size() >= 3)
    {
        CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(configuration->getTexturesConfigurations().at(2));
        std::string path = configurationTexture->getTextureFilename();
        QPixmap pixmap(QString(path.c_str()));
        if(pixmap.data_ptr() == nullptr)
        {
            path = bundlepath() + configurationTexture->getTextureFilename();
            pixmap = QPixmap(QString(path.c_str()));
        }
        ui->m_texture3Image->setPixmap(pixmap);
    }
}

void CMainWindow::on_m_cullFaceCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setCulling(ui->m_cullFaceCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_materialEnabledCheckBox_clicked()
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setEnabled(ui->m_materialEnabledCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_cullModeComboBox_currentIndexChanged(const QString&)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setCullingMode(g_stringToGLenum[ui->m_cullModeComboBox->currentText().toUtf8().constData()]);
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_depthTestCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setDepthTest(ui->m_depthTestCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_depthMaskCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setDepthMask(ui->m_depthMaskCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_blendingCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setBlending(ui->m_blendingCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_blendingSourceComboBox_currentIndexChanged(const QString &)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setBlendingFunctionSource(g_stringToGLenum[ui->m_blendingSourceComboBox->currentText().toUtf8().constData()]);
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_blendingDesinationComboBox_currentIndexChanged(const QString &)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setBlendingFunctionDestination(g_stringToGLenum[ui->m_blendingDesinationComboBox->currentText().toUtf8().constData()]);
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_clippingCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setClipping(ui->m_clippingCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_clippingXSpinBox_valueChanged(double)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setClippingX(ui->m_clippingXSpinBox->value());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_clippingYSpinBox_valueChanged(double)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setClippingY(ui->m_clippingYSpinBox->value());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_clippingZSpinBox_valueChanged(double)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setClippingZ(ui->m_clippingZSpinBox->value());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_clippingWSpinBox_valueChanged(double)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setClippingW(ui->m_clippingWSpinBox->value());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_reflectingCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setReflecting(ui->m_reflectingCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_shadowingCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setShadowing(ui->m_shadowingCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_debuggingCheckBox_stateChanged(int)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        configuration->setDebugging(ui->m_debuggingCheckBox->isChecked());
        CMainWindow::commitCurrentConfigurationMaterial();
    }
}

void CMainWindow::on_m_shaderButton_clicked()
{
    
}

void CMainWindow::on_m_texture1LoadButton_clicked()
{
    QString recentOpenPath = m_recentOpenPath.length() != 0 ? QString(m_recentOpenPath.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentOpenPath, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture1Image->setPixmap(pixmap);
        
        if(m_goeConfigurationsMaterials.size() != 0)
        {
            CSharedConfigurationMaterial configurationMaterial = m_goeConfigurationsMaterials.at(ui->m_materialsComboBox->currentIndex());
            if(configurationMaterial->getTexturesConfigurations().size() >= 1)
            {
                CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(configurationMaterial->getTexturesConfigurations().at(0));
                configurationTexture->setTextureFilename(filename.toUtf8().constData());
            }
            CMainWindow::commitCurrentConfigurationMaterial();
        }
    }
}

void CMainWindow::on_m_texture2LoadButton_clicked()
{
    QString recentOpenPath = m_recentOpenPath.length() != 0 ? QString(m_recentOpenPath.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentOpenPath, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture2Image->setPixmap(pixmap);
        
        if(m_goeConfigurationsMaterials.size() != 0)
        {
            CSharedConfigurationMaterial configurationMaterial = m_goeConfigurationsMaterials.at(ui->m_materialsComboBox->currentIndex());
            if(configurationMaterial->getTexturesConfigurations().size() >= 2)
            {
                CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(configurationMaterial->getTexturesConfigurations().at(1));
                configurationTexture->setTextureFilename(filename.toUtf8().constData());
            }
            CMainWindow::commitCurrentConfigurationMaterial();
        }
    }
}

void CMainWindow::on_m_texture3LoadButton_clicked()
{
    QString recentOpenPath = m_recentOpenPath.length() != 0 ? QString(m_recentOpenPath.c_str()) : "";
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), recentOpenPath, tr("Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
        QPixmap pixmap(filename);
        ui->m_texture3Image->setPixmap(pixmap);
        
        if(m_goeConfigurationsMaterials.size() != 0)
        {
            CSharedConfigurationMaterial configurationMaterial = m_goeConfigurationsMaterials.at(ui->m_materialsComboBox->currentIndex());
            if(configurationMaterial->getTexturesConfigurations().size() >= 3)
            {
                CSharedConfigurationTexture configurationTexture = std::static_pointer_cast<CConfigurationTexture>(configurationMaterial->getTexturesConfigurations().at(2));
                configurationTexture->setTextureFilename(filename.toUtf8().constData());
            }
            CMainWindow::commitCurrentConfigurationMaterial();
        }
    }
}

void CMainWindow::on_m_goeSettingsTab_currentChanged(int)
{
    
}

void CMainWindow::on_m_drawCurrentRadioButton_clicked()
{
    
}

void CMainWindow::on_m_drawAllRadioButton_clicked()
{
    
}

void CMainWindow::on_m_materialsComboBox_currentIndexChanged(int index)
{
    if(m_goeConfigurationsMaterials.size() != 0)
    {
        CMainWindow::updateGOEUIConfigurationMaterial(m_goeConfigurationsMaterials.at(index));
    }
}

void CMainWindow::on_m_addModelToSceneButton_clicked()
{
    QString configurationFilename = ui->m_modelsList->currentItem()->text();
    m_mseTransition->getUIToSceneCommands()->execute<UICommandMSEAddGameObjectToScene::COMMAND>(UICommandMSEAddGameObjectToScene::GUID, configurationFilename.toUtf8().constData());
}

void CMainWindow::on_m_modelsList_currentRowChanged(int)
{
    QString configurationFilename = ui->m_modelsList->currentItem()->text();
    m_gopTransition->getUIToSceneCommands()->execute<UICommandGOPSetGameObjectConfigurationName::COMMAND>(UICommandGOPSetGameObjectConfigurationName::GUID,
                                                                                                          configurationFilename.toUtf8().constData());
}

CSharedConfigurationMaterial CMainWindow::getCurrentConfigurationMaterial(void)
{
    CSharedConfigurationMaterial configuration = nullptr;
    if(static_cast<i32>(m_goeConfigurationsMaterials.size()) > ui->m_materialsComboBox->currentIndex() &&
       ui->m_materialsComboBox->currentIndex() >= 0)
    {
        configuration = m_goeConfigurationsMaterials.at(ui->m_materialsComboBox->currentIndex());
    }
    return configuration;
}

void CMainWindow::commitCurrentConfigurationMaterial(void)
{
    CSharedConfigurationMaterial configuration = CMainWindow::getCurrentConfigurationMaterial();
    if(configuration)
    {
        m_goeTransition->getUIToSceneCommands()->execute<UICommandGOEUpdateConfigurationMaterial::COMMAND>(UICommandGOEUpdateConfigurationMaterial::GUID,
                                                                                                           configuration);
    }
}

void CMainWindow::generatorStatisticsUpdate(const std::string& operationName, E_HEIGHTMAP_GENERATION_STATUS status, const std::string&)
{
    if(operationName == "Heightmap Generation..." && status == E_HEIGHTMAP_GENERATION_STATUS_ENDED)
    {
        ui->m_oglWindow->setVisible(true);
        ui->m_heightmapGenerationStatisticLabel->setVisible(false);
    }
    ui->m_heightmapGenerationStatisticLabel->setText(QString::fromUtf8(operationName.c_str()));
}
