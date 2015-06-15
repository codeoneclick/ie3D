//
//  CGraphicsContext_iOS.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"
#include "IOGLWindow.h"

#if defined(__IOS__)

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

class CGraphicsContext_ios : public IGraphicsContext
{
private:
    
protected:
    
    EAGLContext* m_context;
    EAGLContext* m_backgroundContext;
    
public:
    
    CGraphicsContext_ios(ISharedOGLWindowRef window);
    ~CGraphicsContext_ios(void);
    
    void* getAPIContext(void) const;
    
    void makeCurrent(void) const;
    void draw(void) const;
    
    void beginBackgroundContext(void);
    void endBackgroundContext(void);
};

std::shared_ptr<IGraphicsContext> createGraphicsContext_ios(ISharedOGLWindowRef window)
{
    return std::make_shared<CGraphicsContext_ios>(window);
};

CGraphicsContext_ios::CGraphicsContext_ios(ISharedOGLWindowRef window)
{
    m_window = window;
    
    const UIView* hwnd = (__bridge UIView*)m_window->getHWND();
    assert([hwnd.layer isKindOfClass:[CAEAGLLayer class]]);
    
#if defined(__OPENGL_30__)
    m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
#else
    m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
#endif
    assert(m_context != nullptr);
    
    ui8 result = [EAGLContext setCurrentContext:m_context];
    assert(result == true);
    
    ieGenRenderbuffers(1, &m_renderBuffer);
    ieBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    [m_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:static_cast<CAEAGLLayer*>(hwnd.layer)];
    
    ieGenFramebuffers(1, &m_frameBuffer);
    ieBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    ieFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderBuffer);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

CGraphicsContext_ios::~CGraphicsContext_ios(void)
{
    
}

void* CGraphicsContext_ios::getAPIContext(void) const
{
    return (__bridge void *)m_context;
}

void CGraphicsContext_ios::makeCurrent(void) const
{
    ui8 result = [EAGLContext setCurrentContext:m_context];
    assert(result == true);
}

void CGraphicsContext_ios::draw(void) const
{
    assert(m_context != nullptr);
    [m_context presentRenderbuffer:GL_RENDERBUFFER];
}

void CGraphicsContext_ios::beginBackgroundContext(void)
{
#if defined(__OPENGL_30__)
    
    m_backgroundContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3 sharegroup:m_context.sharegroup];
#else
    
    m_backgroundContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:m_context.sharegroup];
    
#endif
    
    assert(m_backgroundContext != nullptr);
    
    ui8 result = [EAGLContext setCurrentContext:m_backgroundContext];
    assert(result == true);
}

void CGraphicsContext_ios::endBackgroundContext(void)
{
    ui8 result = [EAGLContext setCurrentContext:m_context];
    assert(result == true);

}

#endif