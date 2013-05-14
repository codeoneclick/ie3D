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

class CGameLoopExecutor
{
private:
    
protected:
    
    std::set<std::shared_ptr<IGameLoopHandler> > m_handlers;
    
public:
    
    CGameLoopExecutor(void);
    ~CGameLoopExecutor(void);
    
    void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
    void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
  
    void OnGameLoopUpdate();
};

#ifdef __APPLE__

void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler);
void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler);

#else

void Run(void);

#endif


#endif 
