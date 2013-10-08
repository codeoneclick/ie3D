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

std::shared_ptr<IGameTransition> CKOTHWorkflow::CreateXcomInGameTransition(const std::string &_filename, void *_hwnd)
{
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::CreateGraphicsContext(_hwnd, E_PLATFORM_API_IOS);
    std::shared_ptr<IInputContext> inputContext = std::make_shared<IInputContext>(_hwnd);
    std::shared_ptr<CKOTHInGameTransition> gameXcomInGameTransition = std::make_shared<CKOTHInGameTransition>(_filename, graphicsContext, inputContext, m_resourceAccessor, m_templateAccessor);
    m_templateAccessor->LoadGameTransitionTemplate(_filename, gameXcomInGameTransition);
    assert(gameXcomInGameTransition != nullptr);
    return gameXcomInGameTransition;
}