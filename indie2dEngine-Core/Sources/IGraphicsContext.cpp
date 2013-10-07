//
//  IGraphicsContext.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"
#include "CGraphicsContext_iOS.mm"

IGraphicsContext::IGraphicsContext(void)
{
}

IGraphicsContext::~IGraphicsContext(void)
{
    
}

std::shared_ptr<IGraphicsContext> IGraphicsContext::CreateGraphicsContext(const void* _hwnd, E_PLATFORM_API _api)
{
    switch (_api){
        case E_PLATFORM_API_IOS:
            
            break;
            
        case E_PLATFORM_API_WIN32:
            
            break;
            
        default:
            break;
    }
}