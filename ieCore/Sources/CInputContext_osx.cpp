//
//  CInputMgr.cpp
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#include "IInputContext.h"
#include "IInputTapRecognizerHandler.h"

#if defined(__OSX__)

IInputContext::IInputContext(void* _hwnd)
{

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

#endif