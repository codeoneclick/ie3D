//
//  CGraphicsContext_iOS.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGraphicsContext_iOS.h"

CGraphicsContext_iOS::CGraphicsContext_iOS(const CAEAGLLayer* _iOSGLLayer)
{
    m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    assert(m_context != nullptr);
    
    ui8 result = [EAGLContext setCurrentContext:m_context];
    assert(result == true);
    
    glGenRenderbuffers(1, &m_renderBufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferHandle);
    [m_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_iOSGLLayer];
    
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