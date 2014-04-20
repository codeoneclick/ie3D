//
//  CKOTHWorkflow.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CKOTHWorkflow_h
#define CKOTHWorkflow_h

#include "HCommon.h"
#include "IGameController.h"
#include "HDeclaration.h"

class CKOTHWorkflow : public IGameController
{
private:
    
protected:
    
public:
    
    CKOTHWorkflow(void);
    ~CKOTHWorkflow(void);
    
    std::shared_ptr<IGameTransition> CreateKOTHInGameTransition(const std::string& filename, ISharedOGLWindowRef window);
};

#endif 
