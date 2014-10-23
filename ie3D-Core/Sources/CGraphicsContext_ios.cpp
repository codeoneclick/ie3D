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
    
public:
    
    CGraphicsContext_ios(ISharedOGLWindowRef window);
    ~CGraphicsContext_ios(void);
    
    void draw(void) const;
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
    
    glGenRenderbuffers(1, &m_renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    [m_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:static_cast<CAEAGLLayer*>(hwnd.layer)];
    
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderBuffer);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

CGraphicsContext_ios::~CGraphicsContext_ios(void)
{
    
}

void CGraphicsContext_ios::draw(void) const
{
    assert(m_context != nullptr);
    [m_context presentRenderbuffer:GL_RENDERBUFFER];
}

#endif