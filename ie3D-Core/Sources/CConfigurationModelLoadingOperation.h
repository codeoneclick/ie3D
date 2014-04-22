//
//  CConfigurationModelLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationModelLoadingOperation_h
#define CConfigurationModelLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationModelLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationModelLoadingOperation(void);
    ~CConfigurationModelLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif 
