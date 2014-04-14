//
//  CConfigurationParticleEmitterSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationParticleEmitterSerializer_h
#define CConfigurationParticleEmitterSerializer_h

#include "IConfigurationSerializer.h"

class CConfigurationParticleEmitterSerializer final : public IConfigurationSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationParticleEmitterSerializer(void);
    ~CConfigurationParticleEmitterSerializer(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    void deserialize(const std::string& filename,
                     ISharedConfigurationRef configuration);
};

#endif 
