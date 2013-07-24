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
#include "ITemplate.h"

COceanTemplateLoadingOperation::COceanTemplateLoadingOperation(void)
{
    
}

COceanTemplateLoadingOperation::~COceanTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> COceanTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<COceanTemplateSerializer> oceanSerializer = std::make_shared<COceanTemplateSerializer>();
    std::shared_ptr<SOceanTemplate> oceanTemplate = std::static_pointer_cast<SOceanTemplate>(oceanSerializer->Serialize(_filename));
    assert(oceanTemplate != nullptr);
    for(auto iterator : oceanTemplate->m_materialsFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        oceanTemplate->m_materialsTemplates.push_back(materialTemplate);
    }
    return oceanTemplate;
}