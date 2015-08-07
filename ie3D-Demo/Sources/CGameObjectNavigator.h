//
//  CGameObjectNavigator.h
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef CGameObjectNavigator_h
#define CGameObjectNavigator_h

#include "HCommon.h"
#include "HDEDeclaration.h"
#include "CCollisionMgr.h"
#include "HDEEnums.h"

class IGameObjectNavigatorHandler
{
private:
    
    friend class CGameObjectNavigator;
    
protected:
    
    virtual std::tuple<glm::vec2, glm::vec2> getBox2dBouningBox(void) = 0;
    virtual void onPositionChanged(const glm::vec3& position) = 0;
    virtual void onRotationChanged(const glm::vec3& rotation) = 0;
    
public:
    
    IGameObjectNavigatorHandler(void) = default;
    ~IGameObjectNavigatorHandler(void) = default;
};

class CGameObjectNavigator : public IBox2dCollider
{
private:
    
protected:
    
    f32 m_moveForwardSpeed;
    f32 m_moveBackwardSpeed;
    f32 m_strafeSpeed;
    f32 m_steerSpeed;
    
    CSharedLandscape m_landscape;
    
    glm::vec3 m_currentPosition;
    glm::vec3 m_currentRotation;
    
    glm::vec3 m_maxBound;
    glm::vec3 m_minBound;
    
    E_NAVIGATION_MOVE m_moveState;
    E_NAVIGATION_STEER m_steerState;
    
    std::set<ISharedGameObjectNavigatorHandler> m_handlers;
    
    void updateRotationOnHeightmapSurface(const glm::vec3& position);
    
    void notifyHandlersAboutPositionChanged(void);
    void notifyHandlersAboutRotationChanged(void);
    
    void onBox2dCollision(void);
    void onBox2dPositionChanged(const glm::vec3& position);
    void onBox2dRotationYChanged(f32 angle);
    
    glm::vec2 getBox2dCenter(void) const;
    std::tuple<glm::vec2, glm::vec2> getBox2dBoundingBox(void) const;
    
public:
    
    CGameObjectNavigator(f32 moveForwardSpeed,
                         f32 moveBackwardSpeed,
                         f32 strafeSpeed,
                         f32 steerSpeed,
                         CSharedLandscapeRef landscape,
                         const glm::vec3& maxBound,
                         const glm::vec3& minBound);
    
    void update(f32 deltatime);
    
    void setPosition(const glm::vec3& position);
    void setRotation(const glm::vec3& rotation);
    
    void moveForward(void);
    void moveBackward(void);
    void moveLeft(void);
    void moveRight(void);
    
    void steerLeft(void);
    void steerRight(void);
    
    void addNavigatorHandler(ISharedGameObjectNavigatorHandlerRef handler);
    void removeNavigatorHadler(ISharedGameObjectNavigatorHandlerRef handler);
};

#endif
