//
//  IComponent.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef IComponent_h
#define IComponent_h

#include "HCommon.h"
#include "HEnums.h"
#include "HDeclaration.h"

class IComponent
{
private:
    
protected:
    
public:
    IComponent(void) = default;
    virtual ~IComponent(void) = default;
    
    virtual E_COMPONENT_CLASS getClass(void) const;
};

#endif
