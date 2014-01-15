//
//  CGameLoopExecutor_iOS.c
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"

#if defined(__IOS__)

#include "IGameLoopHandler.h"
#include <Foundation/Foundation.h>
#include <UIKit/UIKit.h>
#include "CFPSCounter.h"

@interface CGameLoopExecutor_iOS : NSObject

@property(nonatomic, unsafe_unretained) CGameLoopExecutor* m_gameLoopExecutor;

+ (CGameLoopExecutor_iOS*)SharedInstance;

- (void)connectToGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler;
- (void)disconnectFromGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler;

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

- (void)connectToGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler
{
    assert(self.m_gameLoopExecutor != nullptr);
    self.m_gameLoopExecutor->ConnectToGameLoop(_handler);
}

- (void)disconnectFromGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler
{
    assert(self.m_gameLoopExecutor != nullptr);
    self.m_gameLoopExecutor->DisconnectFromGameLoop(_handler);
}

- (void)onUpdate:(CADisplayLink*)displayLink
{
    assert(self.m_gameLoopExecutor != nullptr);
    assert(self.m_gameLoopExecutor->Get_FPSCounter() != nullptr);
    self.m_gameLoopExecutor->Get_FPSCounter()->Reset();
#if defined(__USE_GCDPP__)
    gcdpp::impl::UpdateMainQueue();
#endif
    self.m_gameLoopExecutor->OnGameLoopUpdate();
    self.m_gameLoopExecutor->Get_FPSCounter()->Submit();
}

- (ui32)getFramesPerSecond;
{
    assert(self.m_gameLoopExecutor != nullptr);
    assert(self.m_gameLoopExecutor->Get_FPSCounter() != nullptr);
    return self.m_gameLoopExecutor->Get_FPSCounter()->Get_FramesPerSecond();
}

- (ui32)getTrianglesPerSecond;
{
    assert(self.m_gameLoopExecutor != nullptr);
    assert(self.m_gameLoopExecutor->Get_FPSCounter() != nullptr);
    return self.m_gameLoopExecutor->Get_FPSCounter()->Get_TrianglesPerSecond();
}

- (void)incTrianglesCount:(ui32)_value
{
    assert(self.m_gameLoopExecutor != nullptr);
    assert(self.m_gameLoopExecutor->Get_FPSCounter() != nullptr);
    return self.m_gameLoopExecutor->Get_FPSCounter()->Inc_TrianglesCount(_value);
}

@end

void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    [[CGameLoopExecutor_iOS SharedInstance] connectToGameLoop:_handler];
};

void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    [[CGameLoopExecutor_iOS SharedInstance] disconnectFromGameLoop:_handler];
};

ui32 Get_FramesPerSecond(void)
{
    return [[CGameLoopExecutor_iOS SharedInstance] getFramesPerSecond];
};

ui32 Get_TrianglesPerSecond(void)
{
    return [[CGameLoopExecutor_iOS SharedInstance] getTrianglesPerSecond];
};

void Inc_TrianglesCount(ui32 _value)
{
    [[CGameLoopExecutor_iOS SharedInstance] incTrianglesCount:_value];
};

#endif

