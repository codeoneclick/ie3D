//
//  CWorldSpaceRenderOperationTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CWorldSpaceRenderOperationTemplateLoadingOperation_h
#define CWorldSpaceRenderOperationTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CWorldSpaceRenderOperationTemplateLoadingOperation  : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CWorldSpaceRenderOperationTemplateLoadingOperation(void);
    ~CWorldSpaceRenderOperationTemplateLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif 
