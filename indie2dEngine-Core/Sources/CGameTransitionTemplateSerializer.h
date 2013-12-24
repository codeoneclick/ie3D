//
//  CGameTransitionTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameTransitionTemplateSerializer_h
#define CGameTransitionTemplateSerializer_h

#include "ITemplateSerializer.h"

class CGameTransitionTemplateSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CGameTransitionTemplateSerializer(void);
    ~CGameTransitionTemplateSerializer(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
    void Deserialize(const std::string& _filename, std::shared_ptr<I_RO_TemplateCommon> _template);
};

#endif 

