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
m_camera(camera),
m_speed(0.0)
{
    assert(m_gameObject != nullptr);
    m_position = m_gameObject->getPosition();
    m_rotation = m_gameObject->getRotation();
}

ICharacterController::~ICharacterController(void)
{
    
}

void ICharacterController::onPositionChanged(const glm::vec3& position)
{
    m_position = position;
}

void ICharacterController::onRotationChanged(const glm::vec3& rotation)
{
    m_rotation = rotation;
}

void ICharacterController::increaseSpeed(void)
{
    if(m_speed < 1.0)
    {
        m_speed += 0.025;
    }
}

void ICharacterController::decreaseSpeed(void)
{
    if(m_speed > 0.0)
    {
        m_speed -= 0.05;
    }
}

void ICharacterController::update(f32 deltatime)
{
    assert(m_gameObject != nullptr);
    
    f32 fov = glm::mix(60.0, 75.0, m_speed);
    m_camera->setFov(fov);
    
    m_gameObject->setPosition(m_position);
    m_camera->Set_LookAt(glm::vec3(m_position.x, m_position.y + glm::mix(6.0, 3.0, m_speed), m_position.z));
    
    f32 currentCameraRotation = glm::mix(glm::degrees(m_camera->Get_Rotation()) - 90.0f, m_gameObject->getRotation().y, 0.1);
    glm::vec3 currentGameObjectRotation = glm::mix(glm::vec3(m_gameObject->getRotation().x, m_rotation.y, m_gameObject->getRotation().z),
                                                   m_rotation, 0.25);
    m_gameObject->setRotation(currentGameObjectRotation);
    m_camera->Set_Rotation(glm::radians(currentCameraRotation));
}