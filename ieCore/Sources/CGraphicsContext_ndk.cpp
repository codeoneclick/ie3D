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

#define k_MAX_RETRIES 128

class CGraphicsContext_ndk : public IGraphicsContext
{
private:
    
protected:

    EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
    
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
    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
	EGLint majorVersion, minorVersion, format;
    ui32 retries = 0;
	while(!eglInitialize(m_display, &majorVersion, &minorVersion) && retries < k_MAX_RETRIES)
	{
		NDK_LOG("eglInitialize() failed. Retries : %i", retries);
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		retries++;
	}
    
    if(retries == k_MAX_RETRIES)
    {
        assert(false);
        return;
    }
    
	eglBindAPI(EGL_OPENGL_ES_API);
    
	EGLint attributes[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE, 5,
        EGL_DEPTH_SIZE, 16,
        EGL_STENCIL_SIZE, 0,
        EGL_NONE
    };
    
    EGLConfig config;
	i32 configs;
	if (!eglChooseConfig(m_display, attributes, &config, 1, &configs) || (configs != 1))
	{
		NDK_LOG("eglChooseConfig() failed.");
		return;
	}
    
    if(!eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format))
    {
        NDK_LOG("eglGetConfigAttrib() failed.");
		return;
    }
    
    ANativeWindow_setBuffersGeometry(IGraphicsContext::Get_AWindow(), 0, 0, format);
    
	m_surface = eglCreateWindowSurface(m_display, config, IGraphicsContext::Get_AWindow(), NULL);
    
	if(m_surface == EGL_NO_SURFACE)
	{
		NDK_LOG("eglCreateWindowSurface() failed.");
		return;
	}
    
    EGLint contextAttributess[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    m_context = eglCreateContext(m_display, config, NULL, contextAttributess);
    
    if(m_context == EGL_NO_CONTEXT)
    {
        NDK_LOG("eglCreateContext() failed.");
        return;
    }
    
	if(!eglMakeCurrent(m_display, m_surface, m_surface, m_context))
    {
        NDK_LOG("eglMakeCurrent() failed.");
        return;
    }
    
	GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
	i32 bindedFrameBufferHandle = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFrameBufferHandle);
	m_frameBufferHandle = bindedFrameBufferHandle;
    
	i32 bindedRenderBufferHandle = 0;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &bindedRenderBufferHandle);
	m_renderBufferHandle = bindedRenderBufferHandle;
}

CGraphicsContext_ndk::~CGraphicsContext_ndk(void)
{
    
}

void CGraphicsContext_ndk::Output(void) const
{
    eglSwapBuffers(m_display, m_surface);
}

#endif