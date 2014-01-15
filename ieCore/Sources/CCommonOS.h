//
//  CCommonOS.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/16/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CCommonOS_h
#define CCommonOS_h

#include "HCommon.h"

std::string Get_BundlePath(void);

ui32 Get_ScreenWidth(void);
ui32 Get_ScreenHeight(void);

f32 Get_Random(f32 _minValue, f32 _maxValue);

#if defined(__OSX__)

void Set_BundlePath(const std::string& _path);
void Set_ScreenWidth(ui32 _width);
void Set_ScreenHeight(ui32 _height);

#endif

#endif 
