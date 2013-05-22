//
//  IGameLoopHandler.h
//  indi2dEngine-Core
//
//  Created by Sergey Sergeev on 5/7/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IGameLoopHandler_h
#define IGameLoopHandler_h

#include "HCommon.h"


class CGameLoopCommands final
{
private:
    
    typedef std::function<void(f32)> __GAME_LOOP_UPDATE_COMMAND;
    
    friend class IGameLoopHandler;
    friend class CGameLoopExecutor;
    
    __GAME_LOOP_UPDATE_COMMAND m_gameLoopUpdateCommand;
    
protected:
    
    CGameLoopCommands(void);
    
    void _ConnectGameLoopUpdateCommand(const __GAME_LOOP_UPDATE_COMMAND& _command);
    void _ExecuteGameLoopUpdateCommand(f32 _deltatime);
    
public:
    
    ~CGameLoopCommands(void);
};

class IGameLoopHandler : public std::enable_shared_from_this<IGameLoopHandler>
{
private:
    
    friend class CGameLoopExecutor;
    
    CGameLoopCommands m_commands;
    
    void _ConnectCommands(void);
    inline CGameLoopCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    IGameLoopHandler(void);
    
    virtual void _OnGameLoopUpdate(f32 _deltatime) = 0;
    
public:
    
    virtual ~IGameLoopHandler(void);
};

#endif
