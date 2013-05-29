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

CTemplateMgr::CTemplateMgr(void) :
m_modelTemplateLoader(std::make_shared<CModelTemplateLoader>()),
m_particleEmitterTemplateLoader(std::make_shared<CParticleEmitterTemplateLoader>())
{

}

CTemplateMgr::~CTemplateMgr(void)
{
    
}

void CTemplateMgr::LoadModelTemplate(const std::string &_filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    assert(m_modelTemplateLoader != nullptr);
    assert(_handler != nullptr);
    m_modelTemplateLoader->Load(_filename, _handler);
}

void CTemplateMgr::LoadParticleEmitterTemplate(const std::string &_filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    assert(m_particleEmitterTemplateLoader != nullptr);
    assert(_handler != nullptr);
    m_particleEmitterTemplateLoader->Load(_filename, _handler);
}