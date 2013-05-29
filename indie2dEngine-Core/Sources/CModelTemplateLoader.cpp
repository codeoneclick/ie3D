//
//  CModelTemplateLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CModelTemplateLoader.h"
#include "CModelTemplateLoadingOperation.h"
#include "ITemplate.h"
#include "ITemplateLoadingHandler.h"

CModelTemplateLoader::CModelTemplateLoader(void)
{
    
}

CModelTemplateLoader::~CModelTemplateLoader(void)
{
    
}

void CModelTemplateLoader::_Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::shared_ptr<CModelTemplateLoadingOperation> operation = std::make_shared<CModelTemplateLoadingOperation>();
    std::shared_ptr<SModelTemplate> modelTemplate = std::static_pointer_cast<SModelTemplate>(operation->Serialize(_filename));
    assert(modelTemplate != nullptr);
    assert(_handler != nullptr);
    _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(modelTemplate);
}

void CModelTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::async(std::launch::async, &CModelTemplateLoader::_Load, this, _filename, _handler);
}
