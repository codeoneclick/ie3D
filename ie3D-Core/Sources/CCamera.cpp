//
//  CCamera.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCamera.h"
#include "CFrustum.h"

CCamera::CCamera(f32 fov, f32 near, f32 far, glm::ivec4 viewport) :
m_fov(fov),
m_near(near),
m_far(far),
m_rotation(0.0),
m_viewport(viewport)
{
    m_aspect = static_cast<f32>(viewport.z) / static_cast<f32>(viewport.w);
    m_matrixP = glm::perspective(m_fov, m_aspect, m_near, m_far);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

CCamera::~CCamera(void)
{
    
}

void CCamera::onSceneUpdate(f32 deltatime)
{
    m_position.y = m_distance.y;
    m_position.x = m_lookAt.x + cosf(-m_rotation) * -m_distance.x;
    m_position.z = m_lookAt.z + sinf(-m_rotation) * -m_distance.z;
    m_matrixV = glm::lookAt(m_position, m_lookAt, m_up);
    
    glm::vec3 position = m_position;
    position.y = -position.y;
    glm::vec3 lookAt = m_lookAt;
    lookAt.y = -lookAt.y;
    m_matrixIV = glm::lookAt(position, lookAt, m_up * -1.0f);
    
    m_matrixN = glm::inverse(m_matrixV);
    m_matrixN = glm::transpose(m_matrixN);
    
    m_matrixVP = m_matrixP * m_matrixV;
    m_matrixIVP = m_matrixP * m_matrixIV;
}

glm::mat4x4 CCamera::getCMatrix(const glm::vec3 &position)
{
    glm::vec3 direction = m_position - position;
    direction = glm::normalize(direction);
    
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::cross(direction, up);
    right = glm::normalize(right);
    direction = glm::cross(right, direction);
    
    glm::mat4x4 matrixC;
    matrixC[0][0] = right.x;
    matrixC[0][1] = right.y;
    matrixC[0][2] = right.z;
    matrixC[0][3] = 0.0f;
    matrixC[1][0] = up.x;
    matrixC[1][1] = up.y;
    matrixC[1][2] = up.z;
    matrixC[1][3] = 0.0f;
    matrixC[2][0] = direction.x;
    matrixC[2][1] = direction.y;
    matrixC[2][2] = direction.z;
    matrixC[2][3] = 0.0f;
    
    matrixC[3][0] = position.x;
    matrixC[3][1] = position.y;
    matrixC[3][2] = position.z;
    matrixC[3][3] = 1.0f;
    
    return matrixC;
}

glm::mat4x4 CCamera::getSMatrix(const glm::vec3 &position)
{
    glm::vec3 direction = position - m_position;
    direction = glm::normalize(direction);
    
    glm::vec3 up = glm::vec3(m_matrixV[1][0],
                             m_matrixV[1][1],
                             m_matrixV[1][2]);
    up = glm::normalize(up);
    
    glm::vec3 right = glm::cross(direction, up);
    right = glm::normalize(right);
    
    up = glm::cross(direction, right);
    up = glm::normalize(up);
    
    glm::mat4x4 matrixS;
    matrixS[0][0] = right.x;
    matrixS[0][1] = right.y;
    matrixS[0][2] = right.z;
    matrixS[0][3] = 0.0f;
    matrixS[1][0] = up.x;
    matrixS[1][1] = up.y;
    matrixS[1][2] = up.z;
    matrixS[1][3] = 0.0f;
    matrixS[2][0] = direction.x;
    matrixS[2][1] = direction.y;
    matrixS[2][2] = direction.z;
    matrixS[2][3] = 0.0f;
    
    matrixS[3][0] = position.x;
    matrixS[3][1] = position.y;
    matrixS[3][2] = position.z;
    matrixS[3][3] = 1.0f;
    
    return matrixS;
}
