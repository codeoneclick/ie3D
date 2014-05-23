//
//  CInputMgr.cpp
//  iGaia-CartoonPanzers
//
//  Created by Sergey Sergeev on 3/27/13.
//
//

#include "IInputContext.h"
#include "IOGLWindow.h"

#if defined(__OSX__)

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@interface InputHWND : NSView

@property(nonatomic, unsafe_unretained) IInputContext* m_context;

@end

@implementation InputHWND

- (void)mouseDown:(NSEvent*)event
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gestureRecognizerPressed(glm::ivec2(point.x, self.frame.size.height - point.y));
}

- (void)rightMouseDown:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gestureRecognizerPressed(glm::ivec2(point.x, self.frame.size.height - point.y), true);
}

- (void)mouseMoved:(NSEvent *)event
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gestureRecognizerMoved(glm::ivec2(point.x, self.frame.size.height - point.y));
}

- (void)mouseDragged:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gestureRecognizerMoved(glm::ivec2(point.x, self.frame.size.height - point.y));
}

- (void)rightMouseDragged:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gestureRecognizerMoved(glm::ivec2(point.x, self.frame.size.height - point.y));
}

- (void)mouseUp:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gestureRecognizerReleased(glm::ivec2(point.x, self.frame.size.height - point.y));
}

- (void)rightMouseUp:(NSEvent *)event;
{
    CGPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
    self.m_context->gestureRecognizerReleased(glm::ivec2(point.x, self.frame.size.height - point.y), true);
}

@end

class IInputContext_osx : public IInputContext
{
private:
    
protected:
    
public:
    
    IInputContext_osx(ISharedOGLWindowRef window);
    ~IInputContext_osx(void);
};

std::shared_ptr<IInputContext> createInputContext_osx(ISharedOGLWindowRef window)
{
    return std::make_shared<IInputContext_osx>(window);
};

IInputContext_osx::IInputContext_osx(ISharedOGLWindowRef window)
{
    NSView* view = (__bridge NSView*)window->getHWND();
    
    InputHWND* inputView = [[InputHWND alloc] init];
    inputView.m_context = this;
    inputView.frame = CGRectMake(0.0f,
                                 0.0f,
                                 view.frame.size.width,
                                 view.frame.size.height);
    [view addSubview:inputView];
}

IInputContext_osx::~IInputContext_osx(void)
{
    
}

#endif