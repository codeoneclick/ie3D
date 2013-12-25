//
//  COceanTeamplateLoadingOperation.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COceanTemplateLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "COceanTemplateSerializer.h"
#include "CTemplateGameObjects.h"

COceanTemplateLoadingOperation::COceanTemplateLoadingOperation(void)
{
    
}

COceanTemplateLoadingOperation::~COceanTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> COceanTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<COceanTemplateSerializer> oceanSerializer = std::make_shared<COceanTemplateSerializer>();
    std::shared_ptr<COceanTemplate> oceanTemplate = std::static_pointer_cast<COceanTemplate>(oceanSerializer->Serialize(_filename));
    assert(oceanTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = oceanTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        oceanTemplate->Set_Template(Get_TemplateAttributeKey(oceanTemplate->kGameObjectMaterialsTemplatesNode,
                                                             oceanTemplate->kGameObjectMaterialTemplateNode,
                                                             oceanTemplate->kGameObjectMaterialFilenameAttribute),
                                    materialTemplate);
    }
    return oceanTemplate;
}