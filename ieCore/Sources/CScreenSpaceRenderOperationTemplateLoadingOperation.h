//
//  CScreenSpaceRenderOperationTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CScreenSpaceRenderOperationTemplateLoadingOperation_h
#define CScreenSpaceRenderOperationTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CScreenSpaceRenderOperationTemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CScreenSpaceRenderOperationTemplateLoadingOperation(void);
    ~CScreenSpaceRenderOperationTemplateLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif 
