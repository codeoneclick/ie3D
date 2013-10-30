//
//  CGraphicsContext_osx.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"
#include "IOGLWindow.h"
#include <Cocoa/Cocoa.h>

#if defined(__OSX__)

class CGraphicsContext_osx : public IGraphicsContext
{
private:
    
protected:
    
    EGLDisplay m_eglDisplay;
	EGLConfig m_eglConfig;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;
	EGLNativeWindowType	m_eglWindow;
    
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
    
    m_eglWindow = _window;
    
	if(m_eglDisplay == EGL_NO_DISPLAY)
    {
		m_eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
    }
    
	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(m_eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		NSLog(@"eglInitialize() failed.");
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
    
	int iConfigs;
	if (!eglChooseConfig(m_eglDisplay, attributes, &m_eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		NSLog(@"eglChooseConfig() failed.");
		return;
	}
    
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_eglWindow, NULL);
    
	if(m_eglSurface == EGL_NO_SURFACE)
	{
		NSLog(@"eglCreateWindowSurface() failed.");
		return;
	}
    
    EGLint contextAttrs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL, contextAttrs);
    
    if(m_eglContext == EGL_NO_CONTEXT)
    {
        NSLog(@"eglCreateContext() failed.");
        return;
    }
    
	if(!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext))
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
	assert(eglSwapBuffers(m_eglDisplay, m_eglSurface) == EGL_TRUE);
#endif
    
#if defined(__USE_OPENGL__)
	SwapBuffers(m_hDC);	
#endif
}

#endif