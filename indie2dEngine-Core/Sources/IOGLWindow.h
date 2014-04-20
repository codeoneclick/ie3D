//
//  IOGLWindow.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/17/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IOGLWindow_h
#define IOGLWindow_h

#include "HCommon.h"

#if defined(__IOS__)

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include <QuartzCore/QuartzCore.h>

@interface OpenGLView : UIView

@property (nonatomic, assign) UIInterfaceOrientation orientation;

@end

#elif defined(__OSX__)

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@interface OpenGLView : NSOpenGLView

@end

#endif

class IOGLWindow
{
private:
    
protected:
    
#if defined(__IOS__) || defined(__OSX__)
    
    void* m_hwnd;
    
#elif defined(__NDK__)
    
#elif defined(__WIN32__)
    
    HWND m_hwnd;
	HDC	m_hdc;
    
#endif
    
public:
    
    IOGLWindow(void* hwnd);
    virtual ~IOGLWindow(void);
    
    virtual ui32 getWidth(void);
    virtual ui32 getHeight(void);
    
#if defined(__WIN32__)
    
    HWND getHWND(void) const;
    HDC getHDC(void) const;
    
#elif defined(__NDK__)
    
#elif defined(__IOS__) || defined(__OSX__)
    
    const void* getHWND(void) const;
    
#endif
    
};

#endif 
