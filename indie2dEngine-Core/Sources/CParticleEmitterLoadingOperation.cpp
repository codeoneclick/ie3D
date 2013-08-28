//
//  CParticleEmitterLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitterLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "CParticleEmitterTemplateSerializer.h"
#include "ITemplate.h"

CParticleEmitterLoadingOperation::CParticleEmitterLoadingOperation(void)
{
    
}

CParticleEmitterLoadingOperation::~CParticleEmitterLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> CParticleEmitterLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CParticleEmitterTemplateSerializer> particleEmitterSerializer = std::make_shared<CParticleEmitterTemplateSerializer>();
    std::shared_ptr<SModelTemplate> particleEmitterTemplate = std::static_pointer_cast<SModelTemplate>(particleEmitterSerializer->Serialize(_filename));
    assert(particleEmitterTemplate != nullptr);
    for(const auto& iterator : particleEmitterTemplate->m_materialsFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        particleEmitterTemplate->m_materialsTemplates.push_back(materialTemplate);
    }
    return particleEmitterTemplate;
}