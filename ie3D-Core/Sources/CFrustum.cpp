//
//  CFrustum.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 5/24/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CFrustum.h"
#include "CCamera.h"

CFrustum::CFrustum(CSharedCameraRef camera) :
m_camera(camera)
{

}

CFrustum::~CFrustum(void)
{

}

glm::vec4 CFrustum::createPlane(const glm::vec3 &point_01, const glm::vec3 &point_02, const glm::vec3 &point_03)
{
    glm::vec3 edge_01, edge_02;
	edge_01 = point_01 - point_02;
	edge_02 = point_03 - point_02;
    glm::vec3 normal = glm::normalize(glm::cross(edge_01, edge_02));
	f32 offset = -glm::dot(normal, point_02);
    return glm::vec4(normal, offset);
}

inline f32 CFrustum::getDistanceToPlane(const glm::vec4& plane, const glm::vec3& point)
{
    static glm::vec3 abc = glm::vec3(0.0);
    abc = CFrustum::getPlaneABC(plane);
    static f32 d = 0.0;
    d = CFrustum::getPlaneD(plane);
    return (d + glm::dot(abc, point)) * -1.0;
}

inline glm::vec3 CFrustum::getPlaneABC(const glm::vec4& plane)
{
    return glm::vec3(plane.x, plane.y, plane.z);
}

inline f32 CFrustum::getPlaneD(const glm::vec4& plane)
{
    return plane.w;
}

void CFrustum::onSceneUpdate(f32 deltatime)
{
    if(m_camera != nullptr)
    {
        f32 tan = tanf(glm::radians(m_camera->getFOV()));
        f32 nearHeight = m_camera->getNear() * tan;
        f32 nearWidth = nearHeight * m_camera->getAspect();
        f32 farHeight = m_camera->getFar()  * tan;
        f32 farWidth = farHeight * m_camera->getAspect();
        
        glm::vec3 basis_Z = glm::normalize(m_camera->getPosition() - m_camera->getLookAt());
        glm::vec3 basis_X = glm::normalize(glm::cross(m_camera->getUpVector(), basis_Z));
        glm::vec3 basis_Y = glm::cross(basis_Z, basis_X);
        
        glm::vec3 nearOffset = m_camera->getPosition() - basis_Z * m_camera->getNear();
        glm::vec3 farOffset = m_camera->getPosition() - basis_Z * m_camera->getFar();
        
        glm::vec3 nearTopLeftPoint = nearOffset + basis_Y * nearHeight - basis_X * nearWidth;
        glm::vec3 nearTopRightPoint = nearOffset + basis_Y * nearHeight + basis_X * nearWidth;
        glm::vec3 nearBottomLeftPoint = nearOffset - basis_Y * nearHeight - basis_X * nearWidth;
        glm::vec3 nearBottomRightPoint = nearOffset - basis_Y * nearHeight + basis_X * nearWidth;
        
        glm::vec3 farTopLeftPoint = farOffset + basis_Y * farHeight - basis_X * farWidth;
        glm::vec3 farTopRightPoint = farOffset + basis_Y * farHeight + basis_X * farWidth;
        glm::vec3 farBottomLeftPoint = farOffset - basis_Y * farHeight - basis_X * farWidth;
        glm::vec3 farBottomRightPoint = farOffset - basis_Y * farHeight + basis_X * farWidth;
        
        m_planes[E_FRUSTUM_PLANE_TOP] = CFrustum::createPlane(nearTopRightPoint, nearTopLeftPoint, farTopLeftPoint);
        m_planes[E_FRUSTUM_PLANE_BOTTOM] = CFrustum::createPlane(nearBottomLeftPoint, nearBottomRightPoint, farBottomRightPoint);
        m_planes[E_FRUSTUM_PLANE_LEFT] = CFrustum::createPlane(nearTopLeftPoint, nearBottomLeftPoint, farBottomLeftPoint);
        m_planes[E_FRUSTUM_PLANE_RIGHT] = CFrustum::createPlane(nearBottomRightPoint, nearTopRightPoint, farBottomRightPoint);
        m_planes[E_FRUSTUM_PLANE_NEAR] = CFrustum::createPlane(nearTopLeftPoint, nearTopRightPoint, nearBottomRightPoint);
        m_planes[E_FRUSTUM_PLANE_FAR] = CFrustum::createPlane(farTopRightPoint, farTopLeftPoint, farBottomLeftPoint);
    }
}

E_FRUSTUM_BOUND_RESULT CFrustum::isPointInFrustum(const glm::vec3& _point)
{
    for(ui32 i = 0; i < E_FRUSTUM_PLANE_MAX; ++i)
    {
        if(CFrustum::getDistanceToPlane(m_planes[i], _point) < 0.0f)
        {
            return E_FRUSTUM_BOUND_RESULT_OUTSIDE;
        }
    }
    return E_FRUSTUM_BOUND_RESULT_INSIDE;
}

E_FRUSTUM_BOUND_RESULT CFrustum::isSphereInFrumstum(const glm::vec3& _center, f32 _radius)
{
    E_FRUSTUM_BOUND_RESULT result = E_FRUSTUM_BOUND_RESULT_INSIDE;
    for(ui32 i = 0; i < E_FRUSTUM_PLANE_MAX; ++i)
    {
        f32 distance = CFrustum::getDistanceToPlane(m_planes[i], _center);
        if (distance < -_radius)
        {
            return E_FRUSTUM_BOUND_RESULT_OUTSIDE;
        }
        else if (distance < _radius)
        {
            result =  E_FRUSTUM_BOUND_RESULT_INTERSECT;
        }
    }
    return result;
}

#define kMaxPointsInBoundingBox 8

E_FRUSTUM_BOUND_RESULT CFrustum::isBoundBoxInFrustum(const glm::vec3& maxBound,
                                                     const glm::vec3& minBound)
{
    glm::vec3 points[kMaxPointsInBoundingBox];
    points[0] = minBound;
    points[1] = glm::vec3(minBound.x, minBound.y, maxBound.z);
    points[2] = glm::vec3(maxBound.x, minBound.y, minBound.z);
    points[3] = glm::vec3(maxBound.x, minBound.y, maxBound.z);
    
    points[4] = maxBound;
    points[5] = glm::vec3(maxBound.x, maxBound.y, minBound.z);
    points[6] = glm::vec3(minBound.x, maxBound.y, maxBound.z);
    points[7] = glm::vec3(minBound.x, maxBound.y, minBound.z);
    
    E_FRUSTUM_BOUND_RESULT result = E_FRUSTUM_BOUND_RESULT_INSIDE;
    ui32 pointsIn, pointsOut;
    
    for(ui32 i = 0; i < E_FRUSTUM_PLANE_MAX; ++i)
    {
        pointsIn = 0;
        pointsOut = 0;
        for (ui32 j = 0; j < kMaxPointsInBoundingBox && (pointsIn == 0 || pointsOut == 0); ++j)
        {
            CFrustum::getDistanceToPlane(m_planes[i], points[j]) < 0.0 ? ++pointsOut : ++pointsIn;
        }
        if (!pointsIn)
        {
            result = E_FRUSTUM_BOUND_RESULT_OUTSIDE;
            break;
        }
        else if (pointsOut)
        {
            result = E_FRUSTUM_BOUND_RESULT_INTERSECT;
        }
    }
    return result;
}