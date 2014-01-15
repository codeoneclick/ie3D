//
//  IGraphicsContext.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"

class CGraphicsContext_iOS;

#if defined(__NDK__)

ANativeWindow* IGraphicsContext::m_AWindow = nullptr;

#endif

extern std::shared_ptr<IGraphicsContext> CreateGraphicsContext_iOS(const void* _hwnd);
extern std::shared_ptr<IGraphicsContext> CreateGraphicsContext_win32(const void* _hwnd);
extern std::shared_ptr<IGraphicsContext> CreateGraphicsContext_osx(const void* _hwnd);
extern std::shared_ptr<IGraphicsContext> CreateGraphicsContext_ndk(const void* _hwnd);

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
#if defined(__IOS__)
            
            context = CreateGraphicsContext_iOS(_hwnd);
            m_contexts.push_back(context);
            
#else
            
            assert(false);
            
#endif
        }
            break;
            
        case E_PLATFORM_API_WIN32:
        {
#if defined(__WIN32__)
            
            context = CreateGraphicsContext_win32(_hwnd);
            m_contexts.push_back(context);
            
#else
            assert(false);
            
#endif
        }
            break;
#if defined(__OSX__)
        case E_PLATFORM_API_OSX:
        {
            context = CreateGraphicsContext_osx(_hwnd);
            m_contexts.push_back(context);
        }
            break;
#else
            assert(false);
#endif
#if defined(__NDK__)
        case E_PLATFORM_API_NDK:
        {
            context = CreateGraphicsContext_ndk(_hwnd);
            m_contexts.push_back(context);
        }
            break;
#else
            assert(false);
#endif
            
        default:
        {
            assert(false);
        }
            break;
    }
    assert(context != nullptr);
    return context;
}