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
#include "CTemplateGameObjects.h"

CConfigurationWSRenderOperationLoadingOperation::CConfigurationWSRenderOperationLoadingOperation(void)
{
    
}

CConfigurationWSRenderOperationLoadingOperation::~CConfigurationWSRenderOperationLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationWSRenderOperationLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationWSRenderOperationSerializer> worldSpaceRenderOperationTemplateSerializer = std::make_shared<CConfigurationWSRenderOperationSerializer>();
    std::shared_ptr<CConfigurationWSRenderOperation> worldSpaceRenderOperationTemplate = std::static_pointer_cast<CConfigurationWSRenderOperation>(worldSpaceRenderOperationTemplateSerializer->Serialize(_filename));
    assert(worldSpaceRenderOperationTemplate != nullptr);
    return worldSpaceRenderOperationTemplate;
}