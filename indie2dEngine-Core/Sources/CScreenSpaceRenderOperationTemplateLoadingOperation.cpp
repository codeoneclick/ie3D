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
#include "CTemplateGameObjects.h"

CConfigurationSSRenderOperationLoadingOperation::CConfigurationSSRenderOperationLoadingOperation(void)
{
    
}

CConfigurationSSRenderOperationLoadingOperation::~CConfigurationSSRenderOperationLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationSSRenderOperationLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationSSRenderOperationSerializer> screenSpaceRenderOperationTemplateSerializer = std::make_shared<CConfigurationSSRenderOperationSerializer>();
    std::shared_ptr<CConfigurationSSRenderOperation> screenSpaceRenderOperationTemplate = std::static_pointer_cast<CConfigurationSSRenderOperation>(screenSpaceRenderOperationTemplateSerializer->Serialize(_filename));
    assert(screenSpaceRenderOperationTemplate != nullptr);
    std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(screenSpaceRenderOperationTemplate->Get_MaterialTemplateFilename()));
    assert(materialTemplate != nullptr);
    screenSpaceRenderOperationTemplate->Set_Template(getConfigurationAttributeKey(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMainNode,
                                                     screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMaterialTemplateFilenameAttribute),
                                                     materialTemplate);
    return screenSpaceRenderOperationTemplate;
}