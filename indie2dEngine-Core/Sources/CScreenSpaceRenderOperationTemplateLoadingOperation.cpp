//
//  CScreenSpaceRenderOperationTemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CScreenSpaceRenderOperationTemplateLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "CScreenSpaceRenderOperationTemplateSerializer.h"
#include "ITemplate.h"

CScreenSpaceRenderOperationTemplateLoadingOperation::CScreenSpaceRenderOperationTemplateLoadingOperation(void)
{
    
}

CScreenSpaceRenderOperationTemplateLoadingOperation::~CScreenSpaceRenderOperationTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> CScreenSpaceRenderOperationTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CScreenSpaceRenderOperationTemplateSerializer> screenSpaceRenderOperationTemplateSerializer = std::make_shared<CScreenSpaceRenderOperationTemplateSerializer>();
    std::shared_ptr<SScreenSpaceRenderOperationTemplate> screenSpaceRenderOperationTemplate = std::static_pointer_cast<SScreenSpaceRenderOperationTemplate>(screenSpaceRenderOperationTemplateSerializer->Serialize(_filename));
    assert(screenSpaceRenderOperationTemplate != nullptr);
    std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(screenSpaceRenderOperationTemplate->m_materialTemplateFilename));
    assert(materialTemplate != nullptr);
    screenSpaceRenderOperationTemplate->m_materialTemplate = materialTemplate;
    return screenSpaceRenderOperationTemplate;
}