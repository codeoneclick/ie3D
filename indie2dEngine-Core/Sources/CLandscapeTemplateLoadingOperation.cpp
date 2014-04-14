//
//  CConfigurationLandscapeLoadingOperation.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationLandscapeLoadingOperation.h"
#include "CConfigurationMaterialLoadingOperation.h"
#include "CConfigurationLandscapeSerializer.h"
#include "CTemplateGameObjects.h"

CConfigurationLandscapeLoadingOperation::CConfigurationLandscapeLoadingOperation(void)
{
    
}

CConfigurationLandscapeLoadingOperation::~CConfigurationLandscapeLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CConfigurationLandscapeLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CConfigurationLandscapeSerializer> landscapeSerializer = std::make_shared<CConfigurationLandscapeSerializer>();
    std::shared_ptr<CConfigurationLandscape> landscapeTemplate = std::static_pointer_cast<CConfigurationLandscape>(landscapeSerializer->Serialize(_filename));
    assert(landscapeTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = landscapeTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        landscapeTemplate->Set_Template(Get_TemplateAttributeKey(landscapeTemplate->kGameObjectMaterialsTemplatesNode,
                                                                 landscapeTemplate->kGameObjectMaterialTemplateNode,
                                                                 landscapeTemplate->kGameObjectMaterialFilenameAttribute),
                                        materialTemplate);
    }

    std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(landscapeTemplate->Get_SplattingDiffuseMaterialFilename()));
    assert(materialTemplate != nullptr);
    
    landscapeTemplate->Set_Template(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                             landscapeTemplate->kLandscapeSplattingDiffuseMaterialFilenameAttribute),
                                    materialTemplate);
    
    materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
    materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(landscapeTemplate->Get_SplattingNormalMaterialFilename()));
    assert(materialTemplate != nullptr);
    landscapeTemplate->Set_Template(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                             landscapeTemplate->kLandscapeSplattingNormalMaterialFilenameAttribute),
                                    materialTemplate);
    
    std::vector<std::string> edgesMaterialsTemplatesFilenames = landscapeTemplate->Get_EdgesMaterialsFilenames();
    for(const auto& iterator : edgesMaterialsTemplatesFilenames)
    {
        std::shared_ptr<CConfigurationMaterialLoadingOperation> materialLoadingOperation = std::make_shared<CConfigurationMaterialLoadingOperation>();
        std::shared_ptr<CConfigurationMaterial> materialTemplate = std::static_pointer_cast<CConfigurationMaterial>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        landscapeTemplate->Set_Template(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                                 landscapeTemplate->kLandscapeEdgesNode,
                                                                 landscapeTemplate->kLandscapeEdgesMaterialsTemplatesNode,
                                                                 landscapeTemplate->kLandscapeEdgeMaterialTemplateNode,
                                                                 landscapeTemplate->kLandscapeEdgeMaterialTemplateFilenameAttribute),
                                       materialTemplate);
    }
    
    return landscapeTemplate;
}