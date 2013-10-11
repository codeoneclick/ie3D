#include "CGameViewController_win32.h"
#include "IOGLWindow.h"
#include "CKOTHWorkflow.h"
#include "IGameTransition.h"

CGameViewController_win32::CGameViewController_win32(void)
{
	m_window = new IOGLWindow();
	m_workflow = new CKOTHWorkflow();
    std::shared_ptr<IGameTransition> transition = static_cast<CKOTHWorkflow*>(m_workflow)->CreateKOTHInGameTransition("main.transition.xml", (void*)(m_window));
    m_workflow->RegisterTransition(transition);
    m_workflow->GoToTransition("main.transition.xml");
}

CGameViewController_win32::~CGameViewController_win32(void)
{

}