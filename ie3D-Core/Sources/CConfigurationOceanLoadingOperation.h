//
//  CConfigurationOceanLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationOceanLoadingOperation_h
#define CConfigurationOceanLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationOceanLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationOceanLoadingOperation(void);
    ~CConfigurationOceanLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif 
