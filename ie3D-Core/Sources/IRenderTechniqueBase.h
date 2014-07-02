//
//  IRenderTechniqueBase.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 7/2/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef IRenderTechniqueBase_h
#define IRenderTechniqueBase_h

#include "HCommon.h"
#include "HDeclaration.h"

class IRenderTechniqueBase
{
private:
    
protected:
    
    ui32 m_frameWidth;
    ui32 m_frameHeight;
    ui32 m_frameBuffer;
    glm::vec4 m_clearColor;
    std::string m_name;
    ui32 m_index;
    
public:
    
    IRenderTechniqueBase(ui32 frameWidth, ui32 frameHeight, const std::string& name, ui32 index);
    virtual ~IRenderTechniqueBase(void);
    
    ui32 getIndex(void) const;
    
    ui32 getFrameWidth(void) const;
    ui32 getFrameHeight(void) const;
    
    std::string getName(void) const;
    
    void setClearColor(const glm::vec4& clearColor);
    
    virtual void bind(void) = 0;
    virtual void unbind(void) = 0;
    virtual void draw(void) = 0;
};

#endif
