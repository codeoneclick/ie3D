//
//  CRenderTarget.cpp
//  ie3D-Core
//
//  Created by sergey.sergeev on 8/8/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "CRenderTarget.h"
#include "IGraphicsContext.h"

#if defined(__OSX__)

#define __PBO__ 1

#endif

CRenderTarget::CRenderTarget(ISharedGraphicsContextRef graphicsContext, GLint format, ui32 width, ui32 height) :
m_graphicsContext(graphicsContext),
m_size(glm::ivec2(width, height)),
m_format(format)
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
    
#if defined(__PBO__)
    
    ui32 size = m_size.x * m_size.y;
    if(m_format == GL_RGBA)
    {
        size *= 4;
    }
    else if(m_format == GL_RGB)
    {
        size *= 3;
    }
        
    ieGenBuffers(1, &m_pixelBuffer);
    ieBindBuffer(GL_PIXEL_PACK_BUFFER, m_pixelBuffer);
    ieBufferData(GL_PIXEL_PACK_BUFFER, size, NULL, GL_STREAM_READ);
    ieBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    
#endif
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
    
#if defined(__PBO__)
    
    if(m_pixelBuffer != 0)
    {
        ieDeleteBuffers(1, &m_pixelBuffer);
    }
    
#endif
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

void CRenderTarget::end(ui8* data)
{
    if(data)
    {
#if defined(__PBO__)
        
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        ieBindBuffer(GL_PIXEL_PACK_BUFFER, m_pixelBuffer);
        glReadPixels(0, 0, m_size.x, m_size.y, m_format, GL_UNSIGNED_BYTE, NULL);
        
        ui8* pointer = static_cast<ui8*>(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
        if (pointer)
        {
            ui32 size = m_size.x * m_size.y;
            if(m_format == GL_RGBA)
            {
                size *= 4;
            }
            else if(m_format == GL_RGB)
            {
                size *= 3;
            }
            
            memcpy(data, pointer, size);
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        }
#else
        glReadPixels(0, 0, m_size.x, m_size.y, m_format, GL_UNSIGNED_BYTE, data);
        
#endif
    }
    assert(m_graphicsContext != nullptr);
    ieBindFramebuffer(GL_FRAMEBUFFER, m_graphicsContext->getFrameBuffer());
    ieViewport(0, 0, m_graphicsContext->getWidth(), m_graphicsContext->getHeight());
}
