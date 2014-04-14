//
//  CConfigurationOceanLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationOceanLoader.h"
#include "CConfigurationOceanLoadingOperation.h"
#include "CTemplateGameObjects.h"
#include "ITemplateLoadingHandler.h"

CConfigurationOceanLoader::CConfigurationOceanLoader(void)
{
    
}

CConfigurationOceanLoader::~CConfigurationOceanLoader(void)
{
    
}

void CConfigurationOceanLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
#if defined(__USE_GCDPP__)
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
#endif
        std::shared_ptr<CConfigurationOceanLoadingOperation> operation = std::make_shared<CConfigurationOceanLoadingOperation>();
        std::shared_ptr<CConfigurationOcean> oceanTemplate = std::static_pointer_cast<CConfigurationOcean>(operation->Serialize(_filename));
        assert(oceanTemplate != nullptr);
        assert(_handler != nullptr);
#if defined(__USE_GCDPP__)
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<CConfigurationOcean>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<CConfigurationOcean> _template)
        {
            _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, oceanTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
#else
		_handler->_Get_Commands()._ExecuteTemplateLoadedCommand(oceanTemplate);
#endif
}