//
//  CConfigurationModelLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationModelLoadingOperation_h
#define CConfigurationModelLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CConfigurationModelLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationModelLoadingOperation(void);
    ~CConfigurationModelLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif 
