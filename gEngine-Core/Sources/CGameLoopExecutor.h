//
//  CGameLoopExecutor.h
//  gEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CGameLoopExecutor_h
#define CGameLoopExecutor_h

#include "HCommon.h"
#include "IGameLoopHandler.h"

class CGameLoopExecutor
{
private:
    
protected:
    
    std::set<TSPIGameLoopHandler> m_handlers;
    
public:
    
    CGameLoopExecutor(void);
    ~CGameLoopExecutor(void);
    
    void ConnectToGameLoop(TSPIGameLoopHandler _handler);
    void DisconnectFromGameLoop(TSPIGameLoopHandler _handler);
  
    void OnGameLoopUpdate();
};

#ifdef __APPLE__

void ConnectToGameLoop(TSPIGameLoopHandler _handler);
void DisconnectFromGameLoop(TSPIGameLoopHandler _handler);

#else

void Run(void);

#endif


#endif 
