//
//  CKOTHWorkflow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CKOTHWorkflow.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "CKOTHInGameTransition.h"
#include "CTemplateAccessor.h"

CKOTHWorkflow::CKOTHWorkflow(void)
{
    
}

CKOTHWorkflow::~CKOTHWorkflow(void)
{
    
}

std::shared_ptr<IGameTransition> CKOTHWorkflow::CreateKOTHInGameTransition(const std::string &_filename, void *_hwnd)
{
#if defined (__IOS__)
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::CreateGraphicsContext(_hwnd, E_PLATFORM_API_IOS);
#elif defined(__WIN32__)
	std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::CreateGraphicsContext(_hwnd, E_PLATFORM_API_WIN32);
#endif
    std::shared_ptr<IInputContext> inputContext = std::make_shared<IInputContext>(_hwnd);
    std::shared_ptr<CKOTHInGameTransition> gameKOTHInGameTransition = std::make_shared<CKOTHInGameTransition>(_filename, graphicsContext, inputContext, m_resourceAccessor, m_templateAccessor);
    m_templateAccessor->LoadGameTransitionTemplate(_filename, gameKOTHInGameTransition);
    assert(gameKOTHInGameTransition != nullptr);
    return gameKOTHInGameTransition;
}