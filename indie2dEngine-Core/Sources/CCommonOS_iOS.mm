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