//
//  CMEConfigurationLandscapeBrushLoadingOperation.cpp
//  ie3D-Demo
//
//  Created by Sergey Sergeev on 6/7/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CMEConfigurationLandscapeBrushLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CMEConfigurationLandscapeBrushSerializer.h"
#include "CMEConfigurationGameObjects.h"

CMEConfigurationLandscapeBrushLoadingOperation::CMEConfigurationLandscapeBrushLoadingOperation(void)
{
    
}

CMEConfigurationLandscapeBrushLoadingOperation::~CMEConfigurationLandscapeBrushLoadingOperation(void)
{
    
}

ISharedConfiguration CMEConfigurationLandscapeBrushLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CMEConfigurationLandscapeBrushSerializer> landscapeBrushSerializer = std::make_shared<CMEConfigurationLandscapeBrushSerializer>();
    std::shared_ptr<CMEConfigurationLandscapeBrush> landscapeBrushConfiguration = std::static_pointer_cast<CMEConfigurationLandscapeBrush>(landscapeBrushSerializer->serialize(filename));
    assert(landscapeBrushConfiguration != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = landscapeBrushConfiguration->getMaterialsConfigurationsFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(iterator));
        assert(materialConfiguration != nullptr);
        landscapeBrushConfiguration->setConfiguration(getConfigurationAttributeKey(landscapeBrushConfiguration->kGameObjectMaterialsConfigurationsNode,
                                                                                   landscapeBrushConfiguration->kGameObjectMaterialConfigurationNode,
                                                                                   landscapeBrushConfiguration->kGameObjectMaterialFilenameAttribute),
                                                      materialConfiguration);
    }
    return landscapeBrushConfiguration;
}