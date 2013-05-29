//
//  ITemplateSerializer.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ITemplateSerializer_h
#define ITemplateSerializer_h

#include "HCommon.h"

class ITemplate;
class ITemplateSerializer
{
private:
    
protected:
    
public:
    
    ITemplateSerializer(void);
    virtual ~ITemplateSerializer(void);
    
    virtual std::shared_ptr<ITemplate> Serialize(const std::string& _filename) = 0;
};

#endif
