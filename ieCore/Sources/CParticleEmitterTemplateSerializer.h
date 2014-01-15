//
//  CParticleEmitterTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CParticleEmitterTemplateSerializer_h
#define CParticleEmitterTemplateSerializer_h

#include "ITemplateSerializer.h"

class CParticleEmitterTemplateSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CParticleEmitterTemplateSerializer(void);
    ~CParticleEmitterTemplateSerializer(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif 
