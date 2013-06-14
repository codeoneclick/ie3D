//
//  CGameLoopExecutor_iOS.c
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"
#include "IGameLoopHandler.h"
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>

@interface CGameLoopExecutor_iOS : NSObject

@property(nonatomic, unsafe_unretained) CGameLoopExecutor* m_gameLoopExecutor;

+ (CGameLoopExecutor_iOS*)SharedInstance;

- (void)ConnectToGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler;
- (void)DisconnectFromGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler;

@end

@implementation CGameLoopExecutor_iOS

+ (CGameLoopExecutor_iOS*)SharedInstance
{
    static CGameLoopExecutor_iOS *instance = nil;
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        instance = [[self alloc] init];
    });
    return instance;
}

- (id)init
{
    self = [super init];
    if(self)
    {
        self.m_gameLoopExecutor = new CGameLoopExecutor();
        CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(onUpdate:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }
    return self;
}

- (void)ConnectToGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler
{
    assert(self.m_gameLoopExecutor != nullptr);
    self.m_gameLoopExecutor->ConnectToGameLoop(_handler);
}

- (void)DisconnectFromGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler
{
    assert(self.m_gameLoopExecutor != nullptr);
    self.m_gameLoopExecutor->DisconnectFromGameLoop(_handler);
}

- (void)onUpdate:(CADisplayLink*)displayLink
{
    assert(self.m_gameLoopExecutor != nullptr);
    thread_concurrency_main_queue_update();
    self.m_gameLoopExecutor->OnGameLoopUpdate();
}

@end

CGameLoopExecutor::CGameLoopExecutor(void)
{
    
}

CGameLoopExecutor::~CGameLoopExecutor(void)
{
    m_handlers.clear();
}

void CGameLoopExecutor::OnGameLoopUpdate(void)
{
    for(auto handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteGameLoopUpdateCommand(0.0f);
    }
}

void CGameLoopExecutor::ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    m_handlers.insert(_handler);
}

void CGameLoopExecutor::DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    m_handlers.erase(_handler);
}

void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    [[CGameLoopExecutor_iOS SharedInstance] ConnectToGameLoop:_handler];
}

void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    [[CGameLoopExecutor_iOS SharedInstance] DisconnectFromGameLoop:_handler];
}

