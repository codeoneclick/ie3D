//
//  COutpuRenderOperationTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef COutputRenderOperationTemplateLoadingOperation_h
#define COutputRenderOperationTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class COutputRenderOperationTemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    COutputRenderOperationTemplateLoadingOperation(void);
    ~COutputRenderOperationTemplateLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    
};

#endif 
