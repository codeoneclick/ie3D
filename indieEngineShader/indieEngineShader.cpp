#include "indieEngineShader.h"
#include "ui_indieEngineShader.h"

#include <Cocoa/Cocoa.h>
#include "IOGLWindow.h"
#include "CIESAWorkflow.h"
#include "CCommonOS.h"
#include "CGameLoopExecutor.h"

indieEngineShader::indieEngineShader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::indieEngineShader)
{
    ui->setupUi(this);
    
    
}

indieEngineShader::~indieEngineShader()
{
    delete ui;
}

void indieEngineShader::Execute(void)
{
    NSView* view = reinterpret_cast<NSView*>(ui->window->winId());
    
    Set_ScreenWidth(static_cast<ui32>(view.frame.size.width));
    Set_ScreenHeight(static_cast<ui32>(view.frame.size.height));
    
    CIESAWorkflow* workflow = new CIESAWorkflow();
    std::shared_ptr<IGameTransition> transition = workflow->CreateIESAMainTransition("main.transition.xml", (__bridge void*)view);
    workflow->RegisterTransition(transition);
    workflow->GoToTransition("main.transition.xml");
}

void indieEngineShader::on_pushButton_clicked()
{
    
}

void indieEngineShader::closeEvent(QCloseEvent *)
{
    TerminateGameLoop();
}
