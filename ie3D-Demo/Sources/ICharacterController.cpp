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
#include "CLandscape.h"

ICharacterController::ICharacterController(ISharedGameObjectRef gameObject,
                                           CSharedLandscapeRef landscape,
                                           CSharedCameraRef camera) :
IGameObjectNavigatorHandler(),
m_gameObject(gameObject),
m_landscape(landscape),
m_camera(camera),
m_speed(0.0)
{
    assert(m_gameObject != nullptr);
    m_position = m_gameObject->getPosition();
    m_rotation = m_gameObject->getRotation();
    m_cameraPrecomputedDistance = m_camera->getDistanceToLookAt();
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
    
    if(m_camera)
    {
        f32 fov = glm::mix(45.0, 55.0, m_speed);
        m_camera->setFOV(fov);
        
        m_gameObject->setPosition(m_position);
        m_camera->setLookAt(glm::vec3(m_position.x, m_position.y + glm::mix(16.0, 8.0, m_speed), m_position.z));
        
        f32 currentCameraRotation = glm::mix(glm::degrees(m_camera->getRotation()) - 90.0f, m_gameObject->getRotation().y, 0.1);
        glm::vec3 currentGameObjectRotation = glm::mix(glm::vec3(m_gameObject->getRotation().x, m_rotation.y, m_gameObject->getRotation().z),
                                                       m_rotation, 0.25);
        m_gameObject->setRotation(currentGameObjectRotation);
        m_camera->setRotation(glm::radians(currentCameraRotation));
        
        glm::vec3 cameraPosition = m_camera->getPosition();
        if(cameraPosition.x < m_landscape->getHeightmapSize().x &&
           cameraPosition.z < m_landscape->getHeightmapSize().y &&
           cameraPosition.x >= 0.0 &&
           cameraPosition.z >= 0.0)
        {
            m_cameraPrecomputedDistance.y = m_landscape->getHeight(cameraPosition) + 48.0f;
        }
        
        glm::vec3 currentCameraDistanceToLookAt = glm::mix(m_camera->getDistanceToLookAt(), m_cameraPrecomputedDistance,
                                                           5.0f * deltatime);
        currentCameraDistanceToLookAt.y = glm::clamp(currentCameraDistanceToLookAt.y, 0.0f, m_cameraPrecomputedDistance.y);
        m_camera->setDistanceToLookAt(currentCameraDistanceToLookAt);
    }
    else
    {
        m_gameObject->setPosition(m_position);
        glm::vec3 currentGameObjectRotation = glm::mix(glm::vec3(m_gameObject->getRotation().x, m_rotation.y, m_gameObject->getRotation().z),
                                                       m_rotation, 0.25);
        m_gameObject->setRotation(currentGameObjectRotation);
    }
}