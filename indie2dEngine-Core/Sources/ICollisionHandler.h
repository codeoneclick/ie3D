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
    
    typedef std::function<std::vector<std::shared_ptr<IGameObject> >(void)> __GET_TARGETS_COMMAND;
    typedef std::function<void(const glm::vec3& _position, std::shared_ptr<IGameObject>)> __COLLISION_COMMAND;
    
    friend class ICollisionHandler;
    friend class CCollisionMgr;
    
    __GET_TARGETS_COMMAND m_getTargetsCommand;
    __COLLISION_COMMAND m_collisionCommand;
    
protected:
    
    CCollisionCommands(void);
    
    void _ConnectGetTargetsCommand(const __GET_TARGETS_COMMAND& _command);
    void _ConnectCollisionCommand(const __COLLISION_COMMAND& _command);
    
    std::vector<std::shared_ptr<IGameObject> > _ExecuteGetTargetsCommand(void);
    void _ExecuteCollisionCommand(const glm::vec3& _position, std::shared_ptr<IGameObject> _target);
    
public:
    
    ~CCollisionCommands(void);
};

class ICollisionHandler
{
private:
    
    friend class CCollisionMgr;
    
    CCollisionCommands m_commands;
    
    void _ConnectCommands(void);
    
    inline CCollisionCommands _Get_Commands(void)
    {
        return m_commands;
    };
    
protected:
    
    ICollisionHandler(void);
    
    virtual std::vector<std::shared_ptr<IGameObject> > _OnGetTargets(void) = 0;
    virtual void _OnCollision(const glm::vec3& _position, std::shared_ptr<IGameObject> _target) = 0;
    
public:
    
    virtual ~ICollisionHandler(void);
};

#endif 
