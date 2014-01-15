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
#include "CTemplateGameObjects.h"

CParticleEmitterLoadingOperation::CParticleEmitterLoadingOperation(void)
{
    
}

CParticleEmitterLoadingOperation::~CParticleEmitterLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CParticleEmitterLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CParticleEmitterTemplateSerializer> particleEmitterSerializer = std::make_shared<CParticleEmitterTemplateSerializer>();
    std::shared_ptr<CParticleEmitterTemplate> particleEmitterTemplate = std::static_pointer_cast<CParticleEmitterTemplate>(particleEmitterSerializer->Serialize(_filename));
    assert(particleEmitterTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = particleEmitterTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        particleEmitterTemplate->Set_Template(Get_TemplateAttributeKey(particleEmitterTemplate->kGameObjectMaterialsTemplatesNode,
                                                                       particleEmitterTemplate->kGameObjectMaterialTemplateNode,
                                                                       particleEmitterTemplate->kGameObjectMaterialFilenameAttribute),
                                              materialTemplate);
    }
    return particleEmitterTemplate;
}