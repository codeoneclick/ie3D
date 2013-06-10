//
//  CGameTransitionTemplateLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameTransitionTemplateLoader.h"
#include "CGameTransitionTemplateLoadingOperation.h"
#include "ITemplate.h"
#include "ITemplateLoadingHandler.h"

CGameTransitionTemplateLoader::CGameTransitionTemplateLoader(void)
{
    
}

CGameTransitionTemplateLoader::~CGameTransitionTemplateLoader(void)
{
    
}

void CGameTransitionTemplateLoader::_Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::shared_ptr<CGameTransitionTemplateLoadingOperation> gameTransitionTemplateLoadingOperation = std::make_shared<CGameTransitionTemplateLoadingOperation>();
    std::shared_ptr<SGameTransitionTemplate> gameTransitionTemplate = std::static_pointer_cast<SGameTransitionTemplate>(gameTransitionTemplateLoadingOperation->Serialize(_filename));
    assert(gameTransitionTemplate != nullptr);
    assert(_handler != nullptr);
    _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(gameTransitionTemplate);
}

void CGameTransitionTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::async(std::launch::async, &CGameTransitionTemplateLoader::_Load, this, _filename, _handler);
}
