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
#include "ITemplate.h"

CLandscapeTemplateLoadingOperation::CLandscapeTemplateLoadingOperation(void)
{
    
}

CLandscapeTemplateLoadingOperation::~CLandscapeTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> CLandscapeTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CLandscapeTemplateSerializer> landscapeSerializer = std::make_shared<CLandscapeTemplateSerializer>();
    std::shared_ptr<SLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<SLandscapeTemplate>(landscapeSerializer->Serialize(_filename));
    assert(landscapeTemplate != nullptr);
    for(const auto& iterator : landscapeTemplate->m_materialsFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        landscapeTemplate->m_materialsTemplates.push_back(materialTemplate);
    }
    
    std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(landscapeTemplate->m_splattingDiffuseMaterialFilename));
    assert(materialTemplate != nullptr);
    landscapeTemplate->m_splattingDiffuseMaterial = materialTemplate;
    
    materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
    materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(landscapeTemplate->m_splattingNormalMaterialFilename));
    assert(materialTemplate != nullptr);
    landscapeTemplate->m_splattingNormalMaterial = materialTemplate;
    
    for(const auto& iterator : landscapeTemplate->m_edgesMaterialsFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        landscapeTemplate->m_edgesMaterials.push_back(materialTemplate);
    }
    
    return landscapeTemplate;
}