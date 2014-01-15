//
//  CGraphicsContext_win32.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"
#include "IOGLWindow.h"

#if defined(__WIN32__)

class CGraphicsContext_win32 : public IGraphicsContext
{
private:

protected:
 
    EGLDisplay m_eglDisplay;
	EGLConfig m_eglConfig;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;
	EGLNativeWindowType	m_eglWindow;
	HDC m_hDC;
    
public:
    
	CGraphicsContext_win32(const HWND& _hWND, const HDC& _hDC);
    ~CGraphicsContext_win32(void);

    void Output(void) const;
};

std::shared_ptr<IGraphicsContext> CreateGraphicsContext_win32(const void* _hwnd)
{
	IOGLWindow* window = (IOGLWindow*)(_hwnd);
	assert(window != nullptr);
	return std::make_shared<CGraphicsContext_win32>(window->Get_HWND(), window->Get_HDC());
};

CGraphicsContext_win32::CGraphicsContext_win32(const HWND& _hWND, const HDC& _hDC)
{
#if defined(__USE_OPENGL__)
	m_hDC = _hDC;
	HGLRC hRC = NULL;		
	if (!(hRC = wglCreateContext(m_hDC)))
	{
		MessageBox(0, L"wglCreateContext() failed.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	if(!wglMakeCurrent(m_hDC, hRC))	
	{
		MessageBox(0, L"wglMakeCurrent() failed.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return;						
	}
#endif

#if defined(__USE_OPENGLES__)
	m_eglWindow = _hWND;
	m_eglDisplay = eglGetDisplay(_hDC);

	if(m_eglDisplay == EGL_NO_DISPLAY)
		m_eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(m_eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		MessageBox(0, L"eglInitialize() failed.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,	0,
		EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,
		EGL_DEPTH_SIZE,	EGL_DONT_CARE,
		EGL_NONE
	};

	int iConfigs;
	if (!eglChooseConfig(m_eglDisplay, pi32ConfigAttribs, &m_eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		MessageBox(0, L"eglChooseConfig() failed.", L"indieEngine", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_eglWindow, NULL);

	if(m_eglSurface == EGL_NO_SURFACE)
	{
		eglGetError();
		m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, NULL, NULL);
	}

	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL, ai32ContextAttribs);

	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
#endif

	i32 bindedFrameBufferHandle = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFrameBufferHandle);
	m_frameBufferHandle = bindedFrameBufferHandle;

	i32 bindedRenderBufferHandle = 0;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &bindedRenderBufferHandle);
	m_renderBufferHandle = bindedRenderBufferHandle;
}

CGraphicsContext_win32::~CGraphicsContext_win32(void)
{

}

void CGraphicsContext_win32::Output(void) const
{
#if defined(__USE_OPENGLES__)
	assert(eglSwapBuffers(m_eglDisplay, m_eglSurface) == EGL_TRUE);
#endif

#if defined(__USE_OPENGL__)
	SwapBuffers(m_hDC);	
#endif
}

#endif