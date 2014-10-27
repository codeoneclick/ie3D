//
//  CConfigurationLandscapeLoadingOperation.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationLandscapeLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationLandscapeSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationLandscapeLoadingOperation::CConfigurationLandscapeLoadingOperation(void)
{
    
}

CConfigurationLandscapeLoadingOperation::~CConfigurationLandscapeLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationLandscapeLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationLandscapeSerializer> landscapeSerializer = std::make_shared<CConfigurationLandscapeSerializer>();
    std::shared_ptr<CConfigurationLandscape> landscapeConfiguration = std::static_pointer_cast<CConfigurationLandscape>(landscapeSerializer->serialize(filename));
    assert(landscapeConfiguration != nullptr);
    CConfigurationMaterialLoadingOperation::serializeGameObjectMaterialsConfigurations(landscapeConfiguration);
    
    std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(landscapeConfiguration->getSplattingDiffuseMaterialFilename()));
    assert(materialConfiguration != nullptr);
    
    landscapeConfiguration->setConfiguration(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                                          landscapeConfiguration->kLandscapeSplattingDiffuseMaterialFilenameAttribute),
                                             materialConfiguration);
    
    materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(landscapeConfiguration->getSplattingNormalMaterialFilename()));
    assert(materialConfiguration != nullptr);
    landscapeConfiguration->setConfiguration(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                                          landscapeConfiguration->kLandscapeSplattingNormalMaterialFilenameAttribute),
                                             materialConfiguration);
    
    std::vector<std::string> edgesMaterialsConfigurationsFilenames = landscapeConfiguration->getEdgesMaterialsFilenames();
    for(const auto& iterator : edgesMaterialsConfigurationsFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        landscapeConfiguration->setConfiguration(getConfigurationAttributeKey(landscapeConfiguration->kLandscapeMainNode,
                                                                              landscapeConfiguration->kLandscapeEdgesNode,
                                                                              landscapeConfiguration->kLandscapeEdgesMaterialsConfigurationsNode,
                                                                              landscapeConfiguration->kLandscapeEdgeMaterialConfigurationNode,
                                                                              landscapeConfiguration->kLandscapeEdgeMaterialConfigurationFilenameAttribute),
                                                 materialConfiguration);
    }
    
    return landscapeConfiguration;
}