//
//  CConfigurationWSRenderOperationLoadingOperation.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CConfigurationWSRenderOperationLoadingOperation_h
#define CConfigurationWSRenderOperationLoadingOperation_h

#include "IConfigurationLoadingOperation.h"

class CConfigurationWSRenderOperationLoadingOperation  : public IConfigurationLoadingOperation
{
private:
    
protected:
    
public:
    
    CConfigurationWSRenderOperationLoadingOperation(void);
    ~CConfigurationWSRenderOperationLoadingOperation(void);
    
    std::shared_ptr<I_RO_TemplateCommon> Serialize(const std::string& _filename);
};

#endif 
