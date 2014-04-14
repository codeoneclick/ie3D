//
//  CConfigurationParticleEmitterLoader.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationParticleEmitterLoader_h
#define CConfigurationParticleEmitterLoader_h

#include "ITemplateLoader.h"

class ITemplateLoadingHandler;
class CConfigurationParticleEmitterLoader final : public ITemplateLoader
{
private:
    
protected:
    
public:
    
    CConfigurationParticleEmitterLoader(void);
    ~CConfigurationParticleEmitterLoader(void);
    
    void Load(const std::string& _filename, std::shared_ptr<ITemplateLoadingHandler> _handler);
};

#endif 
