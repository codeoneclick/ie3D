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
#include "HDeclaration.h"

class IGraphicsContext : public std::enable_shared_from_this<IGraphicsContext>
{
private:
    
protected:
    
    ui32 m_frameBuffer;
    ui32 m_renderBuffer;
	ui32 m_depthBuffer;
    
    ISharedOGLWindow m_window;
    
#if defined(__NDK__)
    
    ANativeWindow* m_NDKwindow;
    
#endif
    
    IGraphicsContext(void) = default;
    
public:
    
    virtual ~IGraphicsContext(void) = default;
    
    static std::shared_ptr<IGraphicsContext> createGraphicsContext(ISharedOGLWindowRef window,
                                                                   E_PLATFORM_API api);
    
    ui32 getFrameBuffer(void) const;
    ui32 getRenderBuffer(void) const;
    ui32 getDepthBuffer(void) const;
    
    ui32 getWidth(void) const;
    ui32 getHeight(void) const;
    
    virtual void makeCurrent(void) const = 0;
    virtual void draw(void) const = 0;
};

#endif
