//
//  CParticleEmitterTemplateLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitterTemplateLoader.h"
#include "CParticleEmitterLoadingOperation.h"
#include "ITemplate.h"
#include "ITemplateLoadingHandler.h"

CParticleEmitterTemplateLoader::CParticleEmitterTemplateLoader(void)
{
    
}

CParticleEmitterTemplateLoader::~CParticleEmitterTemplateLoader(void)
{
    
}

void CParticleEmitterTemplateLoader::_Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::shared_ptr<CParticleEmitterLoadingOperation> operation = std::make_shared<CParticleEmitterLoadingOperation>();
    std::shared_ptr<SParticleEmitterTemplate> particleEmitterTemplate = std::static_pointer_cast<SParticleEmitterTemplate>(operation->Serialize(_filename));
    assert(particleEmitterTemplate != nullptr);
    assert(_handler != nullptr);
     _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(particleEmitterTemplate);
}

void CParticleEmitterTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::async(std::launch::async, &CParticleEmitterTemplateLoader::_Load, this, _filename, _handler);
}
