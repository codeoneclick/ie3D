//
//  CGameLoopExecutor.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"
#include "CFPSCounter.h"

#if defined(__WIN32__)

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
	MSG msg;
	memset(&msg, 0, sizeof(msg));
	while(WM_QUIT != msg.message)
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			assert(CGameLoopExecutor::Get_Instance() != nullptr);
			assert(CGameLoopExecutor::Get_Instance()->Get_FPSCounter() != nullptr);
			CGameLoopExecutor::Get_Instance()->Get_FPSCounter()->Reset();
			CGameLoopExecutor::Get_Instance()->OnGameLoopUpdate();
			CGameLoopExecutor::Get_Instance()->Get_FPSCounter()->Submit();
			std::wstring strfps = std::to_wstring(Get_FramesPerSecond());
			SetWindowText(GetActiveWindow(), strfps.c_str());
		}
	}
};

#endif