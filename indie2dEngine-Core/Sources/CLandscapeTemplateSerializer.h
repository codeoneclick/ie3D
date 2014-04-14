//
//  CConfigurationLandscapeSerializer.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationLandscapeSerializer_h
#define CConfigurationLandscapeSerializer_h

#include "ITemplateSerializer.h"

class CConfigurationLandscapeSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationLandscapeSerializer(void);
    ~CConfigurationLandscapeSerializer(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif
