//
//  CParticleEmitterLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitterLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationParticleEmitterSerializer.h"
#include "CTemplateGameObjects.h"

CParticleEmitterLoadingOperation::CParticleEmitterLoadingOperation(void)
{
    
}

CParticleEmitterLoadingOperation::~CParticleEmitterLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CParticleEmitterLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationParticleEmitterSerializer> particleEmitterSerializer = std::make_shared<CConfigurationParticleEmitterSerializer>();
    std::shared_ptr<CConfigurationParticleEmitter> particleEmitterTemplate = std::static_pointer_cast<CConfigurationParticleEmitter>(particleEmitterSerializer->Serialize(_filename));
    assert(particleEmitterTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = particleEmitterTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        particleEmitterTemplate->Set_Template(Get_TemplateAttributeKey(particleEmitterTemplate->kGameObjectMaterialsTemplatesNode,
                                                                       particleEmitterTemplate->kGameObjectMaterialTemplateNode,
                                                                       particleEmitterTemplate->kGameObjectMaterialFilenameAttribute),
                                              materialTemplate);
    }
    return particleEmitterTemplate;
}