#include "IGraphicsContext.h"

CGLContext_Win32::CGLContext_Win32(const HWND& _hWND, const HDC& _hDC)
{
	m_eglWindow = _hWND;
	m_eglDisplay = eglGetDisplay(_hDC);

	if(m_eglDisplay == EGL_NO_DISPLAY)
		m_eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(m_eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		MessageBox(0, L"eglInitialize() failed.", L"iGaia-CartoonPanzers", MB_OK | MB_ICONEXCLAMATION);
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
		MessageBox(0, L"eglChooseConfig() failed.", L"iGaia-CartoonPanzers", MB_OK | MB_ICONEXCLAMATION);
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

	i32 bindedFrameBufferHandle = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bindedFrameBufferHandle);
	m_frameBufferHandle = bindedFrameBufferHandle;

	i32 bindedRenderBufferHandle = 0;
	glGetIntegerv(GL_RENDERBUFFER_BINDING, &bindedRenderBufferHandle);
	m_renderBufferHandle = bindedRenderBufferHandle;
}

CGLContext_Win32::~CGLContext_Win32(void)
{

}

void CGLContext_Win32::Present(void) const
{
	assert(eglSwapBuffers(m_eglDisplay, m_eglSurface) == EGL_TRUE);
}