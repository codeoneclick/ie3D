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

class CGraphicsContext_osx : public IGraphicsContext
{
private:
    
protected:
    
    EGLDisplay m_display;
	EGLSurface m_surface;
	EGLContext m_context;
    
public:
    
	CGraphicsContext_osx(ISharedOGLWindowRef window);
    ~CGraphicsContext_osx(void);
    
    void draw(void) const;
};

std::shared_ptr<IGraphicsContext> createGraphicsContext_osx(ISharedOGLWindowRef window)
{
	assert(window != nullptr);
	return std::make_shared<CGraphicsContext_osx>(window);
};

CGraphicsContext_osx::CGraphicsContext_osx(ISharedOGLWindowRef window)
{
    m_window = window;
    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
	EGLint majorVersion, minorVersion;
    
	if(!eglInitialize(m_display, &majorVersion, &minorVersion))
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
		assert(false);
        return;
	}
    
    EGLNativeWindowType nativeWindow = (EGLNativeWindowType)window->getHWND();
	m_surface = eglCreateWindowSurface(m_display, config, nativeWindow, NULL);
    
	if(m_surface == EGL_NO_SURFACE)
	{
		assert(false);
        return;
	}
    
    EGLint contextAttributess[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    m_context = eglCreateContext(m_display, config, NULL, contextAttributess);
    
    if(m_context == EGL_NO_CONTEXT)
    {
        assert(false);
        return;
    }
    
	if(!eglMakeCurrent(m_display, m_surface, m_surface, m_context))
    {
        assert(false);
        return;
    }
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
	i32 bindedFrameBuffer = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFrameBuffer);
	m_frameBuffer = bindedFrameBuffer;
    
	i32 bindedRenderBuffer = 0;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &bindedRenderBuffer);
	m_renderBuffer = bindedRenderBuffer;
}

CGraphicsContext_osx::~CGraphicsContext_osx(void)
{
    
}

void CGraphicsContext_osx::draw(void) const
{
	eglSwapBuffers(m_display, m_surface);
}

#endif
