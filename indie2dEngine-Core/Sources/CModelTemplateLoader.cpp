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

void CModelTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
        std::shared_ptr<CModelTemplateLoadingOperation> operation = std::make_shared<CModelTemplateLoadingOperation>();
        std::shared_ptr<SModelTemplate> modelTemplate = std::static_pointer_cast<SModelTemplate>(operation->Serialize(_filename));
        assert(modelTemplate != nullptr);
        assert(_handler != nullptr);
        
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<SModelTemplate>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<SModelTemplate> _template)
        {
            _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, modelTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
}
