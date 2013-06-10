//
//  COutpuRenderOperationTemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COutputRenderOperationTemplateLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "COutputRenderOperationTemplateSerializer.h"
#include "ITemplate.h"

COutputRenderOperationTemplateLoadingOperation::COutputRenderOperationTemplateLoadingOperation(void)
{
    
}

COutputRenderOperationTemplateLoadingOperation::~COutputRenderOperationTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> COutputRenderOperationTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<COutputRenderOperationTemplateSerializer> outputRenderOperationTemplateSerializer = std::make_shared<COutputRenderOperationTemplateSerializer>();
    std::shared_ptr<SOutputRenderOperationTemplate> outputRenderOperationTemplate = std::static_pointer_cast<SOutputRenderOperationTemplate>(outputRenderOperationTemplateSerializer->Serialize(_filename));
    assert(outputRenderOperationTemplate != nullptr);
    std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(outputRenderOperationTemplate->m_materialTemplateFilename));
    assert(materialTemplate != nullptr);
    outputRenderOperationTemplate->m_materialTemplate = materialTemplate;
    return outputRenderOperationTemplate;
}