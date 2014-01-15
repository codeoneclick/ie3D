//
//  CCollisionMgr.cpp
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#include "CCollisionMgr.h"
#include "ICollisionHandler.h"
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

void CCollisionMgr::RegisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler)
{
    m_handlers.insert(_handler);
}

void CCollisionMgr::UnregisterCollisionHandler(std::shared_ptr<ICollisionHandler> _handler)
{
    m_handlers.erase(_handler);
}

void CCollisionMgr::_OnInputTapRecognizerPressed(const glm::ivec2& _point)
{
    assert(m_camera != nullptr);
    glm::vec3 origin, direction;
    CCollisionMgr::Unproject(_point, m_camera->Get_ViewMatrix(), m_camera->Get_ProjectionMatrix(), m_camera->Get_Viewport(), &origin, &direction);
    
    for(const auto& handler : m_handlers)
    {
        std::vector<std::shared_ptr<IGameObject> > colliders = handler->_Get_Commands()._ExecuteGetCollidersCommand();
        for(const auto& collider : colliders)
        {
            glm::vec3 point;
            if(CCollisionMgr::_CollisionPoint(collider->Get_BoundVertexBuffer(), collider->Get_BoundIndexBuffer(), glm::mat4x4(1.0f), origin, direction, &point))
            {
                handler->_Get_Commands()._ExecuteCollisionCommand(point, collider);
            }
        }
    }
}

void CCollisionMgr::_OnInputTapRecognizerMoved(const glm::ivec2& _point)
{
    
}

void CCollisionMgr::_OnInputTapRecognizerReleased(const glm::ivec2& _point)
{
    
}

void CCollisionMgr::Unproject(const glm::ivec2& _point, const glm::mat4x4& _viewMatrix, const glm::mat4x4& _projectionMatrix, glm::ivec4 _viewport,glm::vec3* _origin, glm::vec3* _direction)
{
    glm::vec3 direction, origin;
    float screenX =  -((( 2.0f * _point.x ) / _viewport[2]) - 1.0f ) / _projectionMatrix[0][0];
    float screenY =  ((( 2.0f * _point.y ) / _viewport[3]) - 1.0f ) / _projectionMatrix[1][1];
    glm::mat4x4 viewInverseMatrix = glm::inverse(_viewMatrix);
    
    direction.x  = (screenX * viewInverseMatrix[0][0] + screenY * viewInverseMatrix[1][0] + viewInverseMatrix[2][0]);
    direction.y  = (screenX * viewInverseMatrix[0][1] + screenY * viewInverseMatrix[1][1] + viewInverseMatrix[2][1]);
    direction.z  = (screenX * viewInverseMatrix[0][2] + screenY * viewInverseMatrix[1][2] + viewInverseMatrix[2][2]);
    
    origin.x = viewInverseMatrix[3][0];
    origin.y = viewInverseMatrix[3][1];
    origin.z = viewInverseMatrix[3][2];
    
    (*_direction) = direction;
    (*_origin) = origin;
}


bool CCollisionMgr::_TriangleIntersection(const glm::vec3 &_trianglePoint_01, glm::vec3 &_trianglePoint_02, glm::vec3 &_trianglePoint_03, const glm::vec3 &_origin, const glm::vec3 &_direction, glm::vec3 *_intersectPoint)
{
    glm::vec3 edge_01 = _trianglePoint_02 - _trianglePoint_01;
	glm::vec3 edge_02 = _trianglePoint_03 - _trianglePoint_01;
    
    glm::vec3 pVector = glm::cross(_direction, edge_02);
	f32 determinant = glm::dot(edge_01, pVector);
	if(fabs(determinant) < 0.0001f)
    {
        return false;
    }
    
    f32 invDeterminant = 1.0f / determinant;
	glm::vec3 tVector = _origin - _trianglePoint_01;
    
	f32 u = glm::dot(tVector, pVector) * invDeterminant;
    if ( u < -0.0001f || u > 1.0001f )
    {
        return false;
    }
    
    glm::vec3 qVector = glm::cross(tVector, edge_01);
	f32 v = glm::dot(_direction, qVector) * invDeterminant;
	if ( v < -0.0001f || (v + u) > 1.0001f )
    {
        return false;
    }
    
	(*_intersectPoint) = _trianglePoint_01 + (edge_01 * u) + (edge_02 * v);
	return true;
}

bool CCollisionMgr::_CollisionPoint(std::shared_ptr<CVertexBuffer> _vertexBuffer, std::shared_ptr<CIndexBuffer> _indexBuffer, const glm::mat4x4& _worldMatrix, const glm::vec3& _origin, const glm::vec3& _direction, glm::vec3* _point)
{
    SHardwareVertex* vertexData = _vertexBuffer->Lock();
    ui16* indexData = _indexBuffer->Lock();
    ui32 numIndices = _indexBuffer->Get_Size();

    for(ui32 index = 0; index < numIndices; index += 3)
    {
        glm::vec4 value = _worldMatrix * glm::vec4(vertexData[indexData[index + 0]].m_position, 1.0f);
        glm::vec3 point_01 = glm::vec3(value.x, value.y, value.z);
        value = _worldMatrix * glm::vec4(vertexData[indexData[index + 1]].m_position, 1.0f);
        glm::vec3 point_02 = glm::vec3(value.x, value.y, value.z);
        value = _worldMatrix * glm::vec4(vertexData[indexData[index + 2]].m_position, 1.0f);
        glm::vec3 point_03 = glm::vec3(value.x, value.y, value.z);
        
        if(CCollisionMgr::_TriangleIntersection(point_01, point_02, point_03, _origin, _direction, _point))
        {
            return true;
        }
    }
    return false;
}
