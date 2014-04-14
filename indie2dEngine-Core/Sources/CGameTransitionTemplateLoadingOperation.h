//
//  CConfigurationGameTransitionLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationGameTransitionLoadingOperation_h
#define CConfigurationGameTransitionLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CConfigurationGameTransitionLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationGameTransitionLoadingOperation(void);
    ~CConfigurationGameTransitionLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif
