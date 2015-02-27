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
    
	NSOpenGLContext *m_context;
    
public:
    
	CGraphicsContext_osx(ISharedOGLWindowRef window);
    ~CGraphicsContext_osx(void);
    
    void makeCurrent(void) const;
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
    
    NSOpenGLPixelFormatAttribute attributes[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
#if defined(__OPENGL_30__)
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
#endif
        0
    };
    
    NSOpenGLPixelFormat *pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    if (!pixelformat)
    {
        assert(false);
    }
	   
    m_context = [[NSOpenGLContext alloc] initWithFormat:pixelformat shareContext:[NSOpenGLContext currentContext]];
    NSOpenGLView *view = (__bridge NSOpenGLView*)m_window->getHWND();
    [view setPixelFormat:pixelformat];
    [view setOpenGLContext:m_context];
    [m_context makeCurrentContext];
    
    GLint swap = 1;
    [m_context setValues:&swap forParameter:NSOpenGLCPSwapInterval];
    
#if defined(__OPENGL_30__)
    CGLEnable([m_context CGLContextObj], kCGLCECrashOnRemovedFunctions);
#endif
    
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

void CGraphicsContext_osx::makeCurrent(void) const
{
    [m_context makeCurrentContext];
}

void CGraphicsContext_osx::draw(void) const
{
    CGLFlushDrawable([m_context CGLContextObj]);
}

#endif
