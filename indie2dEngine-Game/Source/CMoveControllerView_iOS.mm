//
//  CMoveControllerView_iOS.m
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CMoveControllerView_iOS.h"
#include "IMoveControllerHandler.h"
#include "HKOTHEnums.h"

class CMoveController
{
private:
    
protected:
    
    std::set<std::shared_ptr<IMoveControllerHandler> > m_handlers;
    
public:
    
    CMoveController(void);
    ~CMoveController(void);
    
    void OnUpdate(ui32 _direction);
    
    void RegisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);
    void UnregisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler);
};

CMoveController::CMoveController(void)
{
    
}

CMoveController::~CMoveController(void)
{
    m_handlers.clear();
}

void CMoveController::OnUpdate(ui32 _direction)
{
    for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteMoveControllerUpdateCommand(_direction);
    }
}

void CMoveController::RegisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler)
{
    m_handlers.insert(_handler);
}

void CMoveController::UnregisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler)
{
    m_handlers.erase(_handler);
}

static CMoveControllerView_iOS* g_moveControllerView = nil;

@interface CMoveControllerView_iOS()

@property(nonatomic, assign) NSInteger m_maxOffsetX;
@property(nonatomic, assign) NSInteger m_minOffsetX;
@property(nonatomic, assign) NSInteger m_maxOffsetY;
@property(nonatomic, assign) NSInteger m_minOffsetY;
@property(nonatomic, strong) UIImageView* m_background;
@property(nonatomic, strong) UIImageView* m_control;
@property(nonatomic, unsafe_unretained) CMoveController* m_moveController;

@end

@implementation CMoveControllerView_iOS

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
    }
    return self;
}

- (void)awakeFromNib
{
    [self setBackgroundColor:[UIColor clearColor]];
    
    self.m_maxOffsetX = self.frame.size.width - 32;
    self.m_minOffsetX = 32;
    self.m_maxOffsetY = self.frame.size.height - 32;
    self.m_minOffsetY = 32;
    
    self.m_background = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
    [self.m_background setBackgroundColor:[UIColor blackColor]];
    [self.m_background setAlpha:0.25f];
    [self addSubview:self.m_background];
    
    self.m_control = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width / 2 - (self.frame.size.width / 3) / 2, self.frame.size.height / 2 - (self.frame.size.height / 3) / 2, self.frame.size.width / 3, self.frame.size.height / 3)];
    [self.m_control setBackgroundColor:[UIColor whiteColor]];
    [self.m_control setAlpha:0.5];
    [self addSubview:self.m_control];
    
    self.m_moveController = new CMoveController();
    assert(g_moveControllerView == nil);
    g_moveControllerView = self;
}

- (void)dealloc
{
    assert(g_moveControllerView != nil);
    g_moveControllerView = nil;
}

- (void)registerMoveControllerHandler:(std::shared_ptr<IMoveControllerHandler>)_handler;
{
    assert(self.m_moveController != nullptr);
    self.m_moveController->RegisterMoveControllerHandler(_handler);
}

- (void)unregisterMoveControllerHandler:(std::shared_ptr<IMoveControllerHandler>)_handler;
{
    assert(self.m_moveController != nullptr);
    self.m_moveController->UnregisterMoveControllerHandler(_handler);
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch*touch in touches)
    {
        CGPoint TouchLocation = [touch locationInView:self];
        [self update:TouchLocation];
    }
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch*touch in touches)
    {
        CGPoint TouchLocation = [touch locationInView:self];
        [self update:TouchLocation];
    }
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch*touch in touches)
    {
        assert(self.m_moveController != nullptr);
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NONE);
        CGRect frame = self.m_control.frame;
        frame.origin.x = self.frame.size.width / 2 - (self.frame.size.width / 3) / 2;
        frame.origin.y = self.frame.size.height / 2 - (self.frame.size.height / 3) / 2;
        self.m_control.frame = frame;
    }
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event
{
    for (UITouch*touch in touches)
    {
        assert(self.m_moveController != nullptr);
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NONE);
        CGRect frame = self.m_control.frame;
        frame.origin.x = self.frame.size.width / 2 - (self.frame.size.width / 3) / 2;
        frame.origin.y = self.frame.size.height / 2 - (self.frame.size.height / 3) / 2;
        self.m_control.frame = frame;
    }
}

- (void)update:(CGPoint)touchPoint
{
    if(touchPoint.x > _m_maxOffsetX && touchPoint.y > _m_maxOffsetY)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NORTH_EAST);
    }
    else if(touchPoint.x > _m_maxOffsetX && touchPoint.y < _m_minOffsetY)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_SOUTH_WEST);
    }
    else if(touchPoint.x < _m_minOffsetX && touchPoint.y > _m_maxOffsetY)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NORTH_WEST);
    }
    else if(touchPoint.x < _m_minOffsetX && touchPoint.y < _m_minOffsetY)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_SOUTH_EAST);
    }
    else if(touchPoint.x > _m_maxOffsetX)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_WEST);
    }
    else if(touchPoint.x < _m_minOffsetX)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_EAST);
    }
    else if(touchPoint.y > _m_maxOffsetY)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NORTH);
    }
    else if(touchPoint.y < _m_minOffsetY)
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_SOUTH);
    }
    else
    {
        self.m_moveController->OnUpdate(E_MOVE_CONTROLLER_DIRECTION_NONE);
    }
    
    CGRect frame = self.m_control.frame;
    frame.origin.x = touchPoint.x - frame.size.width / 2;
    frame.origin.y = touchPoint.y - frame.size.height / 2;
    self.m_control.frame = frame;
}

@end

void RegisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler)
{
    if(g_moveControllerView != nil)
    {
        [g_moveControllerView registerMoveControllerHandler:_handler];
    }
}

void UnregisterMoveControllerHandler(std::shared_ptr<IMoveControllerHandler> _handler)
{
    if(g_moveControllerView != nil)
    {
        [g_moveControllerView unregisterMoveControllerHandler:_handler];
    }
}
