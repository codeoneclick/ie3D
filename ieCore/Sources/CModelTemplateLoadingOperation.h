//
//  CModelTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CModelTemplateLoadingOperation_h
#define CModelTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CModelTemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CModelTemplateLoadingOperation(void);
    ~CModelTemplateLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif 
