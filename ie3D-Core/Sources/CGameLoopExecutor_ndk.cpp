//
//  CGameLoopExecutor_ndk.c
//  indieEngineCore
//
//  Created by Sergey Sergeev on 11/29/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"
#include "CFPSCounter.h"

#if defined(__NDK__)

void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
	CGameLoopExecutor::Get_Instance()->ConnectToGameLoop(_handler);
};

void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
	CGameLoopExecutor::Get_Instance()->DisconnectFromGameLoop(_handler);
};

ui32 Get_FramesPerSecond(void)
{
	assert(CGameLoopExecutor::Get_Instance() != nullptr);
    assert(CGameLoopExecutor::Get_Instance()->Get_FPSCounter() != nullptr);
    return CGameLoopExecutor::Get_Instance()->Get_FPSCounter()->Get_FramesPerSecond();
};

ui32 Get_TrianglesPerSecond(void)
{
    assert(CGameLoopExecutor::Get_Instance() != nullptr);
    assert(CGameLoopExecutor::Get_Instance()->Get_FPSCounter() != nullptr);
    return CGameLoopExecutor::Get_Instance()->Get_FPSCounter()->Get_TrianglesPerSecond();
};

void Inc_TrianglesCount(ui32 _value)
{
	assert(CGameLoopExecutor::Get_Instance() != nullptr);
    assert(CGameLoopExecutor::Get_Instance()->Get_FPSCounter() != nullptr);
    return CGameLoopExecutor::Get_Instance()->Get_FPSCounter()->Inc_TrianglesCount(_value);
};

void Run(void)
{
    NDK_LOG("Run Loop");
    assert(CGameLoopExecutor::Get_Instance() != nullptr);
    assert(CGameLoopExecutor::Get_Instance()->Get_FPSCounter() != nullptr);
    CGameLoopExecutor::Get_Instance()->Get_FPSCounter()->Reset();
    CGameLoopExecutor::Get_Instance()->OnGameLoopUpdate();
    CGameLoopExecutor::Get_Instance()->Get_FPSCounter()->Submit();
};

#endif
