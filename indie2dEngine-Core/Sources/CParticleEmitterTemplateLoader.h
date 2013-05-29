//
//  CParticleEmitterTemplateLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CParticleEmitterTemplateLoader_h
#define CParticleEmitterTemplateLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CParticleEmitterTemplateLoader final : public ITemplateLoader
{
private:
    
protected:
    
    void _Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
    
public:
    
    CParticleEmitterTemplateLoader(void);
    ~CParticleEmitterTemplateLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};

#endif 
