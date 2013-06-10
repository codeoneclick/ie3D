//
//  CTemplateAccessor.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTemplateAccessor.h"
#include "CModelTemplateLoader.h"
#include "CParticleEmitterTemplateLoader.h"
#include "CGameTransitionTemplateLoader.h"

CTemplateAccessor::CTemplateAccessor(void)
{
    m_loaders[E_TEMPLATE_LOADERS_MODEL] = std::make_shared<CModelTemplateLoader>();
    m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER] = std::make_shared<CParticleEmitterTemplateLoader>();
    m_loaders[E_TEMPLATE_LOADERS_GAME_TRANSITIONS] = std::make_shared<CGameTransitionTemplateLoader>();
}

CTemplateAccessor::~CTemplateAccessor(void)
{
    
}

void CTemplateAccessor::LoadModelTemplate(const std::string &_filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_MODEL] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_MODEL]->Load(_filename, _handler);
}

void CTemplateAccessor::LoadParticleEmitterTemplate(const std::string &_filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER]->Load(_filename, _handler);
}

void CTemplateAccessor::LoadGameTransitionTemplate(const std::string &_filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_GAME_TRANSITIONS] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_GAME_TRANSITIONS]->Load(_filename, _handler);
}