//
//  CKOTHWorkflow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CKOTHWorkflow.h"
#include "CGraphicsContext_iOS.h"
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
	UIView* hwnd = (__bridge UIView*)_hwnd;
    assert([[hwnd layer] isKindOfClass:[CAEAGLLayer class]]);
    std::shared_ptr<CGraphicsContext_iOS> graphicsContext = std::make_shared<CGraphicsContext_iOS>(static_cast<CAEAGLLayer*>(hwnd.layer));
    std::shared_ptr<IInputContext> inputContext = std::make_shared<IInputContext>(_hwnd);
    std::shared_ptr<CKOTHInGameTransition> gameXcomInGameTransition = std::make_shared<CKOTHInGameTransition>(_filename, graphicsContext, inputContext, m_resourceAccessor, m_templateAccessor);
    m_templateAccessor->LoadGameTransitionTemplate(_filename, gameXcomInGameTransition);
    assert(gameXcomInGameTransition != nullptr);
    return gameXcomInGameTransition;
}