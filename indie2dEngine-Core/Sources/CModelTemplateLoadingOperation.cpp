//
//  CModelTemplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModelTemplateLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "CModelTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CModelTemplateLoadingOperation::CModelTemplateLoadingOperation(void)
{
    
}

CModelTemplateLoadingOperation::~CModelTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CModelTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CModelTemplateSerializer> modelSerializer = std::make_shared<CModelTemplateSerializer>();
    std::shared_ptr<CModelTemplate> modelTemplate = std::static_pointer_cast<CModelTemplate>(modelSerializer->Serialize(_filename));
    assert(modelTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = modelTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        modelTemplate->Set_Template(Get_TemplateAttributeKey(modelTemplate->kGameObjectMaterialsTemplatesNode,
                                                             modelTemplate->kGameObjectMaterialTemplateNode,
                                                             modelTemplate->kGameObjectMaterialFilenameAttribute),
                                    materialTemplate);
    }
    return modelTemplate;
}