//
//  COutpuRenderOperationTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationORenderOperationLoadingOperation_h
#define CConfigurationORenderOperationLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationORenderOperationLoadingOperation : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationORenderOperationLoadingOperation(void);
    ~CConfigurationORenderOperationLoadingOperation(void);
    
    ISharedConfiguration serialize(const std::string& filename);
    
};

#endif 
