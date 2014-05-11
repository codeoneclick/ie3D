//
//  CConfigurationAccessor.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationAccessor.h"
#include "CConfigurationGameObjects.h"
#include "CConfigurationModelLoadingOperation.h"
#include "CConfigurationOceanLoadingOperation.h"
#include "CConfigurationSkyBoxLoadingOperation.h"
#include "CConfigurationLandscapeLoadingOperation.h"
#include "CConfigurationParticleEmitterLoadingOperation.h"
#include "CConfigurationGameTransitionLoadingOperation.h"

CConfigurationAccessor::CConfigurationAccessor(void)
{
    
}

CConfigurationAccessor::~CConfigurationAccessor(void)
{
    
}

void CConfigurationAccessor::LoadModelConfiguration(const std::string& filename,
                                                    ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CConfigurationModelLoadingOperation> operation = std::make_shared<CConfigurationModelLoadingOperation>();
    std::shared_ptr<CConfigurationModel> modelConfiguration = std::static_pointer_cast<CConfigurationModel>(operation->serialize(filename));
    assert(modelConfiguration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(modelConfiguration, true);
}

void CConfigurationAccessor::LoadOceanConfiguration(const std::string& filename,
                                                    ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CConfigurationOceanLoadingOperation> operation = std::make_shared<CConfigurationOceanLoadingOperation>();
    std::shared_ptr<CConfigurationOcean> oceanConfiguration = std::static_pointer_cast<CConfigurationOcean>(operation->serialize(filename));
    assert(oceanConfiguration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(oceanConfiguration, true);
}

void CConfigurationAccessor::LoadLandscapeConfiguration(const std::string& filename,
                                                        ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CConfigurationLandscapeLoadingOperation> operation = std::make_shared<CConfigurationLandscapeLoadingOperation>();
    std::shared_ptr<CConfigurationLandscape> landscapeConfiguration = std::static_pointer_cast<CConfigurationLandscape>(operation->serialize(filename));
    assert(landscapeConfiguration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(landscapeConfiguration, true);
}

void CConfigurationAccessor::LoadParticleEmitterConfiguration(const std::string& filename,
                                                              ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CConfigurationParticleEmitterLoadingOperation> operation = std::make_shared<CConfigurationParticleEmitterLoadingOperation>();
    std::shared_ptr<CConfigurationParticleEmitter> particleEmitterConfiguration = std::static_pointer_cast<CConfigurationParticleEmitter>(operation->serialize(filename));
    assert(particleEmitterConfiguration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(particleEmitterConfiguration, true);
}

void CConfigurationAccessor::LoadGameTransitionConfiguration(const std::string& filename,
                                                             ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CConfigurationGameTransitionLoadingOperation> operation = std::make_shared<CConfigurationGameTransitionLoadingOperation>();
    std::shared_ptr<CConfigurationGameTransition> gameTransitionConfiguration = std::static_pointer_cast<CConfigurationGameTransition>(operation->serialize(filename));
    assert(gameTransitionConfiguration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(gameTransitionConfiguration, true);
}

void CConfigurationAccessor::loadSkyBoxConfiguration(const std::string& filename,
                                                     ISharedConfigurationLoadingHandlerRef handler)
{
    std::shared_ptr<CConfigurationSkyBoxLoadingOperation> operation = std::make_shared<CConfigurationSkyBoxLoadingOperation>();
    std::shared_ptr<CConfigurationSkyBox> skyBoxConfiguration = std::static_pointer_cast<CConfigurationSkyBox>(operation->serialize(filename));
    assert(skyBoxConfiguration != nullptr);
    assert(handler != nullptr);
    handler->onConfigurationLoaded(skyBoxConfiguration, true);
}