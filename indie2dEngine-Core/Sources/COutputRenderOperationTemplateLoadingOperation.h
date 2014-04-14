//
//  COutpuRenderOperationTemplateLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationORenderOperationLoadingOperation_h
#define CConfigurationORenderOperationLoadingOperation_h

#include "ITemplateLoadingOperation.h"

class CConfigurationORenderOperationLoadingOperation : public ITemplateLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationORenderOperationLoadingOperation(void);
    ~CConfigurationORenderOperationLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
    
};

#endif 
