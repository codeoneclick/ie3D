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
    
    virtual std::vector<ISharedGameObject> colliders(void) = 0;
    virtual void onCollision(const glm::vec3& position, ISharedGameObjectRef gameObject) = 0;
    
public:
    
    virtual ~ICollisionHandler(void) = default;
};

class CCollisionMgr final : public IGestureRecognizerHandler
{
private:
    
protected:
    
    CSharedCamera m_camera;
    std::set<ISharedCollisionHandler> m_handlers;
    
    bool collisionPoint(CSharedVertexBufferRef vertexBuffer,
                        CSharedIndexBufferRef indexBuffer,
                        const glm::mat4x4& worldMatrix,
                        const glm::vec3& origin,
                        const glm::vec3& direction,
                        glm::vec3* _point);
    
    static bool triangleIntersection(const glm::vec3& trianglePoint_01,
                                     const glm::vec3& trianglePoint_02,
                                     const glm::vec3& trianglePoint_03,
                                     const glm::vec3& origin,
                                     const glm::vec3& direction,
                                     glm::vec3* intersectPoint);
    
    void onGestureRecognizerPressed(const glm::ivec2& point, bool isRightButton = false);
    void onGestureRecognizerMoved(const glm::ivec2& point);
    void onGestureRecognizerReleased(const glm::ivec2& point, bool isRightButton = false);
    
public:
    
    CCollisionMgr(void);
    ~CCollisionMgr(void);
    
    void setCamera(CSharedCameraRef camera);

    
    static void unproject(const glm::ivec2& point,
                          const glm::mat4x4& viewMatrix,
                          const glm::mat4x4& projectionMatrix,
                          const glm::ivec4& viewport,
                          glm::vec3* origin,
                          glm::vec3* direction);
    
    void addCollisionHandler(ISharedCollisionHandlerRef handler);
    void removeCollisionHandler(ISharedCollisionHandlerRef handler);
    
    static bool isIntersected(CSharedCameraRef camera,
                              const glm::ivec2& point,
                              glm::vec3* intersectPoint);
};

#endif 
