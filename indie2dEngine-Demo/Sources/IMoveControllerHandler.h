//
//  CMoveControllerHandler.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef IMoveControllerHandler_h
#define IMoveControllerHandler_h

#include "HCommon.h"

class CMoveControllerCommands final
{
private:
    
    typedef std::function<void(ui32)> __MOVE_CONTROLLER_UPDATE_COMMAND;
    
    friend class IMoveControllerHandler;
    friend class CMoveController;
    
    __MOVE_CONTROLLER_UPDATE_COMMAND m_moveControllerUpdateCommand;
    
protected:
    
    CMoveControllerCommands(void);
    
    void _ConnectMoveControllerUpdateCommand(const __MOVE_CONTROLLER_UPDATE_COMMAND& _command);

    void _ExecuteMoveControllerUpdateCommand(ui32 _direction);
    
public:
    
    ~CMoveControllerCommands(void);
};

class IMoveControllerHandler
{
private:
    
    friend class CMoveController;
    
    CMoveControllerCommands m_commands;
    
    void _ConnectCommands(void);
    
    inline CMoveControllerCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    IMoveControllerHandler(void);
    
    virtual void _OnMoveControllerUpdate(ui32 _direction) = 0;
    
public:
    
    virtual ~IMoveControllerHandler(void);
};


#endif
