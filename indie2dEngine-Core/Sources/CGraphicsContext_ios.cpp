//
//  CGraphicsContext_iOS.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"

#if defined(__IOS__)

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

class CGraphicsContext_iOS final : public IGraphicsContext
{
private:
    
protected:
    
    EAGLContext* m_context;
    
public:
    
    CGraphicsContext_iOS(const CAEAGLLayer* _hwnd);
    ~CGraphicsContext_iOS(void);
    
    void Output(void) const;
};

std::shared_ptr<IGraphicsContext> CreateGraphicsContext_iOS(const void* _hwnd)
{
    const UIView* hwnd = (__bridge UIView*)_hwnd;
    assert([hwnd.layer isKindOfClass:[CAEAGLLayer class]]);
    return std::make_shared<CGraphicsContext_iOS>(hwnd.layer);
};

CGraphicsContext_iOS::CGraphicsContext_iOS(const CAEAGLLayer* _hwnd)
{
    m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    assert(m_context != nullptr);
    
    ui8 result = [EAGLContext setCurrentContext:m_context];
    assert(result == true);
    
    glGenRenderbuffers(1, &m_renderBufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferHandle);
    [m_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_hwnd];
    
    glGenFramebuffers(1, &m_frameBufferHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferHandle);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_renderBufferHandle);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

CGraphicsContext_iOS::~CGraphicsContext_iOS(void)
{
    
}

void CGraphicsContext_iOS::Output(void) const
{
    assert(m_context != nullptr);
    [m_context presentRenderbuffer:GL_RENDERBUFFER];
}

#endif