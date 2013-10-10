//
//  CGameLoopExecutor_iOS.c
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"

#ifdef __APPLE__

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

const ui32 CGameLoopExecutor::k_MAX_FRAME_RATE = 24;

CGameLoopExecutor::CGameLoopExecutor(void) :
m_fpsCounter(std::make_shared<CFPSCounter>())
{
    
}

CGameLoopExecutor::~CGameLoopExecutor(void)
{
    m_handlers.clear();
}

void CGameLoopExecutor::OnGameLoopUpdate(void)
{
    static std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    f32 deltatime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / (k_MAX_FRAME_RATE * 1000.0f);
    lastTime = currentTime;
    for(const auto& handler : m_handlers)
    {
        handler->_Get_Commands()._ExecuteGameLoopUpdateCommand(deltatime);
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

