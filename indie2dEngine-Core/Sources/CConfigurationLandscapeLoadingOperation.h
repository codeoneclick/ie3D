//
//  CConfigurationLandscapeLoadingOperation.h
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/25/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationLandscapeLoadingOperation_h
#define CConfigurationLandscapeLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationLandscapeLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationLandscapeLoadingOperation(void);
    ~CConfigurationLandscapeLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};


#endif
