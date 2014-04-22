//
//  IOGLWindow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IOGLWindow.h"

#if defined(__OSX__)

@interface OpenGLView()

@property(nonatomic, assign) CVDisplayLinkRef m_displayLink;

@end

@implementation OpenGLView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        NSOpenGLPixelFormatAttribute attributes[] =
        {
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFADepthSize, 24,
            NULL
        };
        
        NSOpenGLPixelFormat *pixelformat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
        NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pixelformat shareContext:nil];
        [self setPixelFormat:pixelformat];
        [self setOpenGLContext:context];
    }
    return self;
}

- (void)prepareOpenGL
{
	[super prepareOpenGL];
	[[self openGLContext] makeCurrentContext];
	GLint swap = 1;
	[[self openGLContext] setValues:&swap forParameter:NSOpenGLCPSwapInterval];
}

@end

IOGLWindow::IOGLWindow(void* hwnd) :
m_hwnd(hwnd)
{
    assert(m_hwnd != nullptr);
}

IOGLWindow::~IOGLWindow(void)
{
    
}

const void* IOGLWindow::getHWND(void) const
{
    return m_hwnd;
}

ui32 IOGLWindow::getWidth(void)
{
    assert(m_hwnd != nullptr);
    const NSView *hwnd = (__bridge NSView*)m_hwnd;
    return static_cast<ui32>(hwnd.frame.size.width);
}

ui32 IOGLWindow::getHeight(void)
{
    assert(m_hwnd != nullptr);
    const NSView* hwnd = (__bridge NSView*)m_hwnd;
    return static_cast<ui32>(hwnd.frame.size.height);
}

#endif
