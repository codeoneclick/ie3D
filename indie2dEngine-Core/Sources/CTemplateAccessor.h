//
//  CTemplateAccessor.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CTemplateAccessor_h
#define CTemplateAccessor_h

#include "HCommon.h"
#include "HEnums.h"

class ITemplateLoader;
class ITemplateLoadingHandler;
class CTemplateAccessor
{
private:
    
protected:
    
    std::shared_ptr<ITemplateLoader> m_loaders[E_TEMPLATE_LOADERS_MAX];

public:
    
    CTemplateAccessor(void);
    ~CTemplateAccessor(void);
    
    void LoadModelTemplate(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
    void LoadOceanTemplate(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
    void LoadParticleEmitterTemplate(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
    void LoadGameTransitionTemplate(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};

#endif 
