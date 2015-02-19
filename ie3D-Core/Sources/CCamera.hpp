//
//  CCamera.hpp
//  ie3D-Core
//
//  Created by Sergey Sergeev on 2/19/15.
//  Copyright (c) 2015 Sergey Sergeev. All rights reserved.
//

inline glm::mat4 CCamera::getVMatrix(void) const
{
    return m_matrixV;
}

inline glm::mat4 CCamera::getPMatrix(void) const
{
    return m_matrixP;
}

inline glm::mat4 CCamera::getNMatrix(void) const
{
    return m_matrixN;
}

inline glm::mat4 CCamera::getIVMatrix(void) const
{
    return m_matrixIV;
}

inline glm::mat4 CCamera::getVPMatrix(void) const
{
    return m_matrixVP;
}

inline glm::mat4 CCamera::getIVPMatrix(void) const
{
    return m_matrixIVP;
}

inline void CCamera::setPosition(const glm::vec3& position)
{
    m_position = position;
}

inline glm::vec3 CCamera::getPosition(void) const
{
    return m_position;
}

inline void CCamera::setLookAt(const glm::vec3& lookAt)
{
    m_lookAt = lookAt;
}

inline glm::vec3 CCamera::getLookAt(void) const
{
    return m_lookAt;
}

inline glm::vec3 CCamera::getUpVector(void) const
{
    return m_up;
}

inline void CCamera::setRotation(f32 rotation)
{
    m_rotation = rotation;
}

inline f32 CCamera::getRotation(void) const
{
    return m_rotation;
}

inline void CCamera::setDistanceToLookAt(const glm::vec3& distance)
{
    m_distance = distance;
}

inline glm::vec3 CCamera::getDistanceToLookAt(void) const
{
    return m_distance;
}

inline void CCamera::setFOV(f32 fov)
{
    m_fov = fov;
    m_matrixP = glm::perspective(m_fov, m_aspect, m_near, m_far);
}

inline f32 CCamera::getFOV(void) const
{
    return m_fov;
}

inline f32 CCamera::getAspect(void) const
{
    return m_aspect;
}

inline f32 CCamera::getNear(void) const
{
    return m_near;
}

inline f32 CCamera::getFar(void) const
{
    return m_far;
}

inline glm::ivec4 CCamera::getViewport(void) const
{
    return m_viewport;
}