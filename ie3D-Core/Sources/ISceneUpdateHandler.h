//
//  ISceneUpdateHandler.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ISceneUpdateHandler_h
#define ISceneUpdateHandler_h

#include "HCommon.h"
#include "HDeclaration.h"

class ISceneUpdateHandler
{
private:
    
protected:
    
    ISceneUpdateHandler(void);
    
public:
    
    virtual ~ISceneUpdateHandler(void);
    
    virtual void onSceneUpdate(f32 deltatime) = 0;
};

#endif 
