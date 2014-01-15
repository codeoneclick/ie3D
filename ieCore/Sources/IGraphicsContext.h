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

#if defined(__NDK__)

struct ANativeWindow;

#endif

class IGraphicsContext : public std::enable_shared_from_this<IGraphicsContext>
{
private:
    
protected:
    
    ui32 m_frameBufferHandle;
    ui32 m_renderBufferHandle;
	ui32 m_depthBufferHandle;
    
    static std::vector<std::shared_ptr<IGraphicsContext> > m_contexts;
    
#if defined(__NDK__)
    
    static ANativeWindow* m_AWindow;
    
#endif
    
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
    };
    
    inline const i32 Get_DepthBufferHandle(void) const
    {
        return m_depthBufferHandle;
    };
    
#if defined(__NDK__)
    
    void static Set_AWindow(ANativeWindow* _AWindow)
    {
        assert(_AWindow != nullptr);
        m_AWindow = _AWindow;
    };
    
    static ANativeWindow* Get_AWindow(void)
    {
        assert(m_AWindow != nullptr);
        return m_AWindow;
    };
    
#endif
    
    virtual void Output(void) const = 0;
};

#endif
