//
//  COceanTeamplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationOceanLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationOceanSerializer.h"
#include "CTemplateGameObjects.h"

CConfigurationOceanLoadingOperation::CConfigurationOceanLoadingOperation(void)
{
    
}

CConfigurationOceanLoadingOperation::~CConfigurationOceanLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationOceanLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationOceanSerializer> oceanSerializer = std::make_shared<CConfigurationOceanSerializer>();
    std::shared_ptr<CConfigurationOcean> oceanTemplate = std::static_pointer_cast<CConfigurationOcean>(oceanSerializer->Serialize(_filename));
    assert(oceanTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = oceanTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        oceanTemplate->Set_Template(Get_TemplateAttributeKey(oceanTemplate->kGameObjectMaterialsTemplatesNode,
                                                             oceanTemplate->kGameObjectMaterialTemplateNode,
                                                             oceanTemplate->kGameObjectMaterialFilenameAttribute),
                                    materialTemplate);
    }
    return oceanTemplate;
}