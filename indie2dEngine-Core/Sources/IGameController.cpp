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

static IGameController* g_workflow = nullptr;

IGameController::IGameController(void) :
m_templateAccessor(std::make_shared<CConfigurationAccessor>()),
m_resourceAccessor(std::make_shared<CResourceAccessor>()),
m_currentTransition(nullptr)
{
    g_workflow = this;
}

IGameController::~IGameController(void)
{
    g_workflow = nullptr;
    m_transitions.clear();
}

void IGameController::RegisterTransition(std::shared_ptr<IGameTransition> _transition)
{
    assert(m_transitions.find(_transition->Get_Guid()) == m_transitions.end());
    m_transitions.insert(std::make_pair(_transition->Get_Guid(), _transition));
}

void IGameController::UnregisterTransition(std::shared_ptr<IGameTransition> _transition)
{
    assert(m_transitions.find(_transition->Get_Guid()) != m_transitions.end());
    m_transitions.erase(m_transitions.find(_transition->Get_Guid()));
}

void IGameController::GoToTransition(const std::string &_guid)
{
    assert(m_transitions.find(_guid) != m_transitions.end());
    if(m_currentTransition != nullptr)
    {
        m_currentTransition->_OnDeactivate();
        DisconnectFromGameLoop(m_currentTransition);
    }
    m_currentTransition = m_transitions.find(_guid)->second;
    m_currentTransition->_OnActivate();
    ConnectToGameLoop(m_currentTransition);
}

ui32 IGameController::Get_CurrentNumTriagles(void)
{
    return m_currentTransition != nullptr ? m_currentTransition->Get_CurrentNumTriangles() : 0;
}

ui32 IGameController::Get_TotalNumTriangles(void)
{
    return m_currentTransition != nullptr ? m_currentTransition->Get_TotalNumTriangles() : 0;
}

ui32 Get_CurrentNumTriagles(void)
{
    assert(g_workflow != nullptr);
    return g_workflow->Get_CurrentNumTriagles();
}

ui32 Get_TotalNumTriangles(void)
{
    assert(g_workflow != nullptr);
    return g_workflow->Get_TotalNumTriangles();
}
