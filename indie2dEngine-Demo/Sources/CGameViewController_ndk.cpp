//
//  CGameViewController_ndk.cpp
//  indieEngineDemo
//
//  Created by Sergey Sergeev on 11/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameViewController_ndk.h"

#if defined(__NDK__)

#include "IOGLWindow.h"
#include "CKOTHWorkflow.h"
#include "IGameTransition.h"

CGameViewController_ndk::CGameViewController_ndk(void)
{
	m_window = new IOGLWindow();
	m_workflow = new CKOTHWorkflow();
    std::shared_ptr<IGameTransition> transition = static_cast<CKOTHWorkflow*>(m_workflow)->CreateKOTHInGameTransition("main.transition.xml", nullptr);
    //m_workflow->RegisterTransition(transition);
    //m_workflow->GoToTransition("main.transition.xml");
}

CGameViewController_ndk::~CGameViewController_ndk(void)
{
    
}

#endif