//
//  COutpuRenderOperationTemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationORenderOperationLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationORenderOperationSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationORenderOperationLoadingOperation::CConfigurationORenderOperationLoadingOperation(void)
{
    
}

CConfigurationORenderOperationLoadingOperation::~CConfigurationORenderOperationLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationORenderOperationLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationORenderOperationSerializer> outputRenderOperationTemplateSerializer = std::make_shared<CConfigurationORenderOperationSerializer>();
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationConfiguration = std::static_pointer_cast<CConfigurationORenderOperation>(outputRenderOperationTemplateSerializer->serialize(filename));
    assert(outputRenderOperationConfiguration != nullptr);
    std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    std::shared_ptr<CConfigurationMaterial> materialConfiguration = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->serialize(outputRenderOperationConfiguration->getMaterialConfigurationFilename()));
    assert(materialConfiguration != nullptr);
    outputRenderOperationConfiguration->setConfiguration(getConfigurationAttributeKey(outputRenderOperationConfiguration->kORenderOperationMainNode,
                                                                                      outputRenderOperationConfiguration->kORenderOperationMaterialConfigurationFilenameAttribute),
                                                         materialConfiguration);
    return outputRenderOperationConfiguration;
}