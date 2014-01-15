//
//  CInputContext_ndk.c
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "IInputContext.h"
#include "IInputTapRecognizerHandler.h"

#if defined(__NDK__)

IInputContext* IInputContext::m_sharedInstance = nullptr;

IInputContext::IInputContext(void* _hwnd)
{
    m_sharedInstance = this;
}

IInputContext::~IInputContext(void)
{
    
}

void IInputContext::TapRecognizerPressed(const glm::ivec2& _point)
{
	for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteInputTapRecognizerDidPressedCommand(_point);
    }
}

void IInputContext::TapRecognizerMoved(const glm::ivec2& _point)
{
	for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteInputTapRecognizerDidMovedCommand(_point);
    }
}

void IInputContext::TapRecognizerReleased(const glm::ivec2& _point)
{
	for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteInputTapRecognizerDidReleasedCommand(_point);
    }
}

void IInputContext::RegisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{
    m_handlers.insert(_handler);
}

void IInputContext::UnregisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{
    m_handlers.erase(_handler);
}

void IInputContext::NativeCallTapRecognizerPressed(const glm::ivec2& _point)
{
    assert(m_sharedInstance != nullptr);
    m_sharedInstance->TapRecognizerPressed(_point);
}

void IInputContext::NativeCallTapRecognizerMoved(const glm::ivec2& _point)
{
    assert(m_sharedInstance != nullptr);
    m_sharedInstance->TapRecognizerMoved(_point);
}

void IInputContext::NativeCallTapRecognizerReleased(const glm::ivec2& _point)
{
    assert(m_sharedInstance != nullptr);
    m_sharedInstance->TapRecognizerReleased(_point);
}

#endif
