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

void CGameTransitionTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
        std::shared_ptr<CGameTransitionTemplateLoadingOperation> gameTransitionTemplateLoadingOperation = std::make_shared<CGameTransitionTemplateLoadingOperation>();
        std::shared_ptr<SGameTransitionTemplate> gameTransitionTemplate = std::static_pointer_cast<SGameTransitionTemplate>(gameTransitionTemplateLoadingOperation->Serialize(_filename));
        assert(gameTransitionTemplate != nullptr);
        assert(_handler != nullptr);
        
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<SGameTransitionTemplate>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<SGameTransitionTemplate> _template)
        {
             _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, gameTransitionTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
}
