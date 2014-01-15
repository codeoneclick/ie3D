//
//  CCamera.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCamera.h"
#include "CFrustum.h"

CCamera::CCamera(f32 _fov, f32 _near, f32 _far, glm::ivec4 _viewport) :
m_fov(_fov),
m_near(_near),
m_far(_far),
m_viewport(_viewport)
{
    m_aspect = static_cast<f32>(_viewport.z) / static_cast<f32>(_viewport.w);
    m_projection = glm::perspective(m_fov, m_aspect, m_near, m_far);
    m_altitude = 0.0f;
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_frustum =  std::make_shared<CFrustum>(this);
}

CCamera::~CCamera(void)
{
    
}

void CCamera::_OnSceneUpdate(f32 _deltatime)
{
    m_position.y = m_look.y + m_height;
    m_position.x = m_look.x + cosf(-m_rotation) * -m_distance;
    m_position.z = m_look.z + sinf(-m_rotation) * -m_distance;
    m_view = glm::lookAt(m_position, m_look, m_up);
    
    glm::vec3 position = m_position;
    position.y = -position.y + m_altitude * 2.0f;
    glm::vec3 look = m_look;
    look.y = -look.y + m_altitude * 2.0f;
    m_reflection = glm::lookAt(position, look, m_up * -1.0f);
    
    m_matrixNormal = glm::inverse(m_view);
    m_matrixNormal = glm::transpose(m_matrixNormal);
    
    assert(m_frustum != nullptr);
    m_frustum->Update();
}

glm::mat4x4 CCamera::Get_CylindricalMatrixForPosition(const glm::vec3 &_position)
{
    glm::vec3 direction = m_position - _position;
    direction = glm::normalize(direction);
    
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::cross(direction, up);
    right = glm::normalize(right);
    direction = glm::cross(right, direction);
    
    glm::mat4x4 cylindricalMatrix;
    cylindricalMatrix[0][0] = right.x;
    cylindricalMatrix[0][1] = right.y;
    cylindricalMatrix[0][2] = right.z;
    cylindricalMatrix[0][3] = 0.0f;
    cylindricalMatrix[1][0] = up.x;
    cylindricalMatrix[1][1] = up.y;
    cylindricalMatrix[1][2] = up.z;
    cylindricalMatrix[1][3] = 0.0f;
    cylindricalMatrix[2][0] = direction.x;
    cylindricalMatrix[2][1] = direction.y;
    cylindricalMatrix[2][2] = direction.z;
    cylindricalMatrix[2][3] = 0.0f;
    
    cylindricalMatrix[3][0] = _position.x;
    cylindricalMatrix[3][1] = _position.y;
    cylindricalMatrix[3][2] = _position.z;
    cylindricalMatrix[3][3] = 1.0f;
    
    return cylindricalMatrix;
}

glm::mat4x4 CCamera::Get_SphericalMatrixForPosition(const glm::vec3 &_position)
{
    glm::vec3 direction = _position - m_position;
    direction = glm::normalize(direction);
    
    glm::vec3 up = glm::vec3(m_view[1][0], m_view[1][1], m_view[1][2]);
    up = glm::normalize(up);
    
    glm::vec3 right = glm::cross(direction, up);
    right = glm::normalize(right);
    
    up = glm::cross(direction, right);
    up = glm::normalize(up);
    
    glm::mat4x4 sphericalMatrix;
    sphericalMatrix[0][0] = right.x;
    sphericalMatrix[0][1] = right.y;
    sphericalMatrix[0][2] = right.z;
    sphericalMatrix[0][3] = 0.0f;
    sphericalMatrix[1][0] = up.x;
    sphericalMatrix[1][1] = up.y;
    sphericalMatrix[1][2] = up.z;
    sphericalMatrix[1][3] = 0.0f;
    sphericalMatrix[2][0] = direction.x;
    sphericalMatrix[2][1] = direction.y;
    sphericalMatrix[2][2] = direction.z;
    sphericalMatrix[2][3] = 0.0f;
    
    sphericalMatrix[3][0] = _position.x;
    sphericalMatrix[3][1] = _position.y;
    sphericalMatrix[3][2] = _position.z;
    sphericalMatrix[3][3] = 1.0f;
    
    return sphericalMatrix;
}

