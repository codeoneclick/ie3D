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
#include "ITemplate.h"

CModelTemplateLoadingOperation::CModelTemplateLoadingOperation(void)
{
    
}

CModelTemplateLoadingOperation::~CModelTemplateLoadingOperation(void)
{
    
}

std::shared_ptr<ITemplate> CModelTemplateLoadingOperation::Serialize(const std::string& _filename)
{
    std::shared_ptr<CModelTemplateSerializer> modelSerializer = std::make_shared<CModelTemplateSerializer>();
    std::shared_ptr<SModelTemplate> modelTemplate = std::static_pointer_cast<SModelTemplate>(modelSerializer->Serialize(_filename));
    assert(modelTemplate != nullptr);
    for(const auto& iterator : modelTemplate->m_materialsFilenames)
    {
        std::shared_ptr<CMaterialTemplateLoadingOperation> materialLoadingOperation = std::make_shared<CMaterialTemplateLoadingOperation>();
        std::shared_ptr<SMaterialTemplate> materialTemplate = std::static_pointer_cast<SMaterialTemplate>(materialLoadingOperation->Serialize(iterator));
        assert(materialTemplate != nullptr);
        modelTemplate->m_materialsTemplates.push_back(materialTemplate);
    }
    return modelTemplate;
}