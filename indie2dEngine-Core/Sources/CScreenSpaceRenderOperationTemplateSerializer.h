//
//  CScreenSpaceOperationTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationSSRenderOperationSerializer_h
#define CConfigurationSSRenderOperationSerializer_h

#include "ITemplateSerializer.h"

class CConfigurationSSRenderOperationSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CConfigurationSSRenderOperationSerializer(void);
    ~CConfigurationSSRenderOperationSerializer(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif


