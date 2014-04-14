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
#include "CTemplateGameObjects.h"

CConfigurationORenderOperationLoadingOperation::CConfigurationORenderOperationLoadingOperation(void)
{
    
}

CConfigurationORenderOperationLoadingOperation::~CConfigurationORenderOperationLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationORenderOperationLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationORenderOperationSerializer> outputRenderOperationTemplateSerializer = std::make_shared<CConfigurationORenderOperationSerializer>();
    std::shared_ptr<CConfigurationORenderOperation> outputRenderOperationTemplate = std::static_pointer_cast<CConfigurationORenderOperation>(outputRenderOperationTemplateSerializer->Serialize(_filename));
    assert(outputRenderOperationTemplate != nullptr);
    std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(outputRenderOperationTemplate->Get_MaterialTemplateFilename()));
    assert(materialTemplate != nullptr);
    outputRenderOperationTemplate->Set_Template(getConfigurationAttributeKey(outputRenderOperationTemplate->kOutputRenderOperationMainNode,
                                                                         outputRenderOperationTemplate->kOutputRenderOperationMaterialTemplateFilenameAttribute),
                                                materialTemplate);
    return outputRenderOperationTemplate;
}