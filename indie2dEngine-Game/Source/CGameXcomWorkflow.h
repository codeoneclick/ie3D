//
//  CGameXcomWorkflow.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameXcomWorkflow_h
#define CGameXcomWorkflow_h

#include "HCommon.h"
#include "IGameWorkflow.h"

class CGameXcomWorkflow : public IGameWorkflow
{
private:
    
protected:
    
public:
    
    CGameXcomWorkflow(void);
    ~CGameXcomWorkflow(void);
    
    std::shared_ptr<IGameTransition> CreateXcomInGameTransition(const std::string& _filename, void* _hwnd);
};

#endif 
