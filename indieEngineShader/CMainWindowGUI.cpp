#include "CMainWindowGUI.h"
#include "ui_CMainWindowGUI.h"

#if defined(__OSX__)
#include <Cocoa/Cocoa.h>
#endif

#if defined(__OSX__) || defined(__WIN32__)
#include "IOGLWindow.h"
#include "CIESAWorkflow.h"
#include "СIESAMainTransition.h"
#include "CCommonOS.h"
#include "CGameLoopExecutor.h"
#endif

CMainWindowGUI::CMainWindowGUI(QWidget *parent) :
    QMainWindow(parent),
#if defined(__OSX__) || defined(__WIN32__)

    m_iesaWorkflow(nullptr),
    m_iesaTransition(nullptr),

#endif
    ui(new Ui::CMainWindowGUI)
{
    ui->setupUi(this);
}

CMainWindowGUI::~CMainWindowGUI()
{
    delete ui;
}

void CMainWindowGUI::on_pushButton_clicked()
{
    m_shaderCompileGUI = new CShaderCompileGUI(this);
    
#if defined(__OSX__) || defined(__WIN32__)
    
    m_shaderCompileGUI->Set_Transition(m_iesaTransition);
    
#endif
    
    m_shaderCompileGUI->exec();
}

void CMainWindowGUI::on_pushButton_2_clicked()
{
    m_materialSettingsGUI = new CMaterialSettingsGUI(this);
    m_materialSettingsGUI->show();
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
