//
//  CConfigurationAtmosphericScatteringLoadingOperation.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 9/24/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationAtmosphericScatteringLoadingOperation_h
#define CConfigurationAtmosphericScatteringLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationAtmosphericScatteringLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationAtmosphericScatteringLoadingOperation(void);
    ~CConfigurationAtmosphericScatteringLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
