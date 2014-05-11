//
//  CConfigurationSkyBoxLoadingOperation.cpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 5/6/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationSkyBoxLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationSkyBoxSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationSkyBoxLoadingOperation::CConfigurationSkyBoxLoadingOperation(void)
{
    
}

CConfigurationSkyBoxLoadingOperation::~CConfigurationSkyBoxLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationSkyBoxLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationSkyBoxSerializer> skyBoxSerializer = std::make_shared<CConfigurationSkyBoxSerializer>();
    std::shared_ptr<CConfigurationSkyBox> skyBoxConfiguration = std::static_pointer_cast<CConfigurationSkyBox>(skyBoxSerializer->serialize(filename));
    assert(skyBoxConfiguration != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = skyBoxConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        skyBoxConfiguration->setConfiguration(getConfigurationAttributeKey(skyBoxConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                           skyBoxConfiguration->kGameObjectMaterialConfigurationNode,
                                                                           skyBoxConfiguration->kGameObjectMaterialFilenameAttribute),
                                              materialConfiguration);
    }
    return skyBoxConfiguration;
}