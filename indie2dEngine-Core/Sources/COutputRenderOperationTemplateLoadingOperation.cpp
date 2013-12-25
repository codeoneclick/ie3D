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
#include "CTemplateGameObjects.h"

COutputRenderOperationTemplateLoadingOperation::COutputRenderOperationTemplateLoadingOperation(void)
{
    
}

COutputRenderOperationTemplateLoadingOperation::~COutputRenderOperationTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> COutputRenderOperationTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<COutputRenderOperationTemplateSerializer> outputRenderOperationTemplateSerializer = std::make_shared<COutputRenderOperationTemplateSerializer>();
    std::shared_ptr<COutputRenderOperationTemplate> outputRenderOperationTemplate = std::static_pointer_cast<COutputRenderOperationTemplate>(outputRenderOperationTemplateSerializer->Serialize(_filename));
    assert(outputRenderOperationTemplate != nullptr);
    std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(outputRenderOperationTemplate->Get_MaterialTemplateFilename()));
    assert(materialTemplate != nullptr);
    outputRenderOperationTemplate->Set_Template(Get_TemplateAttributeKey(outputRenderOperationTemplate->kOutputRenderOperationMainNode,
                                                                         outputRenderOperationTemplate->kOutputRenderOperationMaterialTemplateFilenameAttribute),
                                                materialTemplate);
    return outputRenderOperationTemplate;
}