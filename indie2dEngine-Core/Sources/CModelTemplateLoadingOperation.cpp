//
//  CConfigurationModelLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationModelLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationModelSerializer.h"
#include "CTemplateGameObjects.h"

CConfigurationModelLoadingOperation::CConfigurationModelLoadingOperation(void)
{
    
}

CConfigurationModelLoadingOperation::~CConfigurationModelLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationModelLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationModelSerializer> modelSerializer = std::make_shared<CConfigurationModelSerializer>();
    std::shared_ptr<CConfigurationModel> modelTemplate = std::static_pointer_cast<CConfigurationModel>(modelSerializer->Serialize(_filename));
    assert(modelTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = modelTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        modelTemplate->Set_Template(Get_TemplateAttributeKey(modelTemplate->kGameObjectMaterialsTemplatesNode,
                                                             modelTemplate->kGameObjectMaterialTemplateNode,
                                                             modelTemplate->kGameObjectMaterialFilenameAttribute),
                                    materialTemplate);
    }
    return modelTemplate;
}