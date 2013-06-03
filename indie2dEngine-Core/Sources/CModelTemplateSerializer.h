//
//  CModelTemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/28/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CModelTemplateSerializer_h
#define CModelTemplateSerializer_h

#include "ITemplateSerializer.h"

class CModelTemplateSerializer final : public ITemplateSerializer
{
private:
    
protected:
    
public:
    
    CModelTemplateSerializer(void);
    ~CModelTemplateSerializer(void);
    
    std::shared_ptr<ITemplate> Serialize(const std::string& _filename);
    std::shared_ptr<ITemplate> Serialize(const std::string& _host, ui32 _port, const std::string& _filename);
};

#endif 
