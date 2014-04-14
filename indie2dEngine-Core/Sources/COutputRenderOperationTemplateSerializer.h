//
//  CConfigurationORenderOperationSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationORenderOperationSerializer_h
#define CConfigurationORenderOperationSerializer_h

#include "ITemplateSerializer.h"

class CConfigurationORenderOperationSerializer  final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationORenderOperationSerializer(void);
    ~CConfigurationORenderOperationSerializer(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif

