//
//  CParticleEmitterLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationParticleEmitterLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationParticleEmitterSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationParticleEmitterLoadingOperation::CConfigurationParticleEmitterLoadingOperation(void)
{
    
}

CConfigurationParticleEmitterLoadingOperation::~CConfigurationParticleEmitterLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationParticleEmitterLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationParticleEmitterSerializer> particleEmitterSerializer = std::make_shared<CConfigurationParticleEmitterSerializer>();
    std::shared_ptr<CConfigurationParticleEmitter> particleEmitterConfiguration = std::static_pointer_cast<CConfigurationParticleEmitter>(particleEmitterSerializer->serialize(filename));
    assert(particleEmitterConfiguration != nullptr);
    CConfigurationMaterialLoadingOperation::serializeGameObjectMaterialsConfigurations(particleEmitterConfiguration);
    return particleEmitterConfiguration;
}