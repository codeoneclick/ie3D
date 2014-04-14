//
//  CConfigurationOceanSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationOceanSerializer_h
#define CConfigurationOceanSerializer_h

#include "ITemplateSerializer.h"

class CConfigurationOceanSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationOceanSerializer(void);
    ~CConfigurationOceanSerializer(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif
