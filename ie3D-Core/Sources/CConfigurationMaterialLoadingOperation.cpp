//
//  CConfigurationMaterialLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationMaterialSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationMaterialLoadingOperation::CConfigurationMaterialLoadingOperation(void)
{
    
}

CConfigurationMaterialLoadingOperation::~CConfigurationMaterialLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationMaterialLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationMaterialSerializer> serializer = std::make_shared<CConfigurationMaterialSerializer>();
    return serializer->serialize(filename);
}

void CConfigurationMaterialLoadingOperation::serializeGameObjectMaterialsConfigurations(CSharedConfigurationGameObjectRef gameObjectConfiguration)
{
    std::vector<std::string> materialsConfigurationsFilenames = gameObjectConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsConfigurationsFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        gameObjectConfiguration->setConfiguration(getConfigurationAttributeKey(gameObjectConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                               gameObjectConfiguration->kGameObjectMaterialConfigurationNode,
                                                                               gameObjectConfiguration->kGameObjectMaterialFilenameAttribute),
                                                  materialConfiguration);
    }
}