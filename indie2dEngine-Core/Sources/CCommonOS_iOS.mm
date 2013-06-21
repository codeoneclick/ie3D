//
//  CCommonOS.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCommonOS.h"

#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

#include <mach/mach.h>
#include <mach/mach_time.h>

std::string Get_BundlePath(void)
{
    std::string path([[[NSBundle mainBundle] resourcePath] UTF8String]);
    path.append("/");
    return path;
};

ui32 Get_ScreenWidth(void)
{
    return [UIScreen mainScreen].bounds.size.height;
};

ui32 Get_ScreenHeight(void)
{
    return [UIScreen mainScreen].bounds.size.width;
};

ui64 Get_TickCount(void)
{
    static mach_timebase_info_data_t timebaseInfo;
    uint64_t machTime = mach_absolute_time();
    if (timebaseInfo.denom == 0 )
    {
        (void)mach_timebase_info(&timebaseInfo);
    }
    uint64_t milliseconds = ((machTime / 1000000) * timebaseInfo.numer) / timebaseInfo.denom;
    return milliseconds;
}

f32 Get_Random(f32 _minValue, f32 _maxValue)
{
    f32 random = (((f32)arc4random()/0x100000000)*(_maxValue - _minValue) + _minValue);
    return random;
}