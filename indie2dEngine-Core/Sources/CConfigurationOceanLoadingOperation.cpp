//
//  COceanTeamplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationOceanLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationOceanSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationOceanLoadingOperation::CConfigurationOceanLoadingOperation(void)
{
    
}

CConfigurationOceanLoadingOperation::~CConfigurationOceanLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationOceanLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationOceanSerializer> oceanSerializer = std::make_shared<CConfigurationOceanSerializer>();
    std::shared_ptr<CConfigurationOcean> oceanConfiguration = std::static_pointer_cast<CConfigurationOcean>(oceanSerializer->serialize(filename));
    assert(oceanConfiguration != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = oceanConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        oceanConfiguration->setConfiguration(getConfigurationAttributeKey(oceanConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                          oceanConfiguration->kGameObjectMaterialConfigurationNode,
                                                                          oceanConfiguration->kGameObjectMaterialFilenameAttribute),
                                             materialConfiguration);
    }
    return oceanConfiguration;
}