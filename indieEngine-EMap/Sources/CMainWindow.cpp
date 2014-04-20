#include "CMainWindow.h"
#include "ui_CMainWindow.h"

#if defined(__OSX__) || defined(__WIN32__)

#include "CEditableSceneController.h"
#include "CEditableSceneTransition.h"
#include "CEditableScene.h"
#include "IOGLWindow.h"

#endif

#if defined(__OSX__)

#include <Cocoa/Cocoa.h>

#endif

CMainWindow::CMainWindow(QWidget *parent) :
QMainWindow(parent),
#if defined(__OSX__) || defined(__WIN32__)

m_editableSceneController(nullptr),
m_editableSceneTransition(nullptr),

#endif
ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::execute(void)
{
#if defined(__OSX__) || defined(__WIN32__)
    
    NSView* view = reinterpret_cast<NSView*>(ui->m_oglWindow->winId());
    
    std::shared_ptr<IOGLWindow> window = std::make_shared<IOGLWindow>((__bridge void*)view);
    m_editableSceneController = std::make_shared<CEditableSceneController>();
    m_editableSceneTransition = std::static_pointer_cast<CEditableSceneTransition>(m_editableSceneController->createEditableSceneTransition("main.transition.xml", window));
    m_editableSceneController->RegisterTransition(m_editableSceneTransition);
    m_editableSceneController->GoToTransition("main.transition.xml");
    
#endif
}