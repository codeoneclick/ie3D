//
//  IOGLWindow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IOGLWindow.h"

#if defined(__OSX__)

@interface IOGLWindow()

@property(nonatomic, assign) CVDisplayLinkRef m_displayLink;

@end

@implementation IOGLWindow

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

- (void) prepareOpenGL
{
	[super prepareOpenGL];
	[[self openGLContext] makeCurrentContext];
	GLint swap = 1;
	[[self openGLContext] setValues:&swap forParameter:NSOpenGLCPSwapInterval];
}

@end

#endif
