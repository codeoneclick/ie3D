//
//  CConfigurationSSRenderOperationLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationSSRenderOperationLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationSSRenderOperationSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationSSRenderOperationLoadingOperation::CConfigurationSSRenderOperationLoadingOperation(void)
{
    
}

CConfigurationSSRenderOperationLoadingOperation::~CConfigurationSSRenderOperationLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationSSRenderOperationLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationSSRenderOperationSerializer> screenSpaceRenderOperationConfigurationSerializer = std::make_shared<CConfigurationSSRenderOperationSerializer>();
    std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationSSRenderOperation>(screenSpaceRenderOperationConfigurationSerializer->serialize(filename));
    assert(screenSpaceRenderOperationConfiguration != nullptr);
    std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(screenSpaceRenderOperationConfiguration->getMaterialConfigurationFilename()));
    assert(materialConfiguration != nullptr);
    screenSpaceRenderOperationConfiguration->setConfiguration(getConfigurationAttributeKey(screenSpaceRenderOperationConfiguration->kSSRenderOperationMainNode,
                                                                                           screenSpaceRenderOperationConfiguration->kSSRenderOperationMaterialConfigurationFilenameAttribute),
                                                              materialConfiguration);
    return screenSpaceRenderOperationConfiguration;
}