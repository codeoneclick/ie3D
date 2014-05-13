//
//  CDemoGameController.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 6/12/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CDemoGameController_h
#define CDemoGameController_h

#include "HCommon.h"
#include "IGameController.h"
#include "HDeclaration.h"

class CDemoGameController : public IGameController
{
private:
    
protected:
    
public:
    
    CDemoGameController(void);
    ~CDemoGameController(void);
    
    std::shared_ptr<IGameTransition> CreateKOTHInGameTransition(const std::string& filename, ISharedOGLWindowRef window);
};

#endif 
