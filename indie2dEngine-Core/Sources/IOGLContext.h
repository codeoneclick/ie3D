//
//  IOGLContext.h
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IOGLContext_h
#define IOGLContext_h

#include "HCommon.h"

class IOGLContext
{
private:
    
protected:
    
    ui32 m_frameBufferHandle;
    ui32 m_renderBufferHandle;
	ui32 m_depthBufferHandle;
    
public:
    
    IOGLContext(void);
    virtual ~IOGLContext(void);
    
    inline const ui32 Get_FrameBufferHandle(void) const
    {
        return m_frameBufferHandle;
    };
    
    inline const ui32 Get_RenderBufferHandle(void) const
    {
        return m_renderBufferHandle;
    }
    
    inline const i32 Get_DepthBufferHandle(void) const
    {
        return m_depthBufferHandle;
    };
    
    virtual void Output(void) const = 0;
};

#endif
