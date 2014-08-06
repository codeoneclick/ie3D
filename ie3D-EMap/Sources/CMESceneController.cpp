//
//  CMESceneController.cpp
//  indieEngine-EMap
//
//  Created by Sergey Sergeev on 4/20/14.
//
//

#include "CMESceneController.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"
#include "CMESceneTransition.h"
#include "CConfigurationAccessor.h"
#include "CMEConfigurationAccessor.h"

std::shared_ptr<IGameTransition> CMESceneController::createEditableSceneTransition(const std::string& filename,
                                                                                   ISharedOGLWindowRef window)
{
#if defined(__OSX__)
    std::shared_ptr<IGraphicsContext> graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_OSX);
    std::shared_ptr<IInputContext> gestureRecognizerContext = IInputContext::createInputContext(window, E_PLATFORM_API_OSX);
#endif
    m_configurationAccessor = std::make_shared<CMEConfigurationAccessor>();
    
    std::shared_ptr<CMESceneTransition> mainTransition = std::make_shared<CMESceneTransition>(filename,
                                                                                              graphicsContext,
                                                                                              gestureRecognizerContext,
                                                                                              m_resourceAccessor,
                                                                                              m_configurationAccessor);
    mainTransition->initScene();
    m_configurationAccessor->loadGameTransitionConfiguration(filename, mainTransition);
    assert(mainTransition != nullptr);
    return mainTransition;
}