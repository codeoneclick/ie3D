//
//  CGameViewController_win32.h
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/3/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController_win32.h"

#if defined(__WIN32__)

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

#endif