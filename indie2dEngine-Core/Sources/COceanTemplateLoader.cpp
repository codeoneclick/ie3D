//
//  COceanTemplateLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "COceanTemplateLoader.h"
#include "COceanTemplateLoadingOperation.h"
#include "ITemplate.h"
#include "ITemplateLoadingHandler.h"

COceanTemplateLoader::COceanTemplateLoader(void)
{
    
}

COceanTemplateLoader::~COceanTemplateLoader(void)
{
    
}

void COceanTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
        std::shared_ptr<COceanTemplateLoadingOperation> operation = std::make_shared<COceanTemplateLoadingOperation>();
        std::shared_ptr<SOceanTemplate> oceanTemplate = std::static_pointer_cast<SOceanTemplate>(operation->Serialize(_filename));
        assert(oceanTemplate != nullptr);
        assert(_handler != nullptr);
        
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<SOceanTemplate>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<SOceanTemplate> _template)
        {
            _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, oceanTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
}