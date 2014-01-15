//
//  CGameLoopExecutor_osx.c
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"

#if defined(__OSX__)

#include "IGameLoopHandler.h"
#include <Cocoa/Cocoa.h>
#include "CFPSCounter.h"

@interface CGameLoopExecutor_osx : NSObject

@property(nonatomic, unsafe_unretained) CGameLoopExecutor* m_gameLoopExecutor;
@property(nonatomic, strong) NSTimer* m_looper;

+ (CGameLoopExecutor_osx*)SharedInstance;

- (void)connectToGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler;
- (void)disconnectFromGameLoop:(std::shared_ptr<IGameLoopHandler>)_handler;

@end

@implementation CGameLoopExecutor_osx

+ (CGameLoopExecutor_osx*)SharedInstance
{
    static CGameLoopExecutor_osx *instance = nil;
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
        _m_looper = [NSTimer scheduledTimerWithTimeInterval:(1.0 / 60.0f) target:self selector:@selector(onUpdate) userInfo:nil repeats:YES];
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

- (void)terminateGameLoop
{
    [self.m_looper invalidate];
}

- (void)onUpdate
{
    assert(self.m_gameLoopExecutor != nullptr);
    assert(self.m_gameLoopExecutor->Get_FPSCounter() != nullptr);
    self.m_gameLoopExecutor->Get_FPSCounter()->Reset();
    gcdpp::impl::UpdateMainQueue();
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
    [[CGameLoopExecutor_osx SharedInstance] connectToGameLoop:_handler];
};

void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    [[CGameLoopExecutor_osx SharedInstance] disconnectFromGameLoop:_handler];
};

void TerminateGameLoop(void)
{
    [[CGameLoopExecutor_osx SharedInstance] terminateGameLoop];
};

ui32 Get_FramesPerSecond(void)
{
    return [[CGameLoopExecutor_osx SharedInstance] getFramesPerSecond];
};

ui32 Get_TrianglesPerSecond(void)
{
    return [[CGameLoopExecutor_osx SharedInstance] getTrianglesPerSecond];
};

void Inc_TrianglesCount(ui32 _value)
{
    [[CGameLoopExecutor_osx SharedInstance] incTrianglesCount:_value];
};

#endif

