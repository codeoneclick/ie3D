//
//  CTemplateAccessor.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CTemplateAccessor.h"
#include "CModelTemplateLoader.h"
#include "COceanTemplateLoader.h"
#include "CLandscapeTemplateLoader.h"
#include "CParticleEmitterTemplateLoader.h"
#include "CGameTransitionTemplateLoader.h"
#include "CGUITemplateLoader.h"

CTemplateAccessor::CTemplateAccessor(void)
{
    m_loaders[E_TEMPLATE_LOADERS_MODEL] = std::make_shared<CModelTemplateLoader>();
    m_loaders[E_TEMPLATE_LOADERS_OCEAN] = std::make_shared<COceanTemplateLoader>();
    m_loaders[E_TEMPLATE_LOADERS_LANDSCAPE] = std::make_shared<CLandscapeTemplateLoader>();
    m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER] = std::make_shared<CParticleEmitterTemplateLoader>();
    m_loaders[E_TEMPLATE_LOADERS_GAME_TRANSITIONS] = std::make_shared<CGameTransitionTemplateLoader>();
	m_loaders[E_TEMPLATE_LOADERS_GUI] = std::make_shared<CGUITemplateLoader>();
}

CTemplateAccessor::~CTemplateAccessor(void)
{
    
}

void CTemplateAccessor::LoadModelTemplate(const std::string &_filename, const std::shared_ptr<ITemplateLoadingHandler>& _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_MODEL] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_MODEL]->Load(_filename, _handler);
}

void CTemplateAccessor::LoadOceanTemplate(const std::string &_filename, const std::shared_ptr<ITemplateLoadingHandler>& _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_OCEAN] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_OCEAN]->Load(_filename, _handler);
}

void CTemplateAccessor::LoadLandscapeTemplate(const std::string &_filename, const std::shared_ptr<ITemplateLoadingHandler>& _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_LANDSCAPE] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_LANDSCAPE]->Load(_filename, _handler);
}

void CTemplateAccessor::LoadParticleEmitterTemplate(const std::string &_filename, const std::shared_ptr<ITemplateLoadingHandler>& _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_PARTICLE_EMITTER]->Load(_filename, _handler);
}

void CTemplateAccessor::LoadGameTransitionTemplate(const std::string &_filename, const std::shared_ptr<ITemplateLoadingHandler>& _handler)
{
    assert(m_loaders[E_TEMPLATE_LOADERS_GAME_TRANSITIONS] != nullptr);
    assert(_handler != nullptr);
    m_loaders[E_TEMPLATE_LOADERS_GAME_TRANSITIONS]->Load(_filename, _handler);
}

void CTemplateAccessor::LoadGUITemplate(const std::string& _filename, const std::shared_ptr<ITemplateLoadingHandler>& _handler, E_GUI_LOADING_SCHEME _scheme)
{
	assert(m_loaders[E_TEMPLATE_LOADERS_GUI] != nullptr);
	assert(_handler != nullptr);
	std::static_pointer_cast<CGUITemplateLoader>(m_loaders[E_TEMPLATE_LOADERS_GUI])->Load(_filename, _handler, _scheme);
}