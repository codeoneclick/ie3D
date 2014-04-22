//
//  CConfigurationModelLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationModelLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationModelSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationModelLoadingOperation::CConfigurationModelLoadingOperation(void)
{
    
}

CConfigurationModelLoadingOperation::~CConfigurationModelLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationModelLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationModelSerializer> modelSerializer = std::make_shared<CConfigurationModelSerializer>();
    std::shared_ptr<CConfigurationModel> modelConfiguration = std::static_pointer_cast<CConfigurationModel>(modelSerializer->serialize(filename));
    assert(modelConfiguration != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = modelConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        modelConfiguration->setConfiguration(getConfigurationAttributeKey(modelConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                          modelConfiguration->kGameObjectMaterialConfigurationNode,
                                                                          modelConfiguration->kGameObjectMaterialFilenameAttribute),
                                             materialConfiguration);
    }
    return modelConfiguration;
}