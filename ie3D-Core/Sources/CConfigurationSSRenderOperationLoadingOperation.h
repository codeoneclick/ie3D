//
//  CConfigurationSSRenderOperationLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationSSRenderOperationLoadingOperation_h
#define CConfigurationSSRenderOperationLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationSSRenderOperationLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationSSRenderOperationLoadingOperation(void);
    ~CConfigurationSSRenderOperationLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
};

#endif 
