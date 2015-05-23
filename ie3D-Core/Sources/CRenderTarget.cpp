//
//  CRenderTarget.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/8/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CRenderTarget.h"
#include "IGraphicsContext.h"

CRenderTarget::CRenderTarget(ISharedGraphicsContextRef graphicsContext, GLint format, ui32 width, ui32 height) :
m_graphicsContext(graphicsContext),
m_size(glm::ivec2(width, height))
{
    ieGenTextures(1, &m_colorAttachment);
    ieBindTexture(GL_TEXTURE_2D, m_colorAttachment);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ieTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    ieTexImage2D(GL_TEXTURE_2D, 0, format,
                 m_size.x,
                 m_size.y,
                 0, format, GL_UNSIGNED_BYTE, NULL);
    
    ieGenRenderbuffers(1, &m_depthAttachment);
    ieBindRenderbuffer(GL_RENDERBUFFER, m_depthAttachment);
    ieRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16,
                          m_size.x,
                          m_size.y);
    
    ieGenFramebuffers(1, &m_frameBuffer);
    ieBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    ieFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);
    ieFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttachment);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

CRenderTarget::~CRenderTarget(void)
{
    if(m_frameBuffer != 0)
    {
        ieDeleteFramebuffers(1, &m_frameBuffer);
    }
    if(m_colorAttachment != 0)
    {
        ieDeleteTextures(1, &m_colorAttachment);
    }
    if(m_depthAttachment != 0)
    {
        ieDeleteRenderbuffers(1, &m_depthAttachment);
    }
}

void CRenderTarget::clear(void)
{
    ieClearColor(0.0, 0.0, 0.0, 1.0);
    ieClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void CRenderTarget::begin(void)
{
    ieBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    ieViewport(0, 0, m_size.x, m_size.y);
}

void CRenderTarget::end(void)
{
    assert(m_graphicsContext != nullptr);
    ieBindFramebuffer(GL_FRAMEBUFFER, m_graphicsContext->getFrameBuffer());
    ieViewport(0, 0, m_graphicsContext->getWidth(), m_graphicsContext->getHeight());
}
