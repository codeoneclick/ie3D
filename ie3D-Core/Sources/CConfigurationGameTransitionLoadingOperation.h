//
//  CConfigurationGameTransitionLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationGameTransitionLoadingOperation_h
#define CConfigurationGameTransitionLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationGameTransitionLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationGameTransitionLoadingOperation(void);
    ~CConfigurationGameTransitionLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif
