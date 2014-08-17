//
//  ICharacterController.cpp
//  ie3D-Demo
//
//  Created by sergey.sergeev on 8/12/14.
//  Copyright (c) 2014 Sergey Sergeev. All rights reserved.
//

#include "ICharacterController.h"
#include "IGameObject.h"
#include "CCamera.h"

ICharacterController::ICharacterController(ISharedGameObjectRef gameObject,
                                           CSharedCameraRef camera) :
IGameObjectNavigatorHandler(),
m_gameObject(gameObject),
m_camera(camera)
{
    
}

ICharacterController::~ICharacterController(void)
{
    
}

void ICharacterController::onPositionChanged(const glm::vec3& position)
{
    assert(m_gameObject != nullptr);
    m_gameObject->setPosition(position);
    m_camera->Set_LookAt(glm::vec3(position.x, position.y + 12.0, position.z));
}

void ICharacterController::onRotationChanged(const glm::vec3& rotation)
{
    assert(m_gameObject != nullptr);
    m_gameObject->setRotation(glm::vec3(rotation.x,
                                        rotation.y,
                                        rotation.z));
    m_camera->Set_Rotation(glm::radians(rotation.y - 90.0));
}