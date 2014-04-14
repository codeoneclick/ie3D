//
//  CConfigurationGameTransitionLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CConfigurationGameTransitionLoader.h"
#include "CConfigurationGameTransitionLoadingOperation.h"
#include "CTemplateGameObjects.h"
#include "ITemplateLoadingHandler.h"

CConfigurationGameTransitionLoader::CConfigurationGameTransitionLoader(void)
{
    
}

CConfigurationGameTransitionLoader::~CConfigurationGameTransitionLoader(void)
{
    
}

void CConfigurationGameTransitionLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
#if defined(__USE_GCDPP__)
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
#endif
        std::shared_ptr<CConfigurationGameTransitionLoadingOperation> gameTransitionTemplateLoadingOperation = std::make_shared<CConfigurationGameTransitionLoadingOperation>();
        std::shared_ptr<CConfigurationGameTransition> gameTransitionTemplate = std::static_pointer_cast<CConfigurationGameTransition>(gameTransitionTemplateLoadingOperation->Serialize(_filename));
        assert(gameTransitionTemplate != nullptr);
        assert(_handler != nullptr);
#if defined(__USE_GCDPP__)
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<CConfigurationGameTransition>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<CConfigurationGameTransition> _template)
        {
             _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, gameTransitionTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
#else
	 _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(gameTransitionTemplate);
#endif
}
