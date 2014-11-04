//
//  IGameController.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameController.h"
#include "IGameTransition.h"
#include "CConfigurationAccessor.h"
#include "CResourceAccessor.h"
#include "CGameLoopExecutor.h"
#include "IGraphicsContext.h"
#include "IInputContext.h"

IGameController::IGameController(ISharedOGLWindowRef window) :
m_configurationAccessor(std::make_shared<CConfigurationAccessor>()),
m_resourceAccessor(std::make_shared<CResourceAccessor>()),
m_currentTransition(nullptr)
{
#if defined (__IOS__)
    m_graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_IOS);
    m_gestureRecognizerContext = IInputContext::createInputContext(window, E_PLATFORM_API_IOS);
#elif defined(__WIN32__)
    m_graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_WIN32);
    m_gestureRecognizerContext = IInputContext::createInputContext(window, E_PLATFORM_API_WIN32);
#elif defined(__NDK__)
    m_graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_NDK);
    m_gestureRecognizerContext = IInputContext::createInputContext(window, E_PLATFORM_API_NDK);
#elif defined(__OSX__)
    m_graphicsContext = IGraphicsContext::createGraphicsContext(window, E_PLATFORM_API_OSX);
    m_gestureRecognizerContext = IInputContext::createInputContext(window, E_PLATFORM_API_OSX);
#endif
}

IGameController::~IGameController(void)
{
    m_transitions.clear();
}

void IGameController::addTransition(ISharedGameTransitionRef transition)
{
    assert(m_graphicsContext != nullptr);
    assert(m_gestureRecognizerContext != nullptr);
    
    assert(m_transitions.find(transition->getGuid()) == m_transitions.end());
    transition->setupOnce(m_graphicsContext, m_gestureRecognizerContext,
                          m_resourceAccessor, m_configurationAccessor);
    transition->initScene();
    m_configurationAccessor->loadGameTransitionConfiguration(transition->getGuid(), transition);
    m_transitions.insert(std::make_pair(transition->getGuid(), transition));
}

void IGameController::removeTransition(ISharedGameTransitionRef transition)
{
    assert(m_transitions.find(transition->getGuid()) != m_transitions.end());
    m_transitions.erase(m_transitions.find(transition->getGuid()));
}

void IGameController::addChildTransition(ISharedGameTransitionRef transition)
{
    assert(m_chilrenTransitions.find(transition->getGuid()) == m_chilrenTransitions.end());
    transition->setupOnce(m_graphicsContext, m_gestureRecognizerContext,
                          m_resourceAccessor, m_configurationAccessor);
    transition->initScene();
    m_configurationAccessor->loadGameTransitionConfiguration(transition->getGuid(), transition);
    m_chilrenTransitions.insert(std::make_pair(transition->getGuid(), transition));
}

void IGameController::removeChildTransition(ISharedGameTransitionRef transition)
{
    assert(m_chilrenTransitions.find(transition->getGuid()) != m_chilrenTransitions.end());
    m_chilrenTransitions.erase(m_transitions.find(transition->getGuid()));
}

void IGameController::activateChildTransition(const std::string& guid)
{
    assert(m_chilrenTransitions.find(guid) != m_chilrenTransitions.end());
    ISharedGameTransition transition = m_chilrenTransitions.find(guid)->second;
    transition->_OnActivate();
    ConnectToGameLoop(transition);
}

void IGameController::deactivateChildTransition(const std::string& guid)
{
    assert(m_chilrenTransitions.find(guid) != m_chilrenTransitions.end());
    ISharedGameTransition transition = m_chilrenTransitions.find(guid)->second;
    transition->_OnDeactivate();
    DisconnectFromGameLoop(transition);
}

void IGameController::gotoTransition(const std::string &guid)
{
    assert(m_transitions.find(guid) != m_transitions.end());
    if(m_currentTransition != nullptr)
    {
        m_currentTransition->_OnDeactivate();
        DisconnectFromGameLoop(m_currentTransition);
    }
    m_currentTransition = m_transitions.find(guid)->second;
    m_currentTransition->_OnActivate();
    ConnectToGameLoop(m_currentTransition);
}

ui32 IGameController::getCurrentNumTriagles(void)
{
    return m_currentTransition != nullptr ? m_currentTransition->getFrameNumTriangles() : 0;
}

ui32 IGameController::getTotalNumTriangles(void)
{
    return m_currentTransition != nullptr ? m_currentTransition->getSceneNumTriangles() : 0;
}
