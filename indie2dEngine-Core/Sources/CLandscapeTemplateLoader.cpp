//
//  CLandscapeTemplateLoader.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CLandscapeTemplateLoader.h"
#include "CLandscapeTemplateLoadingOperation.h"
#include "CTemplateGameObjects.h"
#include "ITemplateLoadingHandler.h"

CLandscapeTemplateLoader::CLandscapeTemplateLoader(void)
{
    
}

CLandscapeTemplateLoader::~CLandscapeTemplateLoader(void)
{
    
}

void CLandscapeTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
#if defined(__USE_GCDPP__)
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
#endif
        std::shared_ptr<CLandscapeTemplateLoadingOperation> operation = std::make_shared<CLandscapeTemplateLoadingOperation>();
        std::shared_ptr<CLandscapeTemplate> landscapeTemplate = std::static_pointer_cast<CLandscapeTemplate>(operation->Serialize(_filename));
        assert(landscapeTemplate != nullptr);
        assert(_handler != nullptr);
#if defined(__USE_GCDPP__)
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<CLandscapeTemplate>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<CLandscapeTemplate> _template)
        {
            _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, landscapeTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
#else
    _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(landscapeTemplate);
#endif
}
