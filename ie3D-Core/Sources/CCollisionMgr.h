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

class ICollisionHandler
{
private:
    
    friend class CCollisionMgr;
    
protected:
    
    ICollisionHandler(void) = default;
    
    virtual std::vector<ISharedGameObject> colliders(void);
    virtual void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject, E_INPUT_BUTTON inputButton);
    
public:
    
    virtual ~ICollisionHandler(void) = default;
};

class CCollisionMgr final : public IGestureRecognizerHandler
{
private:
    
protected:
    
    CSharedCamera m_camera;
    std::set<ISharedCollisionHandler> m_handlers;
    
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
    
public:
    
    CCollisionMgr(void);
    ~CCollisionMgr(void);
    
    void setCamera(CSharedCameraRef camera);

    
    static void unproject(const glm::ivec2& point,
                          const glm::mat4x4& viewMatrix,
                          const glm::mat4x4& projectionMatrix,
                          const glm::ivec4& viewport,
                          glm::ray *ray);
    
    void addCollisionHandler(ISharedCollisionHandlerRef handler);
    void removeCollisionHandler(ISharedCollisionHandlerRef handler);
    
    static bool isTrianglesIntersected(CSharedCameraRef camera,
                                       const std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>>& triangles,
                                       const glm::ivec2& point,
                                       glm::vec3* intersectPoint);
    
    static bool isGameObjectIntersected(CSharedCameraRef camera,
                                        ISharedGameObjectRef gameObject,
                                        const glm::ivec2& point,
                                        glm::vec3* intersectedPoint);
};

#endif 
