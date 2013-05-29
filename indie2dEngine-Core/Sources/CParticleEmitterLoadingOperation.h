//
//  CParticleEmitterLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CParticleEmitterLoadingOperation_h
#define CParticleEmitterLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CParticleEmitterLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CParticleEmitterLoadingOperation(void);
    ~CParticleEmitterLoadingOperation(void);
    
    std::shared_ptr<ITemplate> Serialize(const std::string& _filename);
};

#endif 
