#include "CMainWindowGUI.h"
#include "ui_CMainWindowGUI.h"
#include "QFileDialog.h"

#if defined(__OSX__)
#include <Cocoa/Cocoa.h>
#endif

#if defined(__OSX__) || defined(__WIN32__)
#include "IOGLWindow.h"
#include "CIESAWorkflow.h"
#include "СIESAMainTransition.h"
#include "CCommonOS.h"
#include "CGameLoopExecutor.h"
#include "IGameObjectExtension.h"
#include "CTemplateGameObjects.h"
#include "CUICommon.h"
#endif

CMainWindowGUI::CMainWindowGUI(QWidget *parent) :
    QMainWindow(parent),
#if defined(__OSX__) || defined(__WIN32__)

    m_iesaWorkflow(nullptr),
    m_iesaTransition(nullptr),
    m_mode(""),

#endif
    ui(new Ui::CMainWindowGUI)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(on_buttonOpen_clicked()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_buttonSave_clicked()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(on_buttonExit_clicked()));
    
    ui->btn_shader_compile->setDisabled(true);
    ui->btn_shader_compile->setStyleSheet(kBtnDisableStyleSheet);
    
    ui->btn_open->setDisabled(true);
    ui->btn_open->setStyleSheet(kBtnDisableStyleSheet);
    
    ui->btn_save->setDisabled(true);
    ui->btn_save->setStyleSheet(kBtnDisableStyleSheet);
    
    ui->btn_saveas->setDisabled(true);
    ui->btn_saveas->setStyleSheet(kBtnDisableStyleSheet);
    
    ui->btn_material_editor->setDisabled(true);
    ui->btn_material_editor->setStyleSheet(kBtnDisableStyleSheet);
}

CMainWindowGUI::~CMainWindowGUI()
{
    delete ui;
}

void CMainWindowGUI::on_pushButton_2_clicked()
{

}

void CMainWindowGUI::on_pushButton_3_clicked()
{
    m_materialExportGUI = new CMaterialExportGUI(this);
    m_materialExportGUI->show();
}


void CMainWindowGUI::Execute(void)
{
#if defined(__OSX__) || defined(__WIN32__)
    
    NSView* view = reinterpret_cast<NSView*>(ui->opengl_window->winId());
    
    Set_ScreenWidth(static_cast<ui32>(view.frame.size.width));
    Set_ScreenHeight(static_cast<ui32>(view.frame.size.height));
    
    m_iesaWorkflow = std::make_shared<CIESAWorkflow>();
    m_iesaTransition = std::static_pointer_cast<CIESAMainTransition>(m_iesaWorkflow->CreateIESAMainTransition("main.transition.xml", (__bridge void*)view));
    m_iesaWorkflow->RegisterTransition(std::static_pointer_cast<IGameTransition>(m_iesaTransition));
    m_iesaWorkflow->GoToTransition("main.transition.xml");
    
#endif
}

void CMainWindowGUI::closeEvent(QCloseEvent *)
{
#if defined(__OSX__) || defined(__WIN32__)
    
    TerminateGameLoop();
    
#endif
}

void CMainWindowGUI::on_materials_list_currentIndexChanged(const QString &arg1)
{
#if defined(__OSX__) || defined(__WIN32__)
    
    m_mode = arg1.toUtf8().constData();

#endif
}

void CMainWindowGUI::on_buttonOpen_clicked(void)
{
}

void CMainWindowGUI::on_buttonSave_clicked(void)
{
    std::cout<<"save action"<<std::endl;
}

void CMainWindowGUI::on_buttonExit_clicked(void)
{
    std::cout<<"exit action"<<std::endl;
}

void CMainWindowGUI::on_btn_shader_compile_clicked()
{
    m_shaderCompileGUI = new CShaderCompileGUI(this);
    
#if defined(__OSX__) || defined(__WIN32__)
    
    m_shaderCompileGUI->Set_Mode(m_mode);
    m_shaderCompileGUI->Set_Transition(m_iesaTransition);
    
#endif
    
    m_shaderCompileGUI->exec();
}

void CMainWindowGUI::on_btn_content_path_clicked()
{
    QString path = QFileDialog::getExistingDirectory();
    if (path.isEmpty())
    {
        return;
    }
    else
    {
        ui->lbl_content_path->setText("path: " + path);
        ui->btn_open->setDisabled(false);
        ui->btn_open->setStyleSheet(kBtnEnableStyleSheet);
        Set_BundlePath(path.toUtf8().constData());
    }
}

void CMainWindowGUI::on_btn_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open..."), "", tr("Files (*.xml)"));
    if (filename.isEmpty())
    {
        return;
    }
    else
    {
#if defined(__OSX__) || defined(__WIN32__)
        
        m_iesaTransition->LoadGameObject(filename.toUtf8().constData());
        ITemplateLoadingHandler::TEMPLATE_LOADING_HANDLER handler;
        std::function<void(const std::shared_ptr<I_RO_TemplateCommon>&)> function = [handler, this](const std::shared_ptr<I_RO_TemplateCommon>& _template)
        {
            ui->materials_list->clear();
            std::shared_ptr<CGameObjectTemplate> gameObjectTemplate = std::static_pointer_cast<CGameObjectTemplate>(_template);
            
            for(const auto& iterator : gameObjectTemplate->Get_MaterialsTemplates())
            {
                std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(iterator);
                ui->materials_list->addItem(materialTemplate->Get_RenderOperationName().c_str());
            }

            m_mode = ui->materials_list->currentText().toUtf8().constData();
            ui->btn_shader_compile->setDisabled(ui->materials_list->count() == 0);
            ui->btn_shader_compile->setStyleSheet(ui->materials_list->count() == 0 ? kBtnDisableStyleSheet : kBtnFocusStyleSheet);
            
            ui->btn_material_editor->setDisabled(ui->materials_list->count() == 0);
            ui->btn_material_editor->setStyleSheet(ui->materials_list->count() == 0 ? kBtnDisableStyleSheet : kBtnFocusStyleSheet);
            
            ui->btn_save->setDisabled(false);
            ui->btn_save->setStyleSheet(kBtnEnableStyleSheet);
            
            ui->btn_saveas->setDisabled(false);
            ui->btn_saveas->setStyleSheet(kBtnEnableStyleSheet);
        };
        handler = std::make_shared<std::function<void(const std::shared_ptr<I_RO_TemplateCommon>&)>>(function);
        m_iesaTransition->Get_GameObjectExtension()->Get_Template(handler);
        
#endif
    }
}

void CMainWindowGUI::on_btn_save_clicked()
{

}

void CMainWindowGUI::on_btn_saveas_clicked()
{

}

void CMainWindowGUI::on_btn_material_editor_clicked()
{
    m_materialSettingsGUI = new CMaterialSettingsGUI(this);
    
#if defined(__OSX__) || defined(__WIN32__)
    
    m_materialSettingsGUI->Set_Mode(m_mode);
    m_materialSettingsGUI->Set_Transition(m_iesaTransition);
    
#endif
    
    m_materialSettingsGUI->show();
}
