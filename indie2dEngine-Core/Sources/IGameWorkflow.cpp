//
//  IGameWorkflow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/6/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGameWorkflow.h"
#include "IGameTransition.h"
#include "CTemplateAccessor.h"
#include "CResourceAccessor.h"

IGameWorkflow::IGameWorkflow(void) :
m_templateAccessor(std::make_shared<CTemplateAccessor>()),
m_resourceAccessor(std::make_shared<CResourceAccessor>()),
m_currentTransition(nullptr)
{
    
}

IGameWorkflow::~IGameWorkflow(void)
{
    m_transitions.clear();
}

void IGameWorkflow::RegisterTransition(std::shared_ptr<IGameTransition> _transition)
{
    assert(m_transitions.find(_transition->Get_Guid()) == m_transitions.end());
    m_transitions.insert(std::make_pair(_transition->Get_Guid(), _transition));
}

void IGameWorkflow::UnregisterTransition(std::shared_ptr<IGameTransition> _transition)
{
    assert(m_transitions.find(_transition->Get_Guid()) != m_transitions.end());
    m_transitions.erase(m_transitions.find(_transition->Get_Guid()));
}

void IGameWorkflow::GoToTransition(const std::string &_guid)
{
    assert(m_transitions.find(_guid) != m_transitions.end());
    if(m_currentTransition != nullptr)
    {
        m_currentTransition->_OnDeactivate();
    }
    m_currentTransition = m_transitions.find(_guid)->second;
    m_currentTransition->_OnActivate();
}