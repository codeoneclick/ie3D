//
//  IInputContext.c
//  indieEngineCore
//
//  Created by Sergey Sergeev on 4/20/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "IInputContext.h"

extern std::shared_ptr<IInputContext> createInputContext_ios(ISharedOGLWindowRef window);
extern std::shared_ptr<IInputContext> createInputContext_win32(ISharedOGLWindowRef window);
extern std::shared_ptr<IInputContext> createInputContext_osx(ISharedOGLWindowRef window);
extern std::shared_ptr<IInputContext> createInputContext_ndk(ISharedOGLWindowRef window);

std::shared_ptr<IInputContext> IInputContext::createInputContext(ISharedOGLWindowRef window,
                                                                 E_PLATFORM_API api)
{
    std::shared_ptr<IInputContext> context = nullptr;
    switch (api)
    {
        case E_PLATFORM_API_IOS:
        {
#if defined(__IOS__)
            context = createInputContext_ios(window);
#else
            assert(false);
#endif
        }
            break;
            
        case E_PLATFORM_API_WIN32:
        {
#if defined(__WIN32__)
            context = createInputContext_win32(window);
#else
            assert(false);
#endif
        }
            break;
            
        case E_PLATFORM_API_OSX:
        {
#if defined(__OSX__)
            context = createInputContext_osx(window);
#else
            assert(false);
#endif
        }
            break;
        case E_PLATFORM_API_NDK:
        {
#if defined(__NDK__)
            context = createInputContext_ndk(window);
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

IInputContext::~IInputContext(void)
{
    m_handlers.clear();
}

void IInputContext::gestureRecognizerPressed(const glm::ivec2& point, bool isRightButton)
{
    std::for_each(m_handlers.begin(), m_handlers.end(), [point, isRightButton](ISharedGestureRecognizerHandler handler) {
        handler->onGestureRecognizerPressed(point, isRightButton);
    });
}

void IInputContext::gestureRecognizerMoved(const glm::ivec2& point)
{
    std::for_each(m_handlers.begin(), m_handlers.end(), [point](ISharedGestureRecognizerHandler handler) {
        handler->onGestureRecognizerMoved(point);
    });
}

void IInputContext::gestureRecognizerReleased(const glm::ivec2& point, bool isRightButton)
{
    std::for_each(m_handlers.begin(), m_handlers.end(), [point, isRightButton](ISharedGestureRecognizerHandler handler) {
        handler->onGestureRecognizerReleased(point, isRightButton);
    });
}

void IInputContext::addGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    m_handlers.insert(handler);
}

void IInputContext::removeGestureRecognizerHandler(ISharedGestureRecognizerHandlerRef handler)
{
    m_handlers.erase(handler);
}