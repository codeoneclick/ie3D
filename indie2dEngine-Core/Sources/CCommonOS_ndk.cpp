//
//  CCommonOS_ndk.c
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCommonOS.h"

#if defined(__NDK__)

std::string Get_BundlePath(void)
{
    return "";
};

ui32 Get_ScreenWidth(void)
{
    return 640;
};

ui32 Get_ScreenHeight(void)
{
    return 480;
};

f32 Get_Random(f32 _minValue, f32 _maxValue)
{
    return 0.0f;
};

#endif