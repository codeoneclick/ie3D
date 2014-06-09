//
//  CCollisionMgr.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCollisionMgr.h"
#include "CCamera.h"
#include "IGameObject.h"
#include "CMesh.h"
#include "CVertexBuffer.h"
#include "CIndexBuffer.h"

CCollisionMgr::CCollisionMgr(void) :
m_camera(nullptr)
{
    
}

CCollisionMgr::~CCollisionMgr(void)
{
    
}

void CCollisionMgr::setCamera(CSharedCameraRef camera)
{
    assert(camera != nullptr);
    m_camera = camera;
}

void CCollisionMgr::addCollisionHandler(ISharedCollisionHandlerRef handler)
{
    m_handlers.insert(handler);
}

void CCollisionMgr::removeCollisionHandler(ISharedCollisionHandlerRef handler)
{
    m_handlers.erase(handler);
}

void CCollisionMgr::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    assert(m_camera != nullptr);
    glm::vec3 origin, direction;
    CCollisionMgr::unproject(point,
                             m_camera->Get_ViewMatrix(),
                             m_camera->Get_ProjectionMatrix(),
                             m_camera->Get_Viewport(),
                             &origin,
                             &direction);
    
    for(const auto& handler : m_handlers)
    {
        std::vector<ISharedGameObject> colliders = handler->colliders();
        for(const auto& collider : colliders)
        {
            glm::vec3 point;
            if(CCollisionMgr::collisionPoint(collider->getCollisionVertexBuffer(),
                                             collider->getCollisionIndexBuffer(),
                                             glm::mat4x4(1.0f),
                                             origin,
                                             direction,
                                             &point))
            {
                handler->onCollision(point, collider, inputButton);
            }
        }
    }
}

void CCollisionMgr::onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    
}

void CCollisionMgr::onGestureRecognizerMoved(const glm::ivec2& point)
{
    
}

void CCollisionMgr::onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    
}

void CCollisionMgr::unproject(const glm::ivec2& point,
                              const glm::mat4x4& viewMatrix,
                              const glm::mat4x4& projectionMatrix,
                              const glm::ivec4& viewport,
                              glm::vec3* _origin, glm::vec3* _direction)
{
    glm::vec3 direction, origin;
    float screenX =  -((( 2.0f * point.x ) / viewport[2]) - 1.0f ) / projectionMatrix[0][0];
    float screenY =  ((( 2.0f * point.y ) / viewport[3]) - 1.0f ) / projectionMatrix[1][1];
    glm::mat4x4 viewInverseMatrix = glm::inverse(viewMatrix);
    
    direction.x  = (screenX * viewInverseMatrix[0][0] + screenY * viewInverseMatrix[1][0] + viewInverseMatrix[2][0]);
    direction.y  = (screenX * viewInverseMatrix[0][1] + screenY * viewInverseMatrix[1][1] + viewInverseMatrix[2][1]);
    direction.z  = (screenX * viewInverseMatrix[0][2] + screenY * viewInverseMatrix[1][2] + viewInverseMatrix[2][2]);
    
    origin.x = viewInverseMatrix[3][0];
    origin.y = viewInverseMatrix[3][1];
    origin.z = viewInverseMatrix[3][2];
    
    (*_direction) = direction;
    (*_origin) = origin;
}

bool CCollisionMgr::isIntersected(CSharedCameraRef camera,
                                  const glm::ivec2& point,
                                  glm::vec3* intersectPoint)
{
    glm::vec3 origin, direction;
    CCollisionMgr::unproject(point,
                             camera->Get_ViewMatrix(),
                             camera->Get_ProjectionMatrix(),
                             camera->Get_Viewport(),
                             &origin,
                             &direction);
    if(CCollisionMgr::triangleIntersection(glm::vec3(-4096.0, 0.0, -4096.0),
                                           glm::vec3( 4096.0, 0.0, -4096.0),
                                           glm::vec3(-4096.0, 0.0,  4096.0),
                                           origin,
                                           direction, intersectPoint))
    {
        return true;
    }
    return CCollisionMgr::triangleIntersection(glm::vec3( 4096.0, 0.0,  4096.0),
                                               glm::vec3( 4096.0, 0.0, -4096.0),
                                               glm::vec3(-4096.0, 0.0,  4096.0),
                                               origin,
                                               direction, intersectPoint);
}

bool CCollisionMgr::triangleIntersection(const glm::vec3& trianglePoint_01,
                                         const glm::vec3& trianglePoint_02,
                                         const glm::vec3& trianglePoint_03,
                                         const glm::vec3 &origin,
                                         const glm::vec3 &direction,
                                         glm::vec3 *intersectPoint)
{
    glm::vec3 edge_01 = trianglePoint_02 - trianglePoint_01;
	glm::vec3 edge_02 = trianglePoint_03 - trianglePoint_01;
    
    glm::vec3 pVector = glm::cross(direction, edge_02);
	f32 determinant = glm::dot(edge_01, pVector);
	if(fabs(determinant) < 0.0001f)
    {
        return false;
    }
    
    f32 invDeterminant = 1.0f / determinant;
	glm::vec3 tVector = origin - trianglePoint_01;
    
	f32 u = glm::dot(tVector, pVector) * invDeterminant;
    if ( u < -0.0001f || u > 1.0001f )
    {
        return false;
    }
    
    glm::vec3 qVector = glm::cross(tVector, edge_01);
	f32 v = glm::dot(direction, qVector) * invDeterminant;
	if ( v < -0.0001f || (v + u) > 1.0001f )
    {
        return false;
    }
    
	(*intersectPoint) = trianglePoint_01 + (edge_01 * u) + (edge_02 * v);
	return true;
}

bool CCollisionMgr::collisionPoint(CSharedVertexBufferRef vertexBuffer,
                                   CSharedIndexBufferRef indexBuffer,
                                   const glm::mat4x4& worldMatrix,
                                   const glm::vec3& origin,
                                   const glm::vec3& direction,
                                   glm::vec3* point)
{
    SAttributeVertex* vertexData = vertexBuffer->lock();
    ui16* indexData = indexBuffer->lock();
    ui32 numIndices = indexBuffer->getSize();

    for(ui32 index = 0; index < numIndices; index += 3)
    {
        glm::vec4 value = worldMatrix * glm::vec4(vertexData[indexData[index + 0]].m_position, 1.0f);
        glm::vec3 point_01 = glm::vec3(value.x, value.y, value.z);
        value = worldMatrix * glm::vec4(vertexData[indexData[index + 1]].m_position, 1.0f);
        glm::vec3 point_02 = glm::vec3(value.x, value.y, value.z);
        value = worldMatrix * glm::vec4(vertexData[indexData[index + 2]].m_position, 1.0f);
        glm::vec3 point_03 = glm::vec3(value.x, value.y, value.z);
        
        if(CCollisionMgr::triangleIntersection(point_01, point_02, point_03, origin, direction, point))
        {
            return true;
        }
    }
    return false;
}
