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
    i32 m_zOrder;
    
    typedef std::function<void(CSharedMaterialRef material)> T_DRAW_COMMAND;
    T_DRAW_COMMAND m_drawCommand;
    
protected:
    
public:
    
    CComponentRendering(ISharedConfigurationGameObjectRef configuration);
    ~CComponentRendering(void);
    
    CSharedMaterial getMaterial(const std::string& techniqueName) const;
    i32 getZOrder(void) const;
    
    void setDrawCommand(const T_DRAW_COMMAND& command);
    void draw(const std::string& techniqueName);
};

#endif
