#include "indieEngineShader.h"
#include "ui_indieEngineShader.h"

#include <Cocoa/Cocoa.h>
#include "IOGLWindow.h"
#include "CIESAWorkflow.h"

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

void indieEngineShader::on_pushButton_clicked()
{
    NSView* view = reinterpret_cast<NSView*>(ui->window->winId());
    CIESAWorkflow* workflow = new CIESAWorkflow();
    std::shared_ptr<IGameTransition> transition = workflow->CreateIESAMainTransition("main.transition.xml", (__bridge void*)view);
    workflow->RegisterTransition(transition);
    workflow->GoToTransition("main.transition.xml");
}
