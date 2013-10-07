//
//  IGraphicsContext.h
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGraphicsContext_h
#define IGraphicsContext_h

#include "HCommon.h"
#include "HEnums.h"

class IGraphicsContext
{
private:
    
protected:
    
    ui32 m_frameBufferHandle;
    ui32 m_renderBufferHandle;
	ui32 m_depthBufferHandle;
    
public:
    
    IGraphicsContext(void);
    virtual ~IGraphicsContext(void);
    
    static std::shared_ptr<IGraphicsContext> CreateGraphicsContext(const void* _hwnd, E_PLATFORM_API _api);
    
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
