//
//  CGameTransitionTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameTransitionTemplateLoadingOperation_h
#define CGameTransitionTemplateLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CGameTransitionTemplateLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CGameTransitionTemplateLoadingOperation(void);
    ~CGameTransitionTemplateLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif
