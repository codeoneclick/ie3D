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
#include "CTemplateGameObjects.h"

CScreenSpaceRenderOperationTemplateLoadingOperation::CScreenSpaceRenderOperationTemplateLoadingOperation(void)
{
    
}

CScreenSpaceRenderOperationTemplateLoadingOperation::~CScreenSpaceRenderOperationTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CScreenSpaceRenderOperationTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CScreenSpaceRenderOperationTemplateSerializer> screenSpaceRenderOperationTemplateSerializer = std::make_shared<CScreenSpaceRenderOperationTemplateSerializer>();
    std::shared_ptr<CScreenSpaceRenderOperationTemplate> screenSpaceRenderOperationTemplate = std::static_pointer_cast<CScreenSpaceRenderOperationTemplate>(screenSpaceRenderOperationTemplateSerializer->Serialize(_filename));
    assert(screenSpaceRenderOperationTemplate != nullptr);
    std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(screenSpaceRenderOperationTemplate->Get_MaterialTemplateFilename()));
    assert(materialTemplate != nullptr);
    screenSpaceRenderOperationTemplate->Set_Template(Get_TemplateAttributeKey(screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMainNode,
                                                     screenSpaceRenderOperationTemplate->kScreenSpaceRenderOperationMaterialTemplateFilenameAttribute),
                                                     materialTemplate);
    return screenSpaceRenderOperationTemplate;
}