//
//  CTemplateMgr.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTemplateMgr.h"
#include "CModelTemplateLoader.h"
#include "CParticleEmitterTemplateLoader.h"

CTemplateMgr::CTemplateMgr(void)
{
    m_loaders[E_TEMPLATE_LOADERS_MODEL] = std::make_shared<CModelTemplateLoader>();
    m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER] = std::make_shared<CParticleEmitterTemplateLoader>();
}

CTemplateMgr::~CTemplateMgr(void)
{
    
}

void CTemplateMgr::LoadModelTemplate(const std::string &_filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_MODEL] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_MODEL]->Load(_filename, _handler);
}

void CTemplateMgr::LoadParticleEmitterTemplate(const std::string &_filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER]->Load(_filename, _handler);
}