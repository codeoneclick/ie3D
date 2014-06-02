//
//  CInputMgr.cpp
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#if defined(__IOS__)

#include "IInputContext.h"
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include "IOGLWindow.h"

@interface InputHWND : UIView

@property(nonatomic, unsafe_unretained) IInputContext* m_context;

@end

@implementation InputHWND

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gestureRecognizerPressed(glm::ivec2(point.x, point.y), E_INPUT_BUTTON_MOUSE_LEFT);
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gestureRecognizerMoved(glm::ivec2(point.x, point.y), E_INPUT_BUTTON_MOUSE_LEFT);
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gestureRecognizerReleased(glm::ivec2(point.x, point.y), E_INPUT_BUTTON_MOUSE_LEFT);
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    assert(self.m_context != nullptr);
    for (UITouch* touch in touches)
    {
        CGPoint point = [touch locationInView:self];
        self.m_context->gestureRecognizerReleased(glm::ivec2(point.x, point.y), E_INPUT_BUTTON_MOUSE_LEFT);
    }
}

@end

class IInputContext_ios : public IInputContext
{
private:
    
protected:
    
public:
    
    IInputContext_ios(ISharedOGLWindowRef window);
    ~IInputContext_ios(void);
};

std::shared_ptr<IInputContext> createInputContext_ios(ISharedOGLWindowRef window)
{
    return std::make_shared<IInputContext_ios>(window);
};

IInputContext_ios::IInputContext_ios(ISharedOGLWindowRef window)
{
    UIView* view = (__bridge UIView*)window->getHWND();
    
    InputHWND* inputView = [[InputHWND alloc] init];
    inputView.m_context = this;
    inputView.frame = CGRectMake(0.0f,
                                 0.0f,
                                 view.frame.size.width,
                                 view.frame.size.height);
    [view addSubview:inputView];
}

IInputContext_ios::~IInputContext_ios(void)
{
    
}

#endif


