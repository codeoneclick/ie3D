//
//  ITemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ITemplateLoadingOperation_h
#define ITemplateLoadingOperation_h

#include "HCommon.h"
#include "HEnums.h"

class ITemplate;
class ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    ITemplateLoadingOperation(void);
    virtual ~ITemplateLoadingOperation(void);
    
    virtual std::shared_ptr<ITemplate> Serialize(const std::string& _filename) = 0;
};

#endif 
