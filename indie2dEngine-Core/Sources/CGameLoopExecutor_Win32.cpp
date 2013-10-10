//
//  CGameLoopExecutor.cpp
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CGameLoopExecutor.h"

#ifdef __WIN32__

bool Run(void)
{
	return true;
}

void ConnectToGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
   
};

void DisconnectFromGameLoop(std::shared_ptr<IGameLoopHandler> _handler)
{
    
};

#endif