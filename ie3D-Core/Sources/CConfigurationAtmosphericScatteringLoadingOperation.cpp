//
//  CConfigurationAtmosphericScatteringLoadingOperation.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 9/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationAtmosphericScatteringLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationAtmosphericScatteringSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationAtmosphericScatteringLoadingOperation::CConfigurationAtmosphericScatteringLoadingOperation(void)
{
    
}

CConfigurationAtmosphericScatteringLoadingOperation::~CConfigurationAtmosphericScatteringLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationAtmosphericScatteringLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationAtmosphericScatteringSerializer> atmosphericScatteringSerializer = std::make_shared<CConfigurationAtmosphericScatteringSerializer>();
    std::shared_ptr<CConfigurationAtmosphericScattering> atmosphericScatteringConfiguration = std::static_pointer_cast<CConfigurationAtmosphericScattering>(atmosphericScatteringSerializer->serialize(filename));
    assert(atmosphericScatteringConfiguration != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = atmosphericScatteringConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        atmosphericScatteringConfiguration->setConfiguration(getConfigurationAttributeKey(atmosphericScatteringConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                                          atmosphericScatteringConfiguration->kGameObjectMaterialConfigurationNode,
                                                                                          atmosphericScatteringConfiguration->kGameObjectMaterialFilenameAttribute),
                                                             materialConfiguration);
    }
    return atmosphericScatteringConfiguration;
}