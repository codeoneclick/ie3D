//
//  CCollisionMgr.h
//  indie2dEngine
//
//  Created by Sergey Sergeev on 7/19/13.
//  Copyright (c) 2013 Sergey Sergeev. All rights reserved.
//

#ifndef CCollisionMgr_h
#define CCollisionMgr_h

#include "HCommon.h"
#include "IInputContext.h"
#include "HDeclaration.h"
#include "IGameLoopHandler.h"

class ITouchCollider
{
private:
    
protected:
    
public:
    
    ITouchCollider(void) = default;
    virtual ~ITouchCollider(void) = default;
    
    virtual std::vector<ISharedGameObject> getColliders(void) const;
    virtual void onTouchCollision(const glm::vec3& point, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton);
};

class IBox2dCollider
{
private:
    
protected:
    
    b2Body* m_box2dBody;
    b2BodyDef m_box2dBodyDefinition;
    
public:
    
    IBox2dCollider(void);
    virtual ~IBox2dCollider(void) = default;
    
    void init(std::shared_ptr<b2World> box2dScene);
    
    b2Body* getBox2dBody(void) const;
    b2BodyDef* getBox2dBodyDefinition(void);
    
    virtual void onBox2dCollision(void);
    virtual void onBox2dPositionChanged(const glm::vec3& position);
    virtual void onBox2dRotationYChanged(f32 angle);
    
    virtual glm::vec2 getBox2dCenter(void) const;
    virtual std::tuple<glm::vec2, glm::vec2> getBox2dBoundingBox(void) const;
};

class CCollisionMgr final : public IGestureRecognizerHandler, public b2ContactListener, public IGameLoopHandler
{
private:
    
    CSharedCamera m_camera;
    std::shared_ptr<b2World> m_box2dScene;
    
    std::set<ISharedTouchCollider> m_touchColliders;
    std::set<ISharedBox2dCollider> m_box2dColliders;
 
protected:
    
    
    
    static bool collisionPoint(CSharedVertexBufferRef vertexBuffer,
                               CSharedIndexBufferRef indexBuffer,
                               const glm::mat4x4& worldMatrix,
                               const glm::ray& ray,
                               glm::vec3* point);
    
    static bool triangleIntersection(const glm::vec3& trianglePoint_01,
                                     const glm::vec3& trianglePoint_02,
                                     const glm::vec3& trianglePoint_03,
                                     const glm::ray& ray,
                                     glm::vec3* intersectPoint);
    
    void onGestureRecognizerPressed(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerDragged(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, E_INPUT_BUTTON inputButton);
    void onGestureRecognizerWheelScroll(E_SCROLL_WHEEL_DIRECTION direction);
    
    void onKeyUp(i32 key);
    void onKeyDown(i32 key);
    
    void _OnGameLoopUpdate(f32 _deltatime);
    
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    
public:
    
    CCollisionMgr(void);
    ~CCollisionMgr(void);
    
    void setCamera(CSharedCameraRef camera);
    
    
    static void unproject(const glm::ivec2& point,
                          const glm::mat4x4& viewMatrix,
                          const glm::mat4x4& projectionMatrix,
                          const glm::ivec4& viewport,
                          glm::ray *ray);
    
    void addTouchCollider(ISharedTouchColliderRef collider);
    void removeTouchCollider(ISharedTouchColliderRef collider);
    
    void setBox2dScene(const glm::vec2 &minBound, const glm::vec2 &maxBound);
    void addBox2dCollider(ISharedBox2dColliderRef collider, bool isStatic);
    void removeBox2dCollider(ISharedBox2dColliderRef collider);
    
    static bool isTrianglesIntersected(CSharedCameraRef camera,
                                       const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& triangles,
                                       const glm::ivec2& point,
                                       glm::vec3* intersectPoint);
    
    static bool isGameObjectIntersected(CSharedCameraRef camera,
                                        ISharedGameObjectRef gameObject,
                                        const glm::ivec2& point,
                                        glm::vec3* intersectedPoint,
                                        bool isIncludeTransformation,
                                        bool isUpdateTransformation = false);
    
    static bool isGameObjectBoundIntersected(CSharedCameraRef camera,
                                             ISharedGameObjectRef gameObject,
                                             const glm::ivec2& point,
                                             bool isIncludeTransformation,
                                             bool isUpdateTransformation = false);
};

#endif
