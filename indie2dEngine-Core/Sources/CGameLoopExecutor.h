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

#ifdef __APPLE__

void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler);

#else

bool Run(void);
void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler);

#endif

ui32 Get_FramesPerSecond(void);
ui32 Get_TrianglesPerSecond(void);
void Inc_TrianglesCount(ui32 _value);

#endif
