//
//  CIESAWorkflow.cpp
//  indieEngineShader
//
//  Created by Sergey Sergeev on 10/30/13.
//
//

#include "CIESAWorkflow.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "СIESAMainTransition.h"
#include "CTemplateAccessor.h"

CIESAWorkflow::CIESAWorkflow(void)
{
    
}

CIESAWorkflow::~CIESAWorkflow(void)
{
    
}

std::shared_ptr<IGameTransition> CIESAWorkflow::CreateIESAMainTransition(const std::string &_filename, void *_hwnd)
{
#if defined (__IOS__)
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::CreateGraphicsContext(_hwnd, E_PLATFORM_API_IOS);
#elif defined(__WIN32__)
	std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::CreateGraphicsContext(_hwnd, E_PLATFORM_API_WIN32);
#elif defined(__OSX__)
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::CreateGraphicsContext(_hwnd, E_PLATFORM_API_OSX);
#endif
    std::shared_ptr<IInputContext> inputContext = std::make_shared<IInputContext>(_hwnd);
    std::shared_ptr<CIESAMainTransition> mainTransition = std::make_shared<CIESAMainTransition>(_filename, graphicsContext, inputContext, m_resourceAccessor, m_templateAccessor);
    m_templateAccessor->LoadGameTransitionTemplate(_filename, mainTransition);
    assert(mainTransition != nullptr);
    return mainTransition;
}