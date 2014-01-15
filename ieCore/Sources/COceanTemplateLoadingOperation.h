//
//  COceanTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef COceanTemplateLoadingOperation_h
#define COceanTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class COceanTemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    COceanTemplateLoadingOperation(void);
    ~COceanTemplateLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};


#endif 
