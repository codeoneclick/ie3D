//
//  ICharacterController.h
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#ifndef ICharacterController_h
#define ICharacterController_h

#include "CGameObjectNavigator.h"
#include "HDEDeclaration.h"

class ICharacterController : public IGameObjectNavigatorHandler
{
private:
    
protected:
    
    std::tuple<glm::vec2, glm::vec2> getBox2dBouningBox(void);
    void onPositionChanged(const glm::vec3& position);
    void onRotationChanged(const glm::vec3& rotation);
    
    ISharedGameObject m_gameObject;
    CSharedLandscape m_landscape;
    CSharedCamera m_camera;
    
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_cameraPrecomputedDistance;
    
    f32 m_speed;
    
public:
    
    ICharacterController(ISharedGameObjectRef gameObject,
                         CSharedLandscapeRef landscape,
                         CSharedCameraRef camera = nullptr);
    ~ICharacterController(void);
    
    void increaseSpeed(void);
    void decreaseSpeed(void);
    
    void update(f32 deltatime);
};

#endif
