//
//  IGraphicsContext.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"

class CGraphicsContext_iOS;

extern std::shared_ptr<IGraphicsContext> CreateGraphicsContext_iOS(const void* _hwnd);
extern std::shared_ptr<IGraphicsContext> CreateGraphicsContext_Win32(const void* _hwnd);

std::vector<std::shared_ptr<IGraphicsContext> > IGraphicsContext::m_contexts;

IGraphicsContext::IGraphicsContext(void)
{
}

IGraphicsContext::~IGraphicsContext(void)
{
    std::vector<std::shared_ptr<IGraphicsContext>>::iterator iterator = std::find(m_contexts.begin(), m_contexts.end(), shared_from_this());
    assert(iterator != m_contexts.end());
    m_contexts.erase(iterator);
}

std::shared_ptr<IGraphicsContext> IGraphicsContext::CreateGraphicsContext(const void* _hwnd, E_PLATFORM_API _api)
{
    std::shared_ptr<IGraphicsContext> context = nullptr;
    switch (_api)
    {
        case E_PLATFORM_API_IOS:
        {
            context = CreateGraphicsContext_iOS(_hwnd);
            m_contexts.push_back(context);
        }
            break;
            
        case E_PLATFORM_API_WIN32:
        {
            //context = CreateGraphicsContext_Win32(_hwnd);
            //m_contexts.push_back(context);
        }
            break;
            
        default:
        {
            assert(false);
        }
            break;
    }
    assert(context != nullptr);
    return context;
}