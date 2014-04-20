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
#include "CConfigurationAccessor.h"
#include "IOGLWindow.h"

CKOTHWorkflow::CKOTHWorkflow(void)
{
    
}

CKOTHWorkflow::~CKOTHWorkflow(void)
{
    
}

std::shared_ptr<IGameTransition> CKOTHWorkflow::CreateKOTHInGameTransition(const std::string &_filename, ISharedOGLWindowRef window)
{
#if defined (__IOS__)
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_IOS);
    std::shared_ptr<IInputContext> inputContext = IInputContext::createInputContext(window, E_PLATFORM_API_IOS);
#elif defined(__WIN32__)
	std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_WIN32);
    std::shared_ptr<IInputContext> inputContext = IInputContext::createInputContext(window, E_PLATFORM_API_WIN32);
#elif defined(__NDK__)
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_NDK);
    std::shared_ptr<IInputContext> inputContext = IInputContext::createInputContext(window, E_PLATFORM_API_NDK);
#endif

    std::shared_ptr<CKOTHInGameTransition> gameKOTHInGameTransition = std::make_shared<CKOTHInGameTransition>(_filename, graphicsContext, inputContext, m_resourceAccessor, m_templateAccessor);
    m_templateAccessor->LoadGameTransitionConfiguration(_filename, gameKOTHInGameTransition);
    assert(gameKOTHInGameTransition != nullptr);
    return gameKOTHInGameTransition;
}