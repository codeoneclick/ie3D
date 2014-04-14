//
//  IConfigurationLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IConfigurationLoadingOperation_h
#define IConfigurationLoadingOperation_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    IConfigurationLoadingOperation(void);
    virtual ~IConfigurationLoadingOperation(void);
    
    virtual ISharedConfiguration serialize(const std::string& filename) = 0;
};

#endif 
