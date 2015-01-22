//
//  CComponentRender.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 1/22/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

#ifndef CComponentRendering_h
#define CComponentRendering_h

#include "IComponent.h"
#include "HDeclaration.h"

class CComponentRendering : public IComponent
{
private:
    
    std::unordered_map<std::string, CSharedMaterial> m_materials;
    
protected:
    
public:
    
    CComponentRendering(void);
    ~CComponentRendering(void);
};

#endif
