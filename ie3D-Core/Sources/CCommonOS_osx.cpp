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
static std::string g_path = "";

std::string bundlepath(void)
{
    std::string path([[[NSBundle mainBundle] resourcePath] UTF8String]);
    path.append("/");
    return path;
};

std::string executablepath(void)
{
    std::string path([[[[[[NSBundle mainBundle] resourcePath]
                         stringByDeletingLastPathComponent]
                        stringByDeletingLastPathComponent]
                       stringByDeletingLastPathComponent] UTF8String]);
    path.append("/");
    return path;
};

void memory_usage(void)
{
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t error = task_info(mach_task_self(),
                                   TASK_BASIC_INFO,
                                   (task_info_t)&info,
                                   &size);
    if(error == KERN_SUCCESS)
    {
        std::cout<<"memory usage: "<<info.resident_size<<" b"<<std::endl;
        std::cout<<"||"<<std::endl;
        std::cout<<"memory usage: "<<info.resident_size / 1024<<" kb"<<std::endl;
        std::cout<<"||"<<std::endl;
        std::cout<<"memory usage: "<<info.resident_size/ (1024 * 1024)<<" mb"<<std::endl;
    }
}

void Set_BundlePath(const std::string& _path)
{
    g_path = _path;
    g_path.append("/");
}

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