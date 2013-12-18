//
//  CInputMgr.cpp
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#if defined(__IOS__)

#include "IInputContext.h"
#include "IInputTapRecognizerHandler.h"
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

@interface CInputRecognizerWindow : UIView

@property(nonatomic, unsafe_unretained) IInputContext* m_context;

@end

@implementation CInputRecognizerWindow

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->TapRecognizerPressed(glm::ivec2(point.x, point.y));
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->TapRecognizerMoved(glm::ivec2(point.x, point.y));
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->TapRecognizerReleased(glm::ivec2(point.x, point.y));
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->TapRecognizerReleased(glm::ivec2(point.x, point.y));
    }
}

@end

IInputContext::IInputContext(void* _window)
{
    UIView* window = (__bridge UIView*)_window;

    CInputRecognizerWindow* inputRecognizerWindow = [CInputRecognizerWindow new];
    inputRecognizerWindow.m_context = this;
    inputRecognizerWindow.frame = CGRectMake(0.0f,
                                             0.0f,
                                             window.frame.size.width,
                                             window.frame.size.height);
    [window addSubview:inputRecognizerWindow];
}

IInputContext::~IInputContext(void)
{

}

void IInputContext::RegisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{
    m_handlers.insert(_handler);
}

void IInputContext::UnregisterTapRecognizerHandler(std::shared_ptr<IInputTapRecognizerHandler> _handler)
{
    m_handlers.erase(_handler);
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

#endif


