//
//  CGraphicsContext_ndk.c
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"
#include "IOGLWindow.h"

#if defined(__NDK__)

class CGraphicsContext_ndk : public IGraphicsContext
{
private:
    
protected:

    
public:
    
	CGraphicsContext_ndk(void);
    ~CGraphicsContext_ndk(void);
    
    void Output(void) const;
};

std::shared_ptr<IGraphicsContext> CreateGraphicsContext_ndk(const void* _hwnd)
{
	return std::make_shared<CGraphicsContext_ndk>();
};

CGraphicsContext_ndk::CGraphicsContext_ndk(void)
{
	i32 bindedFrameBufferHandle = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFrameBufferHandle);
	m_frameBufferHandle = bindedFrameBufferHandle;
    
	i32 bindedRenderBufferHandle = 0;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &bindedRenderBufferHandle);
	m_renderBufferHandle = bindedRenderBufferHandle;
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}

CGraphicsContext_ndk::~CGraphicsContext_ndk(void)
{
    
}

void CGraphicsContext_ndk::Output(void) const
{

}

#endif