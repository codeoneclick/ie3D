//
//  CConfigurationLandscapeLoader.cpp
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationLandscapeLoader.h"
#include "CConfigurationLandscapeLoadingOperation.h"
#include "CTemplateGameObjects.h"
#include "ITemplateLoadingHandler.h"

CConfigurationLandscapeLoader::CConfigurationLandscapeLoader(void)
{
    
}

CConfigurationLandscapeLoader::~CConfigurationLandscapeLoader(void)
{
    
}

void CConfigurationLandscapeLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
#if defined(__USE_GCDPP__)
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
#endif
        std::shared_ptr<CConfigurationLandscapeLoadingOperation> operation = std::make_shared<CConfigurationLandscapeLoadingOperation>();
        std::shared_ptr<CConfigurationLandscape> landscapeTemplate = std::static_pointer_cast<CConfigurationLandscape>(operation->Serialize(_filename));
        assert(landscapeTemplate != nullptr);
        assert(_handler != nullptr);
#if defined(__USE_GCDPP__)
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<CConfigurationLandscape>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<CConfigurationLandscape> _template)
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
