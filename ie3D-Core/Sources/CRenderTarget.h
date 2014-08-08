//
//  CRenderTarget.h
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/8/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CRenderTarget_h
#define CRenderTarget_h

#include "HCommon.h"
#include "HDeclaration.h"

class CRenderTarget
{
private:
    
protected:
    
    ui32 m_frameBuffer;
    ui32 m_colorAttachment;
    ui32 m_depthAttachment;
    
    glm::ivec2 m_size;
    
    ISharedGraphicsContext m_graphicsContext;
    
public:
    
    CRenderTarget(ISharedGraphicsContextRef graphicsContext, ui32 width, ui32 height);
    ~CRenderTarget(void);
    
    void clear(void);
    void begin(void);
    void end(void);
};

#endif
