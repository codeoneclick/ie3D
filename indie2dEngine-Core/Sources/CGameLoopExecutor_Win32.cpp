//
//  CGameLoopExecutor.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"
#include "CFPSCounter.h"

#ifndef __APPLE__

const ui32 CGameLoopExecutor::k_MAX_FRAME_RATE = 24;
CGameLoopExecutor* CGameLoopExecutor::m_instance = nullptr;

CGameLoopExecutor* CGameLoopExecutor::Get_Instance(void)
{
	if(m_instance == nullptr)
	{
		m_instance = new CGameLoopExecutor();
	}
	return m_instance;
}

CGameLoopExecutor::CGameLoopExecutor(void) :
m_fpsCounter(std::make_shared<CFPSCounter>())
{
	m_instance = this;
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
			std::cout<<"[FPS] "<<Get_FramesPerSecond()<<std::endl;
		}
	}
};

#endif