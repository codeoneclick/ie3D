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

std::vector<ISharedGameObject> ITouchCollider::getColliders(void) const
{
    return std::vector<ISharedGameObject>();
}

void ITouchCollider::onTouchCollision(const glm::vec3& position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton)
{
    
}

IBox2dCollider::IBox2dCollider(void) :
m_box2dBody(nullptr)
{
    
}

void IBox2dCollider::init(std::shared_ptr<b2World> box2dScene)
{
    m_box2dBody = box2dScene->CreateBody(&m_box2dBodyDefinition);
}

b2Body* IBox2dCollider::getBox2dBody(void) const
{
    return m_box2dBody;
}

b2BodyDef* IBox2dCollider::getBox2dBodyDefinition(void)
{
    return &m_box2dBodyDefinition;
}

void IBox2dCollider::onBox2dCollision(void)
{
    
}

void IBox2dCollider::onBox2dPositionChanged(const glm::vec3& position)
{
    
}

void IBox2dCollider::onBox2dRotationYChanged(f32 angle)
{
    
}

glm::vec2 IBox2dCollider::getBox2dCenter(void) const
{
    return glm::vec2(0.0f);
}

glm::vec2 IBox2dCollider::getBox2dMaxBound(void) const
{
    return glm::vec2(0.0f);
}

glm::vec2 IBox2dCollider::getBox2dMinBound(void) const
{
    return glm::vec2(0.0f);
}

CCollisionMgr::CCollisionMgr(void) :
m_camera(nullptr),
m_box2dScene(nullptr)
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

void CCollisionMgr::CCollisionMgr::addTouchCollider(const std::shared_ptr<ITouchCollider>& collider)
{
    assert(collider);
    m_touchColliders.insert(collider);
}

void CCollisionMgr::removeTouchCollider(const std::shared_ptr<ITouchCollider>& collider)
{
    assert(collider);
    m_touchColliders.erase(collider);
}

void CCollisionMgr::setBox2dScene(const glm::vec2 &minBound, const glm::vec2 &maxBound)
{
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    m_box2dScene = std::make_shared<b2World>(gravity);
    m_box2dScene->SetContinuousPhysics(true);
    m_box2dScene->SetContactListener(this);
    
    b2BodyDef boundBodyDefinition;
    boundBodyDefinition.position.Set(0, 0);
    b2Body* boundBody = m_box2dScene->CreateBody(&boundBodyDefinition);
    
    b2EdgeShape boundingBox;
    boundingBox.Set(b2Vec2(minBound.x, minBound.y), b2Vec2(maxBound.x, minBound.y));
    boundBody->CreateFixture(&boundingBox, 0);
    boundingBox.Set(b2Vec2(minBound.x, maxBound.y), b2Vec2(maxBound.x, maxBound.y));
    boundBody->CreateFixture(&boundingBox, 0);
    boundingBox.Set(b2Vec2(minBound.x, maxBound.y), b2Vec2(minBound.x, minBound.y));
    boundBody->CreateFixture(&boundingBox, 0);
    boundingBox.Set(b2Vec2(maxBound.x, maxBound.y), b2Vec2(maxBound.x, minBound.y));
    boundBody->CreateFixture(&boundingBox, 0);
}

void CCollisionMgr::addBox2dCollider(ISharedBox2dColliderRef collider, bool isStatic)
{
    assert(collider);
    assert(m_box2dScene);
    
    m_box2dColliders.insert(collider);
    
    collider->getBox2dBodyDefinition()->type = isStatic == true ? b2_staticBody : b2_dynamicBody;
    collider->getBox2dBodyDefinition()->position.Set(collider->getBox2dCenter().x, collider->getBox2dCenter().y);
    collider->getBox2dBodyDefinition()->userData = collider.get();
    collider->init(m_box2dScene);
    b2PolygonShape box2dShape;
    
    glm::vec2 minBound = collider->getBox2dMinBound();
    glm::vec2 maxBound = collider->getBox2dMaxBound();
    
    box2dShape.SetAsBox((maxBound.x - minBound.x) / 2.0f,
                        (maxBound.y - minBound.y) / 2.0f);
    collider->getBox2dBody()->CreateFixture(&box2dShape, 1);
}

void CCollisionMgr::removeBox2dCollider(ISharedBox2dColliderRef collider)
{
    assert(collider);
    assert(m_box2dScene);
    
    m_box2dColliders.erase(collider);
    m_box2dScene->DestroyBody(collider->getBox2dBody());
}

void CCollisionMgr::_OnGameLoopUpdate(f32 deltatime)
{
    if(m_box2dScene)
    {
        m_box2dScene->Step(1.0f / 30.0f, 1, 1);
        
        for(const auto& collider : m_box2dColliders)
        {
            collider->onBox2dPositionChanged(glm::vec3(collider->getBox2dBody()->GetPosition().x,
                                                       0.0f,
                                                       collider->getBox2dBody()->GetPosition().y));
            collider->onBox2dRotationYChanged(collider->getBox2dBody()->GetAngle());
            std::cout<<"box2d velocity: "<<collider->getBox2dBody()->GetLinearVelocity().x<<", "<<collider->getBox2dBody()->GetLinearVelocity().y<<std::endl;
        }
    }
}

void CCollisionMgr::BeginContact(b2Contact* contact)
{
    IBox2dCollider* bodyUserData_01 = static_cast<IBox2dCollider*>(contact->GetFixtureA()->GetBody()->GetUserData());
    IBox2dCollider* bodyUserData_02 = static_cast<IBox2dCollider*>(contact->GetFixtureB()->GetBody()->GetUserData());
    
    if(bodyUserData_01 && bodyUserData_02)
    {
        bodyUserData_01->onBox2dCollision();
        bodyUserData_02->onBox2dCollision();
    }
}

void CCollisionMgr::EndContact(b2Contact* contact)
{
    
}


void CCollisionMgr::onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton)
{
    assert(m_camera != nullptr);
    glm::ray ray;
    CCollisionMgr::unproject(point, m_camera->getVMatrix(), m_camera->getPMatrix(), m_camera->getViewport(), &ray);
    
    for(const auto& collider : m_touchColliders)
    {
        std::vector<ISharedGameObject> colliders = collider->getColliders();
        for(const ISharedGameObject& gameObject : colliders)
        {
            glm::vec3 point;
            if(!glm::intersect(ray,
                               gameObject->getMinBound() + gameObject->getPosition(),
                               gameObject->getMaxBound() + gameObject->getPosition()))
            {
                continue;
            }
            if(CCollisionMgr::collisionPoint(gameObject->getCollisionVertexBuffer(),
                                             gameObject->getCollisionIndexBuffer(),
                                             glm::mat4x4(1.0f),
                                             ray,
                                             &point))
            {
                collider->onTouchCollision(point, gameObject, inputButton);
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

void CCollisionMgr::onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction)
{
    
}

void CCollisionMgr::onKeyUp(i32 key)
{
    
}

void CCollisionMgr::onKeyDown(i32 key)
{
    
}

void CCollisionMgr::unproject(const glm::ivec2& point,
                              const glm::mat4x4& viewMatrix,
                              const glm::mat4x4& projectionMatrix,
                              const glm::ivec4& viewport,
                              glm::ray* ray)
{
    glm::vec3 direction, origin;
    f32 screenX =  -((( 2.0f * point.x ) / viewport[2]) - 1.0f ) / projectionMatrix[0][0];
    f32 screenY =  ((( 2.0f * point.y ) / viewport[3]) - 1.0f ) / projectionMatrix[1][1];
    glm::mat4x4 viewInverseMatrix = glm::inverse(viewMatrix);
    
    direction.x  = (screenX * viewInverseMatrix[0][0] + screenY * viewInverseMatrix[1][0] + viewInverseMatrix[2][0]);
    direction.y  = (screenX * viewInverseMatrix[0][1] + screenY * viewInverseMatrix[1][1] + viewInverseMatrix[2][1]);
    direction.z  = (screenX * viewInverseMatrix[0][2] + screenY * viewInverseMatrix[1][2] + viewInverseMatrix[2][2]);
    
    origin.x = viewInverseMatrix[3][0];
    origin.y = viewInverseMatrix[3][1];
    origin.z = viewInverseMatrix[3][2];
    
    ray->setDirection(direction);
    ray->setOrigin(origin);
}

bool CCollisionMgr::isTrianglesIntersected(CSharedCameraRef camera,
                                       const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& triangles,
                                       const glm::ivec2& point,
                                       glm::vec3* intersectPoint)
{
    glm::ray ray;
    CCollisionMgr::unproject(point,
                             camera->getVMatrix(),
                             camera->getPMatrix(),
                             camera->getViewport(),
                             &ray);
    for(const auto& iterator : triangles)
    {
        if(CCollisionMgr::triangleIntersection(std::get<0>(iterator),
                                               std::get<1>(iterator),
                                               std::get<2>(iterator),
                                               ray, intersectPoint))
        {
            return true;
        }

    }
    return false;
}

bool CCollisionMgr::isGameObjectBoundIntersected(CSharedCameraRef camera,
                                                 ISharedGameObjectRef gameObject,
                                                 const glm::ivec2& point,
                                                 bool isIncludeTransformation,
                                                 bool isUpdateTransformation)
{
    assert(camera != nullptr);
    assert(gameObject != nullptr);
    glm::ray ray;
    CCollisionMgr::unproject(point,
                             camera->getVMatrix(),
                             camera->getPMatrix(),
                             camera->getViewport(),
                             &ray);
    
    glm::vec3 minBound;
    glm::vec3 maxBound;
    
    if(isIncludeTransformation)
    {
        if(isUpdateTransformation)
        {
            std::tuple<glm::vec3, glm::vec3> bounds = gameObject->getBounds(gameObject->getMMatrix());
            minBound = std::get<0>(bounds);
            maxBound = std::get<1>(bounds);
        }
        else
        {
            glm::vec4 vector = gameObject->getMMatrix() * glm::vec4(gameObject->getMinBound(), 1.0);
            minBound = glm::vec3(vector.x, vector.y, vector.z);
            vector = gameObject->getMMatrix() * glm::vec4(gameObject->getMaxBound(), 1.0);
            maxBound = glm::vec3(vector.x, vector.y, vector.z);
        }
    }
    else
    {
        minBound = gameObject->getMinBound();
        maxBound = gameObject->getMaxBound();
    }
    
    return glm::intersect(ray,
                          minBound,
                          maxBound);
}

bool CCollisionMgr::isGameObjectIntersected(CSharedCameraRef camera,
                                            ISharedGameObjectRef gameObject,
                                            const glm::ivec2& point,
                                            glm::vec3* intersectedPoint,
                                            bool isIncludeTransformation,
                                            bool isUpdateTransformation)
{
    assert(camera != nullptr);
    assert(gameObject != nullptr);
    glm::ray ray;
    CCollisionMgr::unproject(point,
                             camera->getVMatrix(),
                             camera->getPMatrix(),
                             camera->getViewport(),
                             &ray);
    
    glm::vec3 minBound;
    glm::vec3 maxBound;
    
    if(isIncludeTransformation)
    {
        if(isUpdateTransformation)
        {
            std::tuple<glm::vec3, glm::vec3> bounds = gameObject->getBounds(gameObject->getMMatrix());
            minBound = std::get<0>(bounds);
            maxBound = std::get<1>(bounds);
        }
        else
        {
            glm::vec4 vector = gameObject->getMMatrix() * glm::vec4(gameObject->getMinBound(), 1.0);
            minBound = glm::vec3(vector.x, vector.y, vector.z);
            vector = gameObject->getMMatrix() * glm::vec4(gameObject->getMaxBound(), 1.0);
            maxBound = glm::vec3(vector.x, vector.y, vector.z);
        }
    }
    else
    {
        minBound = gameObject->getMinBound();
        maxBound = gameObject->getMaxBound();
    }
    
    if(!glm::intersect(ray,
                       minBound,
                       maxBound))
    {
        return false;
    }
    
    return CCollisionMgr::collisionPoint(gameObject->getCollisionVertexBuffer(),
                                         gameObject->getCollisionIndexBuffer(),
                                         gameObject->getMMatrix(),
                                         ray,
                                         intersectedPoint);
}

bool CCollisionMgr::triangleIntersection(const glm::vec3& trianglePoint_01,
                                         const glm::vec3& trianglePoint_02,
                                         const glm::vec3& trianglePoint_03,
                                         const glm::ray& ray,
                                         glm::vec3 *intersectPoint)
{
    glm::vec3 edge_01 = trianglePoint_02 - trianglePoint_01;
	glm::vec3 edge_02 = trianglePoint_03 - trianglePoint_01;
    
    glm::vec3 pVector = glm::cross(ray.getDirection(), edge_02);
	f32 determinant = glm::dot(edge_01, pVector);
	if(fabs(determinant) < 0.0001f)
    {
        return false;
    }
    
    f32 invDeterminant = 1.0f / determinant;
	glm::vec3 tVector = ray.getOrigin() - trianglePoint_01;
    
	f32 u = glm::dot(tVector, pVector) * invDeterminant;
    if ( u < -0.0001f || u > 1.0001f )
    {
        return false;
    }
    
    glm::vec3 qVector = glm::cross(tVector, edge_01);
	f32 v = glm::dot(ray.getDirection(), qVector) * invDeterminant;
	if ( v < -0.0001f || (v + u) > 1.0001f )
    {
        return false;
    }
    
	(*intersectPoint) = trianglePoint_01 + (edge_01 * u) + (edge_02 * v);
	return true;
}

bool CCollisionMgr::collisionPoint(CSharedVertexBufferRef vertexBuffer,
                                   CSharedIndexBufferRef indexBuffer,
                                   const glm::mat4x4& transformation,
                                   const glm::ray& ray,
                                   glm::vec3* point)
{
    SAttributeVertex* vertexData = vertexBuffer->lock();
    ui16* indexData = indexBuffer->lock();
    ui32 numIndices = indexBuffer->getUsedSize();

    for(ui32 i = 0; i < numIndices; i += 3)
    {
        ui32 index = indexData[i];
        if(index >= vertexBuffer->getUsedSize())
        {
            continue;
        }
        glm::vec4 value = transformation * glm::vec4(vertexData[index].m_position, 1.0f);
        glm::vec3 point_01 = glm::vec3(value.x, value.y, value.z);
        
        index = indexData[i + 1];
        if(index >= vertexBuffer->getUsedSize())
        {
            continue;
        }
        value = transformation * glm::vec4(vertexData[index].m_position, 1.0f);
        glm::vec3 point_02 = glm::vec3(value.x, value.y, value.z);
        
        index = indexData[i + 2];
        if(index >= vertexBuffer->getUsedSize())
        {
            continue;
        }
        value = transformation * glm::vec4(vertexData[index].m_position, 1.0f);
        glm::vec3 point_03 = glm::vec3(value.x, value.y, value.z);
        
        if(CCollisionMgr::triangleIntersection(point_01, point_02, point_03, ray, point))
        {
            return true;
        }
    }
    return false;
}
