//
//  CLandscapeTemplateLoadingOperation.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CLandscapeTemplateLoadingOperation.h"
#include "CMaterialTemplateLoadingOperation.h"
#include "CLandscapeTemplateSerializer.h"
#include "CTemplateGameObjects.h"

CLandscapeTemplateLoadingOperation::CLandscapeTemplateLoadingOperation(void)
{
    
}

CLandscapeTemplateLoadingOperation::~CLandscapeTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<I_RO_TemplateCommon> CLandscapeTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CLandscapeTemplateSerializer> landscapeSerializer = std::make_shared<CLandscapeTemplateSerializer>();
    std::shared_ptr<CLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<CLandscapeTemplate>(landscapeSerializer->Serialize(_filename));
    assert(landscapeTemplate != nullptr);
    std::vector<std::string> materialsTemplatesFilenames = landscapeTemplate->Get_MaterialsTemplatesFilenames();
    for(const auto& iterator : materialsTemplatesFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        landscapeTemplate->Set_Template(Get_TemplateAttributeKey(landscapeTemplate->kGameObjectMaterialsTemplatesNode,
                                                                 landscapeTemplate->kGameObjectMaterialTemplateNode,
                                                                 landscapeTemplate->kGameObjectMaterialFilenameAttribute),
                                        materialTemplate);
    }

    std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(landscapeTemplate->Get_SplattingDiffuseMaterialFilename()));
    assert(materialTemplate != nullptr);
    
    landscapeTemplate->Set_Template(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                             landscapeTemplate->kLandscapeSplattingDiffuseMaterialFilenameAttribute),
                                    materialTemplate);
    
    materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(landscapeTemplate->Get_SplattingNormalMaterialFilename()));
    assert(materialTemplate != nullptr);
    landscapeTemplate->Set_Template(Get_TemplateAttributeKey(landscapeTemplate->kLandscapeMainNode,
                                                             landscapeTemplate->kLandscapeSplattingNormalMaterialFilenameAttribute),
                                    materialTemplate);
    
    std::vector<std::string> edgesMaterialsTemplatesFilenames = landscapeTemplate->Get_EdgesMaterialsFilenames();
    for(const auto& iterator : edgesMaterialsTemplatesFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<CMaterialTemplate> materialTemplate = std::static_pointer_cast<CMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
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