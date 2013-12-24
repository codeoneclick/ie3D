//
//  CParticleEmitterTemplateLoader.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CParticleEmitterTemplateLoader.h"
#include "CParticleEmitterLoadingOperation.h"
#include "CTemplateGameObjects.h"
#include "ITemplateLoadingHandler.h"

CParticleEmitterTemplateLoader::CParticleEmitterTemplateLoader(void)
{
    
}

CParticleEmitterTemplateLoader::~CParticleEmitterTemplateLoader(void)
{
    
}

void CParticleEmitterTemplateLoader::Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
{
#if defined(__USE_GCDPP__)
    std::function<void(std::string, std::shared_ptr<ITemplateLoadingHandler>)> function = [](std::string _filename, std::shared_ptr<ITemplateLoadingHandler> _handler)
    {
#endif
        std::shared_ptr<CParticleEmitterLoadingOperation> operation = std::make_shared<CParticleEmitterLoadingOperation>();
        std::shared_ptr<CParticleEmitterTemplate> particleEmitterTemplate = std::static_pointer_cast<CParticleEmitterTemplate>(operation->Serialize(_filename));
        assert(particleEmitterTemplate != nullptr);
        assert(_handler != nullptr);
        
#if defined(__USE_GCDPP__)
        std::function<void(std::shared_ptr<ITemplateLoadingHandler>, std::shared_ptr<CParticleEmitterTemplate>)> function = []( std::shared_ptr<ITemplateLoadingHandler> _handler, std::shared_ptr<CParticleEmitterTemplate> _template)
        {
            _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(_template);
        };
        gcdpp::impl::DispatchAsync(gcdpp::queue::GetMainQueue(), function, _handler, particleEmitterTemplate);
    };
    gcdpp::impl::DispatchAsync(gcdpp::queue::GetGlobalQueue(gcdpp::queue::GCDPP_DISPATCH_QUEUE_PRIORITY_LOW), function, _filename, _handler);
#else
	 _handler->_Get_Commands()._ExecuteTemplateLoadedCommand(particleEmitterTemplate);
#endif
}
