//
//  ISceneUpdateHandler.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/23/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ISceneUpdateHandler_h
#define ISceneUpdateHandler_h

#include "HCommon.h"

class CSceneUpdateCommands final
{
private:
    
    typedef std::function<void(f32)> __SCENE_UPDATE_COMMAND;
    
    friend class ISceneUpdateHandler;
    friend class CSceneUpdateMgr;
    
    __SCENE_UPDATE_COMMAND m_sceneUpdateCommand;
    
protected:
    
    CSceneUpdateCommands(void);
    
    void _ConnectSceneUpdateCommand(const __SCENE_UPDATE_COMMAND& _command);
    void _ExecuteSceneUpdateCommand(f32 _deltatime);
    
public:
    
    ~CSceneUpdateCommands(void);
};

class ISceneUpdateHandler
{
private:
    
    friend class CSceneUpdateMgr;
    
    CSceneUpdateCommands m_commands;
    
    void _ConnectCommands(void);
    inline CSceneUpdateCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    ISceneUpdateHandler(void);
    
    virtual void _OnSceneUpdate(f32 _deltatime) = 0;
    
public:
    
    virtual ~ISceneUpdateHandler(void);
};

#endif 
