//
//  CMaterialTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CMaterialTemplateLoadingOperation_h
#define CMaterialTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CMaterialTemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CMaterialTemplateLoadingOperation(void);
    ~CMaterialTemplateLoadingOperation(void);
    
    std::shared_ptr<ITemplate> Serialize(const std::string& _filename);
};

#endif 
