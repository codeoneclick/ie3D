//
//  CGameLoopExecutor.c
//  indie2dEngine
//
//  Created by Sergey Sergeev on 10/21/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"
#include "CFPSCounter.h"

#if defined(__WIN32__) || defined(__NDK__)

CGameLoopExecutor* CGameLoopExecutor::m_instance = nullptr;

CGameLoopExecutor* CGameLoopExecutor::Get_Instance(void)
{
	if(m_instance == nullptr)
	{
		m_instance = new CGameLoopExecutor();
	}
	return m_instance;
}

#endif

CGameLoopExecutor::CGameLoopExecutor(void) :
m_fpsCounter(std::make_shared<CFPSCounter>())
{
#if defined(__WIN32__) || defined(__NDK__)
    
	m_instance = this;
    
#endif
}

CGameLoopExecutor::~CGameLoopExecutor(void)
{
    m_handlers.clear();
}

void CGameLoopExecutor::OnGameLoopUpdate(void)
{
#if defined(__NDK__)
    
    NDK_LOG("OnGameLoopUpdate");
    
#endif
    static std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    f32 deltatime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
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