//
//  CGUITemplateLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGUITemplateLoader.h"
#include "CGUITemplateLoadingOperation.h"
#include "ITemplateLoadingHandler.h"

CGUITemplateLoader::CGUITemplateLoader(void)
{
    
}

CGUITemplateLoader::~CGUITemplateLoader(void)
{
    
}

void CGUITemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
	assert(false);
}

void CGUITemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler, E_GUI_LOADING_SCHEME _scheme)
{
/*#if defined(__USE_GCDPP__)
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [_scheme](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
#endif
        std::shared_ptr<CGUITemplateLoadingOperation> operation = std::make_shared<CGUITemplateLoadingOperation>(_scheme);
        std::shared_ptr<SModelTemplate> guiTemplate = std::static_pointer_cast<SModelTemplate>(operation->Serialize(_filename));
        assert(guiTemplate != nullptr);
        assert(_handler != nullptr);
#if defined(__USE_GCDPP__)
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<SModelTemplate>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<SModelTemplate> _template)
        {
            _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, guiTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
#else
	_handler->_Get_Commands()._ExecuteTemplateLoadedCommand(guiTemplate);
#endif
*/
}
