//
//  CConfigurationParticleEmitterLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationParticleEmitterLoadingOperation_h
#define CConfigurationParticleEmitterLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationParticleEmitterLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationParticleEmitterLoadingOperation(void);
    ~CConfigurationParticleEmitterLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif 
