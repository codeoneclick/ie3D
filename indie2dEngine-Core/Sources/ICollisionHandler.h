//
//  CCollisionHandler.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef ICollisionHandler_h
#define ICollisionHandler_h

#include "HCommon.h"

class IGameObject;

class CCollisionCommands final
{
private:
    
    typedef std::function<std::shared_ptr<IGameObject>(void)> __GET_TARGET_COMMAND;
    typedef std::function<void(const glm::vec3& _position)> __COLLISION_COMMAND;
    
    friend class ICollisionHandler;
    
    __GET_TARGET_COMMAND m_getTargetCommand;
    __COLLISION_COMMAND m_collisionCommand;
    
protected:
    
    CCollisionCommands(void);
    
    void _ConnectGetTargetCommand(const __GET_TARGET_COMMAND& _command);
    void _ConnectCollisionCommand(const __COLLISION_COMMAND& _command);
    
    std::shared_ptr<IGameObject> _ExecuteGetTargetCommand(void);
    void _ExecuteCollisionCommand(const glm::vec3& _position);
    
public:
    
    ~CCollisionCommands(void);
};

class ICollisionHandler
{
private:
    
    CCollisionCommands m_commands;
    
    void _ConnectCommands(void);
    
    inline CCollisionCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    ICollisionHandler(void);
    
    virtual std::shared_ptr<IGameObject> _OnGetTarget(void) = 0;
    virtual void _OnCollision(const glm::vec3& _position) = 0;
    
public:
    
    virtual ~ICollisionHandler(void);
};

#endif 
