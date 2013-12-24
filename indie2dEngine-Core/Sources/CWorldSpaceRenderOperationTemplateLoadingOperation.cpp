//
//  CWorldSpaceRenderOperationTemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CWorldSpaceRenderOperationTemplateLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "CWorldSpaceRenderOperationTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CWorldSpaceRenderOperationTemplateLoadingOperation::CWorldSpaceRenderOperationTemplateLoadingOperation(void)
{
    
}

CWorldSpaceRenderOperationTemplateLoadingOperation::~CWorldSpaceRenderOperationTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CWorldSpaceRenderOperationTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CWorldSpaceRenderOperationTemplateSerializer> worldSpaceRenderOperationTemplateSerializer = std::make_shared<CWorldSpaceRenderOperationTemplateSerializer>();
    std::shared_ptr<CWorldSpaceRenderOperationTemplate> worldSpaceRenderOperationTemplate = std::static_pointer_cast<CWorldSpaceRenderOperationTemplate>(worldSpaceRenderOperationTemplateSerializer->Serialize(_filename));
    assert(worldSpaceRenderOperationTemplate != nullptr);
    return worldSpaceRenderOperationTemplate;
}