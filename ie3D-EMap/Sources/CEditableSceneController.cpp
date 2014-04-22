//
//  CEditableSceneController.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CEditableSceneController.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "CEditableSceneTransition.h"
#include "CConfigurationAccessor.h"

std::shared_ptr<IGameTransition> CEditableSceneController::createEditableSceneTransition(const std::string& filename,
                                                                                         ISharedOGLWindowRef window)
{
#if defined(__OSX__)
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_OSX);
    std::shared_ptr<IInputContext> gestureRecognizerContext = IInputContext::createInputContext(window, E_PLATFORM_API_OSX);
#endif

    std::shared_ptr<CEditableSceneTransition> mainTransition = std::make_shared<CEditableSceneTransition>(filename,
                                                                                                          graphicsContext,
                                                                                                          gestureRecognizerContext,
                                                                                                          m_resourceAccessor,
                                                                                                          m_configurationAccessor);
    m_configurationAccessor->LoadGameTransitionConfiguration(filename, mainTransition);
    assert(mainTransition != nullptr);
    return mainTransition;
}