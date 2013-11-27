//
//  CGraphicsContext_osx.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"
#include "IOGLWindow.h"

#if defined(__OSX__)

#include <Cocoa/Cocoa.h>

#define k_MAX_RETRIES 128

class CGraphicsContext_osx : public IGraphicsContext
{
private:
    
protected:

    EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
	EGLNativeWindowType	m_window;
    
public:
    
	CGraphicsContext_osx(EGLNativeWindowType _elgWindow);
    ~CGraphicsContext_osx(void);
    
    void Output(void) const;
};

std::shared_ptr<IGraphicsContext> CreateGraphicsContext_osx(const void* _hwnd)
{
	EGLNativeWindowType window = (EGLNativeWindowType)(_hwnd);
	assert(window != nullptr);
	return std::make_shared<CGraphicsContext_osx>(window);
};

CGraphicsContext_osx::CGraphicsContext_osx(EGLNativeWindowType _window)
{
#if defined(__USE_OPENGL__)

#endif
    
#if defined(__USE_OPENGLES__)
    
    m_window = _window;
    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
	EGLint majorVersion, minorVersion;
    ui32 retries = 0;
	while(!eglInitialize(m_display, &majorVersion, &minorVersion) && retries < k_MAX_RETRIES)
	{
		NSLog(@"eglInitialize() failed. Retries : %i", retries);
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
        EGL_LEVEL,	0,
		EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,
		EGL_DEPTH_SIZE,	EGL_DONT_CARE,
		EGL_NONE
    };
    
    EGLConfig config;
	i32 configs;
	if (!eglChooseConfig(m_display, attributes, &config, 1, &configs) || (configs != 1))
	{
		NSLog(@"eglChooseConfig() failed.");
		return;
	}
    
	m_surface = eglCreateWindowSurface(m_display, config, m_window, NULL);
    
	if(m_surface == EGL_NO_SURFACE)
	{
		NSLog(@"eglCreateWindowSurface() failed.");
		return;
	}
    
    EGLint contextAttributess[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    m_context = eglCreateContext(m_display, config, NULL, contextAttributess);
    
    if(m_context == EGL_NO_CONTEXT)
    {
        NSLog(@"eglCreateContext() failed.");
        return;
    }
    
	if(!eglMakeCurrent(m_display, m_surface, m_surface, m_context))
    {
        NSLog(@"eglMakeCurrent() failed.");
        return;
    }
    
#endif
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
	i32 bindedFrameBufferHandle = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFrameBufferHandle);
	m_frameBufferHandle = bindedFrameBufferHandle;
    
	i32 bindedRenderBufferHandle = 0;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &bindedRenderBufferHandle);
	m_renderBufferHandle = bindedRenderBufferHandle;
}

CGraphicsContext_osx::~CGraphicsContext_osx(void)
{
    
}

void CGraphicsContext_osx::Output(void) const
{
#if defined(__USE_OPENGLES__)
    
	eglSwapBuffers(m_display, m_surface);
    
#endif
    
#if defined(__USE_OPENGL__)
    
	SwapBuffers(m_hDC);
    
#endif
}

#endif
