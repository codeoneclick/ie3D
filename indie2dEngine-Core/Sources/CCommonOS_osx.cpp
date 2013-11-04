//
//  CCommonOS_osx.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCommonOS.h"

#if defined(__OSX__)

#include <Cocoa/Cocoa.h>

static ui32 g_screenWidth = 0;
static ui32 g_screenHeight = 0;

std::string Get_BundlePath(void)
{
    std::string path([[[NSBundle mainBundle] resourcePath] UTF8String]);
    path.append("/");
    return path;
};

ui32 Get_ScreenWidth(void)
{
    assert(g_screenWidth != 0);
    return g_screenWidth;
};

ui32 Get_ScreenHeight(void)
{
    assert(g_screenHeight != 0);
    return g_screenHeight;
};

void Set_ScreenWidth(ui32 _width)
{
    assert(g_screenWidth == 0);
    g_screenWidth = _width;
}

void Set_ScreenHeight(ui32 _height)
{
    assert(g_screenHeight == 0);
    g_screenHeight = _height;
}

f32 Get_Random(f32 _minValue, f32 _maxValue)
{
    f32 random = (((f32)arc4random()/0x100000000)*(_maxValue - _minValue) + _minValue);
    return random;
};

#endif