//
//  IGraphicsContext.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IGraphicsContext.h"
#include "IOGLWindow.h"

extern std::shared_ptr<IGraphicsContext> createGraphicsContext_ios(const std::shared_ptr<IOGLWindow>& window);
extern std::shared_ptr<IGraphicsContext> createGraphicsContext_win32(const std::shared_ptr<IOGLWindow>& window);
extern std::shared_ptr<IGraphicsContext> createGraphicsContext_osx(const std::shared_ptr<IOGLWindow>& window);
extern std::shared_ptr<IGraphicsContext> createGraphicsContext_ndk(const std::shared_ptr<IOGLWindow>& window);

std::shared_ptr<IGraphicsContext> IGraphicsContext::createGraphicsContext(ISharedOGLWindowRef window,
                                                                          E_PLATFORM_API api)
{
    std::shared_ptr<IGraphicsContext> context = nullptr;
    switch (api)
    {
        case E_PLATFORM_API_IOS:
        {
#if defined(__IOS__)
            context = createGraphicsContext_ios(window);
#else
            assert(false);
#endif
        }
            break;
            
        case E_PLATFORM_API_WIN32:
        {
#if defined(__WIN32__)
            context = createGraphicsContext_win32(window);
#else
            assert(false);
#endif
        }
            break;
            
        case E_PLATFORM_API_OSX:
        {
#if defined(__OSX__)
            context = createGraphicsContext_osx(window);
#else
            assert(false);
#endif
        }
            break;
        case E_PLATFORM_API_NDK:
        {
#if defined(__NDK__)
            context = createGraphicsContext_ndk(window);
#else
            assert(false);
#endif
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

ui32 IGraphicsContext::getFrameBuffer(void) const
{
    return m_frameBuffer;
}

ui32 IGraphicsContext::getRenderBuffer(void) const
{
    return m_renderBuffer;
}

ui32 IGraphicsContext::getDepthBuffer(void) const
{
    return m_depthBuffer;
}

ui32 IGraphicsContext::getWidth(void) const
{
    return m_window->getWidth();
}

ui32 IGraphicsContext::getHeight(void) const
{
    return m_window->getHeight();
}


