//
//  IOGLWindow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IOGLWindow.h"

#if defined(__IOS__)

@implementation OpenGLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame;
{
    if (self = [super initWithFrame:frame])
    {
        super.layer.opaque = YES;
    }
    return self;
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
    const UIView* hwnd = (__bridge UIView*)m_hwnd;
    return static_cast<ui32>(MAX_VALUE(hwnd.frame.size.width, hwnd.frame.size.height));
}

ui32 IOGLWindow::getHeight(void)
{
    assert(m_hwnd != nullptr);
    const UIView* hwnd = (__bridge UIView*)m_hwnd;
    return static_cast<ui32>(MIN_VALUE(hwnd.frame.size.width, hwnd.frame.size.height));
}

#endif