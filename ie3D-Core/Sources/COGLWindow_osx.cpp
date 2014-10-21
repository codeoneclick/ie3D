//
//  IOGLWindow.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#if defined(__OSX__)

#include "IOGLWindow.h"
#include <Cocoa/Cocoa.h>

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
    const NSOpenGLView *hwnd = (__bridge NSOpenGLView *)m_hwnd;
    return static_cast<ui32>(hwnd.frame.size.width);
}

ui32 IOGLWindow::getHeight(void)
{
    assert(m_hwnd != nullptr);
    const NSOpenGLView* hwnd = (__bridge NSOpenGLView *)m_hwnd;
    return static_cast<ui32>(hwnd.frame.size.height);
}

#endif
