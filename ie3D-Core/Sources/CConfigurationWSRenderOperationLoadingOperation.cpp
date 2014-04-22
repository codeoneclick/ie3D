//
//  CConfigurationWSRenderOperationLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationWSRenderOperationLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationWSRenderOperationSerializer.h"
#include "CConfigurationGameObjects.h"

CConfigurationWSRenderOperationLoadingOperation::CConfigurationWSRenderOperationLoadingOperation(void)
{
    
}

CConfigurationWSRenderOperationLoadingOperation::~CConfigurationWSRenderOperationLoadingOperation(void)
{
    
}

ISharedConfiguration CConfigurationWSRenderOperationLoadingOperation::serialize(const std::string& filename)
{
    std::shared_ptr<CConfigurationWSRenderOperationSerializer> worldSpaceRenderOperationConfigurationSerializer = std::make_shared<CConfigurationWSRenderOperationSerializer>();
    std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationConfiguration = std::static_pointer_cast<CConfigurationWSRenderOperation>(worldSpaceRenderOperationConfigurationSerializer->serialize(filename));
    assert(worldSpaceRenderOperationConfiguration != nullptr);
    return worldSpaceRenderOperationConfiguration;
}