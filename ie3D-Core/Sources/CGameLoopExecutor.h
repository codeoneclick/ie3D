//
//  CGameLoopExecutor.h
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameLoopExecutor_h
#define CGameLoopExecutor_h

#include "HCommon.h"
#include "IGameLoopHandler.h"

class CFPSCounter;

class CGameLoopExecutor
{
private:
    
protected:
    
    std::set<std::shared_ptr<IGameLoopHandler> > m_handlers;
    std::shared_ptr<CFPSCounter> m_fpsCounter;
    static const ui32 k_MAX_FRAME_RATE;

#if defined(__WIN32__) || defined(__NDK__)
    
	static CGameLoopExecutor* m_instance;
	static CGameLoopExecutor* Get_Instance(void);

	friend void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
	friend void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler);

	friend ui32 Get_FramesPerSecond(void);
	friend ui32 Get_TrianglesPerSecond(void);
	friend void Inc_TrianglesCount(ui32 _value);

	friend void Run(void);
    
#endif
    
public:
    
    CGameLoopExecutor(void);
    ~CGameLoopExecutor(void);
    
    void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
    void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
  
    void OnGameLoopUpdate();
    
    inline std::shared_ptr<CFPSCounter> Get_FPSCounter(void)
    {
        assert(m_fpsCounter != nullptr);
        return m_fpsCounter;
    };
};

#if defined(__IOS__)

#elif defined(__WIN32__) || defined(__NDK__)

void Run(void);

#endif

void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler);

#if defined(__OSX__)

void TerminateGameLoop(void);

#endif

ui32 Get_FramesPerSecond(void);
ui32 Get_TrianglesPerSecond(void);
void Inc_TrianglesCount(ui32 _value);

#endif
